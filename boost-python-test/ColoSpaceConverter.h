#ifndef __COLORSPACECONVERSION_H__
#define __COLORSPACECONVERSION_H__

#include <math.h>

class ColorSpaceConverter
{
public:
	void rgb2xyY(int R, int G, int B, int xyy[3]) {
		//http://www.brucelindbloom.com

		float r, g, b, X, Y, Z;

		// RGB to XYZ
		r = R/255.f; //R 0..1
		g = G/255.f; //G 0..1
		b = B/255.f; //B 0..1

		if (r <= 0.04045)
			r = r/12;
		else
			r = (float) pow((r+0.055)/1.055f,2.4);

		if (g <= 0.04045)
			g = g/12;
		else
			g = (float) pow((g+0.055)/1.055f,2.4);

		if (b <= 0.04045)
			b = b/12;
		else
			b = (float) pow((b+0.055)/1.055f,2.4);

		X =  0.436052025f*r  + 0.385081593f*g  + 0.143087414f *b;
		Y =  0.222491598f*r  + 0.71688606f *g  + 0.060621486f *b;
		Z =  0.013929122f*r  + 0.097097002f*g  + 0.71418547f  *b;

		float x;
		float y;

		float sum = X + Y + Z;
		if (sum != 0) {
			x = X / sum;
			y = Y / sum;
		}
		else {
			float Xr = 0.964221f;  // reference white
			float Yr = 1.0f;
			float Zr = 0.825211f;

			x = Xr / (Xr + Yr + Zr);
			y = Yr / (Xr + Yr + Zr);
		}

		xyy[0] = (int) (255*x + .5);
		xyy[1] = (int) (255*y + .5);
		xyy[2] = (int) (255*Y + .5);

	} 
	void rgb2xyz(int R, int G, int B, int xyz[3]) {
		float r, g, b, X, Y, Z;

		r = R/255.f; //R 0..1
		g = G/255.f; //G 0..1
		b = B/255.f; //B 0..1

		if (r <= 0.04045)
			r = r/12;
		else
			r = (float) pow((r+0.055)/(float)1.055,2.4);

		if (g <= 0.04045)
			g = g/12;
		else
			g = (float) pow((g+0.055)/(float)1.055,2.4);

		if (b <= 0.04045)
			b = b/12;
		else
			b = (float) pow((b+0.055)/(float)1.055,2.4);

		X =  0.436052025f*r     + 0.385081593f*g + 0.143087414f *b;
		Y =  0.222491598f*r     + 0.71688606f *g + 0.060621486f *b;
		Z =  0.013929122f*r     + 0.097097002f*g + 0.71418547f  *b;

		xyz[1] = (int) (255*Y + .5);
		xyz[0] = (int) (255*X + .5); 
		xyz[2] = (int) (255*Z + .5);    
	} 
	void rgb2luv(int R, int G, int B, int luv[3] ) {
		//http://www.brucelindbloom.com

		float r, g, b, X, Y, Z, yr;
		float L;
		float eps = 216.f/24389.f;
		float k = 24389.f/27.f;

		float Xr = 0.964221f;  // reference white D50
		float Yr = 1.0f;
		float Zr = 0.825211f;

		// RGB to XYZ

		r = R/255.f; //R 0..1
		g = G/255.f; //G 0..1
		b = B/255.f; //B 0..1

		// assuming sRGB (D65)
		if (r <= 0.04045)
			r = r/12;
		else
			r = (float) pow((r+0.055)/1.055,2.4);

		if (g <= 0.04045)
			g = g/12;
		else
			g = (float) pow((g+0.055)/1.055,2.4);

		if (b <= 0.04045)
			b = b/12;
		else
			b = (float) pow((b+0.055)/1.055,2.4);


		X =  0.436052025f*r     + 0.385081593f*g + 0.143087414f *b;
		Y =  0.222491598f*r     + 0.71688606f *g + 0.060621486f *b;
		Z =  0.013929122f*r     + 0.097097002f*g + 0.71418547f  *b;

		// XYZ to Luv

		float u, v, u_, v_, ur_, vr_;

		u_ = 4*X / (X + 15*Y + 3*Z);
		v_ = 9*Y / (X + 15*Y + 3*Z);

		ur_ = 4*Xr / (Xr + 15*Yr + 3*Zr);
		vr_ = 9*Yr / (Xr + 15*Yr + 3*Zr);

		yr = Y/Yr;

		if ( yr > eps )
			L =  (float) (116*pow(yr, (float)(1/3.)) - 16);
		else
			L = k * yr;

		u = 13*L*(u_ -ur_);
		v = 13*L*(v_ -vr_);

		luv[0] = (int) (2.55*L + .5);
		luv[1] = (int) (u + .5); 
		luv[2] = (int) (v + .5);        
	} 
	void rgb2lab(int R, int G, int B, int lab[3]) {
		//http://www.brucelindbloom.com

		float r, g, b, X, Y, Z, fx, fy, fz, xr, yr, zr;
		float Ls, as, bs;
		float eps = 216.f/24389.f;
		float k = 24389.f/27.f;

		float Xr = 0.964221f;  // reference white D50
		float Yr = 1.0f;
		float Zr = 0.825211f;

		// RGB to XYZ
		r = R/255.f; //R 0..1
		g = G/255.f; //G 0..1
		b = B/255.f; //B 0..1

		// assuming sRGB (D65)
		if (r <= 0.04045)
			r = r/12;
		else
			r = (float) pow((r+0.055)/1.055f,2.4);

		if (g <= 0.04045)
			g = g/12;
		else
			g = (float) pow((g+0.055)/1.055f,2.4);

		if (b <= 0.04045)
			b = b/12;
		else
			b = (float) pow((b+0.055)/1.055f,2.4);


		X =  0.436052025f*r     + 0.385081593f*g + 0.143087414f *b;
		Y =  0.222491598f*r     + 0.71688606f *g + 0.060621486f *b;
		Z =  0.013929122f*r     + 0.097097002f*g + 0.71418547f  *b;

		// XYZ to Lab
		xr = X/Xr;
		yr = Y/Yr;
		zr = Z/Zr;

		if ( xr > eps )
			fx =  (float) pow(xr, 1/3.f);
		else
			fx = (float) ((k * xr + 16.) / 116.);

		if ( yr > eps )
			fy =  (float) pow(yr, 1/3.f);
		else
			fy = (float) ((k * yr + 16.) / 116.);

		if ( zr > eps )
			fz =  (float) pow(zr, 1/3.f);
		else
			fz = (float) ((k * zr + 16.) / 116);

		Ls = ( 116 * fy ) - 16;
		as = 500*(fx-fy);
		bs = 200*(fy-fz);

		lab[0] = (int) (2.55*Ls + .5);
		lab[1] = (int) (as + .5); 
		lab[2] = (int) (bs + .5);       
	} 
};

#endif