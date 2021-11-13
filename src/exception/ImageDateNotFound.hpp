#if !defined(__IMAGE_DATE_NOT_FOUND_HPP)
#define __IMAGE_DATE_NOT_FOUND_HPP

struct ImageDateNotFound : public std::exception {
  std::filesystem::path path;
  std::string description;

  ImageDateNotFound(std::filesystem::path path) {
    this->path = path;
    std::ostringstream stream;
    stream << "Couldn't find date in the image's metadata for path" << path.string();
    this->description = stream.str();
  }

  const char *what() const throw() {
    return &description[0];
  }
};

#endif