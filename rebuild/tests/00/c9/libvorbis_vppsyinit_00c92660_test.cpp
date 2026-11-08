// Behaviour test for libvorbis psy.c _vp_psy_init (retail 0x00C92660): a copy of the psychoacoustic lookup setup (with the curve
// helpers it calls) over synthetic ATH / tonemasks tables -- the plain fields, shiftoc / firstoc / total_octave_lines, the
// interpolated ATH, the octave-line and bark-window tables (monotone, windows around each bin), the tone curves and the
// per-curve noise offsets blended from the info's noiseoff bands.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#define P_BANDS 17      /* 62Hz to 16kHz */
#define P_LEVELS 8      /* 30dB to 100dB */
#define P_LEVEL_0 30.   /* 30 dB */
#define P_NOISECURVES 3
#define NOISE_COMPAND_LEVELS 40
#define MAX_ATH 88
#define EHMER_OFFSET 16
#define EHMER_MAX 56
#define VE_BANDS 7
#define PACKETBLOBS 15
#define toOC(n)     (log(n)*1.442695f-5.965784f)
#define fromOC(o)   (exp(((o)+5.965784f)*.693147f))
#define toBARK(n)   (13.1f*atan(.00074f*(n))+2.24f*atan((n)*(n)*1.85e-8f)+1e-4f*(n))
#define rint(x)   (floor((x)+0.5f))
#define alloca(n) malloc(n)
#define _ogg_malloc malloc

typedef struct vorbis_info_psy{
  int   blockflag;

  float ath_adjatt;
  float ath_maxatt;

  float tone_masteratt[P_NOISECURVES];
  float tone_centerboost;
  float tone_decay;
  float tone_abs_limit;
  float toneatt[P_BANDS];

  int noisemaskp;
  float noisemaxsupp;
  float noisewindowlo;
  float noisewindowhi;
  int   noisewindowlomin;
  int   noisewindowhimin;
  int   noisewindowfixed;
  float noiseoff[P_NOISECURVES][P_BANDS];
  float noisecompand[NOISE_COMPAND_LEVELS];

  float max_curve_dB;

  int normal_channel_p;
  int normal_point_p;
  int normal_start;
  int normal_partition;
  double normal_thresh;
} vorbis_info_psy;

typedef struct{
  int   eighth_octave_lines;

  /* for block long/short tuning; encode only */
  float preecho_thresh[VE_BANDS];
  float postecho_thresh[VE_BANDS];
  float stretch_penalty;
  float preecho_minenergy;

  float ampmax_att_per_sec;

  /* channel coupling config */
  int   coupling_pkHz[PACKETBLOBS];  
  int   coupling_pointlimit[2][PACKETBLOBS];  
  int   coupling_prepointamp[PACKETBLOBS];  
  int   coupling_postpointamp[PACKETBLOBS];  
  int   sliding_lowpass[2][PACKETBLOBS];  

} vorbis_info_psy_global;

typedef struct {
  int n;
  struct vorbis_info_psy *vi;

  float ***tonecurves;
  float **noiseoffset;

  float *ath;
  long  *octave;             /* in n.ocshift format */
  long  *bark;

  long  firstoc;
  long  shiftoc;
  int   eighth_octave_lines; /* power of two, please */
  int   total_octave_lines;  
  long  rate; /* cache it */
} vorbis_look_psy;


/* masking.h tables (psy.c file statics; .data of the libpk libvorbis block) */
float ATH[MAX_ATH];
float tonemasks[P_BANDS][6][EHMER_MAX];

static void min_curve(float *c,
		       float *c2){
  int i;  
  for(i=0;i<EHMER_MAX;i++)if(c2[i]<c[i])c[i]=c2[i];
}
static void max_curve(float *c,
		       float *c2){
  int i;  
  for(i=0;i<EHMER_MAX;i++)if(c2[i]>c[i])c[i]=c2[i];
}

static void attenuate_curve(float *c,float att){
  int i;
  for(i=0;i<EHMER_MAX;i++)
    c[i]+=att;
}

static float ***setup_tone_curves(float curveatt_dB[P_BANDS],float binHz,int n,
				  float center_boost, float center_decay_rate){
  int i,j,k,m;
  float ath[EHMER_MAX];
  float workc[P_BANDS][P_LEVELS][EHMER_MAX];
  float athc[P_LEVELS][EHMER_MAX];
  float *brute_buffer=(float*)alloca(n*sizeof(*brute_buffer));

  float ***ret=(float***)_ogg_malloc(sizeof(*ret)*P_BANDS);

  memset(workc,0,sizeof(workc));

  for(i=0;i<P_BANDS;i++){
    /* we add back in the ATH to avoid low level curves falling off to
       -infinity and unnecessarily cutting off high level curves in the
       curve limiting (last step). */

    /* A half-band's settings must be valid over the whole band, and
       it's better to mask too little than too much */  
    int ath_offset=i*4;
    for(j=0;j<EHMER_MAX;j++){
      float min=999.;
      for(k=0;k<4;k++)
	if(j+k+ath_offset<MAX_ATH){
	  if(min>ATH[j+k+ath_offset])min=ATH[j+k+ath_offset];
	}else{
	  if(min>ATH[MAX_ATH-1])min=ATH[MAX_ATH-1];
	}
      ath[j]=min;
    }

    /* copy curves into working space, replicate the 50dB curve to 30
       and 40, replicate the 100dB curve to 110 */
    for(j=0;j<6;j++)
      memcpy(workc[i][j+2],tonemasks[i][j],EHMER_MAX*sizeof(*tonemasks[i][j]));
    memcpy(workc[i][0],tonemasks[i][0],EHMER_MAX*sizeof(*tonemasks[i][0]));
    memcpy(workc[i][1],tonemasks[i][0],EHMER_MAX*sizeof(*tonemasks[i][0]));
    
    /* apply centered curve boost/decay */
    for(j=0;j<P_LEVELS;j++){
      for(k=0;k<EHMER_MAX;k++){
	float adj=center_boost+abs(EHMER_OFFSET-k)*center_decay_rate;
	if(adj<0. && center_boost>0)adj=0.;
	if(adj>0. && center_boost<0)adj=0.;
	workc[i][j][k]+=adj;
      }
    }

    /* normalize curves so the driving amplitude is 0dB */
    /* make temp curves with the ATH overlayed */
    for(j=0;j<P_LEVELS;j++){
      attenuate_curve(workc[i][j],curveatt_dB[i]+100.-(j<2?2:j)*10.-P_LEVEL_0);
      memcpy(athc[j],ath,EHMER_MAX*sizeof(**athc));
      attenuate_curve(athc[j],+100.-j*10.f-P_LEVEL_0);
      max_curve(athc[j],workc[i][j]);
    }

    /* Now limit the louder curves.
       
       the idea is this: We don't know what the playback attenuation
       will be; 0dB SL moves every time the user twiddles the volume
       knob. So that means we have to use a single 'most pessimal' curve
       for all masking amplitudes, right?  Wrong.  The *loudest* sound
       can be in (we assume) a range of ...+100dB] SL.  However, sounds
       20dB down will be in a range ...+80], 40dB down is from ...+60],
       etc... */
    
    for(j=1;j<P_LEVELS;j++){
      min_curve(athc[j],athc[j-1]);
      min_curve(workc[i][j],athc[j]);
    }
  }

  for(i=0;i<P_BANDS;i++){
    int hi_curve,lo_curve,bin;
    ret[i]=(float**)_ogg_malloc(sizeof(**ret)*P_LEVELS);

    /* low frequency curves are measured with greater resolution than
       the MDCT/FFT will actually give us; we want the curve applied
       to the tone data to be pessimistic and thus apply the minimum
       masking possible for a given bin.  That means that a single bin
       could span more than one octave and that the curve will be a
       composite of multiple octaves.  It also may mean that a single
       bin may span > an eighth of an octave and that the eighth
       octave values may also be composited. */
    
    /* which octave curves will we be compositing? */
    bin=floor(fromOC(i*.5)/binHz);
    lo_curve=  ceil(toOC(bin*binHz+1)*2);
    hi_curve=  floor(toOC((bin+1)*binHz)*2);
    if(lo_curve>i)lo_curve=i;
    if(lo_curve<0)lo_curve=0;
    if(hi_curve>=P_BANDS)hi_curve=P_BANDS-1;

    for(m=0;m<P_LEVELS;m++){
      ret[i][m]=(float*)_ogg_malloc(sizeof(***ret)*(EHMER_MAX+2));
      
      for(j=0;j<n;j++)brute_buffer[j]=999.;
      
      /* render the curve into bins, then pull values back into curve.
	 The point is that any inherent subsampling aliasing results in
	 a safe minimum */
      for(k=lo_curve;k<=hi_curve;k++){
	int l=0;

	for(j=0;j<EHMER_MAX;j++){
	  int lo_bin= fromOC(j*.125+k*.5-2.0625)/binHz;
	  int hi_bin= fromOC(j*.125+k*.5-1.9375)/binHz+1;
	  
	  if(lo_bin<0)lo_bin=0;
	  if(lo_bin>n)lo_bin=n;
	  if(lo_bin<l)l=lo_bin;
	  if(hi_bin<0)hi_bin=0;
	  if(hi_bin>n)hi_bin=n;

	  for(;l<hi_bin && l<n;l++)
	    if(brute_buffer[l]>workc[k][m][j])
	      brute_buffer[l]=workc[k][m][j];
	}

	for(;l<n;l++)
	  if(brute_buffer[l]>workc[k][m][EHMER_MAX-1])
	    brute_buffer[l]=workc[k][m][EHMER_MAX-1];

      }

      /* be equally paranoid about being valid up to next half ocatve */
      if(i+1<P_BANDS){
	int l=0;
	k=i+1;
	for(j=0;j<EHMER_MAX;j++){
	  int lo_bin= fromOC(j*.125+i*.5-2.0625)/binHz;
	  int hi_bin= fromOC(j*.125+i*.5-1.9375)/binHz+1;
	  
	  if(lo_bin<0)lo_bin=0;
	  if(lo_bin>n)lo_bin=n;
	  if(lo_bin<l)l=lo_bin;
	  if(hi_bin<0)hi_bin=0;
	  if(hi_bin>n)hi_bin=n;

	  for(;l<hi_bin && l<n;l++)
	    if(brute_buffer[l]>workc[k][m][j])
	      brute_buffer[l]=workc[k][m][j];
	}

	for(;l<n;l++)
	  if(brute_buffer[l]>workc[k][m][EHMER_MAX-1])
	    brute_buffer[l]=workc[k][m][EHMER_MAX-1];

      }


      for(j=0;j<EHMER_MAX;j++){
	int bin=fromOC(j*.125+i*.5-2.)/binHz;
	if(bin<0){
	  ret[i][m][j+2]=-999.;
	}else{
	  if(bin>=n){
	    ret[i][m][j+2]=-999.;
	  }else{
	    ret[i][m][j+2]=brute_buffer[bin];
	  }
	}
      }

      /* add fenceposts */
      for(j=0;j<EHMER_OFFSET;j++)
	if(ret[i][m][j+2]>-200.f)break;  
      ret[i][m][0]=j;
      
      for(j=EHMER_MAX-1;j>EHMER_OFFSET+1;j--)
	if(ret[i][m][j+2]>-200.f)
	  break;
      ret[i][m][1]=j;

    }
  }

  return(ret);
}

static void _vp_psy_init(vorbis_look_psy *p,vorbis_info_psy *vi,
		  vorbis_info_psy_global *gi,int n,long rate){
  long i,j,lo=-99,hi=1;
  long maxoc;
  memset(p,0,sizeof(*p));

  p->eighth_octave_lines=gi->eighth_octave_lines;
  p->shiftoc=rint(log(gi->eighth_octave_lines*8.f)/log(2.f))-1;

  p->firstoc=toOC(.25f*rate*.5/n)*(1<<(p->shiftoc+1))-gi->eighth_octave_lines;
  maxoc=toOC((n+.25f)*rate*.5/n)*(1<<(p->shiftoc+1))+.5f;
  p->total_octave_lines=maxoc-p->firstoc+1;
  p->ath=(float*)_ogg_malloc(n*sizeof(*p->ath));

  p->octave=(long*)_ogg_malloc(n*sizeof(*p->octave));
  p->bark=(long*)_ogg_malloc(n*sizeof(*p->bark));
  p->vi=vi;
  p->n=n;
  p->rate=rate;

  /* set up the lookups for a given blocksize and sample rate */

  for(i=0,j=0;i<MAX_ATH-1;i++){
    int endpos=rint(fromOC((i+1)*.125-2.)*2*n/rate);
    float base=ATH[i];
    if(j<endpos){
      float delta=(ATH[i+1]-base)/(endpos-j);
      for(;j<endpos && j<n;j++){
        p->ath[j]=base+100.;
        base+=delta;
      }
    }
  }

  for(i=0;i<n;i++){
    float bark=toBARK(rate/(2*n)*i); 

    for(;lo+vi->noisewindowlomin<i && 
	  toBARK(rate/(2*n)*lo)<(bark-vi->noisewindowlo);lo++);
    
    for(;hi<=n && (hi<i+vi->noisewindowhimin ||
	  toBARK(rate/(2*n)*hi)<(bark+vi->noisewindowhi));hi++);
    
    p->bark[i]=((lo-1)<<16)+(hi-1);

  }

  for(i=0;i<n;i++)
    p->octave[i]=toOC((i+.25f)*.5*rate/n)*(1<<(p->shiftoc+1))+.5f;

  p->tonecurves=setup_tone_curves(vi->toneatt,rate*.5/n,n,
				  vi->tone_centerboost,vi->tone_decay);
  
  /* set up rolling noise median */
  p->noiseoffset=(float**)_ogg_malloc(P_NOISECURVES*sizeof(*p->noiseoffset));
  for(i=0;i<P_NOISECURVES;i++)
    p->noiseoffset[i]=(float*)_ogg_malloc(n*sizeof(**p->noiseoffset));
  
  for(i=0;i<n;i++){
    float halfoc=toOC((i+.5)*rate/(2.*n))*2.;
    int inthalfoc;
    float del;
    
    if(halfoc<0)halfoc=0;
    if(halfoc>=P_BANDS-1)halfoc=P_BANDS-1;
    inthalfoc=(int)halfoc;
    del=halfoc-inthalfoc;
    
    for(j=0;j<P_NOISECURVES;j++)
      p->noiseoffset[j][i]=
	p->vi->noiseoff[j][inthalfoc]*(1.-del) + 
	p->vi->noiseoff[j][inthalfoc+1]*del;
    
  }
}


static int fail(const char* what) { std::printf("%s\n00c92660_TEST FAIL\n", what); return 1; }

int main()
{
    int i, j, b;
    vorbis_info_psy vi; vorbis_info_psy_global gi; vorbis_look_psy p;
    const int n = 1024; const long rate = 44100;
    for (i = 0; i < MAX_ATH; i++) ATH[i] = -20.f + 0.25f * i;
    for (b = 0; b < P_BANDS; b++) for (j = 0; j < 6; j++) for (i = 0; i < EHMER_MAX; i++)
        tonemasks[b][j][i] = -60.f + 10.f * j - (float)std::abs(i - EHMER_OFFSET);
    memset(&vi, 0, sizeof vi); memset(&gi, 0, sizeof gi);
    gi.eighth_octave_lines = 2;
    vi.noisewindowlo = .15f; vi.noisewindowhi = .15f; vi.noisewindowlomin = 0; vi.noisewindowhimin = 0; vi.noisewindowfixed = 0;
    vi.tone_centerboost = 0.f; vi.tone_decay = 0.f;
    for (i = 0; i < P_BANDS; i++) { vi.toneatt[i] = -2.f; for (j = 0; j < P_NOISECURVES; j++) vi.noiseoff[j][i] = (float)(10 * j + i); }

    _vp_psy_init(&p, &vi, &gi, n, rate);

    if (p.n != n || p.rate != rate || p.vi != &vi || p.eighth_octave_lines != 2) return fail("plain fields");
    /* shiftoc = rint(log2(16)) - 1 = 3; firstoc / total_octave_lines follow psy.c's formulas */
    if (p.shiftoc != 3) return fail("shiftoc");
    {
        long firstoc = (long)(toOC(.25f * rate * .5 / n) * (1 << (p.shiftoc + 1)) - gi.eighth_octave_lines);
        long maxoc = (long)(toOC((n + .25f) * rate * .5 / n) * (1 << (p.shiftoc + 1)) + .5f);
        if (p.firstoc != firstoc || p.total_octave_lines != maxoc - firstoc + 1) return fail("octave range");
    }
    /* ath: ATH + 100 interpolated along the frequency axis; non-decreasing with the synthetic rising table */
    if (!p.ath) return fail("ath alloc");
    for (i = 1; i < n; i++) if (p.ath[i] < p.ath[i - 1] - 1e-3f) return fail("ath monotone");
    if (p.ath[n / 2] < 79.f || p.ath[n / 2] > 103.f) return fail("ath level");
    /* octave lines: p->octave[i] = toOC((i+.25)*.5*rate/n) * 2^(shiftoc+1) + .5, non-decreasing */
    for (i = 1; i < n; i++) if (p.octave[i] < p.octave[i - 1]) return fail("octave monotone");
    if (p.octave[1] != (long)(toOC((1 + .25f) * .5 * rate / n) * (1 << (p.shiftoc + 1)) + .5f)) return fail("octave value");
    /* bark windows: (lo-1)<<16 | (hi-1) with lo <= i < hi and both non-decreasing */
    for (i = 0; i < n; i++) {
        long lo = (p.bark[i] >> 16) + 1, hi = (p.bark[i] & 0xffff) + 1;
        if (lo > i || hi <= i || hi > n + 1) return fail("bark window");
        if (i > 0 && (lo < (p.bark[i - 1] >> 16) + 1 || hi < (p.bark[i - 1] & 0xffff) + 1)) return fail("bark monotone");
    }
    if (!p.tonecurves || !p.tonecurves[0] || !p.tonecurves[0][0]) return fail("tone curves");
    /* noise offsets: the linear blend of noiseoff over the half-octave index, clamped to the band ends */
    for (j = 0; j < P_NOISECURVES; j++) {
        for (i = 0; i < n; i++) {
            float halfoc = (float)(toOC((i + .5) * rate / (2. * n)) * 2.); int ih; float del, want;
            if (halfoc < 0) halfoc = 0;
            if (halfoc >= P_BANDS - 1) halfoc = P_BANDS - 1;
            ih = (int)halfoc; del = halfoc - ih;
            want = (float)(vi.noiseoff[j][ih] * (1. - del) + vi.noiseoff[j][ih + 1] * del);
            if (std::fabs(p.noiseoffset[j][i] - want) > 1e-3f) return fail("noise offset");
        }
        if (p.noiseoffset[j][0] != (float)(10 * j)) return fail("noise offset band 0");
    }
    std::printf("00c92660_TEST PASS\n");
    return 0;
}