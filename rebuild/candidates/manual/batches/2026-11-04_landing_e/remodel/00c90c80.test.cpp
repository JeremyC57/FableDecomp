// Behaviour test for libvorbis vorbis_window (0x00C90C80): the 1.0.1 block.c body over a minimal dsp state -- NULL when the
// block size has no window (window[W] == 0), otherwise the window of (window[W] - halfrate_flag) from _vorbis_window_get.
#include <cstdio>
#include <cstdlib>
typedef struct { int halfrate_flag; } codec_setup_info;
typedef struct { codec_setup_info* codec_setup; } vorbis_info;
typedef struct { int window[2]; } private_state;
typedef struct { vorbis_info* vi; void* backend_state; } vorbis_dsp_state;
static float g_windows[8][4];
static int g_lastN = -1;
float *_vorbis_window_get(int n){ g_lastN = n; return g_windows[n]; }
float *vorbis_window(vorbis_dsp_state *v,int W){
  vorbis_info *vi=v->vi;
  codec_setup_info *ci=vi->codec_setup;
  int hs=ci->halfrate_flag;
  private_state *b=(private_state*)v->backend_state;
  if(b->window[W]-1<0)return NULL;
  return _vorbis_window_get(b->window[W]-hs);
}
int main(){
  codec_setup_info ci; vorbis_info vi; private_state ps; vorbis_dsp_state v;
  vi.codec_setup=&ci; v.vi=&vi; v.backend_state=&ps;
  int ok=1;
  ci.halfrate_flag=0; ps.window[0]=0; ps.window[1]=5;
  if(vorbis_window(&v,0)!=NULL || g_lastN!=-1) ok=0;            /* no short window: NULL, no lookup */
  if(vorbis_window(&v,1)!=g_windows[5] || g_lastN!=5) ok=0;     /* long window at full rate */
  ci.halfrate_flag=1;
  if(vorbis_window(&v,1)!=g_windows[4] || g_lastN!=4) ok=0;     /* halfrate: one size smaller */
  ps.window[0]=1;
  if(vorbis_window(&v,0)!=g_windows[0] || g_lastN!=0) ok=0;
  printf("00c90c80_TEST %s\n", ok ? "PASS" : "FAIL");
  return ok ? 0 : 1;
}
