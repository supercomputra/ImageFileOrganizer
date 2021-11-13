#if !defined(__IMAGE_METADATA_NOT_FOUND_HPP)
#define __IMAGE_METADATA_NOT_FOUND_HPP

struct ImageMetadataNotFound : public std::exception {
  std::filesystem::path path;
  std::string description;

  ImageMetadataNotFound(std::filesystem::path path) {
    this->path = path;
    std::ostringstream stream;
    stream << "Couldn't find image's metadata for path" << path.string();
    this->description = stream.str();
  }

  const char *what() const throw() {
    return &description[0];
  }
};

#endif