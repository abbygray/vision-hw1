#include <math.h>
#include <stdio.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    return get_pixel(im, round(x), round(y), c);
}

image nn_resize(image im, int w, int h)
{
    image resize = make_image(w, h, im.c);

  //  float a = (-0.5 - (im.w-0.5))/(-0.5 - (w - 0.5));
    float a = (float)im.w/(float) w;
    float a_y = (float)im.h/(float) h;
    float b = -0.5 + 0.5 * a;

  //  float a_y = (-0.5 - (im.h-0.5))/(-0.5 - (h - 0.5));
    float b_y = -0.5 + 0.5 * a_y;

    int row;
    int col;

    for (row = 0; row < resize.h; row++){
        for(col = 0; col < resize.w; col++) {
            float c_pos = a * col + b;
            float r_pos = a_y * row + b_y;

            float red = nn_interpolate(im, c_pos, r_pos, 0);
            set_pixel(resize, col, row, 0, red);

            float green = nn_interpolate(im, c_pos, r_pos, 1);
            set_pixel(resize, col, row, 1, green);

            float blue = nn_interpolate(im, c_pos, r_pos, 2);
            set_pixel(resize, col, row, 2, blue);
       }
       col = 0;
    }


    return resize;
}


int round_down(float x) {

    return (int) x;
}

int round_up(float x) {
    return (int) x + 1;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    int left_x = round_down(x); 
    int right_x = round_up(x);
    int top_y = round_down(y);
    int bottom_y = round_up(y);

    if(x < 0) {
        left_x = -1;
        right_x = 0;
    }

    if(y < 0) {
        top_y = -1;
        bottom_y = 0;
    }

    float v1 = get_pixel(im, left_x, top_y, c);
    float v2 = get_pixel(im, right_x, top_y, c);
    float v3 = get_pixel(im, left_x, bottom_y, c);
    float v4 = get_pixel(im, right_x, bottom_y, c);

    float d1 = x - left_x;
    float d2 = right_x - x;
    float d3 = y - top_y;
    float d4 = bottom_y - y;

    float q1 = v1 * d2 + v2 * d1;
    float q2 = v3 * d2 + v4 * d1;
    float q = q1 * d4 + q2 * d3;

    return q;
}

image bilinear_resize(image im, int w, int h)
{
   image resize = make_image(w, h, im.c);

    float a_x = (float)im.w/(float) w;
    float a_y = (float)im.h/(float) h;
    //float a_x = (-0.5 - (h-0.5))/(-0.5 - (im.h - 0.5));
    float b_x = -0.5 + 0.5 * a_x;

   // float a_y = (-0.5 - (w-0.5))/(-0.5 - (im.w - 0.5));
    float b_y = -0.5 + 0.5 * a_y;

    int row;
    int col;

    for (row = 0; row < h; row++){
        for(col = 0; col < w; col++) {
            float xold = a_x * col + b_x;
            float yold = a_y * row + b_y;

            float red = bilinear_interpolate(im, xold, yold, 0);
            set_pixel(resize, col, row, 0, red);

            float green = bilinear_interpolate(im, xold, yold, 1);
            set_pixel(resize, col, row, 1, green);

            float blue = bilinear_interpolate(im, xold, yold, 2);
            set_pixel(resize, col, row, 2, blue);
       }

       col = 0;
    }


    return resize;
}



