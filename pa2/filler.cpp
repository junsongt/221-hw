/**
 * @file filler.cpp
 * Implementation of functions in the filler namespace.
 *
 */
//#include "filler.h"

animation filler::fillSolidDFS(PNG& img, int x, int y, HSLAPixel fillColor,
                               double tolerance, int frameFreq) {
    /**
     * @todo Your code here!
     */
    solidColorPicker s(fillColor);
    return fill<Stack>(img, x, y, s, tolerance, frameFreq);
}

animation filler::fillGridDFS(PNG& img, int x, int y, HSLAPixel gridColor,
                              int gridSpacing, double tolerance, int frameFreq) {
    /**
     * @todo Your code here!
     */
    gridColorPicker g(gridColor, gridSpacing);
    return fill<Stack>(img, x, y, g, tolerance, frameFreq);
}

animation filler::fillGradientDFS(PNG& img, int x, int y,
                                  HSLAPixel fadeColor1, HSLAPixel fadeColor2,
                                  int radius, double tolerance, int frameFreq) {
    /**
     * @todo Your code here!
     */
    gradientColorPicker gt(fadeColor1, fadeColor2, radius, x, y);
    return fill<Stack>(img, x, y, gt, tolerance, frameFreq);
}

animation filler::fillRainDFS(PNG& img, int x, int y,
                              long double freq, double tolerance, int frameFreq) {
    /**
     *  Our code here!
     */

    rainbowColorPicker a(freq);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillSolidBFS(PNG& img, int x, int y, HSLAPixel fillColor,
                               double tolerance, int frameFreq) {
    /**
     * @todo Your code here!
     */
    solidColorPicker s(fillColor);
    return fill<Queue>(img, x, y, s, tolerance, frameFreq);
}

animation filler::fillGridBFS(PNG& img, int x, int y, HSLAPixel gridColor,
                              int gridSpacing, double tolerance, int frameFreq) {
    /**
     * @todo Your code here!
     */
    gridColorPicker g(gridColor, gridSpacing);
    return fill<Queue>(img, x, y, g, tolerance, frameFreq);
}

animation filler::fillGradientBFS(PNG& img, int x, int y,
                                  HSLAPixel fadeColor1, HSLAPixel fadeColor2,
                                  int radius, double tolerance, int frameFreq) {
    /**
     * @todo Your code here!
     */
    gradientColorPicker gt(fadeColor1, fadeColor2, radius, x, y);
    return fill<Queue>(img, x, y, gt, tolerance, frameFreq);
}

animation filler::fillRainBFS(PNG& img, int x, int y,
                              long double freq, double tolerance, int frameFreq) {
    /**
     * Our code here!
     */
    rainbowColorPicker a(freq);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

template <template <class T> class OrderingStructure>
animation filler::fill(PNG& img, int x, int y, colorPicker& fillColor,
                       double tolerance, int frameFreq) {
    /**
     * @todo You need to implement this function!
     *
     * This is the basic description of a flood-fill algorithm: Every fill
     * algorithm requires an ordering structure, which is passed to this
     * function via its template parameter. For a breadth-first-search
     * fill, that structure is a Queue, for a depth-first-search, that
     * structure is a Stack.
     *
     * To begin the algorithm, you simply:
     * a) place the given point in the ordering structure,
     * marking it processed (the way you mark it is a design
     * decision you'll make yourself).
     *
     * b) We have a choice to either change the color, if appropriate,
     * when we * add the point to the OS, or when we take it off.
     * In our test cases, * we have assumed that you will change
     * the color when a point is added to the structure.
     *
     * c) Add the first image to the animation (it's convenient to
     * think of this as the 0th image).
     *
     * THEN, until the structure is empty, you do the following:
     *
     * 1.     Remove a point from the ordering structure, and then...
     *
     *        a.    add its unprocessed neighbors whose color values are
     *              within (or equal to) tolerance distance from the
     *              center's color, to the ordering structure, set their
     *              new color, and mark them as processed.
     *        b.    if it is an appropriate frame, send the current
     *              PNG to the animation (as described below).
     *
     * 2.     When implementing your breadth-first-search and
     *        depth-first-search fills, you will need to explore
     *        neighboring pixels in some order.
     *
     *        While the order in which you examine neighbors does
     *        not matter for a proper fill, you must use the same
     *        order as we do for your animations to come out like
     *        ours! The order you should put neighboring pixels
     *        **ONTO** the queue or stack is as follows:
     *        RIGHT(+x), DOWN(+y), LEFT(-x), UP(-y).
     *        IMPORTANT NOTE: *UP* here means towards the top of
     *        the image, so since an image has smaller y coordinates
     *        at the top, this is in the *negative y*
     *        direction. Similarly, *DOWN* means in the *positive y*
     *        direction.
     *
     *        To reiterate, when you are exploring
     *        (filling out) from a given pixel, you must first try
     *        to fill the pixel to
     *        it's RIGHT, then the one DOWN from it, then to the LEFT and
     *        finally UP. If you do them in a different order, your fill may
     *        still work correctly, but your animations will be different
     *        from the grading scripts!
     *
     * 3.     For every k pixels filled, **starting at the kth pixel**, you
     *        must add a frame to the animation, where k = frameFreq.
     *
     *        For example, if frameFreq is 4, then after the 4th pixel has
     *        been filled you should add a frame to the animation, then
     *        again after the 8th pixel, etc.  You must only add
     *        frames for the number of pixels that have been filled,
     *        not the number that have been checked. So if frameFreq
     *        is set to 1, a pixel should be filled every frame.
     *
     * 4.     Finally, as you leave the function, send one last frame to the
     *        animation. This frame will be the final result of the
     *        fill, and it will be the one we test against.
     */
    int w = img.width();
    int h = img.height();

    animation imgList;

    OrderingStructure<std::pair<int, int>> pointList;

    // create a initial visited map, <row = y coord, col = x coord>
    vector<vector<bool>> visited;
    for (int i = 0; i < h; i++) {
        vector<bool> row;
        for (int j = 0; j < w; j++) {
            row.push_back(false);
        }
        visited.push_back(row);
    }

    // find origin point pixel
    HSLAPixel originPixel = *(img.getPixel(x, y));
    std::pair<int, int> origin(x, y);
    // add the point to structure
    pointList.add(origin);
    // change color
    *(img.getPixel(x, y)) = fillColor(x, y);
    // mark this point in the visited map as true
    visited[y][x] = true;

    imgList.addFrame(img);

    // std::pair<int, int> dir[4] = {
    //     std::pair<int, int>(1, 0),
    //     std::pair<int, int>(0, 1),
    //     std::pair<int, int>(-1, 0),
    //     std::pair<int, int>(0, -1)};

    int count = 0;
    while (!pointList.isEmpty()) {
        // remove point
        std::pair<int, int> center = pointList.remove();
        int cx = center.first;
        int cy = center.second;

        // process neighbours
        for (int i = 0; i < 4; i++) {
            // int nx = cx + dir[i].first;
            // int ny = cy + dir[i].second;

            // assign x, y coord for right/down/left/up
            int nx = cx + sin((M_PI / 2) * (i + 1));
            int ny = cy + sin((M_PI / 2) * i);

            // check if a point is valid
            if (nx < w && nx >= 0 && ny < h && ny >= 0 && (visited[ny][nx] == false)) {
                HSLAPixel currPixel = *(img.getPixel(nx, ny));
                if (originPixel.dist(currPixel) <= tolerance) {
                    std::pair<int, int> neighbour(nx, ny);
                    pointList.add(neighbour);
                    *(img.getPixel(nx, ny)) = fillColor(nx, ny);
                    visited[ny][nx] = true;
                    // increment count
                    count++;
                    // check if it is time to send frame
                    if (count == frameFreq) {
                        imgList.addFrame(img);
                        count = 0;
                    }
                }
            }
        }
    }
    // add last frame
    imgList.addFrame(img);
    return imgList;
}
