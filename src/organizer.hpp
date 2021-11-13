
#if !defined(__ORGANIZER_HPP)
#define __ORGANIZER_HPP

#include <boost/algorithm/string.hpp>
#include <filesystem>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "exifreader.hpp"

typedef EXIFInfo ImageMetadata;

/**
 * @brief Create a copy of string with lowercased characters
 * 
 * @param str 
 * @return std::string 
 */
std::string lowercased(std::string str) {
  return boost::algorithm::to_lower_copy(str);
}

class Organizer {
 private:
  /**
  * @brief Get metadata from given image's file path
  * 
  * @param path 
  * @return ImageMetadata 
  */
  ImageMetadata metadata(std::filesystem::path path);

  /**
   * @brief Create time from givan image file's date
   * 
   * @param path 
   * @return tm 
   */
  tm datetime(std::filesystem::path path);

  /**
   * @brief Create or return path for given dirname
   * 
   * @param dirname 
   * @return std::filesystem::path 
   */
  std::filesystem::path dirpath(std::string dirname);

  /**
   * @brief 
   * 
   * @param root 
   * @return std::vector<std::filesystem::path> 
   */
  std::vector<std::filesystem::path> dirfiles(std::string root);

  /**
   * @brief Modify file image's file path with given prefix
   * 
   * @param path 
   * @param prefix 
   */
  void replace_prefix(std::filesystem::path path, std::string prefix);

 public:
  /**
  * @brief The targetting image extension
  * 
  */
  std::string extension;

  /**
   * @brief The targetting raw image extension
   * 
   */
  std::string raw_extension;

  /**
   * @brief Construct a new Organizer object
   * 
   * @param extension 
   * @param raw_extension 
   */
  Organizer(std::string extension, std::string raw_extension);

  /**
   * @brief Organize all images file inside the root directory by creating directory name based on the image's date
   * 
   * @param root 
   */
  void organize(std::string root);
};

#endif