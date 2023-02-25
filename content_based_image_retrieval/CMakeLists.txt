# SPECIFY THE MINIMUM VERSION OF CMAKE REQUIRED
cmake_minimum_required(VERSION 2.8.12)

# SPECIFY YOUR PROJECT NAME
PROJECT(Real_time_filtering)

# ENABLE C++11
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# MAKE SURE OPENCV IS INSTALLED CORRECTLY
find_package( OpenCV REQUIRED )

# INCLUDE OPENCV DIRECTORIES
include_directories( ${OpenCV_INCLUDE_DIRS} include)

set(db_source src/get_dir_feat.cpp)
ADD_EXECUTABLE(get_dir_feat ${db_source})
TARGET_LINK_LIBRARIES(get_dir_feat ${OpenCV_LIBS} )

set(match_source src/get_top_n_matches.cpp)
ADD_EXECUTABLE(get_top_n_matches ${match_source}) 
TARGET_LINK_LIBRARIES(get_top_n_matches ${OpenCV_LIBS} )


