#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

// Abigail gray, agray3
// Cse 455 18 sp

float get_pixel(image im, int x, int y, int c)
{
    if ( x < 0) {
        x = 0;
    } else if (x >= im.w)  {
        x = im.w - 1; 
    }

    if(y < 0) {
        y = 0;
    } else if (y >= im.h) {
        y = im.h - 1;
    }

    if (c < 0) {
        c = 0;
    } else if (c >= im.c) {
        c = im.c - 1;
    }

    int pixel = x + y * im.w + c * im.w * im.h;
 
    return im.data[pixel];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // if(x < 0 || x >= im.w || y < 0 || y >= im.h || c < 0 || c >= im.c) {
    //     return;
    // }

     if ( x < 0) {
        x = 0;
    } else if (x >= im.w)  {
        x = im.w - 1; 
    }

    if(y < 0) {
        y = 0;
    } else if (y >= im.h) {
        y = im.h - 1;
    }

    if (c < 0) {
        c = 0;
    } else if (c >= im.c) {
        c = im.c - 1;
    }

    int pixel = x + y * im.w + c * im.w * im.h;
    im.data[pixel] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
   
    int numpix = im.w * im.h * im.c;
    int i;

    for (i = 0; i < numpix; i++) {
        copy.data[i] = im.data[i];
    }

    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
  
    int rows;
    int cols;

    for (rows = 0; rows < im.h; rows++){
        for(cols = 0; cols < im.w; cols++) {
            float red = get_pixel(im, cols, rows, 0);
            float green = get_pixel(im, cols, rows, 1);
            float blue = get_pixel(im, cols, rows, 2);

            float v = 0.299 * red + 0.587 * green + 0.114 * blue;
            set_pixel(gray, cols, rows, 0, v);
        }
        cols = 0;
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // add v to every pixel in channel c
    int row_start = c * im.w * im.h;
    int numpix = im.w * im.h;

    for (int i = 0; i < numpix; i++) {
        im.data[row_start + i] += v;
    }

}

void scale_image(image im, int c, float v) {
    int row_start = c * im.w * im.h;
    int numpix = im.w * im.h;

    for (int i = 0; i < numpix; i++) {
        im.data[row_start + i] *= v;
    }
}

void clamp_image(image im)
{
    int numpix = im.w * im.h * im.c;
    int i;

    for (i = 0; i < numpix; i++) {
        if(im.data[i] < 0) {
            im.data[i] = 0.0;
        } else if(im.data[i] > 1) {
            im.data[i] = 1.0;
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    assert(im.c == 3);
    int row;
    int col;

    for (row = 0; row < im.h; row++){
        for(col = 0; col < im.w; col++) {
            float red = get_pixel(im, col, row, 0);
            float green = get_pixel(im, col, row, 1);
            float blue = get_pixel(im, col, row, 2);

            // value
            float val = three_way_max(red, green, blue);
            set_pixel(im, col, row, 2, val);

            // saturation
            float min = three_way_min(red, green, blue);
            float chroma = val - min;
            float sat = chroma / val;
            set_pixel(im, col, row, 1, sat);

            // hue
           
           float hue = 0.0;
           if (val == red) {
                hue = (green - blue) / chroma;
            } else if(val == green) {
                hue = (blue - red) / chroma + 2.0;
            } else if(val == blue) {
                hue = (red - green)/ chroma + 4.0;
            }
        
            if (hue < 0 ){
                hue = hue/6.0 + 1.0;
            } else {
                hue = hue/6.0;
            }

             set_pixel(im, col, row, 0, hue);
          
        }
        col = 0;
    }
}

void hsv_to_rgb(image im)
{
    assert(im.c == 3);
    int row;
    int col;

    for (row = 0; row < im.h; row++){
        for(col = 0; col < im.w; col++) {
            float H1 = get_pixel(im, col, row, 0);
            float S = get_pixel(im, col, row, 1);
            float V = get_pixel(im, col, row, 2);

            float H = H1 * 6.0;
            float C = V * S;
            float X = C * (1 - fabs(fmod(H, 2.0) - 1));

            float rgb[3]; 

             if(0 <= H && H <= 1.0) {
                rgb[0] = C;
                rgb[1] = X;
                rgb[2] = 0.0;
            } else if (1.0 <= H && H <= 2.0){
                rgb[0] = X;
                rgb[1] = C;
                rgb[2] = 0.0;
            } else if (2.0 <= H && H <= 3.0) {
                rgb[0] = 0.0;
                rgb[1] = C;
                rgb[2] = X;
            } else if (3.0 <= H && H <= 4.0) {
                rgb[0] = 0.0;
                rgb[1] = X;
                rgb[2] = C;
            } else if (4.0 <= H && H <= 5.0) {
                rgb[0] = X;
                rgb[1] = 0.0;
                rgb[2] = C;
            } else if (5.0 <= H && H <= 6.0) {
                rgb[0] = C;
                rgb[1] = 0.0;
                rgb[2] = X;
            } else {
                rgb[0] = 0.0;
                rgb[1] = 0.0;
                rgb[2] = 0.0;
            }

            float m = V - C;

            float red = rgb[0] + m;
            set_pixel(im, col, row, 0, red);

            float green = rgb[1] + m;
            set_pixel(im, col, row, 1, green);

            float blue = rgb[2] + m;
            set_pixel(im, col, row, 2, blue);
        }
        col = 0; 
    }
}
