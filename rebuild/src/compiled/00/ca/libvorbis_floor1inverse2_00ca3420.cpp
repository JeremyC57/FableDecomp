// libvorbis::floor1_inverse2(vorbis_block* vb, vorbis_look_floor* in, void* memo, float* out) -- libvorbis floor1.c: render the decoded floor-1 curve into the residue vector (piecewise lines between the used posts through FLOOR1_fromdB_LOOKUP, then the last level to the end of the half block); with no curve (memo null) clear the half block and return 0
// Leaf: @floor1_inverse2@16
// Flags: /Gr
// Grades: 3 (RELOCATION_MATCH on every grade; the later two only respelled ogg_int64_t as os_types.h's __int64 and fixed
//   this header comment)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA3420, 347 bytes, `__fastcall` (vb in ecx, look in edx, memo / out on the stack,
//   `ret 8`). libpk's statically linked Xiph libvorbis ("Xiph.Org libVorbis I 20030909", 1.0.1), lib/floor1.c, reached
//   only through floor1_exportbundle's inverse2 slot (no direct caller).
// Identity (proven): the 1.0.1 body verbatim against the binary: vb->vd (+0x40) ->vi (+4) ->codec_setup (+0x1C)
//   ->blocksizes[vb->W (+0x1C)] / 2; look->vi +0x510, look->posts +0x504, &look->forward_index[1] = look + 0x108,
//   info->mult +0x340, info->postlist +0x344; the `& 0x7fff` used-post test; FLOOR1_fromdB_LOOKUP at 0x013ABE28 (256
//   floats from 1.0649863e-07 to 1.0 at 0x013AC224); the memo-null arm is `rep stosd` over n dwords. Manifest label
//   "Geometry_ScaleVertexArray" is wrong.
// Callee: the floor1.c static render_line 0x00CA1E80 (manifest "CurveInterpolation_InterpolateFloats"): its real 1.0.1
//   body is in this TU as `static __declspec(noinline)`, and VC7.1 then gives it the same private convention retail
//   uses (y1 in ecx; x0 / x1 / y0 / d on the stack) -- no /GL needed. Land the leaf only.
// Data: FLOOR1_fromdB_LOOKUP 0x013ABE28 (floor1.c `static float[256]`, declared extern here so the TU stays a leaf model).
// Layout: the 1.0.1 floor1.c / codec_internal.h / codec.h structures (vorbis_look_floor1 0x520 bytes, as the landed
//   floor1_look 0x00CA1BE0 checks).
#define VIF_POSIT 63
#define VIF_CLASS 16
#define VIF_PARTS 31
typedef __int64 ogg_int64_t;                       // ogg/os_types.h (_WIN32)
extern "C" int __cdecl abs(int);
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(abs, memset)

typedef void vorbis_look_floor;
typedef struct { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; } oggpack_buffer;
typedef struct vorbis_info {
    int version; int channels; long rate;
    long bitrate_upper; long bitrate_nominal; long bitrate_lower; long bitrate_window;
    void* codec_setup;                 // +0x1c
} vorbis_info;
typedef struct vorbis_dsp_state { int analysisp; vorbis_info* vi; } vorbis_dsp_state;
typedef struct vorbis_block {
    float** pcm;                       // +0x00
    oggpack_buffer opb;                // +0x04
    long lW; long W; long nW;          // +0x18
    int pcmend; int mode;              // +0x24
    int eofflag;                       // +0x2c
    ogg_int64_t granulepos;            // +0x30
    ogg_int64_t sequence;              // +0x38
    vorbis_dsp_state* vd;              // +0x40
} vorbis_block;
typedef struct codec_setup_info { long blocksizes[2]; } codec_setup_info;

typedef struct {
  int   partitions;
  int   partitionclass[VIF_PARTS];
  int   class_dim[VIF_CLASS];
  int   class_subs[VIF_CLASS];
  int   class_book[VIF_CLASS];
  int   class_subbook[VIF_CLASS][8];
  int   mult;                          // +0x340
  int   postlist[VIF_POSIT+2];         // +0x344
  float maxover; float maxunder; float maxerr; float twofitweight; float twofitatten;
  int   n;
} vorbis_info_floor1;

typedef struct {
  int sorted_index[VIF_POSIT+2];
  int forward_index[VIF_POSIT+2];      // +0x104
  int reverse_index[VIF_POSIT+2];
  int hineighbor[VIF_POSIT];
  int loneighbor[VIF_POSIT];
  int posts;                           // +0x504
  int n;
  int quant_q;
  vorbis_info_floor1 *vi;              // +0x510
  long phrasebits; long postbits; long frames;
} vorbis_look_floor1;

extern "C" {
extern float FLOOR1_fromdB_LOOKUP[256];   // 0x013ABE28

static __declspec(noinline) void render_line(int x0,int x1,int y0,int y1,float *d){
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

int floor1_inverse2(vorbis_block *vb,vorbis_look_floor *in,void *memo,
                          float *out){
  vorbis_look_floor1 *look=(vorbis_look_floor1 *)in;
  vorbis_info_floor1 *info=look->vi;

  codec_setup_info   *ci=(codec_setup_info*)vb->vd->vi->codec_setup;
  int                  n=ci->blocksizes[vb->W]/2;
  int j;

  if(memo){
    /* render the lines */
    int *fit_value=(int *)memo;
    int hx=0;
    int lx=0;
    int ly=fit_value[0]*info->mult;
    for(j=1;j<look->posts;j++){
      int current=look->forward_index[j];
      int hy=fit_value[current]&0x7fff;
      if(hy==fit_value[current]){

        hy*=info->mult;
        hx=info->postlist[current];

        render_line(lx,hx,ly,hy,out);

        lx=hx;
        ly=hy;
      }
    }
    for(j=hx;j<n;j++)out[j]*=FLOOR1_fromdB_LOOKUP[ly]; /* be certain */
    return(1);
  }
  memset(out,0,sizeof(*out)*n);
  return(0);
}
}