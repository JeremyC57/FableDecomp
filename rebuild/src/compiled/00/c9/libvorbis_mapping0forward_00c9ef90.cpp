// libvorbis::mapping0_forward(vorbis_block* vb) -- mapping0.c encoder: window + MDCT + FFT each channel, compute noise/tone masks and floor1 fits (three plus interpolated fits under bitrate management), couple, then per packet blob encode mode, floors, residue classes and residues and record the blob markers
// Leaf: @mapping0_forward@4
// Flags: /TC /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9EF90, 2346 bytes, LTCG __fastcall (vb in ecx, plain `ret`): a static that is
//   address-taken (mapping0_exportbundle.forward, reached through _mapping_P 0x013A3DDC slot +0x0C), so it keeps the
//   public convention. ebp frame for six alloca buffers (__alloca_probe 0x00BFEA30).
// Library: libvorbis 1.0.1 lib/mapping0.c compiled as C (/TC /Gr) inside libpk; no PDB/RTTI, so the name is the libvorbis one.
//   Body verbatim from gitlab.xiph.org xiph/vorbis tag v1.0.1.
// Identity (two sources): (1) the call graph: _vorbis_block_alloc 0x00C8F0D0, todB 0x00C9EF70 (mapping0.c's scales.h copy),
//   _vorbis_apply_window 0x00C9EA80, mdct_forward 0x00C9C870, drft_forward 0x00C9BC10, _vp_noisemask 0x00C93C40,
//   _vp_tonemask 0x00C93E30, _vp_offset_and_mix 0x00C93F60 (x3), floor1_fit 0x00CA26C0 (x3), vorbis_bitrate_managed
//   0x00C9CFD0, floor1_interpolate_fit 0x00CA2B00, _vp_quantize_couple_memo 0x00C94490 / _sort 0x00C94600,
//   _vp_noise_normalize_sort 0x00C94750, oggpack_write 0x00C90FC0 (x4), floor1_encode 0x00CA2BB0, _vp_remove_floor 0x00C93B30,
//   _vp_noise_normalize 0x00C94810, _vp_couple 0x00C94AE0, the residue class/forward pointers of _residue_P 0x013A3DD0
//   (+0x14 / +0x18), oggpack_writealign 0x00C911B0, oggpack_bytes 0x00C917B0 into packetblob_markers[k]; (2) file order:
//   after mapping0_unpack 0x00C9ED80 and todB 0x00C9EF70, before mapping0_inverse 0x00C9F8C0 (landed), as in mapping0.c.
//   Catalog label "CPlayer::ProcessAudioFrame" (module CPlayer::CPlayer) is propagated junk.
// Shape: todB is a same-TU `static __declspec(noinline)` body (x in eax as in retail); every other callee is an external
//   /Gr __fastcall declaration with its landed spelling. Structures: engine/Vorbis.h plus psy.h's vorbis_info_psy.
// Original libvorbis mapping0.c / psy.h / scales.h; BSD-style terms in reference COPYING.
#include "engine/Vorbis.h"
void * __cdecl _alloca(unsigned int size);
#pragma intrinsic(_alloca)
#define alloca _alloca
void * __cdecl memset(void *dst, int val, unsigned int n);
#pragma intrinsic(memset)
typedef int ogg_int32_t;
#define NULL ((void *)0)
#define P_BANDS 17
#define NOISE_COMPAND_LEVELS 40
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

extern const vorbis_func_residue *_residue_P[];                                      /* 0x013A3DD0 */

extern void *_vorbis_block_alloc(vorbis_block *vb,long bytes);                        /* 0x00C8F0D0 */
extern void _vorbis_apply_window(float *d,int *winno,long *blocksizes,
                                 int lW,int W,int nW);                                /* 0x00C9EA80 */
extern void mdct_forward(mdct_lookup *init, DATA_TYPE *in, DATA_TYPE *out);          /* 0x00C9C870 */
extern void drft_forward(drft_lookup *l,float *data);                                /* 0x00C9BC10 */
extern int vorbis_bitrate_managed(vorbis_block *vb);                                 /* 0x00C9CFD0 */
extern void  oggpack_writealign(oggpack_buffer *b);                                  /* 0x00C911B0 */
extern void  oggpack_write(oggpack_buffer *b,unsigned long value,int bits);          /* 0x00C90FC0 */
extern long  oggpack_bytes(oggpack_buffer *b);                                       /* 0x00C917B0 */

extern void _vp_remove_floor(vorbis_look_psy *p,
			     float *mdct,
			     int *icodedflr,
			     float *residue,
			     int sliding_lowpass);

extern void _vp_noisemask(vorbis_look_psy *p,
			  float *logmdct, 
			  float *logmask);

extern void _vp_tonemask(vorbis_look_psy *p,
			 float *logfft,
			 float *logmask,
			 float global_specmax,
			 float local_specmax);

extern void _vp_offset_and_mix(vorbis_look_psy *p,
			       float *noise,
			       float *tone,
			       int offset_select,
			       float *logmask);

extern float _vp_ampmax_decay(float amp,vorbis_dsp_state *vd);

extern float **_vp_quantize_couple_memo(vorbis_block *vb,
					vorbis_info_psy_global *g,
					vorbis_look_psy *p,
					vorbis_info_mapping0 *vi,
					float **mdct);

extern void _vp_couple(int blobno,
		       vorbis_info_psy_global *g,
		       vorbis_look_psy *p,
		       vorbis_info_mapping0 *vi,
		       float **res,
		       float **mag_memo,
		       int   **mag_sort,
		       int   **ifloor,
		       int   *nonzero,
		       int   sliding_lowpass);

extern void _vp_noise_normalize(vorbis_look_psy *p,
				float *in,float *out,int *sortedindex);

extern void _vp_noise_normalize_sort(vorbis_look_psy *p,
				     float *magnitudes,int *sortedindex);

extern int **_vp_quantize_couple_sort(vorbis_block *vb,
				      vorbis_look_psy *p,
				      vorbis_info_mapping0 *vi,
				      float **mags);



#pragma auto_inline(off)
static __declspec(noinline) float todB(const float *x){                                /* 0x00C9EF70 (scales.h) */
  ogg_int32_t i=*(ogg_int32_t *)x;
  i=i&0x7fffffff;
  return (float)(i * 7.17711438e-7f -764.6161886f);
}
#pragma auto_inline(on)

extern int *floor1_fit(vorbis_block *vb,vorbis_look_floor *look,
		       const float *logmdct,   /* in */
		       const float *logmask);
extern int *floor1_interpolate_fit(vorbis_block *vb,vorbis_look_floor *look,
				   int *A,int *B,
				   int del);
extern int floor1_encode(vorbis_block *vb,vorbis_look_floor *look,
			 int *post,int *ilogmask);


int mapping0_forward(vorbis_block *vb){
  vorbis_dsp_state      *vd=vb->vd;
  vorbis_info           *vi=vd->vi;
  codec_setup_info      *ci=vi->codec_setup;
  private_state         *b=vb->vd->backend_state;
  vorbis_block_internal *vbi=(vorbis_block_internal *)vb->internal;
  int                    n=vb->pcmend;
  int i,j,k;

  int    *nonzero    = alloca(sizeof(*nonzero)*vi->channels);
  float  **gmdct     = _vorbis_block_alloc(vb,vi->channels*sizeof(*gmdct));
  int    **ilogmaskch= _vorbis_block_alloc(vb,vi->channels*sizeof(*ilogmaskch));
  int ***floor_posts = _vorbis_block_alloc(vb,vi->channels*sizeof(*floor_posts));
  
  float global_ampmax=vbi->ampmax;
  float *local_ampmax=alloca(sizeof(*local_ampmax)*vi->channels);
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

    gmdct[i]=_vorbis_block_alloc(vb,n/2*sizeof(**gmdct));

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
    float   *noise        = _vorbis_block_alloc(vb,n/2*sizeof(*noise));
    float   *tone         = _vorbis_block_alloc(vb,n/2*sizeof(*tone));
    
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

      floor_posts[i]=_vorbis_block_alloc(vb,PACKETBLOBS*sizeof(**floor_posts));
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
    float **res_bundle=alloca(sizeof(*res_bundle)*vi->channels);
    float **couple_bundle=alloca(sizeof(*couple_bundle)*vi->channels);
    int *zerobundle=alloca(sizeof(*zerobundle)*vi->channels);
    int **sortindex=alloca(sizeof(*sortindex)*vi->channels);
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
	sortindex[i]=alloca(sizeof(**sortindex)*n/2);
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
	int   *ilogmask=ilogmaskch[i]=
	  _vorbis_block_alloc(vb,n/2*sizeof(**gmdct));
      
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
	  class(vb,b->residue[resnum],couple_bundle,zerobundle,ch_in_bundle);
	
	_residue_P[ci->residue_type[resnum]]->
	  forward(vb,b->residue[resnum],
		  couple_bundle,NULL,zerobundle,ch_in_bundle,classifications);
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
