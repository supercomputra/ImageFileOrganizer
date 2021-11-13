# ImageFileOrganizer

ImageFileOrganizer is the tool to organized image files to be organized in directory with chronological manner. This tool utilizes the date information stored in the exif date owned by the image and create associated directory from it. The directory will be created in format `{ROOTDIR}/YYYY/MM/DD`.

From this:
```
- ROOTDIR
  - HOLIDAY
    - IMG_1023
    - IMG_1024
  - WEDDING
    - IMG_9010
    - IMG_9000
```

To this:
```
- ROOTDIR
  - 2021
    - 09
      - 28
        - IMG_9010
        - IMG_9000
    - 10
      - 03
        - IMG_1023
        - IMG_1024
```

## Getting Started
To get started please specify the target directory on the main.cpp file before executing the command below
```
g++ -std=c++17 main.cpp -o main && ./main 
```
