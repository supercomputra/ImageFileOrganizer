# ImageFileOrganizer

ImageFileOrganizer is the tool to organized image files to be organized in directory with chronological manner. This tool utilizes the date information stored in the exif date owned by the image and create associated directory from it. The directory will be created in format `{ROOTDIR}/YYYY/MM/DD`.

## Getting Started
To get started please specify the target directory on the main.cpp file before executing the command below
```
g++ -std=c++17 main.cpp -o main && ./main 
```
