#define CATCH_CONFIG_MAIN
#include "cs221util/catch.hpp"

// headers from PA's main.cpp
#include "chain.h"
#include "block.h"
#include "cs221util/PNG.h"
#include <iostream>

using namespace cs221util;
using namespace std;

// Test renderBack
TEST_CASE("chain::basic renderBack", "[weight=1][part=chain]")
{

    PNG pngin;
    pngin.readFromFile("images/rbgsmall.png");

    Chain c(pngin, 32, 15);

    PNG orig = c.render(32,15);
    PNG origback = c.renderBack(32,15);

    REQUIRE(origback == orig);
}
// chain copy constructor
TEST_CASE("chain::basic copy constructor", "[weight=1][part=chain]")
{

    PNG pngin;
    pngin.readFromFile("images/rbgsmall.png");

    Chain c(pngin, 32, 15);
    Chain d(c);

    PNG orig = c.render(32,15);
    PNG result = d.render(32,15);

    REQUIRE(result == orig);
}


TEST_CASE("chain::basic odd reverse", "[weight=1][part=chain]")
{
    PNG img;
    img.readFromFile("images/rbgsmall.png");

    int cols = 3;
    int rows = 5;
    Chain c(img, cols,rows);

    c.reverse();
    PNG res = c.render(cols,rows);

    PNG expected; expected.readFromFile("images/rbgsmalloddreverse.png");
    
    REQUIRE(res == expected);
}

TEST_CASE("chain::basic even reverse", "[weight=1][part=chain]")
{
    PNG img;
    img.readFromFile("images/rbgsmall.png");
    int cols = 3;
    int rows = 10;
    Chain c(img, cols,rows);

    c.reverse();
    PNG res = c.render(cols,rows);

    PNG expected; expected.readFromFile("images/rbgsmallevenreverse.png");
    REQUIRE(res == expected);

}


TEST_CASE("chain::basic rotate k = 1", "[weight=1][part=chain]")
{
    PNG img;
    img.readFromFile("images/rbgsmall.png");
    int cols = 2;
    int rows = 10;

    Chain c(img, cols,rows);
    c.rotate(1);

    PNG res = c.render(cols,rows);

    REQUIRE(res == img);
    
}


TEST_CASE("chain::basic rotate k = 2", "[weight=1][part=chain]")
{
    PNG img;
    img.readFromFile("images/rbgsmall.png");
    int cols = 2;
    int rows = 4;

    Chain c(img, cols,rows);
    c.rotate(2);

    PNG res = c.render(cols,rows);


    PNG expected; expected.readFromFile("images/rbgsmallrotate2.png");

    REQUIRE(res == expected);
    
}

TEST_CASE("chain::long rotate k = 12", "[weight=1][part=chain]")
{
    PNG img;
    img.readFromFile("images/rbgsmall.png");
    int cols = 12;
    int rows = 5;

    Chain c(img, cols,rows);
    c.rotate(12);

    PNG res = c.render(cols,rows);


    PNG expected; expected.readFromFile("images/rbgsmallrotate12.png");

    REQUIRE(res == expected);
    
}

TEST_CASE("chain::basic odd rotate k = 2", "[weight=1][part=chain]")
{
    int cols = 5;
    PNG img(cols, 1);
    img.getPixel(0, 0)->h = 0;
    img.getPixel(1, 0)->h = 1;
    img.getPixel(2, 0)->h = 2;
    img.getPixel(3, 0)->h = 3;
    img.getPixel(4, 0)->h = 4;

    Chain c(img, cols,1);
    c.rotate(2);

    PNG res = c.render(cols,1);
    HSLAPixel *p0 = res.getPixel(0, 0);
    HSLAPixel *p1 = res.getPixel(1, 0);
    HSLAPixel *p2 = res.getPixel(2, 0);
    HSLAPixel *p3 = res.getPixel(3, 0);
    HSLAPixel *p4 = res.getPixel(4, 0);

    REQUIRE(p0->h == 1);
    REQUIRE(p1->h == 0);
    REQUIRE(p2->h == 3);
    REQUIRE(p3->h == 2);
    REQUIRE(p4->h == 4);
}

 //* Composite Tests 

TEST_CASE("chain::mirror large even", "[weight=1][part=chain]")
{
    int cols = 10;
    PNG img(cols, cols);
    for (int x = 0; x < cols; x++)
    {
        for (int y = 0; y < cols; y++)
        {
            img.getPixel(x, y)->h = x;
        }
    }

    Chain c(img, cols,cols);
    c.reverse();

    PNG res = c.render(cols,cols);
    for (int x = 0; x < cols; x++)
    {
        for (int y = 0; y < cols; y++)
        {
            int hVal = cols - 1 - x;
            HSLAPixel *pr = res.getPixel(x, y);
            REQUIRE(pr->h == hVal);
        }
    }
}

TEST_CASE("chain::mirror large odd", "[weight=1][part=chain]")
{
    int cols = 15;
    PNG img(cols, cols);
    for (int x = 0; x < cols; x++)
    {
        for (int y = 0; y < cols; y++)
        {
            img.getPixel(x, y)->h = x;
        }
    }

    Chain c(img, cols,cols);
    c.reverse();

    PNG res = c.render(cols,cols);
    for (int x = 0; x < cols; x++)
    {
        for (int y = 0; y < cols; y++)
        {
            int hVal = cols - 1 - x;
            HSLAPixel *pr = res.getPixel(x, y);
            REQUIRE(pr->h == hVal);
        }
    }
}

TEST_CASE("chain::swapRows large rbg", "[weight=1][part=chain]")
{
    PNG pngin;
    pngin.readFromFile("images/rbg.png");


    Chain c(pngin, 12,3);

    c.swapRows(1,2);
    PNG res = c.render(12,3);
    res.writeToFile("images/rbgswaprows.png");
}

TEST_CASE("chain::swapCols large rbg", "[weight=1][part=chain]")
{
    PNG pngin;
    pngin.readFromFile("images/rbg.png");


    Chain c(pngin,12,25);
    c.swapColumns(10,2);

    PNG res = c.render(12,25);
    res.writeToFile("images/rbgswapcols.png");
}
