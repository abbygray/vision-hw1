#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
   int pix;
   int sum = 0;

   for(pix = 0; pix < im.w * im.h * im.c; pix++) {
       sum += im.data[pix];
   }

    for(pix = 0; pix < im.w * im.h * im.c; pix++) {
       im.data[pix] = im.data[pix]/sum;
    }
}

image make_box_filter(int w)
{
    image filter = make_image(w, w, 1);

    int pix;
    for(pix = 0; pix < w * w; pix++) {
        filter.data[pix] = 1;
    }

    l1_normalize(filter);

    return filter;
}

image convolve_image(image im, image filter, int preserve)
{
    // check filter is valid
    assert(filter.c == im.c || filter.c == 1);
    image f;

    if(preserve) {
        // want same number of channels
        f = make_image(im.w, im.h, im.c);
    } else {
        f = make_image(im.w, im.h, 1);
    }

    // iterate over all pixels in im
        // iterate over all in filter to add to sum
        //set pixel in im

    int channel, col, row, frow, fcol;
    int filter_sz = filter.w * filter.h;

    for(channel = 0; channel < im.c; channel++) {
        int f_c = channel;
        if(filter.c == 1) {
            f_c = 0;
        }

        for(row = 0; row < im.h; row++) {
            for(col = 0; col < im.w; col++) {
                float q = 0;

                for(frow = 0; frow < filter.h; frow++) {
                    for(fcol = 0; fcol < filter.w; fcol++) {
                        q += get_pixel(filter, fcol, frow, f_c) * get_pixel(im, col - (filter.w - 1 - fcol), row - (filter.h - 1 - frow), channel);
                    }
                }

                if(preserve) {
                    set_pixel(f, col, row, channel, q);
                } else {
                    float val = get_pixel(f, col, row, 0);
                    set_pixel(f, col, row, 0, q + val);
                }
            }
            col = 0;
        }
        row = 0;
    }
    
    return f;
}

image make_highpass_filter()
{
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 1, 1, 0, 4);

    return filter;
}

image make_sharpen_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_emboss_filter()
{
    // TODO
    return make_image(1,1,1);
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    return make_image(1,1,1);
}

image add_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image sub_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image make_gx_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_gy_filter()
{
    // TODO
    return make_image(1,1,1);
}

void feature_normalize(image im)
{
    // TODO
}

image *sobel_image(image im)
{
    // TODO
    return calloc(2, sizeof(image));
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
