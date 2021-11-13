#if !defined(__IMAGE_CORRUPTED_HPP)
#define __IMAGE_CORRUPTED_HPP

struct ImageCorrupted : public std::exception {
  std::filesystem::path path;
  std::string description;

  ImageCorrupted(std::filesystem::path path) {
    this->path = path;
    std::ostringstream stream;
    stream << "Couldn't open image for path" << path.string();
    this->description = stream.str();
  }

  const char *what() const throw() {
    return &description[0];
  }
};
#endif