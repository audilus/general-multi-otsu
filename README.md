# general-multi-otsu
A recursive multi-otsu implementation for OpenCV 4.7.X in C++.

Number of classes is arbitrary, but performance degrades exponentially at higher classes and with higher resolution images. 

## Usage Example:

```C++
#include "MultiOtsu.h"

//  Input and output images should be greyscale images (cv::IMREAD_GREYSCALE). 
//  Third argument is class count.
MultiOtsu myOtsu;
myOtsu.multiOtsu(inputImage, outputImage, 3);
```

This was written for an assignment initially and performance was never really the goal, but I figured I'd throw it up here since I would have wanted something similar as reference while researching how to implement Multi-Otsu. For future CS students (or myself if I get around to it): Consider introducing multithreading to accelerate the process. OpenCV's parallel_for or standard std::thread objects should be a good place to start.
