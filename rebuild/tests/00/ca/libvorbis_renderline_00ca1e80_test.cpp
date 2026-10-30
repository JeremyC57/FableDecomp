// Behaviour test for libvorbis floor1.c render_line (0x00CA1E80): the Bresenham walk over FLOOR1_fromdB_LOOKUP.
#include <cstdio>
#include <cstdlib>

static float FLOOR1_fromdB_LOOKUP[256];

static void render_line(int x0,int x1,int y0,int y1,float *d){
  int dy=y1-y0;
  int adx=x1-x0;
  int ady=abs(dy);
  int base=dy/adx;
  int sy=(dy<0?base-1:base+1);
  int x=x0;
  int y=y0;
  int err=0;

  ady-=abs(base*adx);

  d[x]*=FLOOR1_fromdB_LOOKUP[y];
  while(++x<x1){
    err=err+ady;
    if(err>=adx){
      err-=adx;
      y+=sy;
    }else{
      y+=base;
    }
    d[x]*=FLOOR1_fromdB_LOOKUP[y];
  }
}

static int fail(void) { std::printf("00ca1e80_TEST FAIL\n"); return 1; }

int main()
{
    int i;
    for (i = 0; i < 256; ++i) FLOOR1_fromdB_LOOKUP[i] = (float)i;   // table value = index, so d[x] records y
    float d[32];
    for (i = 0; i < 32; ++i) d[i] = 1.0f;
    // rising line 2..10 from y 0 to 8: slope 1, y == x - 2; x1 itself untouched
    render_line(2, 10, 0, 8, d);
    for (i = 2; i < 10; ++i) if (d[i] != (float)(i - 2)) return fail();
    if (d[10] != 1.0f || d[1] != 1.0f) return fail();
    // falling shallow line 0..8 from y 100 to 96: the integer Bresenham endpoints
    float e[16];
    for (i = 0; i < 16; ++i) e[i] = 1.0f;
    render_line(0, 8, 100, 96, e);
    if (e[0] != 100.0f) return fail();
    for (i = 1; i < 8; ++i) if (e[i] > e[i - 1] || e[i] < 96.0f) return fail();   // monotone non-increasing
    if (e[8] != 1.0f) return fail();
    // steep line 0..2 from y 10 to 30: base 10, y steps 10 then 20
    float f[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    render_line(0, 2, 10, 30, f);
    if (f[0] != 10.0f || f[1] != 20.0f || f[2] != 1.0f) return fail();
    std::printf("00ca1e80_TEST PASS\n");
    return 0;
}