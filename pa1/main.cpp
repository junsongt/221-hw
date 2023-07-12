/**
 * @file main.cpp
 * A simple C++ program that manipulates an image.
 *
 * @author CS 221: Basic Algorithms and Data Structures
**/

#include "chain.h"
#include "block.h"
#include <iostream>
#include "cs221util/PNG.h"
using namespace cs221util;
using namespace std;

void TestFlipVert()
{
   PNG ht;
   ht.readFromFile("images/rbgsmall.png");
   Block b;
   b.build(ht, 0,0, 60, 60);
   b.flipVert();

   PNG ht_p = ht;
   b.render(ht_p, 0,0);
   ht_p.writeToFile("images/rbgsmallfv.png");
}
void TestFlipHoriz()
{
   PNG ht;
   ht.readFromFile("images/rbgsmall.png");
   Block b;
   b.build(ht, 0,0, 60, 60);
   b.flipHoriz();

   PNG ht_p = ht;
   b.render(ht_p, 0,0);
   ht_p.writeToFile("images/rbgsmallfh.png");
}
int main()
{
   /**
    * Write your own functions to print out variants of different images!
    * You'll find the outputs in the "images" folder.
    * A sample function has been written for you.
    */
   TestFlipVert();
   TestFlipHoriz();

    
   return 0;
}
