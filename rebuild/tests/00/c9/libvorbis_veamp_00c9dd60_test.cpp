// Behaviour test for libvorbis _ve_amp (0x00C9DD60): the 1.0.1 envelope.c body (scales.h todB, mdct_forward mocked as a
// copy) on a quiet-then-loud signal -- the post-echo bit (2) for a falling band, the pre-echo bits (1|4) for a rising one,
// and the per-band amplitude ring / near-DC ring bookkeeping.
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
static void mdct_forward(mdct_lookup*, float* in, float* out) { (void)in; (void)out; }
static float todB(const float *x){
  ogg_int32_t i=*(ogg_int32_t *)x;
  i=i&0x7fffffff;
  return (float)(i * 7.17711438e-7f -764.6161886f);
}
static int _ve_amp(envelope_lookup *ve,
		   vorbis_info_psy_global *gi,
		   float *data,
		   envelope_band *bands,
		   envelope_filter_state *filters,
		   long pos){
  long n=ve->winlength;
  int ret=0;
  long i,j;
  float decay;

  /* we want to have a 'minimum bar' for energy, else we're just
     basing blocks on quantization noise that outweighs the signal
     itself (for low power signals) */

  float minV=ve->minenergy;
  float *vec=(float*)alloca(n*sizeof(*vec));

  /* stretch is used to gradually lengthen the number of windows
     considered prevoius-to-potential-trigger */
  int stretch=max(VE_MINSTRETCH,ve->stretch/2);
  float penalty=gi->stretch_penalty-(ve->stretch/2-VE_MINSTRETCH);
  if(penalty<0.f)penalty=0.f;
  if(penalty>gi->stretch_penalty)penalty=gi->stretch_penalty;
  
  /*_analysis_output_always("lpcm",seq2,data,n,0,0,
    totalshift+pos*ve->searchstep);*/
  
 /* window and transform */
  for(i=0;i<n;i++)
    vec[i]=data[i]*ve->mdct_win[i];
  mdct_forward(&ve->mdct,vec,vec);
  
  /*_analysis_output_always("mdct",seq2,vec,n/2,0,1,0); */

  /* near-DC spreading function; this has nothing to do with
     psychoacoustics, just sidelobe leakage and window size */
  {
    float temp=vec[0]*vec[0]+.7*vec[1]*vec[1]+.2*vec[2]*vec[2];
    int ptr=filters->nearptr;

    /* the accumulation is regularly refreshed from scratch to avoid
       floating point creep */
    if(ptr==0){
      decay=filters->nearDC_acc=filters->nearDC_partialacc+temp;
      filters->nearDC_partialacc=temp;
    }else{
      decay=filters->nearDC_acc+=temp;
      filters->nearDC_partialacc+=temp;
    }
    filters->nearDC_acc-=filters->nearDC[ptr];
    filters->nearDC[ptr]=temp;

    decay*=(1./(VE_NEARDC+1));
    filters->nearptr++;
    if(filters->nearptr>=VE_NEARDC)filters->nearptr=0;
    decay=todB(&decay)*.5-15.f;
  }
  
  /* perform spreading and limiting, also smooth the spectrum.  yes,
     the MDCT results in all real coefficients, but it still *behaves*
     like real/imaginary pairs */
  for(i=0;i<n/2;i+=2){
    float val=vec[i]*vec[i]+vec[i+1]*vec[i+1];
    val=todB(&val)*.5f;
    if(val<decay)val=decay;
    if(val<minV)val=minV;
    vec[i>>1]=val;
    decay-=8.;
  }

  /*_analysis_output_always("spread",seq2++,vec,n/4,0,0,0);*/
  
  /* perform preecho/postecho triggering by band */
  for(j=0;j<VE_BANDS;j++){
    float acc=0.;
    float valmax,valmin;

    /* accumulate amplitude */
    for(i=0;i<bands[j].end;i++)
      acc+=vec[i+bands[j].begin]*bands[j].window[i];
   
    acc*=bands[j].total;

    /* convert amplitude to delta */
    {
      int p,this_=filters[j].ampptr;
      float postmax,postmin,premax=-99999.f,premin=99999.f;
      
      p=this_;
      p--;
      if(p<0)p+=VE_AMP;
      postmax=max(acc,filters[j].ampbuf[p]);
      postmin=min(acc,filters[j].ampbuf[p]);
      
      for(i=0;i<stretch;i++){
	p--;
	if(p<0)p+=VE_AMP;
	premax=max(premax,filters[j].ampbuf[p]);
	premin=min(premin,filters[j].ampbuf[p]);
      }
      
      valmin=postmin-premin;
      valmax=postmax-premax;

      /*filters[j].markers[pos]=valmax;*/
      filters[j].ampbuf[this_]=acc;
      filters[j].ampptr++;
      if(filters[j].ampptr>=VE_AMP)filters[j].ampptr=0;
    }

    /* look at min/max, decide trigger */
    if(valmax>gi->preecho_thresh[j]+penalty){
      ret|=1;
      ret|=4;
    }
    if(valmin<gi->postecho_thresh[j]-penalty)ret|=2;
  }
 
  return(ret);
}
int main()
{
    static envelope_lookup ve; std::memset(&ve, 0, sizeof(ve));
    static float win[16]; for (int i = 0; i < 16; i++) win[i] = 1.f;
    static float bw[1] = { 1.f };
    ve.winlength = 16; ve.minenergy = -100.f; ve.mdct_win = win; ve.stretch = 0;
    for (int j = 0; j < VE_BANDS; j++) { ve.band[j].begin = 0; ve.band[j].end = 1; ve.band[j].window = bw; ve.band[j].total = 1.f; }
    static envelope_filter_state f[VE_BANDS]; std::memset(f, 0, sizeof(f));
    vorbis_info_psy_global gi; for (int j = 0; j < 7; j++) { gi.preecho_thresh[j] = 5.f; gi.postecho_thresh[j] = -5.f; } gi.stretch_penalty = 0.f;
    float quiet[16] = { 0 }, loud[16]; for (int i = 0; i < 16; i++) loud[i] = 1000.f;
    int bad = 0;
    if (_ve_amp(&ve, &gi, quiet, ve.band, f, 0) != 2) bad = 1;          // 0 -> -100 dB floor: post-echo
    if (f[0].ampptr != 1 || f[0].ampbuf[0] != -100.f || f[0].nearptr != 1) bad = 2;
    if (_ve_amp(&ve, &gi, quiet, ve.band, f, 1) != 2) bad = 3;
    if (_ve_amp(&ve, &gi, loud, ve.band, f, 2) != 5) bad = 4;           // jump up: pre-echo (1) and stretch reset (4)
    if (f[3].ampptr != 3 || !(f[3].ampbuf[2] > 50.f)) bad = 5;
    if (bad) { std::printf("00c9dd60_TEST FAIL %d\n", bad); return 1; }
    std::printf("00c9dd60_TEST PASS\n"); return 0;
}