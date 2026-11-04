// Behaviour test for libvorbis _ve_envelope_search (0x00C9E280): the 1.0.1 envelope.c body over a scripted _ve_amp --
// the per-step mark array (pre-echo marks j and j+1), current/cursor bookkeeping and the 0 / -1 / 1 decisions.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
#include <malloc.h>
#else
#include <alloca.h>
#endif
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
#include <stdint.h>
typedef int64_t ogg_int64_t;
#endif
typedef int ogg_int32_t;
#define _ogg_realloc realloc
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
typedef struct { int n; int log2n; float *trig; int *bitrev; float scale; } mdct_lookup;
typedef struct { float preecho_thresh[7]; float postecho_thresh[7]; float stretch_penalty; } vorbis_info_psy_global;
typedef struct { long blocksizes[2]; vorbis_info_psy_global psy_g_param; } codec_setup_info;
typedef struct { int channels; codec_setup_info* codec_setup; } vorbis_info;
typedef struct { void* ve; } private_state;
typedef struct { vorbis_info* vi; float** pcm; int pcm_current; long W; long centerW; void* backend_state; } vorbis_dsp_state;
#define VE_PRE    16
#define VE_WIN    4
#define VE_POST   2
#define VE_AMP    (VE_PRE+VE_POST-1)

#define VE_BANDS  7
#define VE_NEARDC 15

#define VE_MINSTRETCH 2   /* a bit less than short block */
#define VE_MAXSTRETCH 12  /* one-third full block */

typedef struct {
  float ampbuf[VE_AMP];
  int   ampptr;

  float nearDC[VE_NEARDC];
  float nearDC_acc;
  float nearDC_partialacc;
  int   nearptr;

} envelope_filter_state;

typedef struct {
  int begin;
  int end;
  float *window;
  float total;
} envelope_band;

typedef struct {
  int ch;
  int winlength;
  int searchstep;
  float minenergy;

  mdct_lookup  mdct;
  float       *mdct_win;

  envelope_band          band[VE_BANDS];
  envelope_filter_state *filter;
  int   stretch;

  int                   *mark;

  long storage;
  long current;
  long curmark;
  long cursor;
} envelope_lookup;
static int g_trig = -1, g_calls = 0;
static int _ve_amp(envelope_lookup*, vorbis_info_psy_global*, float*, envelope_band*, envelope_filter_state*, long pos)
{ g_calls++; return pos == g_trig ? 1 : 0; }
long _ve_envelope_search(vorbis_dsp_state *v){
  vorbis_info *vi=v->vi;
  codec_setup_info *ci=vi->codec_setup;
  vorbis_info_psy_global *gi=&ci->psy_g_param;
  envelope_lookup *ve=(envelope_lookup *)((private_state *)(v->backend_state))->ve;
  long i,j;

  int first=ve->current/ve->searchstep;
  int last=v->pcm_current/ve->searchstep-VE_WIN;
  if(first<0)first=0;

  /* make sure we have enough storage to match the PCM */
  if(last+VE_WIN+VE_POST>ve->storage){
    ve->storage=last+VE_WIN+VE_POST; /* be sure */
    ve->mark=(int*)_ogg_realloc(ve->mark,ve->storage*sizeof(*ve->mark));
  }

  for(j=first;j<last;j++){
    int ret=0;

    ve->stretch++;
    if(ve->stretch>VE_MAXSTRETCH*2)
      ve->stretch=VE_MAXSTRETCH*2;
    
    for(i=0;i<ve->ch;i++){
      float *pcm=v->pcm[i]+ve->searchstep*(j);
      ret|=_ve_amp(ve,gi,pcm,ve->band,ve->filter+i*VE_BANDS,j);
    }

    ve->mark[j+VE_POST]=0;
    if(ret&1){
      ve->mark[j]=1;
      ve->mark[j+1]=1;
    }

    if(ret&2){
      ve->mark[j]=1;
      if(j>0)ve->mark[j-1]=1;
    }

    if(ret&4)ve->stretch=-1;
  }

  ve->current=last*ve->searchstep;

  {
    long centerW=v->centerW;
    long testW=
      centerW+
      ci->blocksizes[v->W]/4+
      ci->blocksizes[1]/2+
      ci->blocksizes[0]/4;
    
    j=ve->cursor;
    
    while(j<ve->current-(ve->searchstep)){/* account for postecho
                                             working back one window */
      if(j>=testW)return(1);
 
      ve->cursor=j;

      if(ve->mark[j/ve->searchstep]){
	if(j>centerW){

	  #if 0
	  if(j>ve->curmark){
	    float *marker=alloca(v->pcm_current*sizeof(*marker));
	    int l,m;
	    memset(marker,0,sizeof(*marker)*v->pcm_current);
	    fprintf(stderr,"mark! seq=%d, cursor:%fs time:%fs\n",
		    seq,
		    (totalshift+ve->cursor)/44100.,
		    (totalshift+j)/44100.);
	    _analysis_output_always("pcmL",seq,v->pcm[0],v->pcm_current,0,0,totalshift);
	    _analysis_output_always("pcmR",seq,v->pcm[1],v->pcm_current,0,0,totalshift);

	    _analysis_output_always("markL",seq,v->pcm[0],j,0,0,totalshift);
	    _analysis_output_always("markR",seq,v->pcm[1],j,0,0,totalshift);
	    
	    for(m=0;m<VE_BANDS;m++){
	      char buf[80];
	      sprintf(buf,"delL%d",m);
	      for(l=0;l<last;l++)marker[l*ve->searchstep]=ve->filter[m].markers[l]*.1;
	      _analysis_output_always(buf,seq,marker,v->pcm_current,0,0,totalshift);
	    }

	    for(m=0;m<VE_BANDS;m++){
	      char buf[80];
	      sprintf(buf,"delR%d",m);
	      for(l=0;l<last;l++)marker[l*ve->searchstep]=ve->filter[m+VE_BANDS].markers[l]*.1;
	      _analysis_output_always(buf,seq,marker,v->pcm_current,0,0,totalshift);
	    }

	    for(l=0;l<last;l++)marker[l*ve->searchstep]=ve->mark[l]*.4;
	    _analysis_output_always("mark",seq,marker,v->pcm_current,0,0,totalshift);
	   
	    
	    seq++;
	    
	  }
#endif

	  ve->curmark=j;
	  if(j>=testW)return(1);
	  return(0);
	}
      }
      j+=ve->searchstep;
    }
  }
  
  return(-1);
}
int main()
{
    codec_setup_info ci; std::memset(&ci, 0, sizeof(ci)); ci.blocksizes[0] = 256; ci.blocksizes[1] = 2048;
    vorbis_info vi = { 1, &ci };
    static envelope_lookup ve; std::memset(&ve, 0, sizeof(ve));
    static envelope_filter_state f[VE_BANDS];
    ve.ch = 1; ve.searchstep = 64; ve.filter = f; ve.storage = 32; ve.mark = (int*)calloc(32, sizeof(int));
    private_state b = { &ve };
    static float pcm0[4096]; float* pcm[1] = { pcm0 };
    vorbis_dsp_state v = { &vi, pcm, 1000, 0, 0, &b };
    int bad = 0;
    g_trig = 5;                                     // steps 0..10 searched (last = 1000/64 - 4 = 11)
    if (_ve_envelope_search(&v) != 0) bad = 1;      // mark at step 5 -> cursor 320 > centerW 0 -> short block
    if (g_calls != 11 || ve.current != 704 || ve.mark[5] != 1 || ve.mark[6] != 1 || ve.cursor != 320 || ve.curmark != 320) bad = 2;
    if (ve.stretch != 11) bad = 3;
    // no new trigger, cursor restarts past the old mark: nothing found before current - searchstep -> -1
    std::memset(ve.mark, 0, 32 * sizeof(int)); g_trig = -1; ve.cursor = 384;
    if (_ve_envelope_search(&v) != -1) bad = 4;
    // pcm far ahead: the scan reaches testW (0 + 256/4 + 2048/2 + 256/4 = 1152) -> 1
    v.pcm_current = 2000; ve.cursor = 1100; ve.storage = 64; ve.mark = (int*)realloc(ve.mark, 64 * sizeof(int)); std::memset(ve.mark, 0, 64 * sizeof(int));
    if (_ve_envelope_search(&v) != 1) bad = 5;
    if (bad) { std::printf("00c9e280_TEST FAIL %d\n", bad); return 1; }
    std::printf("00c9e280_TEST PASS\n"); return 0;
}