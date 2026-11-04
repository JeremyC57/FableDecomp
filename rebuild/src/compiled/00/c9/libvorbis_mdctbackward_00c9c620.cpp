// libvorbis::mdct_backward(mdct_lookup* init, float* in, float* out) -- mdct.c: inverse MDCT (rotate into the upper half, butterflies, bit-reverse, then rotate + window into the full n-sample output with the mirrored halves)
// Leaf: @mdct_backward@12
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9C620, 588 bytes, LTCG __fastcall (init in ecx, in in edx, out on the stack, `ret 4`).
// Identity (two sources): (1) bound `@mdct_backward@12` already (the landed mapping0_inverse 0x00C9F8C0 calls it per
//   channel with b->transform[vb->W][0]); (2) the body: n2 = n>>1 / n4 = n>>2 offsets, the two rotate loops over
//   init->trig+n4 (iX = in+n2-7 / in+n2-8, stepping 8), mdct_butterflies(init,out+n2,n2) 0x00C9C400 and
//   mdct_bitreverse(init,out) 0x00C9C500 (calls at 0x00C9C715 / 0x00C9C71E), then the rotate+window and two mirror loops --
//   mdct.c mdct_backward verbatim, right after mdct_bitreverse in file order. Catalog label "Mesh_ComputeDetailLOD" is junk.
// Library: libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909") lib/mdct.c inside libpk; no PDB/RTTI, so the names are the
//   libvorbis ones. Bodies are the 1.0.1 mdct.c text verbatim (gitlab.xiph.org xiph/vorbis tag v1.0.1).
// TU: the landed mdct.c statics (mdct_butterfly_8/16/32, _first, _generic, mdct_butterflies, mdct_bitreverse, from the landed
//   0x00C9C400 / 0x00C9C500 rows) as same-TU `static __declspec(noinline)` bodies, then mdct_backward 0x00C9C620 and
//   mdct_forward 0x00C9C870 in source order; VC7.1 gives the statics retail's private conventions (mdct_butterflies:
//   init in eax, x/points on the stack, callee-pop; mdct_bitreverse: init in eax, x in edx). Only the Leaf row is graded.
// Original libvorbis mdct.c; BSD-style terms in reference COPYING.
typedef float DATA_TYPE;
typedef float REG_TYPE;
#define cPI3_8 .38268343236508977175f
#define cPI2_8 .70710678118654752441f
#define cPI1_8 .92387953251128675613f
#define HALVE(x) ((x)*.5f)
extern "C" void* __cdecl _alloca(unsigned int size);
#pragma intrinsic(_alloca)
#define alloca _alloca
typedef struct mdct_lookup { int n; int log2n; DATA_TYPE* trig; int* bitrev; DATA_TYPE scale; } mdct_lookup;
#define MULT_NORM(x) (x)
extern "C" {
static __declspec(noinline) void mdct_butterfly_8(DATA_TYPE* x)
{
    REG_TYPE r0 = x[6] + x[2];
    REG_TYPE r1 = x[6] - x[2];
    REG_TYPE r2 = x[4] + x[0];
    REG_TYPE r3 = x[4] - x[0];

    x[6] = r0 + r2;
    x[4] = r0 - r2;

    r0 = x[5] - x[1];
    r2 = x[7] - x[3];
    x[0] = r1 + r0;
    x[2] = r1 - r0;

    r0 = x[5] + x[1];
    r1 = x[7] + x[3];
    x[3] = r2 + r3;
    x[1] = r2 - r3;
    x[7] = r1 + r0;
    x[5] = r1 - r0;
}

// Same-TU caller: mdct.c mdct_butterfly_16 (retail 0x00C9BF30, calls 0x00C9BFEB / tail jump 0x00C9BFF2), present so the
// static gets retail's private convention. Not graded here.
static __declspec(noinline) void mdct_butterfly_16(DATA_TYPE* x)
{
    REG_TYPE r0 = x[1] - x[9];
    REG_TYPE r1 = x[0] - x[8];

    x[8] += x[0];
    x[9] += x[1];
    x[0] = MULT_NORM((r0 + r1) * cPI2_8);
    x[1] = MULT_NORM((r0 - r1) * cPI2_8);

    r0 = x[3] - x[11];
    r1 = x[10] - x[2];
    x[10] += x[2];
    x[11] += x[3];
    x[2] = r0;
    x[3] = r1;

    r0 = x[12] - x[4];
    r1 = x[13] - x[5];
    x[12] += x[4];
    x[13] += x[5];
    x[4] = MULT_NORM((r0 - r1) * cPI2_8);
    x[5] = MULT_NORM((r0 + r1) * cPI2_8);

    r0 = x[14] - x[6];
    r1 = x[15] - x[7];
    x[14] += x[6];
    x[15] += x[7];
    x[6] = r0;
    x[7] = r1;

    mdct_butterfly_8(x);
    mdct_butterfly_8(x + 8);
}

static __declspec(noinline) void mdct_butterfly_32(DATA_TYPE* x)
{
    REG_TYPE r0 = x[30] - x[14];
    REG_TYPE r1 = x[31] - x[15];

    x[30] += x[14];
    x[31] += x[15];
    x[14] = MULT_NORM(r0 * cPI2_8 + r1 * cPI2_8);
    x[15] = MULT_NORM(r1 * cPI2_8 - r0 * cPI2_8);

    r0 = x[28] - x[12];
    r1 = x[29] - x[13];
    x[28] += x[12];
    x[29] += x[13];
    x[12] = MULT_NORM(r0 * cPI1_8 - r1 * cPI3_8);
    x[13] = MULT_NORM(r0 * cPI3_8 + r1 * cPI1_8);

    r0 = x[26] - x[10];
    r1 = x[27] - x[11];
    x[26] += x[10];
    x[27] += x[11];
    x[10] = MULT_NORM((r0 - r1) * cPI2_8);
    x[11] = MULT_NORM((r0 + r1) * cPI2_8);

    r0 = x[24] - x[8];
    r1 = x[25] - x[9];
    x[24] += x[8];
    x[25] += x[9];
    x[8] = MULT_NORM(r0 * cPI3_8 - r1 * cPI1_8);
    x[9] = MULT_NORM(r1 * cPI3_8 + r0 * cPI1_8);

    r0 = x[22] - x[6];
    r1 = x[7] - x[23];
    x[22] += x[6];
    x[23] += x[7];
    x[6] = r1;
    x[7] = r0;

    r0 = x[4] - x[20];
    r1 = x[5] - x[21];
    x[20] += x[4];
    x[21] += x[5];
    x[4] = MULT_NORM(r1 * cPI1_8 + r0 * cPI3_8);
    x[5] = MULT_NORM(r1 * cPI3_8 - r0 * cPI1_8);

    r0 = x[2] - x[18];
    r1 = x[3] - x[19];
    x[18] += x[2];
    x[19] += x[3];
    x[2] = MULT_NORM((r1 + r0) * cPI2_8);
    x[3] = MULT_NORM((r1 - r0) * cPI2_8);

    r0 = x[0] - x[16];
    r1 = x[1] - x[17];
    x[16] += x[0];
    x[17] += x[1];
    x[0] = MULT_NORM(r1 * cPI3_8 + r0 * cPI1_8);
    x[1] = MULT_NORM(r1 * cPI1_8 - r0 * cPI3_8);

    mdct_butterfly_16(x);
    mdct_butterfly_16(x + 16);
}

static __declspec(noinline) void mdct_butterfly_first(DATA_TYPE* T, DATA_TYPE* x, int points)
{
    DATA_TYPE* x1 = x + points - 8;
    DATA_TYPE* x2 = x + (points >> 1) - 8;
    REG_TYPE r0;
    REG_TYPE r1;

    do {
        r0 = x1[6] - x2[6];
        r1 = x1[7] - x2[7];
        x1[6] += x2[6];
        x1[7] += x2[7];
        x2[6] = MULT_NORM(r1 * T[1] + r0 * T[0]);
        x2[7] = MULT_NORM(r1 * T[0] - r0 * T[1]);

        r0 = x1[4] - x2[4];
        r1 = x1[5] - x2[5];
        x1[4] += x2[4];
        x1[5] += x2[5];
        x2[4] = MULT_NORM(r1 * T[5] + r0 * T[4]);
        x2[5] = MULT_NORM(r1 * T[4] - r0 * T[5]);

        r0 = x1[2] - x2[2];
        r1 = x1[3] - x2[3];
        x1[2] += x2[2];
        x1[3] += x2[3];
        x2[2] = MULT_NORM(r1 * T[9] + r0 * T[8]);
        x2[3] = MULT_NORM(r1 * T[8] - r0 * T[9]);

        r0 = x1[0] - x2[0];
        r1 = x1[1] - x2[1];
        x1[0] += x2[0];
        x1[1] += x2[1];
        x2[0] = MULT_NORM(r1 * T[13] + r0 * T[12]);
        x2[1] = MULT_NORM(r1 * T[12] - r0 * T[13]);

        x1 -= 8;
        x2 -= 8;
        T += 16;

    } while (x2 >= x);
}

static __declspec(noinline) void __stdcall mdct_butterfly_generic(DATA_TYPE* T, DATA_TYPE* x, int points, int trigint)
{
    DATA_TYPE* x1 = x + points - 8;
    DATA_TYPE* x2 = x + (points >> 1) - 8;
    REG_TYPE r0;
    REG_TYPE r1;

    do {
        r0 = x1[6] - x2[6];
        r1 = x1[7] - x2[7];
        x1[6] += x2[6];
        x1[7] += x2[7];
        x2[6] = MULT_NORM(r1 * T[1] + r0 * T[0]);
        x2[7] = MULT_NORM(r1 * T[0] - r0 * T[1]);

        T += trigint;

        r0 = x1[4] - x2[4];
        r1 = x1[5] - x2[5];
        x1[4] += x2[4];
        x1[5] += x2[5];
        x2[4] = MULT_NORM(r1 * T[1] + r0 * T[0]);
        x2[5] = MULT_NORM(r1 * T[0] - r0 * T[1]);

        T += trigint;

        r0 = x1[2] - x2[2];
        r1 = x1[3] - x2[3];
        x1[2] += x2[2];
        x1[3] += x2[3];
        x2[2] = MULT_NORM(r1 * T[1] + r0 * T[0]);
        x2[3] = MULT_NORM(r1 * T[0] - r0 * T[1]);

        T += trigint;

        r0 = x1[0] - x2[0];
        r1 = x1[1] - x2[1];
        x1[0] += x2[0];
        x1[1] += x2[1];
        x2[0] = MULT_NORM(r1 * T[1] + r0 * T[0]);
        x2[1] = MULT_NORM(r1 * T[0] - r0 * T[1]);

        T += trigint;
        x1 -= 8;
        x2 -= 8;

    } while (x2 >= x);
}

static __declspec(noinline) void __stdcall mdct_butterflies(mdct_lookup* init, DATA_TYPE* x, int points)
{
    DATA_TYPE* T = init->trig;
    int stages = init->log2n - 5;
    int i, j;

    if (--stages > 0) {
        mdct_butterfly_first(T, x, points);
    }

    for (i = 1; --stages > 0; i++) {
        for (j = 0; j < (1 << i); j++)
            mdct_butterfly_generic(T, x + (points >> i) * j, points >> i, 4 << i);
    }

    for (j = 0; j < points; j += 32)
        mdct_butterfly_32(x + j);
}

static __declspec(noinline) void mdct_bitreverse(mdct_lookup* init, DATA_TYPE* x)
{
    int        n   = init->n;
    int*       bit = init->bitrev;
    DATA_TYPE* w0  = x;
    DATA_TYPE* w1  = x = w0 + (n >> 1);
    DATA_TYPE* T   = init->trig + n;

    do {
        DATA_TYPE* x0 = x + bit[0];
        DATA_TYPE* x1 = x + bit[1];

        REG_TYPE r0 = x0[1] - x1[1];
        REG_TYPE r1 = x0[0] + x1[0];
        REG_TYPE r2 = MULT_NORM(r1 * T[0] + r0 * T[1]);
        REG_TYPE r3 = MULT_NORM(r1 * T[1] - r0 * T[0]);

        w1 -= 4;

        r0 = HALVE(x0[1] + x1[1]);
        r1 = HALVE(x0[0] - x1[0]);

        w0[0] = r0 + r2;
        w1[2] = r0 - r2;
        w0[1] = r1 + r3;
        w1[3] = r3 - r1;

        x0 = x + bit[2];
        x1 = x + bit[3];

        r0 = x0[1] - x1[1];
        r1 = x0[0] + x1[0];
        r2 = MULT_NORM(r1 * T[2] + r0 * T[3]);
        r3 = MULT_NORM(r1 * T[3] - r0 * T[2]);

        r0 = HALVE(x0[1] + x1[1]);
        r1 = HALVE(x0[0] - x1[0]);

        w0[2] = r0 + r2;
        w1[0] = r0 - r2;
        w0[3] = r1 + r3;
        w1[1] = r3 - r1;

        T   += 4;
        bit += 4;
        w0  += 4;

    } while (w0 < w1);
}

void __fastcall mdct_backward(mdct_lookup *init, DATA_TYPE *in, DATA_TYPE *out){
  int n=init->n;
  int n2=n>>1;
  int n4=n>>2;

  /* rotate */

  DATA_TYPE *iX = in+n2-7;
  DATA_TYPE *oX = out+n2+n4;
  DATA_TYPE *T  = init->trig+n4;

  do{
    oX         -= 4;
    oX[0]       = MULT_NORM(-iX[2] * T[3] - iX[0]  * T[2]);
    oX[1]       = MULT_NORM (iX[0] * T[3] - iX[2]  * T[2]);
    oX[2]       = MULT_NORM(-iX[6] * T[1] - iX[4]  * T[0]);
    oX[3]       = MULT_NORM (iX[4] * T[1] - iX[6]  * T[0]);
    iX         -= 8;
    T          += 4;
  }while(iX>=in);

  iX            = in+n2-8;
  oX            = out+n2+n4;
  T             = init->trig+n4;

  do{
    T          -= 4;
    oX[0]       =  MULT_NORM (iX[4] * T[3] + iX[6] * T[2]);
    oX[1]       =  MULT_NORM (iX[4] * T[2] - iX[6] * T[3]);
    oX[2]       =  MULT_NORM (iX[0] * T[1] + iX[2] * T[0]);
    oX[3]       =  MULT_NORM (iX[0] * T[0] - iX[2] * T[1]);
    iX         -= 8;
    oX         += 4;
  }while(iX>=in);

  mdct_butterflies(init,out+n2,n2);
  mdct_bitreverse(init,out);

  /* roatate + window */

  {
    DATA_TYPE *oX1=out+n2+n4;
    DATA_TYPE *oX2=out+n2+n4;
    DATA_TYPE *iX =out;
    T             =init->trig+n2;
    
    do{
      oX1-=4;

      oX1[3]  =  MULT_NORM (iX[0] * T[1] - iX[1] * T[0]);
      oX2[0]  = -MULT_NORM (iX[0] * T[0] + iX[1] * T[1]);

      oX1[2]  =  MULT_NORM (iX[2] * T[3] - iX[3] * T[2]);
      oX2[1]  = -MULT_NORM (iX[2] * T[2] + iX[3] * T[3]);

      oX1[1]  =  MULT_NORM (iX[4] * T[5] - iX[5] * T[4]);
      oX2[2]  = -MULT_NORM (iX[4] * T[4] + iX[5] * T[5]);

      oX1[0]  =  MULT_NORM (iX[6] * T[7] - iX[7] * T[6]);
      oX2[3]  = -MULT_NORM (iX[6] * T[6] + iX[7] * T[7]);

      oX2+=4;
      iX    +=   8;
      T     +=   8;
    }while(iX<oX1);

    iX=out+n2+n4;
    oX1=out+n4;
    oX2=oX1;

    do{
      oX1-=4;
      iX-=4;

      oX2[0] = -(oX1[3] = iX[3]);
      oX2[1] = -(oX1[2] = iX[2]);
      oX2[2] = -(oX1[1] = iX[1]);
      oX2[3] = -(oX1[0] = iX[0]);

      oX2+=4;
    }while(oX2<iX);

    iX=out+n2+n4;
    oX1=out+n2+n4;
    oX2=out+n2;
    do{
      oX1-=4;
      oX1[0]= iX[3];
      oX1[1]= iX[2];
      oX1[2]= iX[1];
      oX1[3]= iX[0];
      iX+=4;
    }while(oX1>oX2);
  }
}

void __fastcall mdct_forward(mdct_lookup *init, DATA_TYPE *in, DATA_TYPE *out){
  int n=init->n;
  int n2=n>>1;
  int n4=n>>2;
  int n8=n>>3;
  DATA_TYPE *w=(DATA_TYPE*)alloca(n*sizeof(*w)); /* forward needs working space */
  DATA_TYPE *w2=w+n2;

  /* rotate */

  /* window + rotate + step 1 */
  
  REG_TYPE r0;
  REG_TYPE r1;
  DATA_TYPE *x0=in+n2+n4;
  DATA_TYPE *x1=x0+1;
  DATA_TYPE *T=init->trig+n2;
  
  int i=0;
  
  for(i=0;i<n8;i+=2){
    x0 -=4;
    T-=2;
    r0= x0[2] + x1[0];
    r1= x0[0] + x1[2];       
    w2[i]=   MULT_NORM(r1*T[1] + r0*T[0]);
    w2[i+1]= MULT_NORM(r1*T[0] - r0*T[1]);
    x1 +=4;
  }

  x1=in+1;
  
  for(;i<n2-n8;i+=2){
    T-=2;
    x0 -=4;
    r0= x0[2] - x1[0];
    r1= x0[0] - x1[2];       
    w2[i]=   MULT_NORM(r1*T[1] + r0*T[0]);
    w2[i+1]= MULT_NORM(r1*T[0] - r0*T[1]);
    x1 +=4;
  }
    
  x0=in+n;

  for(;i<n2;i+=2){
    T-=2;
    x0 -=4;
    r0= -x0[2] - x1[0];
    r1= -x0[0] - x1[2];       
    w2[i]=   MULT_NORM(r1*T[1] + r0*T[0]);
    w2[i+1]= MULT_NORM(r1*T[0] - r0*T[1]);
    x1 +=4;
  }


  mdct_butterflies(init,w+n2,n2);
  mdct_bitreverse(init,w);

  /* roatate + window */

  T=init->trig+n2;
  x0=out+n2;

  for(i=0;i<n4;i++){
    x0--;
    out[i] =MULT_NORM((w[0]*T[0]+w[1]*T[1])*init->scale);
    x0[0]  =MULT_NORM((w[0]*T[1]-w[1]*T[0])*init->scale);
    w+=2;
    T+=2;
  }
}


}   // extern "C"