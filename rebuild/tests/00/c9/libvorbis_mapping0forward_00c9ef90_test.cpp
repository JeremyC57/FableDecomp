// Behaviour test for libvorbis mapping0_forward (0x00C9EF90): the 1.0.1 mapping0.c body over recorded mocks of its callees,
// two channels, one coupling step, no bitrate management -- the floor-type guard (-1), the analysis calls per channel, the
// single packet blob k = PACKETBLOBS/2 (packet type bit, mode, the long-window lW/nW bits), floor encode + remove-floor +
// normalise per channel, coupling, one residue class/forward per submap, the blob marker and the global ampmax.
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
typedef unsigned int ogg_uint32_t;
#define PACKETBLOBS 15
#define NULL0 0
struct oggpack_buffer { long bytes; };
struct vorbis_info_psy { int normal_channel_p; };
struct vorbis_look_psy { vorbis_info_psy* vi; };
struct vorbis_info_psy_global { int sliding_lowpass[2][PACKETBLOBS]; };
struct vorbis_info_mapping0 { int submaps; int chmuxlist[256]; int floorsubmap[16]; int residuesubmap[16]; int coupling_steps; };
struct codec_setup_info { long blocksizes[2]; vorbis_info_mapping0* map_param[64]; int floor_type[64]; int residue_type[64]; vorbis_info_psy_global psy_g_param; };
struct vorbis_info { int channels; codec_setup_info* codec_setup; };
struct private_state { int window[2]; void** transform[2]; int fft_look[2]; int modebits; void** flr; void** residue; vorbis_look_psy* psy; };
struct vorbis_dsp_state { vorbis_info* vi; void* backend_state; };
struct vorbis_block_internal { float ampmax; int blocktype; ogg_uint32_t packetblob_markers[PACKETBLOBS]; };
struct vorbis_block { float** pcm; oggpack_buffer opb; long lW, W, nW; int pcmend, mode; vorbis_dsp_state* vd; void* internal; };
typedef struct vorbis_block vorbis_block;
typedef void vorbis_look_floor;
typedef void vorbis_look_residue;
struct vorbis_func_residue {
    long** (*class_)(vorbis_block*, vorbis_look_residue*, float**, int*, int);
    int (*forward)(vorbis_block*, vorbis_look_residue*, float**, float**, int*, int, long**);
};
static char g_pool[1 << 16]; static int g_top;
static void* _vorbis_block_alloc(vorbis_block*, long bytes) { void* p = g_pool + g_top; g_top += (int)((bytes + 7) & ~7); return p; }
static float todB(const float* x) { return *x; }
static void _vorbis_apply_window(float*, int*, long*, int, int, int) { }
static void mdct_forward(void*, float* in, float* out) { (void)in; (void)out; }
static void drft_forward(int*, float* d) { d[0] = -3.f; d[1] = 1.f; d[2] = 1.f; }
static int g_noisemask, g_tonemask, g_mix, g_fits, g_encodes, g_remove, g_norm, g_couple, g_class, g_forward, g_align, g_coupleK;
static long g_writes[8][2]; static int g_nwrites;
static void _vp_noisemask(vorbis_look_psy*, float*, float*) { g_noisemask++; }
static void _vp_tonemask(vorbis_look_psy*, float*, float*, float, float) { g_tonemask++; }
static void _vp_offset_and_mix(vorbis_look_psy*, float*, float*, int, float*) { g_mix++; }
static int g_post[4];
static int* floor1_fit(vorbis_block*, vorbis_look_floor*, const float*, const float*) { g_fits++; return g_post; }
static int* floor1_interpolate_fit(vorbis_block*, vorbis_look_floor*, int*, int*, int) { return 0; }
static int floor1_encode(vorbis_block*, vorbis_look_floor*, int* post, int*) { g_encodes++; return post == g_post; }
static int vorbis_bitrate_managed(vorbis_block*) { return 0; }
static float** _vp_quantize_couple_memo(vorbis_block*, vorbis_info_psy_global*, vorbis_look_psy*, vorbis_info_mapping0*, float**) { return 0; }
static int** _vp_quantize_couple_sort(vorbis_block*, vorbis_look_psy*, vorbis_info_mapping0*, float**) { return 0; }
static void _vp_noise_normalize_sort(vorbis_look_psy*, float*, int*) { }
static void _vp_remove_floor(vorbis_look_psy*, float*, int*, float*, int) { g_remove++; }
static void _vp_noise_normalize(vorbis_look_psy*, float*, float*, int*) { g_norm++; }
static void _vp_couple(int k, vorbis_info_psy_global*, vorbis_look_psy*, vorbis_info_mapping0*, float**, float**, int**, int**, int* nz, int)
{ g_couple++; g_coupleK = k; nz[1] = 1; }
static long** res_class(vorbis_block*, vorbis_look_residue*, float**, int*, int ch) { g_class += ch; return 0; }
static int res_forward(vorbis_block*, vorbis_look_residue*, float**, float**, int* zb, int ch, long**) { g_forward += ch; return zb[0] + zb[1]; }
static void oggpack_write(oggpack_buffer*, unsigned long v, int bits) { g_writes[g_nwrites][0] = (long)v; g_writes[g_nwrites][1] = bits; g_nwrites++; }
static void oggpack_writealign(oggpack_buffer*) { g_align++; }
static long oggpack_bytes(oggpack_buffer* b) { return b->bytes; }
static vorbis_func_residue g_res0 = { res_class, res_forward };
static const vorbis_func_residue* _residue_P[1] = { &g_res0 };
static int mapping0_forward(vorbis_block *vb){
  vorbis_dsp_state      *vd=vb->vd;
  vorbis_info           *vi=vd->vi;
  codec_setup_info      *ci=vi->codec_setup;
  private_state *b=(private_state*)vb->vd->backend_state;
  vorbis_block_internal *vbi=(vorbis_block_internal *)vb->internal;
  int                    n=vb->pcmend;
  int i,j,k;

  int *nonzero = (int*)alloca(sizeof(*nonzero)*vi->channels);
  float **gmdct = (float**)_vorbis_block_alloc(vb,vi->channels*sizeof(*gmdct));
  int **ilogmaskch = (int**)_vorbis_block_alloc(vb,vi->channels*sizeof(*ilogmaskch));
  int ***floor_posts = (int***)_vorbis_block_alloc(vb,vi->channels*sizeof(*floor_posts));
  
  float global_ampmax=vbi->ampmax;
  float *local_ampmax=(float*)alloca(sizeof(*local_ampmax)*vi->channels);
  int blocktype=vbi->blocktype;

  int modenumber=vb->W;
  vorbis_info_mapping0 *info=ci->map_param[modenumber];
  vorbis_look_psy *psy_look=
    b->psy+blocktype+(vb->W?2:0);

  vb->mode=modenumber;

  for(i=0;i<vi->channels;i++){
    float scale=4.f/n;
    float scale_dB;

    float *pcm     =vb->pcm[i]; 
    float *logfft  =pcm;

    gmdct[i]=(float*)_vorbis_block_alloc(vb,n/2*sizeof(**gmdct));

    scale_dB=todB(&scale);

#if 0
    if(vi->channels==2)
      if(i==0)
	_analysis_output("pcmL",seq,pcm,n,0,0,total-n/2);
      else
	_analysis_output("pcmR",seq,pcm,n,0,0,total-n/2);
#endif
  
    /* window the PCM data */
    _vorbis_apply_window(pcm,b->window,ci->blocksizes,vb->lW,vb->W,vb->nW);

#if 0
    if(vi->channels==2)
      if(i==0)
	_analysis_output("windowedL",seq,pcm,n,0,0,total-n/2);
      else
	_analysis_output("windowedR",seq,pcm,n,0,0,total-n/2);
#endif

    /* transform the PCM data */
    /* only MDCT right now.... */
    mdct_forward(b->transform[vb->W][0],pcm,gmdct[i]);
    
    /* FFT yields more accurate tonal estimation (not phase sensitive) */
    drft_forward(&b->fft_look[vb->W],pcm);
    logfft[0]=scale_dB+todB(pcm);
    local_ampmax[i]=logfft[0];
    for(j=1;j<n-1;j+=2){
      float temp=pcm[j]*pcm[j]+pcm[j+1]*pcm[j+1];
      temp=logfft[(j+1)>>1]=scale_dB+.5f*todB(&temp);
      if(temp>local_ampmax[i])local_ampmax[i]=temp;
    }

    if(local_ampmax[i]>0.f)local_ampmax[i]=0.f;
    if(local_ampmax[i]>global_ampmax)global_ampmax=local_ampmax[i];

#if 0
    if(vi->channels==2)
      if(i==0)
	_analysis_output("fftL",seq,logfft,n/2,1,0,0);
      else
	_analysis_output("fftR",seq,logfft,n/2,1,0,0);
#endif

  }
  
  {
    float *noise = (float*)_vorbis_block_alloc(vb,n/2*sizeof(*noise));
    float *tone = (float*)_vorbis_block_alloc(vb,n/2*sizeof(*tone));
    
    for(i=0;i<vi->channels;i++){
      /* the encoder setup assumes that all the modes used by any
	 specific bitrate tweaking use the same floor */
      
      int submap=info->chmuxlist[i];
      
      /* the following makes things clearer to *me* anyway */
      float *mdct    =gmdct[i];
      float *logfft  =vb->pcm[i];
      
      float *logmdct =logfft+n/2;
      float *logmask =logfft;

      vb->mode=modenumber;

      floor_posts[i]=(int**)_vorbis_block_alloc(vb,PACKETBLOBS*sizeof(**floor_posts));
      memset(floor_posts[i],0,sizeof(**floor_posts)*PACKETBLOBS);
      
      for(j=0;j<n/2;j++)
	logmdct[j]=todB(mdct+j);

#if 0
      if(vi->channels==2){
	if(i==0)
	  _analysis_output("mdctL",seq,logmdct,n/2,1,0,0);
	else
	  _analysis_output("mdctR",seq,logmdct,n/2,1,0,0);
      }else{
	_analysis_output("mdct",seq,logmdct,n/2,1,0,0);
      }
#endif 
      
      /* first step; noise masking.  Not only does 'noise masking'
         give us curves from which we can decide how much resolution
         to give noise parts of the spectrum, it also implicitly hands
         us a tonality estimate (the larger the value in the
         'noise_depth' vector, the more tonal that area is) */

      _vp_noisemask(psy_look,
		    logmdct,
		    noise); /* noise does not have by-frequency offset
                               bias applied yet */
#if 0
      if(vi->channels==2){
	if(i==0)
	  _analysis_output("noiseL",seq,noise,n/2,1,0,0);
	else
	  _analysis_output("noiseR",seq,noise,n/2,1,0,0);
      }
#endif

      /* second step: 'all the other crap'; all the stuff that isn't
         computed/fit for bitrate management goes in the second psy
         vector.  This includes tone masking, peak limiting and ATH */

      _vp_tonemask(psy_look,
		   logfft,
		   tone,
		   global_ampmax,
		   local_ampmax[i]);

#if 0
      if(vi->channels==2){
	if(i==0)
	  _analysis_output("toneL",seq,tone,n/2,1,0,0);
	else
	  _analysis_output("toneR",seq,tone,n/2,1,0,0);
      }
#endif

      /* third step; we offset the noise vectors, overlay tone
	 masking.  We then do a floor1-specific line fit.  If we're
	 performing bitrate management, the line fit is performed
	 multiple times for up/down tweakage on demand. */
      
      _vp_offset_and_mix(psy_look,
			 noise,
			 tone,
			 1,
			 logmask);

#if 0
      if(vi->channels==2){
	if(i==0)
	  _analysis_output("mask1L",seq,logmask,n/2,1,0,0);
	else
	  _analysis_output("mask1R",seq,logmask,n/2,1,0,0);
      }
#endif

      /* this algorithm is hardwired to floor 1 for now; abort out if
         we're *not* floor1.  This won't happen unless someone has
         broken the encode setup lib.  Guard it anyway. */
      if(ci->floor_type[info->floorsubmap[submap]]!=1)return(-1);

      floor_posts[i][PACKETBLOBS/2]=
	floor1_fit(vb,b->flr[info->floorsubmap[submap]],
		   logmdct,
		   logmask);
      
      /* are we managing bitrate?  If so, perform two more fits for
         later rate tweaking (fits represent hi/lo) */
      if(vorbis_bitrate_managed(vb) && floor_posts[i][PACKETBLOBS/2]){
	/* higher rate by way of lower noise curve */

	_vp_offset_and_mix(psy_look,
			   noise,
			   tone,
			   2,
			   logmask);

#if 0
	if(vi->channels==2){
	  if(i==0)
	    _analysis_output("mask2L",seq,logmask,n/2,1,0,0);
	  else
	    _analysis_output("mask2R",seq,logmask,n/2,1,0,0);
	}
#endif
	
	floor_posts[i][PACKETBLOBS-1]=
	  floor1_fit(vb,b->flr[info->floorsubmap[submap]],
		     logmdct,
		     logmask);
      
	/* lower rate by way of higher noise curve */
	_vp_offset_and_mix(psy_look,
			   noise,
			   tone,
			   0,
			   logmask);

#if 0
	if(vi->channels==2)
	  if(i==0)
	    _analysis_output("mask0L",seq,logmask,n/2,1,0,0);
	  else
	    _analysis_output("mask0R",seq,logmask,n/2,1,0,0);
#endif

	floor_posts[i][0]=
	  floor1_fit(vb,b->flr[info->floorsubmap[submap]],
		     logmdct,
		     logmask);
	
	/* we also interpolate a range of intermediate curves for
           intermediate rates */
	for(k=1;k<PACKETBLOBS/2;k++)
	  floor_posts[i][k]=
	    floor1_interpolate_fit(vb,b->flr[info->floorsubmap[submap]],
				   floor_posts[i][0],
				   floor_posts[i][PACKETBLOBS/2],
				   k*65536/(PACKETBLOBS/2));
	for(k=PACKETBLOBS/2+1;k<PACKETBLOBS-1;k++)
	  floor_posts[i][k]=
	    floor1_interpolate_fit(vb,b->flr[info->floorsubmap[submap]],
				   floor_posts[i][PACKETBLOBS/2],
				   floor_posts[i][PACKETBLOBS-1],
				   (k-PACKETBLOBS/2)*65536/(PACKETBLOBS/2));
      }
    }
  }
  vbi->ampmax=global_ampmax;

  /*
    the next phases are performed once for vbr-only and PACKETBLOB
    times for bitrate managed modes.
    
    1) encode actual mode being used
    2) encode the floor for each channel, compute coded mask curve/res
    3) normalize and couple.
    4) encode residue
    5) save packet bytes to the packetblob vector
    
  */

  /* iterate over the many masking curve fits we've created */

  {
    float **res_bundle=(float**)alloca(sizeof(*res_bundle)*vi->channels);
    float **couple_bundle=(float**)alloca(sizeof(*couple_bundle)*vi->channels);
    int *zerobundle=(int*)alloca(sizeof(*zerobundle)*vi->channels);
    int **sortindex=(int**)alloca(sizeof(*sortindex)*vi->channels);
    float **mag_memo;
    int **mag_sort;

    if(info->coupling_steps){
      mag_memo=_vp_quantize_couple_memo(vb,
					&ci->psy_g_param,
					psy_look,
					info,
					gmdct);    
      
      mag_sort=_vp_quantize_couple_sort(vb,
					psy_look,
					info,
					mag_memo);    
    }

    memset(sortindex,0,sizeof(*sortindex)*vi->channels);
    if(psy_look->vi->normal_channel_p){
      for(i=0;i<vi->channels;i++){
	float *mdct    =gmdct[i];
	sortindex[i]=(int*)alloca(sizeof(**sortindex)*n/2);
	_vp_noise_normalize_sort(psy_look,mdct,sortindex[i]);
      }
    }

    for(k=(vorbis_bitrate_managed(vb)?0:PACKETBLOBS/2);
	k<=(vorbis_bitrate_managed(vb)?PACKETBLOBS-1:PACKETBLOBS/2);
	k++){

      /* start out our new packet blob with packet type and mode */
      /* Encode the packet type */
      oggpack_write(&vb->opb,0,1);
      /* Encode the modenumber */
      /* Encode frame mode, pre,post windowsize, then dispatch */
      oggpack_write(&vb->opb,modenumber,b->modebits);
      if(vb->W){
	oggpack_write(&vb->opb,vb->lW,1);
	oggpack_write(&vb->opb,vb->nW,1);
      }

      /* encode floor, compute masking curve, sep out residue */
      for(i=0;i<vi->channels;i++){
	int submap=info->chmuxlist[i];
	float *mdct    =gmdct[i];
	float *res     =vb->pcm[i];
	int   *ilogmask=ilogmaskch[i]=(int*)
	  (int*)_vorbis_block_alloc(vb,n/2*sizeof(**gmdct));
      
	nonzero[i]=floor1_encode(vb,b->flr[info->floorsubmap[submap]],
				 floor_posts[i][k],
				 ilogmask);
#if 0
	{
	  char buf[80];
	  sprintf(buf,"maskI%c%d",i?'R':'L',k);
	  float work[n/2];
	  for(j=0;j<n/2;j++)
	    work[j]=FLOOR1_fromdB_LOOKUP[ilogmask[j]];
	  _analysis_output(buf,seq,work,n/2,1,1,0);
	}
#endif
	_vp_remove_floor(psy_look,
			 mdct,
			 ilogmask,
			 res,
			 ci->psy_g_param.sliding_lowpass[vb->W][k]);

	_vp_noise_normalize(psy_look,res,res+n/2,sortindex[i]);

	
#if 0
	{
	  char buf[80];
	  float work[n/2];
	  for(j=0;j<n/2;j++)
	    work[j]=FLOOR1_fromdB_LOOKUP[ilogmask[j]]*(res+n/2)[j];
	  sprintf(buf,"resI%c%d",i?'R':'L',k);
	  _analysis_output(buf,seq,work,n/2,1,1,0);

	}
#endif
      }
      
      /* our iteration is now based on masking curve, not prequant and
	 coupling.  Only one prequant/coupling step */
      
      /* quantize/couple */
      /* incomplete implementation that assumes the tree is all depth
         one, or no tree at all */
      if(info->coupling_steps){
	_vp_couple(k,
		   &ci->psy_g_param,
		   psy_look,
		   info,
		   vb->pcm,
		   mag_memo,
		   mag_sort,
		   ilogmaskch,
		   nonzero,
		   ci->psy_g_param.sliding_lowpass[vb->W][k]);
      }
      
      /* classify and encode by submap */
      for(i=0;i<info->submaps;i++){
	int ch_in_bundle=0;
	long **classifications;
	int resnum=info->residuesubmap[i];

	for(j=0;j<vi->channels;j++){
	  if(info->chmuxlist[j]==i){
	    zerobundle[ch_in_bundle]=0;
	    if(nonzero[j])zerobundle[ch_in_bundle]=1;
	    res_bundle[ch_in_bundle]=vb->pcm[j];
	    couple_bundle[ch_in_bundle++]=vb->pcm[j]+n/2;
	  }
	}
	
	classifications=_residue_P[ci->residue_type[resnum]]->
	  class_(vb,b->residue[resnum],couple_bundle,zerobundle,ch_in_bundle);
	
	_residue_P[ci->residue_type[resnum]]->
	  forward(vb,b->residue[resnum],
		  couple_bundle,NULL0,zerobundle,ch_in_bundle,classifications);
      }
      
      /* ok, done encoding.  Mark this protopacket and prepare next. */
      oggpack_writealign(&vb->opb);
      vbi->packetblob_markers[k]=oggpack_bytes(&vb->opb);
      
    }
    
  }

#if 0
  seq++;
  total+=ci->blocksizes[vb->W]/4+ci->blocksizes[vb->nW]/4;
#endif
  return(0);
}

int main()
{
    static float c0[32], c1[32]; float* pcm[2] = { c0, c1 };
    static vorbis_info_mapping0 info; info.submaps = 1; info.coupling_steps = 1;
    static codec_setup_info ci; ci.blocksizes[0] = 16; ci.blocksizes[1] = 32; ci.map_param[1] = &info; ci.floor_type[0] = 1;
    ci.psy_g_param.sliding_lowpass[1][7] = 12;
    vorbis_info vi = { 2, &ci };
    vorbis_info_psy pvi = { 0 }; vorbis_look_psy psy[4] = { { &pvi }, { &pvi }, { &pvi }, { &pvi } };
    void* tr[1] = { 0 }; void* flr[1] = { 0 }; void* res[1] = { 0 };
    private_state b; std::memset(&b, 0, sizeof(b)); b.transform[0] = tr; b.transform[1] = tr; b.modebits = 2; b.flr = flr; b.residue = res; b.psy = psy;
    vorbis_dsp_state vd = { &vi, &b };
    vorbis_block_internal vbi; std::memset(&vbi, 0, sizeof(vbi)); vbi.ampmax = -9999.f; vbi.blocktype = 1;
    vorbis_block vb; std::memset(&vb, 0, sizeof(vb)); vb.pcm = pcm; vb.opb.bytes = 321; vb.lW = 1; vb.W = 1; vb.nW = 0; vb.pcmend = 32; vb.vd = &vd; vb.internal = &vbi;
    int bad = 0;
    if (mapping0_forward(&vb) != 0) bad = 1;
    if (vb.mode != 1 || g_noisemask != 2 || g_tonemask != 2 || g_mix != 2 || g_fits != 2) bad = 2;
    if (g_nwrites != 4 || g_writes[0][0] != 0 || g_writes[0][1] != 1 || g_writes[1][0] != 1 || g_writes[1][1] != 2 ||
        g_writes[2][0] != 1 || g_writes[3][0] != 0) bad = 3;
    if (g_encodes != 2 || g_remove != 2 || g_norm != 2 || g_couple != 1 || g_coupleK != PACKETBLOBS / 2) bad = 4;
    if (g_class != 2 || g_forward != 2 || g_align != 1 || vbi.packetblob_markers[7] != 321) bad = 5;
    // channel 0 logfft[0] = scale_dB + todB(-3) = 4/32 + -3; local max clamps at 0 -> global ampmax 0
    if (vbi.ampmax != 0.f) bad = 6;
    ci.floor_type[0] = 0; g_top = 0;
    if (mapping0_forward(&vb) != -1) bad = 7;
    if (bad) { std::printf("00c9ef90_TEST FAIL %d\n", bad); return 1; }
    std::printf("00c9ef90_TEST PASS\n"); return 0;
}