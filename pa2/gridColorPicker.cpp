#include "gridColorPicker.h"

gridColorPicker::gridColorPicker(HSLAPixel gridColor, int gridSpacing)
{
    
/* Your code here! */
color = gridColor;
spacing = gridSpacing;

}

HSLAPixel gridColorPicker::operator()(int x, int y)
{

/* Your code here! */
    HSLAPixel ret;
    if (x % spacing == 0 || y % spacing == 0) {
        ret = color;
    } else {
        ret.h = 0.0;
        ret.s = 0.0;
        ret.l = 1.0;
    }
    return ret;

}


