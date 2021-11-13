
#include <filesystem>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "src/exifreader.cpp"
#include "src/organizer.cpp"

int main(int argc, char const *argv[]) {
  std::string image_ext = ".JPG";
  Organizer organizer = Organizer(image_ext);
  std::string dirname = "";
  if (dirname == "") {
    std::cout << "Organizing aborted: Please specify your target directory in the main.cpp!" << std::endl;
    return 1;
  }

  organizer.organize(dirname);
}
