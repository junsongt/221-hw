#define CATCH_CONFIG_MAIN
#include "cs221util/catch.hpp"
#include <limits.h>
#include <vector>
#include <sys/stat.h>
#include <iostream>
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"
#include "filler.h"
#include "solidColorPicker.h"
#include "gridColorPicker.h"
#include "gradientColorPicker.h"
#include "rainbowColorPicker.h"

using namespace std;
using namespace cs221util;

#define FUNCTORTESTHEIGHT 60
#define FUNCTORTESTWIDTH 60
#define FUNCTORTESTFREQ 10
#define FUNCTORTESTX 40
#define FUNCTORTESTY 10
#define FUNCTORTESTRADIUS 45
#define FUNCTORTESTGRIDSPACING 8

#define SOLIDX 80
#define SOLIDY 80
#define SOLIDTOLERANCE 0.01
#define SOLIDTESTIMAGE "originals/Canada.png"
#define SOLIDFRAMEFREQ 100

#define GRIDX 120
#define GRIDY 75
#define GRIDTOLERANCE 0.008
#define GRIDTESTIMAGE "originals/mooncake.png"
#define GRIDFRAMEFREQ 50
#define GRIDGRIDSPACING 5

#define GRADIENTX 50
#define GRADIENTY 50
#define GRADIENTTOLERANCE 0.02
#define GRADIENTTESTIMAGE "originals/cloud.png"
#define GRADIENTFRAMEFREQ 100 
#define GRADIENTRADIUS 70

#define RAINFREQ 1.0/1000.0
#define RAINTOLERANCE 0.05
#define RAINX 75
#define RAINY 75
#define RAINTESTIMAGE "originals/leaf.png"
#define RAINFRAMEFREQ 200 

PNG testColorPicker(colorPicker& picker)
{
    PNG img;
    img.resize(FUNCTORTESTWIDTH, FUNCTORTESTHEIGHT);
    HSLAPixel px;

/*
    for (int x = 1; x < FUNCTORTESTWIDTH; x = x + x) {
        for (int y = 1; y < FUNCTORTESTHEIGHT; y = y + y) {
            px = picker(x, y);
            cout << "\toperator()(" << x << ", " << y << ") = {"
                << (int)px.h << ", "
                << (int)px.s << ", "
                << (int)px.l << "}" << endl;
        }
    }
*/

    for (int x = 0; x < FUNCTORTESTWIDTH; x++){
        for (int y = 0; y < FUNCTORTESTHEIGHT; y++){
            HSLAPixel * p = img.getPixel(x,y);
            * p = picker(x, y);
        }
    }

    return img;
}

TEST_CASE("colorPicker::basic solid","[weight=1][part=colorPicker]"){

    HSLAPixel px;
    px.h = 0; px.s = 1.0; px.l = 0.5; // red
    solidColorPicker solidPicker(px); 

    PNG result = testColorPicker(solidPicker);
    result.writeToFile("images/solidColorPickerTest.png");
    PNG expected; expected.readFromFile("soln_images/solidColorPickerTest.png");
    REQUIRE(result == expected);
    
}

TEST_CASE("colorPicker::basic gradient","[weight=1][part=colorPicker]"){
    HSLAPixel px1, px2;
    px1.h = 360.; px1.s = 1.0; px1.l = 0.5; 
    px2.h = 240.; px2.s = 1.0; px2.l = 0.5;

    gradientColorPicker gradientPicker(px1, px2, FUNCTORTESTRADIUS,
                                       FUNCTORTESTX, FUNCTORTESTY);

    PNG result = testColorPicker(gradientPicker);
    result.writeToFile("images/gradientColorPickerTest.png");
    PNG expected; expected.readFromFile("soln_images/gradientColorPickerTest.png");
    REQUIRE(result == expected);
}

TEST_CASE("colorPicker::basic grid","[weight=1][part=colorPicker]"){
    HSLAPixel px1;
    px1.h = 120; px1.s = 1.0; px1.l = 0.25;
    gridColorPicker gridPicker(px1, FUNCTORTESTGRIDSPACING);

    PNG result = testColorPicker(gridPicker);
    result.writeToFile("images/gridColorPickerTest.png");
    PNG expected; expected.readFromFile("soln_images/gridColorPickerTest.png");
    REQUIRE(result == expected);
}

TEST_CASE("colorPicker::basic rainbow","[weight=1][part=colorPicker]"){
    rainbowColorPicker rainPicker(1.0/1000.0);

    PNG result = testColorPicker(rainPicker);
    result.writeToFile("images/rainColorPickerTest.png");
    PNG expected; expected.readFromFile("soln_images/rainColorPickerTest.png");
    REQUIRE(result == expected);
}


TEST_CASE("fill::basic solid dfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(SOLIDTESTIMAGE);
    HSLAPixel px(100., 1.0, 0.5);

    animation anim;
    anim = filler::fillSolidDFS(img, SOLIDX, SOLIDY, px, SOLIDTOLERANCE,
                                      SOLIDFRAMEFREQ);
    PNG result = anim.write("images/dfssolid.gif"); 
    result.writeToFile("images/dfssolid.png");

    PNG expected; expected.readFromFile("soln_images/dfssolid.png");
    REQUIRE(result==expected);

}


TEST_CASE("fill::basic solid bfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(SOLIDTESTIMAGE);
    HSLAPixel px(100., 1.0, 0.5);

    animation anim;
    anim = filler::fillSolidBFS(img, SOLIDX, SOLIDY, px, SOLIDTOLERANCE,
                                      SOLIDFRAMEFREQ);
    PNG result = anim.write("images/bfssolid.gif");
    result.writeToFile("images/bfssolid.png");

    PNG expected; expected.readFromFile("soln_images/bfssolid.png");
    REQUIRE(result==expected);
}


TEST_CASE("fill::basic grid dfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(GRIDTESTIMAGE);
    HSLAPixel px(40., 1.0, 0.5);

    animation anim;
    anim = filler::fillGridDFS(img, GRIDX, GRIDY, px, GRIDGRIDSPACING,
                                     GRIDTOLERANCE, GRIDFRAMEFREQ);
    PNG result = anim.write("images/dfsgrid.gif");
    result.writeToFile("images/dfsgrid.png");

    PNG expected; expected.readFromFile("soln_images/dfsgrid.png");
    REQUIRE(result==expected);
}

TEST_CASE("fill::basic grid bfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(GRIDTESTIMAGE);
    HSLAPixel px(200., 1.0, 0.5);

    animation anim;
    anim = filler::fillGridBFS(img, GRIDX, GRIDY, px, GRIDGRIDSPACING,
                                     GRIDTOLERANCE, GRIDFRAMEFREQ);
    PNG result = anim.write("images/bfsgrid.gif");
    result.writeToFile("images/bfsgrid.png");

    PNG expected; expected.readFromFile("soln_images/bfsgrid.png");
    REQUIRE(result==expected);
}

TEST_CASE("fill::basic grad dfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(GRADIENTTESTIMAGE);
    HSLAPixel px1;
    px1.h = 0; px1.s = 1.0; px1.l = 0.5;
    HSLAPixel px2;
    px2.h = 200; px2.s = 1.0; px2.l = 0.5;

    animation anim;
    anim = filler::fillGradientDFS(img, GRADIENTX, GRADIENTY, px1, px2,
                                         GRADIENTRADIUS, GRADIENTTOLERANCE,
                                         GRADIENTFRAMEFREQ);
    PNG result = anim.write("images/dfsgrad.gif");
    result.writeToFile("images/dfsgrad.png");

    PNG expected; expected.readFromFile("soln_images/dfsgrad.png");
    REQUIRE(result==expected);
}

TEST_CASE("fill::basic grad bfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(GRADIENTTESTIMAGE);
    HSLAPixel px1;
    px1.h = 0; px1.s = 1.0; px1.l = 0.5;
    HSLAPixel px2;
    px2.h = 200; px2.s = 1.0; px2.l = 0.5;

    animation anim;
    anim = filler::fillGradientBFS(img, GRADIENTX, GRADIENTY, px1, px2,
                                         GRADIENTRADIUS, GRADIENTTOLERANCE,
                                         GRADIENTFRAMEFREQ);
    PNG result = anim.write("images/bfsgrad.gif");
    result.writeToFile("images/bfsgrad.png");

    PNG expected; expected.readFromFile("soln_images/bfsgrad.png");
    REQUIRE(result==expected);
}

TEST_CASE("fill::basic rainbow dfs","[weight=1][part=fill]"){

    PNG img;
    img.readFromFile(RAINTESTIMAGE);

    animation anim;
    anim = filler::fillRainDFS(img, RAINX, RAINY, RAINFREQ,
                                         RAINTOLERANCE,
                                         RAINFRAMEFREQ);
    PNG result = anim.write("images/dfsrain.gif");
    result.writeToFile("images/dfsrain.png");

    PNG expected; expected.readFromFile("soln_images/dfsrain.png");
    REQUIRE(result==expected);

}
TEST_CASE("fill::basic rainbow bfs","[weight=1][part=fill]"){

    PNG img;
    img.readFromFile(RAINTESTIMAGE);

    animation anim;
    anim = filler::fillRainBFS(img, RAINX, RAINY, RAINFREQ,
                                         RAINTOLERANCE,
                                         RAINFRAMEFREQ);
    PNG result = anim.write("images/bfsrain.gif");
    result.writeToFile("images/bfsrain.png");

    PNG expected; expected.readFromFile("soln_images/bfsrain.png");
    REQUIRE(result==expected);

}

