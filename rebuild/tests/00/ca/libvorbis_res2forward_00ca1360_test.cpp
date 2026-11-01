// Behaviour test for 0x00CA1360 res2_forward (libvorbis res0.c): the res2 body over stand-ins for _vorbis_block_alloc and
// _01forward (the stand-in "codes" each work sample by rounding it, so sofar gets pcm - round(pcm) added).
#include <cstdio>
#include <cmath>
#include <cstdlib>
struct vorbis_block { int pcmend; };
typedef void vorbis_look_residue;
static float g_pool[256];
static int g_calls, g_used, g_ch;
static void* _vorbis_block_alloc(vorbis_block*, long bytes) { return bytes <= (long)sizeof(g_pool) ? g_pool : 0; }
static int _encodepart(float*, int) { return 0; }
static int _01forward(vorbis_block* vb, vorbis_look_residue*, float** in, int ch, long**, int (*)(float*, int))
{
    g_calls++; g_ch = ch;
    int n = vb->pcmend / 2 * g_used;
    for (int i = 0; i < n; i++) in[0][i] = (float)std::floor(in[0][i] + 0.5f);
    return 7;
}
static int res2_forward(vorbis_block *vb,vorbis_look_residue *vl,
		 float **in,float **out,int *nonzero,int ch,
		 long **partword){
  long i,j,k,n=vb->pcmend/2,used=0;
  float *work=(float*)_vorbis_block_alloc(vb,ch*n*sizeof(*work));
  for(i=0;i<ch;i++){
    float *pcm=in[i];
    if(nonzero[i])used++;
    for(j=0,k=i;j<n;j++,k+=ch)
      work[k]=pcm[j];
  }
  if(used){
    long ret=_01forward(vb,vl,&work,1,partword,_encodepart);
    if(out){
      for(i=0;i<ch;i++){
	float *pcm=in[i];
	float *sofar=out[i];
	for(j=0,k=i;j<n;j++,k+=ch)
	  sofar[j]+=pcm[j]-work[k];
      }
    }
    return(ret);
  }else{
    return(0);
  }
}
int main()
{
    vorbis_block vb; vb.pcmend = 14;           // n = 7
    float a[7], b[7], sa[7], sb[7];
    for (int j = 0; j < 7; j++) { a[j] = j * 0.7f; b[j] = -j * 1.3f; sa[j] = 1.0f; sb[j] = 2.0f; }
    float* in[2] = { a, b }; float* out[2] = { sa, sb }; int nz[2] = { 0, 0 };
    if (res2_forward(&vb, 0, in, out, nz, 2, 0) != 0 || g_calls != 0) { std::printf("00ca1360_TEST FAIL\n"); return 1; }
    nz[1] = 1; g_used = 2;
    if (res2_forward(&vb, 0, in, out, nz, 2, 0) != 7 || g_calls != 1 || g_ch != 1) { std::printf("00ca1360_TEST FAIL\n"); return 1; }
    for (int j = 0; j < 7; j++) {
        if (g_pool[2 * j] != (float)std::floor(a[j] + 0.5f) || g_pool[2 * j + 1] != (float)std::floor(b[j] + 0.5f)) { std::printf("00ca1360_TEST FAIL\n"); return 1; }
        if (std::fabs(sa[j] - (1.0f + a[j] - g_pool[2 * j])) > 1e-5f || std::fabs(sb[j] - (2.0f + b[j] - g_pool[2 * j + 1])) > 1e-5f) { std::printf("00ca1360_TEST FAIL\n"); return 1; }
    }
    std::printf("00ca1360_TEST PASS\n");
    return 0;
}