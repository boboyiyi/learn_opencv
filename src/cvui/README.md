## Results

- canny

![Canny](https://github.com/boboyiyi/learnopencv/blob/master/src/cvui/demo/canny.gif)


## Tips

- 单文件库如cvui.h或std_image.h，一般会定义一个宏 如#ifdef CVUI_IMPLEMENTATION，如果在某个源文件中#define CVUI_IMPLEMENTATION，表明这个文件包含了cvui的实现，那么在一个工程中，实现只能有一个，多个文件都想使用cvui怎么办？这个时候我们就在一个文件，比如cvui_impl.cpp中#define CVUI_IMPLEMENTATION，然后其他地方都只要包含cvui.h即可，这样保证了cvui的实现只有一个。