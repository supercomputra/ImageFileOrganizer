#include "organizer.hpp"

#include <cctype>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>

#include "exception.hpp"

Organizer::Organizer(std::string extension, std::string raw_extension = "") {
  this->extension = extension;
  this->raw_extension = raw_extension;
}

std::vector<std::filesystem::path> Organizer::dirfiles(std::string root) {
  std::vector<std::filesystem::path> files;
  std::queue<std::string> dirnames;
  dirnames.push(root);

  while (!dirnames.empty()) {
    std::string current = dirnames.front();
    for (const auto &entry : std::filesystem::directory_iterator(current)) {
      if (!entry.is_directory()) {
        if (entry.path().has_extension()) {
          std::string lower_entry_extension = lowercased(entry.path().extension().string());
          if (!raw_extension.empty()) {
            if (lower_entry_extension == lowercased(raw_extension)) {
              files.push_back(entry.path());
            }
          }

          if (!extension.empty()) {
            if (lower_entry_extension == lowercased(extension)) {
              files.push_back(entry.path());
            }
          }
        }
      } else {
        dirnames.push(entry.path().string());
      }
    }
    dirnames.pop();
  }

  return files;
}

std::filesystem::path Organizer::dirpath(std::string dirname) {
  std::filesystem::path dir(dirname);
  if (std::filesystem::is_directory(dir)) {
    return dir;
  } else {
    std::filesystem::create_directories(dir);
    return dir;
  }
}

void Organizer::replace_prefix(std::filesystem::path path, std::string prefix) {
  std::string origin_filename, new_filename = path.filename().string();
  if (new_filename.size() < prefix.size()) return;
  for (int i = 0; i < prefix.size(); i++) {
    new_filename[i] = prefix[i];
  }
  std::filesystem::rename(path, path.parent_path() / new_filename);
}

void Organizer::organize(std::string root) {
  std::vector<std::filesystem::path> files = Organizer::dirfiles(root);
  std::unordered_map<std::string, tm> lookups;
  int success_count = 0;

  // Collect all files with matched extension
  // Then store the metadata's time in the lookups map
  for (const std::filesystem::path file : files) {
    if (file.has_extension() && (file.extension() == extension || file.extension() == raw_extension)) {
      if (file.has_stem()) {
        try {
          tm t = datetime(file);
          lookups[file.stem().string()] = t;
        } catch (std::exception &e) {
          std::cout << e.what() << std::endl;
        }
      }
    }
  }

  // Process all eligible files
  // Eligble files corresponding time should available in the lookups map
  std::vector<std::filesystem::path> unknowns;
  for (const std::filesystem::path file : files) {
    if (!file.has_stem()) {
      unknowns.push_back(file);
      continue;
    }

    if (lookups.find(file.stem().string()) == lookups.end()) {
      unknowns.push_back(file);
      continue;
    }

    // Create dir with dirname based on date
    std::tm time = lookups[file.stem().string()];

    std::string year = std::to_string(time.tm_year + 1900);
    std::ostringstream month_stream;
    month_stream << std::setw(2) << std::setfill('0') << time.tm_mon + 1;
    std::string month = month_stream.str();
    std::ostringstream day_stream;
    day_stream << std::setw(2) << std::setfill('0') << time.tm_mday;
    std::string day = day_stream.str();
    std::string dirname = root + "/" + year + "/" + month + "/" + day;
    std::filesystem::path dir = dirpath(dirname);

    // Move
    std::filesystem::rename(file, dir / file.filename().string());
    success_count++;
  }

  // Move all non identified time files to Others directory
  // Handle collision for identic file name
  std::set<std::string> existing_stems;
  for (const std::filesystem::path unknown : unknowns) {
    if (unknown.has_stem()) {
      std::string stem = unknown.stem().string();
      int count = 2;
      if (existing_stems.find(stem) != existing_stems.end()) {
        std::string new_stem = stem + "_" + std::to_string(count);
        while (existing_stems.find(new_stem) != existing_stems.end()) {
          count++;
        }
        stem = new_stem;
      }

      std::string filename = stem;
      if (unknown.has_extension()) {
        filename += unknown.extension().string();
      }

      std::filesystem::path dir = dirpath(root + "/" + "Others");
      std::filesystem::rename(unknown, dir / filename);

      existing_stems.insert(stem);
    }
  }

  std::cout << "Successfully organized " << success_count << "/" << files.size() << std::endl;
}

ImageMetadata Organizer::metadata(std::filesystem::path path) {
  FILE *file = fopen(path.string().c_str(), "r");
  if (!file) throw ImageCorrupted(path);
  EXIFReader exif;
  exif.DecodeExif(file);
  fclose(file);
  if (!exif.m_exifinfo->IsExif) throw ImageMetadataNotFound(path);
  return *exif.m_exifinfo;
}

std::tm Organizer::datetime(std::filesystem::path path) {
  ImageMetadata metadata = Organizer::metadata(path);
  const char *strdatetime = metadata.DateTime;
  if (strlen(strdatetime) == 0) {
    throw ImageDateNotFound(path);
  }

  tm time;
  if (strptime(strdatetime, "%Y:%m:%d %H:%M:%S", &time) != NULL) {
    return time;
  } else {
    throw InvalidDateFormatException(path);
  }
}
