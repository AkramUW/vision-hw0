#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    if (x >= im.w){
        x = im.w - 1;
    } else if (x < 0){
        x = 0;
    }
    if (y >= im.h){
        y = im.h - 1;
    } else if (y < 0){
        y = 0;
    }

    if (c >= im.c){
        c = im.c - 1;
    } else if (c < 0){
        c = 0;
    }

    return im.data[x + y * im.w + c*im.w*im.h];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if ((x >= im.w ) || (x < 0 ) || (y >= im.h ) || (y < 0 ) || (c >= im.c ) || (c < 0 )){
        return ;
    } 
    im.data[x + y * im.w + c*im.w*im.h] = v;
    
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    memcpy(&copy, &im, sizeof(image));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);

    for(int i = 0; i < im.w; i++){
        for(int j = 0; j < im.h; j++){
            float y_prime = 0.299 * get_pixel(im, i, j, 0) + 0.587 * get_pixel(im, i, j, 1) + 0.114 * get_pixel(im, i, j, 2);
           set_pixel(gray, i, j, 0, y_prime) ;
        }
    }

    return gray;
}

void shift_image(image im, int c, float v)
{
    for(int i = 0; i < im.w ; i++){
        for (int j = 0; j < im.h; j++){
            //float curr_val = get_pixel(im,i,j,c);
            set_pixel(im, i, j, c, get_pixel(im,i,j,c) + v);
        }
    }   
}

void scale_image(image im, int c, float v)
{
    for(int i = 0; i < im.w ; i++){
        for (int j = 0; j < im.h; j++){
            float curr_val = get_pixel(im,i,j,c);
            set_pixel(im, i, j, c, curr_val*v);
        }
    }   
}

void clamp_image(image im)
{
    for(int i = 0; i < im.w ; i++){
        for (int j = 0; j < im.h; j++){
            for (int k = 0; k < im.c; k++){
                float curr_val = get_pixel(im,i,j,k);
                if (curr_val < 0){ set_pixel(im, i, j,k, 0);}
                if (curr_val > 1){ set_pixel(im, i, j,k, 1);}    
            }       
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
    for(int i = 0; i < im.w ; i++){
        for (int j = 0; j < im.h; j++){
            float v, m, c, s, H_, H, R, G, B;
            R = get_pixel(im, i, j, 0);
            G = get_pixel(im, i, j, 1);
            B = get_pixel(im, i, j, 2);

            v = three_way_max(R,G,B);
            m = three_way_min(R,G,B);
            c = v - m;
            if (v != 0){s = c / v;} else { s = 0;}
            if (c == 0){H_ = 0;}
            else if (v == R){H_ = (G - B) / c;}
            else if (v == G){H_ = ((B - R)/c) + 2;}
            else if (v == B){H_ = ((B - G)/c) + 4;}

            if (H_ < 0) { H = (H_ / 6) + 1;} else {H = H_ / 6;}

            set_pixel(im,i,j,0, H);
            set_pixel(im,i,j,1, s);
            set_pixel(im,i,j,2, v);
        }
    }
}

void hsv_to_rgb(image im)
{
    for(int i = 0; i < im.w ; i++){
        for (int j = 0; j < im.h; j++){
            float V, S, H, maxRGB, minRGB, H_, rgbRange, x1, x2, R, G, B;
            H = get_pixel(im, i, j, 0);
            S = get_pixel(im, i, j, 1);
            V = get_pixel(im, i, j, 2);
            maxRGB = V;
            rgbRange = S * V;
            minRGB = V - rgbRange;
            H_ = 6 * H;
            x1 = fmodf(H_, 1.0);
            x2 = 1.0 - fmodf(H_, 1.0);
            

            if ((H_>= 0 ) && (H_ < 1)){R = V; G = (x1 * rgbRange) + minRGB; B = minRGB;}
            else if ((H_>= 1 ) && (H_ < 2)){R = (x2 * rgbRange) + minRGB; G = maxRGB ; B = minRGB;}
            else if ((H_>= 2 ) && (H_ < 3)){R = minRGB; G = maxRGB; B = maxRGB;}
            else if ((H_>= 3 ) && (H_ < 4)){R = minRGB; G = (x2 * rgbRange) + minRGB; B = maxRGB;}
            else if ((H_>= 4 ) && (H_ < 5)){R = (x1 * rgbRange) + minRGB; G  = minRGB; B = maxRGB;}
            else if ((H_>= 5 ) && (H_ < 6)){R = maxRGB; G = minRGB; B = (x2 * rgbRange) + minRGB;}
            else {R = 0; G = 0; B = 0;}

            set_pixel(im, i,j,0,R);
            set_pixel(im, i,j,1,G);
            set_pixel(im, i,j,2,B);

            

        }
    }
    
}
