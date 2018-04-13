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
   float sum = 0.0;

    for(pix = 0; pix < im.w * im.h * im.c; pix++) {
       sum += im.data[pix];
    }

    int channel;
    for(channel = 0; channel < im.c; channel++){
        scale_image(im, channel, 1.0/sum);
    }
    
}

image make_box_filter(int w)
{
    image filter = make_image(w, w, 1);

    int pix;
    for(pix = 0; pix < w * w; pix++) {
        filter.data[pix] = 1.0;
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
        f = make_image(im.w, im.h, im.c);
   } else {
        f = make_image(im.w, im.h, 1);
    }

    int channel, col, row, frow, fcol;

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
                      //  float before = q;
                        float fil = get_pixel(filter, fcol, frow, f_c);
                        int fx = col + fcol - filter.w/2;
                        int fy = row + frow - filter.h/2;
                        float im_val = get_pixel(im, fx, fy, channel);
                        q = q + (fil * im_val);
                    }
                }

               if(preserve) {
                    set_pixel(f, col, row, channel, q);
                } else {
                    float val = get_pixel(f, col, row, 0);
                    set_pixel(f, col, row, 0, q + val);
                }
            }
            
        }
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
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 1, 1, 0, 5);

    return filter;
}

image make_emboss_filter()
{
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, -2);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 2, 1, 0, 1);
    set_pixel(filter, 1, 2, 0, 1);
    set_pixel(filter, 1, 1, 0, 1);
     set_pixel(filter, 2, 2, 0, 2);

    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: We should use preserve for sharpen and emboss image because we want to preserve the color.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    int s = (int) (sigma * 6) + 1;
    int size = s % 2 != 0? s : s + 1;
    image filter = make_image(size, size, 1);

    int row, col;
    for(row = 0; row < size; row++) {
        for(col = 0; col < size; col++) {
            int x = col - filter.w/2;
            int y = row - filter.h/2;

            float power = -(x * x + y *y)/(2*sigma*sigma);
            float val = 1/(TWOPI * sigma * sigma) * exp(power);

            set_pixel(filter, col, row, 1, val);
        }
    }

    l1_normalize(filter);

    return filter;
}

image add_image(image a, image b)
{
    image result = make_image(a.w, a.h, a.c);
    int channel, row, col;
    
    for(channel = 0; channel < result.c; channel++) {
        for(row = 0; row < result.h; row++) {
            for(col = 0; col < result.w; col++) {\
                float pixa = get_pixel(a, col, row, channel);
                float pixb = get_pixel(b, col, row, channel);
                float sum = pixa + pixb;
                set_pixel(result, col, row, channel, sum);
            }
            col = 0;
        }
        row = 0;
    }
    return result;
}

image sub_image(image a, image b)
{
     image result = make_image(a.w, a.h, a.c);
    int channel, row, col;
    
    for(channel = 0; channel < result.c; channel++) {
        for(row = 0; row < result.h; row++) {
            for(col = 0; col < result.w; col++) {\
                float pixa = get_pixel(a, col, row, channel);
                float pixb = get_pixel(b, col, row, channel);
                float diff = pixa - pixb;
                set_pixel(result, col, row, channel, diff);
            }
           
        }
     
    }
    return result;
    return make_image(1,1,1);
}

image make_gx_filter()
{
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 1);
    set_pixel(filter, 0, 1, 0, -2);
    set_pixel(filter, 2, 1, 0, 2);
    set_pixel(filter, 0, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 1);

    return filter;
}

image make_gy_filter()
{
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, -2);
    set_pixel(filter, 2, 0, 0, -1);
    set_pixel(filter, 0, 2, 0, 1);
    set_pixel(filter, 1, 2, 0, 2);
    set_pixel(filter, 2, 2, 0, 1);

    return filter;
}

void feature_normalize(image im)
{
    int p;
    float sum = 0.0;
    float min = INFINITY;
    float max = -1.0;

    for(p = 0; p < im.w * im.h * im.c; p++) {
       float pix = im.data[p];
       if(pix < min) {
           min = pix;
       } 

       if(pix > max) {
           max = pix;
       }
    }

    int channel, row, col;
    for(channel = 0; channel < im.c; channel++){
        for(row = 0; row < im.h; row++) {
            for(col = 0; col < im.w; col++) {
                if(max - min == 0 ) {
                    set_pixel(im, col, row, channel, 0.0);
                } else {
                    float x = get_pixel(im, col, row, channel);
                    float val = (x - min)/(max - min);
                    set_pixel(im, col, row, channel, val);
                }
            }     
        }
    }
    
}

image *sobel_image(image im)
{    
    image *results = calloc(2, sizeof(image));
    results[0] = make_image(im.w, im.h, im.c);
    results[1] = make_image(im.w, im.h, im.c);

  //  image *magnitude = &results[0];
  //  image *direction = &results[1];

    image xfilter = make_gx_filter();
    image yfilter = make_gy_filter();
    image gx = convolve_image(im, xfilter, 0);
    image gy = convolve_image(im, yfilter, 0);

    int channel, row, col;
    for (channel = 0; channel < im.c; channel++) {
        for (row = 0; row < im.h; row++) {
            for (col = 0; col < im.w; col++) {
                float xpix = get_pixel(gx, col, row, channel);
                float ypix = get_pixel(gy, col, row, channel);
                float val = xpix * xpix + ypix * ypix; 
                set_pixel(results[0], col, row, channel, sqrtf(val));
                
                float dir = atan2f(ypix, xpix);
                set_pixel(results[1], col, row, channel, dir);
            }
        }
    }

    return results;
}

// need to change but this has color for some reason
image colorize_sobel(image im)
{
    image *results = calloc(2, sizeof(image));
    results[0] = make_image(im.w, im.h, im.c);
    results[1] = make_image(im.w, im.h, im.c);

  //  image *magnitude = &results[0];
  //  image *direction = &results[1];

    image xfilter = make_gx_filter();
    image yfilter = make_gy_filter();
    image gx = convolve_image(im, xfilter, 1);
    image gy = convolve_image(im, yfilter, 1);

    int channel, row, col;
    for (channel = 0; channel < im.c; channel++) {
        for (row = 0; row < im.h; row++) {
            for (col = 0; col < im.w; col++) {
                float xpix = get_pixel(gx, col, row, channel);
                float ypix = get_pixel(gy, col, row, channel);
                float val = xpix * xpix + ypix * ypix; 
                set_pixel(results[0], col, row, channel, sqrt(val));
                
                float dir = atan2(ypix, xpix);
                set_pixel(results[1], col, row, channel, dir);
            }
        }
    }

    return *results;
}
