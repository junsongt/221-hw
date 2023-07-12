#include "block.h"

#include <cmath>
#include <iostream>

/* Returns the width, in pixels
 * of the current block
 */
int Block::width() const {
    // Your code here!
    return data[0].size();
}

/* Returns the width, in pixels
 * of the current block
 */
int Block::height() const {
    // Your code here!
    return data.size();
}

/* Given an image whose size is large enough, place
 * the current block in the image so that its upper left corner
 * is at position column, row. Existing pixels in the image
 * will be replaced by those of the block.
 */
void Block::render(PNG &im, int column, int row) const {
    // Your code here!
    for (int i = row; i < row + height(); i++) {
        for (int j = column; j < column + width(); j++) {
            *(im.getPixel(j, i)) = data[i - row][j - column];
        }
    }
}

/* create a block of pixels whose color values are the same as the
 * rectangle in the image described by the upper left corner (column, row)
 * whose size is width x height.
 */
void Block::build(PNG &im, int column, int row, int width, int height) {
    // Your code here!
    for (int i = row; i < height + row; i++) {
        vector<HSLAPixel> rowPixel;
        for (int j = column; j < width + column; j++) {
            rowPixel.push_back(*(im.getPixel(j, i)));
        }
        data.push_back(rowPixel);
    }
}

/* Flip the current block across its horizontal midline.
 * This function changes the existing block.
 */
void Block::flipVert() {
    // Your code here!
    for (int i = 0; i < height() / 2; i++) {
        for (int j = 0; j < width(); j++) {
            swap(data[i][j], data[height() - 1 - i][j]);
        }
    }
}

/* Flip the current block across its vertical midline.
 * This function changes the existing block.
 */
void Block::flipHoriz() {
    // Your code here!
    for (int i = 0; i < height(); i++) {
        for (int j = 0; j < width() / 2; j++) {
            swap(data[i][j], data[i][width() - 1 - j]);
        }
    }
}

/* Rotate the current block 90 degrees counterclockwise.
 * This function changes the existing block.
 */
void Block::rotateRight() {
    // Your code here!
    int h = height();
    int w = width();
    vector<vector<HSLAPixel>> newData;
    for (int i = 0; i < w; i++) {
        vector<HSLAPixel> newRow;
        for (int j = h - 1; j >= 0; j--) {
            newRow.push_back(data[j][i]);
        }
        newData.push_back(newRow);
    }
    data = newData;
}
