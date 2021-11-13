#if !defined(__INVALID_DATE_FORMAT_EXCEPTION_HPP)
#define __INVALID_DATE_FORMAT_EXCEPTION_HPP

struct InvalidDateFormatException : public std::exception {
  std::filesystem::path path;
  std::string description;

  InvalidDateFormatException(std::filesystem::path path) {
    this->path = path;
    std::ostringstream stream;
    stream << "Couldn't find valid formate in the image's metadata date for path" << path.string();
    this->description = stream.str();
  }

  const char *what() const throw() {
    return &description[0];
  }
};

#endif