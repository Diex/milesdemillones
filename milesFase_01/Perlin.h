#include <math.h>
#include <Arduino.h>

/*
Ken Perlins improved noise   -  http://mrl.nyu.edu/~perlin/noise/
C-port:  http://www.fundza.com/c4serious/noise/perlin/perlin.html
by Malcolm Kesson;   arduino port by Peter Chiochetti, Sep 2007 :
-  make permutation constant byte, obsoletes init(), lookup % 256
*/

static const byte p[] = {   151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96,
                            53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6,
                            148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177,
                            33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139,
                            48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92,
                            41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132,
                            187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198,
                            173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85,
                            212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119,
                            248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253,
                            19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34,
                            242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
                            49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4,
                            150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66,
                            215, 61, 156, 180
                        };

double fade(double t) {
  return t * t * t * (t * (t * 6 - 15) + 10);
}
double lerp(double t, double a, double b) {
  return a + t * (b - a);
}
double grad(int hash, double x, double y, double z)
{
  int     h = hash & 15;          /* CONVERT LO 4 BITS OF HASH CODE */
  double  u = h < 8 ? x : y,      /* INTO 12 GRADIENT DIRECTIONS.   */
          v = h < 4 ? y : h == 12 || h == 14 ? x : z;
  return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

#define P(x) p[(x) & 255]

double pnoise(double x, double y, double z)
{
  int   X = (int)floor(x) & 255,             /* FIND UNIT CUBE THAT */
        Y = (int)floor(y) & 255,             /* CONTAINS POINT.     */
        Z = (int)floor(z) & 255;
  x -= floor(x);                             /* FIND RELATIVE X,Y,Z */
  y -= floor(y);                             /* OF POINT IN CUBE.   */
  z -= floor(z);
  double  u = fade(x),                       /* COMPUTE FADE CURVES */
          v = fade(y),                       /* FOR EACH OF X,Y,Z.  */
          w = fade(z);
  int  A = P(X) + Y,
       AA = P(A) + Z,
       AB = P(A + 1) + Z,                    /* HASH COORDINATES OF */
       B = P(X + 1) + Y,
       BA = P(B) + Z,
       BB = P(B + 1) + Z;                    /* THE 8 CUBE CORNERS, */

  return lerp(w, lerp(v, lerp(u, grad(P(AA  ), x, y, z), /* AND ADD */
                              grad(P(BA  ), x - 1, y, z)), /* BLENDED */
                      lerp(u, grad(P(AB  ), x, y - 1, z),      /* RESULTS */
                           grad(P(BB  ), x - 1, y - 1, z))),   /* FROM  8 */
              lerp(v, lerp(u, grad(P(AA + 1), x, y, z - 1), /* CORNERS */
                           grad(P(BA + 1), x - 1, y, z - 1)),    /* OF CUBE */
                   lerp(u, grad(P(AB + 1), x, y - 1, z - 1),
                        grad(P(BB + 1), x - 1, y - 1, z - 1))));
}




int lut[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05,
0x05, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x0A, 0x0A, 0x0B, 0x0B,
0x0C, 0x0C, 0x0D, 0x0D, 0x0E, 0x0F, 0x0F, 0x10, 0x11, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1F, 0x20, 0x21, 0x23, 0x24, 0x26, 0x27, 0x29, 0x2B, 0x2C,
0x2E, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 0x3E, 0x40, 0x43, 0x45, 0x47, 0x4A, 0x4C, 0x4F,
0x51, 0x54, 0x57, 0x59, 0x5C, 0x5F, 0x62, 0x64, 0x67, 0x6A, 0x6D, 0x70, 0x73, 0x76, 0x79, 0x7C,
0x7F, 0x82, 0x85, 0x88, 0x8B, 0x8E, 0x91, 0x94, 0x97, 0x9A, 0x9C, 0x9F, 0xA2, 0xA5, 0xA7, 0xAA,
0xAD, 0xAF, 0xB2, 0xB4, 0xB7, 0xB9, 0xBB, 0xBE, 0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE,
0xD0, 0xD2, 0xD3, 0xD5, 0xD7, 0xD8, 0xDA, 0xDB, 0xDD, 0xDE, 0xDF, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5,
0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xED, 0xEE, 0xEF, 0xEF, 0xF0, 0xF1, 0xF1, 0xF2,
0xF2, 0xF3, 0xF3, 0xF4, 0xF4, 0xF5, 0xF5, 0xF6, 0xF6, 0xF6, 0xF7, 0xF7, 0xF7, 0xF8, 0xF8, 0xF8,
0xF9, 0xF9, 0xF9, 0xF9, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFC,
0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD,
0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF};


