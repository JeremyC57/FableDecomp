// libvorbis::_ve_envelope_search(vorbis_dsp_state* v) -- envelope.c encoder: run _ve_amp over every new search step of every channel, mark pre/post-echo triggers, then scan the marks from the cursor to decide whether the next block must be short (1), long (0) or needs more data (-1)
// Leaf: @_ve_envelope_search@4
// Flags: /TC /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9E280, 541 bytes, LTCG __fastcall (v in ecx, plain `ret`).
// Identity (two sources): (1) bound `@_ve_envelope_search@4` already; it calls realloc (import 0x01440054) for ve->mark and
//   _ve_amp 0x00C9DD60 per channel; (2) the body: first = current/searchstep, last = pcm_current/searchstep - VE_WIN(4),
//   storage = last+VE_WIN+VE_POST(2), stretch clamped to VE_MAXSTRETCH*2 (24), mark[j+VE_POST]=0 and the ret&1/2/4 marks,
//   testW = centerW + blocksizes[W]/4 + blocksizes[1]/2 + blocksizes[0]/4, the cursor scan returning 1 / 0 / -1 --
//   envelope.c _ve_envelope_search verbatim, followed by _ve_envelope_mark 0x00C9E4A0. Catalog label "Mesh_ResizeDetail" is junk.
// Library: libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909") lib/envelope.c compiled as C (/TC /Gr) inside libpk; no
//   PDB/RTTI, so the names are the libvorbis ones. Bodies are the 1.0.1 envelope.c / envelope.h / scales.h text verbatim
//   (gitlab.xiph.org xiph/vorbis tag v1.0.1); min/max are VC <stdlib.h>'s macros, realloc the MSVCR71 import 0x01440054.
// TU: envelope.c's todB copy 0x00C9DD40 (scales.h static, pre-union form as the landed row) and _ve_amp as same-TU
//   `static __declspec(noinline)` bodies under `auto_inline(off)`, then _ve_envelope_search; VC7.1 gives the statics
//   retail's private conventions (todB: x in eax; _ve_amp: ve in eax, the other five arguments on the stack, callee-pop
//   `ret 14h`). Structures: engine/Vorbis.h plus envelope.h's envelope_lookup / envelope_band / envelope_filter_state.
// Original libvorbis envelope.c; BSD-style terms in reference COPYING.
#include "engine/Vorbis.h"
void * __cdecl _alloca(unsigned int size);
#pragma intrinsic(_alloca)
#define alloca _alloca
__declspec(dllimport) void * __cdecl realloc(void *block, unsigned int size);               /* IAT 0x01440054 */
#define _ogg_realloc realloc
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
typedef int ogg_int32_t;

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

struct envelope_lookup {
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
};

void mdct_forward(mdct_lookup *init, DATA_TYPE *in, DATA_TYPE *out);                   /* 0x00C9C870 */

#pragma auto_inline(off)
static __declspec(noinline) float todB(const float *x){                                /* 0x00C9DD40 (scales.h) */
  ogg_int32_t i=*(ogg_int32_t *)x;
  i=i&0x7fffffff;
  return (float)(i * 7.17711438e-7f -764.6161886f);
}

/* fairly straight threshhold-by-band based until we need something
   more complex */
static __declspec(noinline) int _ve_amp(envelope_lookup *ve,
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
  float *vec=alloca(n*sizeof(*vec));

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
      int p,this=filters[j].ampptr;
      float postmax,postmin,premax=-99999.f,premin=99999.f;
      
      p=this;
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
      filters[j].ampbuf[this]=acc;
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
#pragma auto_inline(on)

long _ve_envelope_search(vorbis_dsp_state *v){
  vorbis_info *vi=v->vi;
  codec_setup_info *ci=vi->codec_setup;
  vorbis_info_psy_global *gi=&ci->psy_g_param;
  envelope_lookup *ve=((private_state *)(v->backend_state))->ve;
  long i,j;

  int first=ve->current/ve->searchstep;
  int last=v->pcm_current/ve->searchstep-VE_WIN;
  if(first<0)first=0;

  /* make sure we have enough storage to match the PCM */
  if(last+VE_WIN+VE_POST>ve->storage){
    ve->storage=last+VE_WIN+VE_POST; /* be sure */
    ve->mark=_ogg_realloc(ve->mark,ve->storage*sizeof(*ve->mark));
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