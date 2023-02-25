Compiled Using visual studios 2019
Windows OS (windows 10)

To build project:
from build folder: 
cmake -G "Visual Studio 16 2019" ..
cmake --build . --config Release

Everything below from main directory:
To create feature databases:
./build/Release/get_dir_feat.exe data/olympus/ <method type>
1 corresponds to baseline matching
2 corresponds to single histogram matching
3 corresponds to multi histogram matching
4 corresponds to texture and color histogram matching 

To find Top N matches for methods 1-3:
./build/Release/get_top_n_matches.exe <target image> <feature database> <Number of matches N> <method type> 

To find Top N matches for method 4:
./build/Release/get_top_n_matches.exe <target image> <color feature database> <Number of matches N> <method type> <texture feature database>

