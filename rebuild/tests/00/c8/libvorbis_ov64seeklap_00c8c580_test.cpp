// Behaviour test for libvorbis vorbisfile.c _ov_64_seek_lap (0x00C8C580): OV_EINVAL below OPENED, the init/seek/prime
// errors returned as they come; otherwise the lapping data of the old link (n1 = blocksize >> (1 + halfrate) samples per
// channel) is saved, the seeker runs, and _ov_splice cross-fades it into the new link's first block with the squared
// window of the shorter block (channels the old link lacks are windowed from zero). alloca is a bump allocator here.
#include <cstdio>
#include <cmath>
#if defined(_MSC_VER)
typedef __int64 ogg_int64_t;
#else
typedef long long ogg_int64_t;
#endif
#define NULL 0
#define OV_EINVAL -131
#define OPENED 2
static char g_arena[4096]; static unsigned int g_used = 0;
static void* alloca_(unsigned int n) { void* p = g_arena + g_used; g_used += (n + 7) & ~7u; return p; }
struct vorbis_info { int version; int channels; long rate; int blocksize; };
struct vorbis_dsp_state { int dummy; };
struct OggVorbis_File { int ready_state; vorbis_info* vi; vorbis_dsp_state vd; };
static int g_initset = 0, g_initprime = 0, g_seekResult = 0, g_hs = 0;
static ogg_int64_t g_seekPos = -1;
static vorbis_info g_vi1, g_vi2; static vorbis_info* g_current = &g_vi1;
static float g_w1[8], g_w2[8];
static float g_pcmData[3][8]; static float* g_pcm[3] = { g_pcmData[0], g_pcmData[1], g_pcmData[2] };
static int _ov_initset(OggVorbis_File*) { return g_initset; }
static int _ov_initprime(OggVorbis_File*) { return g_initprime; }
static vorbis_info* ov_info(OggVorbis_File*, int) { return g_current; }
static int ov_halfrate_p(OggVorbis_File*) { return g_hs; }
static int vorbis_info_blocksize(vorbis_info* vi, int) { return vi->blocksize; }
static float* vorbis_window(vorbis_dsp_state*, int) { return g_current == &g_vi1 ? g_w1 : g_w2; }
static int vorbis_synthesis_lapout(vorbis_dsp_state*, float*** pcm) { *pcm = g_pcm; return 8; }
static void _ov_getlap(OggVorbis_File*, vorbis_info* vi, vorbis_dsp_state*, float** lappcm, int lapsize)
{
    for (int c = 0; c < vi->channels; ++c) for (int i = 0; i < lapsize; ++i) lappcm[c][i] = 10.0f * (c + 1);
}
static int seeker(OggVorbis_File*, ogg_int64_t pos) { g_seekPos = pos; g_current = &g_vi2; return g_seekResult; }
static void _ov_splice(float **pcm,float **lappcm,
                       int n1, int n2,
                       int ch1, int ch2,
                       float *w1, float *w2){
  int i,j;
  float *w=w1;
  int n=n1;
  if(n1>n2){
    n=n2;
    w=w2;
  }
  for(j=0;j<ch1 && j<ch2;j++){
    float *s=lappcm[j];
    float *d=pcm[j];
    for(i=0;i<n;i++){
      float wd=w[i]*w[i];
      float ws=1.-wd;
      d[i]=d[i]*wd + s[i]*ws;
    }
  }
  for(;j<ch2;j++){
    float *d=pcm[j];
    for(i=0;i<n;i++){
      float wd=w[i]*w[i];
      d[i]=d[i]*wd;
    }
  }
}
static int _ov_64_seek_lap(OggVorbis_File *vf,ogg_int64_t pos,
                           int (*localseek)(OggVorbis_File *,ogg_int64_t)){
  vorbis_info *vi;
  float **lappcm;
  float **pcm;
  float *w1,*w2;
  int n1,n2,ch1,ch2,hs;
  int i,ret;
  if(vf->ready_state<OPENED)return(OV_EINVAL);
  ret=_ov_initset(vf);
  if(ret)return(ret);
  vi=ov_info(vf,-1);
  hs=ov_halfrate_p(vf);
  ch1=vi->channels;
  n1=vorbis_info_blocksize(vi,0)>>(1+hs);
  w1=vorbis_window(&vf->vd,0);
  lappcm=(float **)alloca_(sizeof(*lappcm)*ch1);
  for(i=0;i<ch1;i++)
    lappcm[i]=(float *)alloca_(sizeof(**lappcm)*n1);
  _ov_getlap(vf,vi,&vf->vd,lappcm,n1);
  ret=localseek(vf,pos);
  if(ret)return ret;
  ret=_ov_initprime(vf);
  if(ret)return(ret);
  vi=ov_info(vf,-1);
  ch2=vi->channels;
  n2=vorbis_info_blocksize(vi,0)>>(1+hs);
  w2=vorbis_window(&vf->vd,0);
  vorbis_synthesis_lapout(&vf->vd,&pcm);
  _ov_splice(pcm,lappcm,n1,n2,ch1,ch2,w1,w2);
  return(0);
}
static int fail(const char* why) { std::printf("00c8c580_TEST FAIL: %s\n", why); return 1; }
int main()
{
    OggVorbis_File vf; vf.ready_state = 1;
    if (_ov_64_seek_lap(&vf, 5, seeker) != OV_EINVAL || g_seekPos != -1) return fail("not opened");
    vf.ready_state = 4; g_initset = -3;
    if (_ov_64_seek_lap(&vf, 5, seeker) != -3 || g_seekPos != -1) return fail("initset error");
    g_initset = 0; g_seekResult = -7;
    if (_ov_64_seek_lap(&vf, 5, seeker) != -7 || g_seekPos != 5) return fail("seek error");
    g_seekResult = 0; g_initprime = -2; g_current = &g_vi1;
    g_vi1.channels = 1; g_vi1.blocksize = 16;
    if (_ov_64_seek_lap(&vf, 6, seeker) != -2) return fail("prime error");
    // the splice: old link 2 channels / 16-sample blocks (n1 = 8), new link 3 channels / 8-sample blocks (n2 = 4)
    g_initprime = 0; g_current = &g_vi1; g_vi1.channels = 2; g_vi1.blocksize = 16; g_vi2.channels = 3; g_vi2.blocksize = 8;
    for (int i = 0; i < 8; ++i) { g_w1[i] = 0.25f; g_w2[i] = 0.5f; }
    for (int c = 0; c < 3; ++c) for (int i = 0; i < 8; ++i) g_pcmData[c][i] = 2.0f;
    if (_ov_64_seek_lap(&vf, 1234, seeker) != 0 || g_seekPos != 1234) return fail("full path");
    // n = n2 = 4 with w2 = 0.5: wd = 0.25; d = 2 * 0.25 + s * 0.75
    for (int i = 0; i < 4; ++i) {
        if (std::fabs(g_pcmData[0][i] - (0.5f + 7.5f)) > 1e-4f || std::fabs(g_pcmData[1][i] - (0.5f + 15.0f)) > 1e-4f
            || std::fabs(g_pcmData[2][i] - 0.5f) > 1e-4f) return fail("splice values");
    }
    if (g_pcmData[0][4] != 2.0f || g_pcmData[2][7] != 2.0f) return fail("splice length");
    // halfrate halves both lap lengths: n1 = 16 >> 2 = 4, n2 = 8 >> 2 = 2; w = w2 as n1 > n2
    g_hs = 1; g_current = &g_vi1;
    for (int c = 0; c < 3; ++c) for (int i = 0; i < 8; ++i) g_pcmData[c][i] = 2.0f;
    if (_ov_64_seek_lap(&vf, 1, seeker) != 0 || g_pcmData[0][1] == 2.0f || g_pcmData[0][2] != 2.0f) return fail("halfrate");
    std::printf("00c8c580_TEST PASS\n");
    return 0;
}