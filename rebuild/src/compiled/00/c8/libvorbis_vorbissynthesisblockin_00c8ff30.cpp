// libvorbis::vorbis_synthesis_blockin(vorbis_dsp_state* v, vorbis_block* vb) -- block.c: overlap-add a decoded block into the dsp state's PCM double buffer (window lapping for the four long/short combinations, copy section), then update pcm_returned / pcm_current, the sample count and the granule position with end trimming
// Vendor-proof: libvorbis-1.0.1(lib/block.c) @vorbis_synthesis_blockin@8 -- 2470 bytes verified against Fable.exe 0x00C8FF30
// Leaf: @vorbis_synthesis_blockin@8
// Flags: /TC /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8FF30, 2470 bytes, LTCG __fastcall (v in ecx, vb in edx, plain `ret`).
// Library: libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909") lib/block.c compiled as C (/TC /Gr), statically linked into
//   libpk. No PDB/RTTI: the name is the libvorbis API name. The body is the 1.0.1 block.c text verbatim (gitlab.xiph.org
//   xiph/vorbis tag v1.0.1), including the post-1.0 halfrate_flag (codec_setup_info +0xE78) shifts.
// Identity (two sources): (1) bound `@vorbis_synthesis_blockin@8` already: _fetch_and_process_packet 0x00C8A540 calls it
//   after vorbis_synthesis_pcmout, and ov_pcm_seek 0x00C8B490 after vorbis_synthesis_trackonly; (2) the body: OV_EINVAL
//   (-131) twice, lW=W / W=vb->W / nW=-1, the sequence check resetting granulepos and backend_state->sample_count (+0xB8),
//   four _vorbis_window_get 0x00C9EA70 calls (window[1]-hs / window[0]-hs), the glue/time/floor/res bit sums, centerW
//   toggling and the int64 >>hs trims (__allshr 0x00F3FD10). It follows vorbis_synthesis_init 0x00C8FF00 in block.c order.
//   Catalog label "AudioDSP_Process" is propagated junk.
// Structures: the shared engine/Vorbis.h (vorbis_dsp_state, vorbis_block, codec_setup_info, private_state).
// Original libvorbis block.c; BSD-style terms in reference COPYING.
#include "engine/Vorbis.h"
#define OV_EINVAL -131
float *_vorbis_window_get(int n);                                                      /* 0x00C9EA70 */

int vorbis_synthesis_blockin(vorbis_dsp_state *v,vorbis_block *vb){
  vorbis_info *vi=v->vi;
  codec_setup_info *ci=vi->codec_setup;
  private_state *b=v->backend_state;
  int hs=ci->halfrate_flag; 
  int i,j;

  if(!vb)return(OV_EINVAL);
  if(v->pcm_current>v->pcm_returned  && v->pcm_returned!=-1)return(OV_EINVAL);
    
  v->lW=v->W;
  v->W=vb->W;
  v->nW=-1;
  
  if((v->sequence==-1)||
     (v->sequence+1 != vb->sequence)){
    v->granulepos=-1; /* out of sequence; lose count */
    b->sample_count=-1;
  }

  v->sequence=vb->sequence;
  
  if(vb->pcm){  /* no pcm to process if vorbis_synthesis_trackonly 
		   was called on block */
    int n=ci->blocksizes[v->W]>>(hs+1);
    int n0=ci->blocksizes[0]>>(hs+1);
    int n1=ci->blocksizes[1]>>(hs+1);

    int thisCenter;
    int prevCenter;
    
    v->glue_bits+=vb->glue_bits;
    v->time_bits+=vb->time_bits;
    v->floor_bits+=vb->floor_bits;
    v->res_bits+=vb->res_bits;
    
    if(v->centerW){
      thisCenter=n1;
      prevCenter=0;
    }else{
      thisCenter=0;
      prevCenter=n1;
    }
    
    /* v->pcm is now used like a two-stage double buffer.  We don't want
       to have to constantly shift *or* adjust memory usage.  Don't
       accept a new block until the old is shifted out */
    
    for(j=0;j<vi->channels;j++){
      /* the overlap/add section */
      if(v->lW){
	if(v->W){
	  /* large/large */
	  float *w=_vorbis_window_get(b->window[1]-hs);
	  float *pcm=v->pcm[j]+prevCenter;
	  float *p=vb->pcm[j];
	  for(i=0;i<n1;i++)
	    pcm[i]=pcm[i]*w[n1-i-1] + p[i]*w[i];
	}else{
	  /* large/small */
	  float *w=_vorbis_window_get(b->window[0]-hs);
	  float *pcm=v->pcm[j]+prevCenter+n1/2-n0/2;
	  float *p=vb->pcm[j];
	  for(i=0;i<n0;i++)
	    pcm[i]=pcm[i]*w[n0-i-1] +p[i]*w[i];
	}
      }else{
	if(v->W){
	  /* small/large */
	  float *w=_vorbis_window_get(b->window[0]-hs);
	  float *pcm=v->pcm[j]+prevCenter;
	  float *p=vb->pcm[j]+n1/2-n0/2;
	  for(i=0;i<n0;i++)
	    pcm[i]=pcm[i]*w[n0-i-1] +p[i]*w[i];
	  for(;i<n1/2+n0/2;i++)
	    pcm[i]=p[i];
	}else{
	  /* small/small */
	  float *w=_vorbis_window_get(b->window[0]-hs);
	  float *pcm=v->pcm[j]+prevCenter;
	  float *p=vb->pcm[j];
	  for(i=0;i<n0;i++)
	    pcm[i]=pcm[i]*w[n0-i-1] +p[i]*w[i];
	}
      }
      
      /* the copy section */
      {
	float *pcm=v->pcm[j]+thisCenter;
	float *p=vb->pcm[j]+n;
	for(i=0;i<n;i++)
	  pcm[i]=p[i];
      }
    }
    
    if(v->centerW)
      v->centerW=0;
    else
      v->centerW=n1;
    
    /* deal with initial packet state; we do this using the explicit
       pcm_returned==-1 flag otherwise we're sensitive to first block
       being short or long */
    
    if(v->pcm_returned==-1){
      v->pcm_returned=thisCenter;
      v->pcm_current=thisCenter;
    }else{
      v->pcm_returned=prevCenter;
      v->pcm_current=prevCenter+
	((ci->blocksizes[v->lW]/4+
	ci->blocksizes[v->W]/4)>>hs);
    }
    
  }

  /* track the frame number... This is for convenience, but also
     making sure our last packet doesn't end with added padding.  If
     the last packet is partial, the number of samples we'll have to
     return will be past the vb->granulepos.
     
     This is not foolproof!  It will be confused if we begin
     decoding at the last page after a seek or hole.  In that case,
     we don't have a starting point to judge where the last frame
     is.  For this reason, vorbisfile will always try to make sure
     it reads the last two marked pages in proper sequence */

  if(b->sample_count==-1){
    b->sample_count=0;
  }else{
    b->sample_count+=ci->blocksizes[v->lW]/4+ci->blocksizes[v->W]/4;
  }
  
  if(v->granulepos==-1){
    if(vb->granulepos!=-1){ /* only set if we have a position to set to */

      v->granulepos=vb->granulepos;

      /* is this a short page? */
      if(b->sample_count>v->granulepos){
	/* corner case; if this is both the first and last audio page,
	   then spec says the end is cut, not beginning */
	if(vb->eofflag){
	  /* trim the end */
	  /* no preceeding granulepos; assume we started at zero (we'd
	     have to in a short single-page stream) */
	  /* granulepos could be -1 due to a seek, but that would result
	     in a long count, not short count */
	  
	  v->pcm_current-=(b->sample_count-v->granulepos)>>hs;
	}else{
	  /* trim the beginning */
	  v->pcm_returned+=(b->sample_count-v->granulepos)>>hs;
	  if(v->pcm_returned>v->pcm_current)
	    v->pcm_returned=v->pcm_current;
	}

      }

    }
  }else{
    v->granulepos+=ci->blocksizes[v->lW]/4+ci->blocksizes[v->W]/4;
    if(vb->granulepos!=-1 && v->granulepos!=vb->granulepos){
      
      if(v->granulepos>vb->granulepos){
	long extra=v->granulepos-vb->granulepos;

	if(extra)
	  if(vb->eofflag){
	    /* partial last frame.  Strip the extra samples off */
	    v->pcm_current-=extra>>hs;
	  } /* else {Shouldn't happen *unless* the bitstream is out of
	       spec.  Either way, believe the bitstream } */
      } /* else {Shouldn't happen *unless* the bitstream is out of
	   spec.  Either way, believe the bitstream } */
      v->granulepos=vb->granulepos;
    }
  }
  
  /* Update, cleanup */
  
  if(vb->eofflag)v->eofflag=1;
  return(0);
  
}