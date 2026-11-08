// libvorbis::vorbis_bitrate_addblock(vorbis_block* vb) -- bitrate.c: queue one encoded block for bitrate management: without binned queueing just park the single packet (swapping oggpack buffers); otherwise add the packet and its PACKETBLOBS bin sizes at the queue head, update the average tracker (per-bin byte accumulators, the floating average bin chosen by floater_interpolate within the slew limits, applied to the centre packets, the tail aged out), then the min/max limiter (stack the recent packets' per-bin bytes, when over/under the hard limits search a new limit with limit_sum and collapse the stack to it, then age the tail out and revise queue_actual), and finally mark what may be flushed; -1 when the queue is full
// Leaf: @vorbis_bitrate_addblock@4
// Flags: /TC /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9CFF0, 2888 bytes, LTCG `extern "C" __fastcall` (vb in ecx, plain `ret`), ebp frame with
//   `and esp,-8` for the double locals. Callees: oggpack_get_buffer 0x00C91800 and oggpack_bytes 0x00C917B0 (`__fastcall`), the
//   bitrate.c statics BINBYTES 0x00C9CB40 (bm in eax, bin in edx, pos pushed), LACING_ADJUST 0x00C9CB60 (bytes in ecx),
//   floater_interpolate 0x00C9CB80 and limit_sum 0x00C9CC60 (bm in ebx), floor [0x01440100] (rint), __ftol2 0x00BFEA70, memmove
//   [0x01440144] cached in edi for the three FIFO shifts; the oggpack_buffer swaps are the memcpy intrinsic (`rep movsd`), the stack
//   blank is `rep stosd`. Caller: mapping0_forward 0x00C9EF90 (landed) at the end of a managed block.
// Identity (two sources): (1) the body is Xiph libvorbis 1.0.1 lib/bitrate.c vorbis_bitrate_addblock verbatim -- vb->vd (+0x40) ->
//   backend_state (+0x68) -> bms (+0x50), ci->bi (+0xD20), the 0x80000000 W flag in queue_actual, PACKETBLOBS/2 = 7.0, the
//   packetblob_markers deltas, the avgfloat slew clamp, the hardmax/hardmin limit searches and the 0x7fffffff actual masks; (2) the
//   landed mapping0_forward binds `@vorbis_bitrate_addblock@4` here and bitrate.c file order places it after vorbis_bitrate_managed
//   0x00C9CFD0 (landed) and before vorbis_bitrate_flushpacket. The catalog label "CLandscapeBackgroundDynamicBuildTemplate::
//   UpdateTerrainGrid" is a propagated error (libpk's Xiph block has no game classes).
// Extent note: the catalogued 2888-byte extent 0x00C9CFF0 holds TWO functions: vorbis_bitrate_addblock (2700 bytes, `ret` at 0x00C9DA7B)
//   and, after the int3 pad, bitrate.c's vorbis_bitrate_flushpacket 0x00C9DA80 (184 bytes), the last function of the file. Both are in
//   this TU; graded against the split extents (trim_oracle.tsv); the chunk oracle's row needs splitting at landing.
// Shape: the same bitrate.c TU as the limit_sum row (0x00C9CC60); only the graded leaf differs.
// Original libvorbis bitrate.c (libvorbis 1.0.1, unmodified function bodies); BSD-style terms in reference COPYING.
// Structures: the shared engine/Vorbis.h (vorbis_block, vorbis_block_internal, vorbis_dsp_state, private_state, bitrate_manager_state,
// codec_setup_info / bitrate_manager_info, oggpack_buffer, ogg_packet).
#include "engine/Vorbis.h"
void* __cdecl memset(void* dst, int c, unsigned int n);
void* __cdecl memcpy(void* dst, const void* src, unsigned int n);
#pragma intrinsic(memset, memcpy)
__declspec(dllimport) void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // IAT [0x01440144]
__declspec(dllimport) double __cdecl floor(double x);                                     // IAT [0x01440100]
double __cdecl fabs(double x);
#pragma intrinsic(fabs)
#define rint(x) (floor((x) + 0.5f))
#define NULL 0

long __fastcall oggpack_bytes(oggpack_buffer* b);                 // 0x00C917B0
unsigned char* __fastcall oggpack_get_buffer(oggpack_buffer* b);  // 0x00C91800

static __declspec(noinline) long BINBYTES(bitrate_manager_state *bm,long pos,long bin){
  int bins=bm->queue_bins;
  return(bm->queue_binned[pos*bins+bin]);
}

#define LIMITBYTES(pos,bin) (bm->minmax_binstack[(pos)*bins*2+((bin)+bins)])

static __declspec(noinline) long LACING_ADJUST(long bytes){
  int addto=bytes/255+1;
  return(bytes+addto);
}

static __declspec(noinline) int __stdcall floater_interpolate(bitrate_manager_state *bm,vorbis_info *vi,
				  double desired_rate){
  int bin=rint(bm->avgfloat);
  double lobitrate,hibitrate;


  lobitrate=(double)(bm->avg_binacc[bin]*8)/bm->avg_sampleacc*vi->rate;
  while(lobitrate>desired_rate && bin>0){
    bin--;
    lobitrate=(double)(bm->avg_binacc[bin]*8)/bm->avg_sampleacc*vi->rate;
  }

  if(bin+1<bm->queue_bins){
    hibitrate=(double)(bm->avg_binacc[bin+1]*8)/bm->avg_sampleacc*vi->rate;
    if(fabs(hibitrate-desired_rate) < fabs(lobitrate-desired_rate))bin++;
  }
  return(bin);
}

/* try out a new limit */
static __declspec(noinline) long __stdcall limit_sum(bitrate_manager_state *bm,int limit){
  int i=bm->minmax_stackptr;
  long acc=bm->minmax_acctotal;
  long bins=bm->queue_bins;
  
  acc-=LIMITBYTES(i,0);
  acc+=LIMITBYTES(i,limit);

  while(i-->0){
    if(bm->minmax_limitstack[i]<=limit)break;
    acc-=LIMITBYTES(i,bm->minmax_limitstack[i]);
    acc+=LIMITBYTES(i,limit);
  }
  return(acc);
}

int __fastcall vorbis_bitrate_addblock(vorbis_block *vb){
  int i; 
  vorbis_block_internal *vbi=vb->internal;
  vorbis_dsp_state      *vd=vb->vd;
  private_state         *b=vd->backend_state; 
  bitrate_manager_state *bm=&b->bms;
  vorbis_info           *vi=vd->vi;
  codec_setup_info      *ci=vi->codec_setup;
  bitrate_manager_info  *bi=&ci->bi;
  int                    eofflag=vb->eofflag;
  int                    head=bm->queue_head;
  int                    next_head=head+1;
  int                    bins=bm->queue_bins;
  int                    minmax_head,new_minmax_head;
  
  ogg_uint32_t           *head_ptr;
  oggpack_buffer          temp;

  if(!bm->queue_binned){
    oggpack_buffer temp;
    /* not a bitrate managed stream, but for API simplicity, we'll
       buffer one packet to keep the code path clean */
    
    if(bm->queue_head)return(-1); /* one has been submitted without
                                     being claimed */
    bm->queue_head++;

    bm->packets[0].packet=oggpack_get_buffer(&vb->opb);
    bm->packets[0].bytes=oggpack_bytes(&vb->opb);
    bm->packets[0].b_o_s=0;
    bm->packets[0].e_o_s=vb->eofflag;
    bm->packets[0].granulepos=vb->granulepos;
    bm->packets[0].packetno=vb->sequence; /* for sake of completeness */

    memcpy(&temp,bm->packetbuffers,sizeof(vb->opb));
    memcpy(bm->packetbuffers,&vb->opb,sizeof(vb->opb));
    memcpy(&vb->opb,&temp,sizeof(vb->opb));

    return(0);
  }

  /* add encoded packet to head */
  if(next_head>=bm->queue_size)next_head=0;
  head_ptr=bm->queue_binned+bins*head;

  /* is there room to add a block? In proper use of the API, this will
     never come up... but guard it anyway */
  if(next_head==bm->avg_tail || next_head==bm->minmax_tail)return(-1);

  /* add the block to the toplevel queue */
  bm->queue_head=next_head;
  bm->queue_actual[head]=(vb->W?0x80000000UL:0);

  /* buffer packet fields */
  bm->packets[head].packet=oggpack_get_buffer(&vb->opb);
  bm->packets[head].bytes=oggpack_bytes(&vb->opb);
  bm->packets[head].b_o_s=0;
  bm->packets[head].e_o_s=vb->eofflag;
  bm->packets[head].granulepos=vb->granulepos;
  bm->packets[head].packetno=vb->sequence; /* for sake of completeness */

  /* swap packet buffers */
  memcpy(&temp,bm->packetbuffers+head,sizeof(vb->opb));
  memcpy(bm->packetbuffers+head,&vb->opb,sizeof(vb->opb));
  memcpy(&vb->opb,&temp,sizeof(vb->opb));

  /* save markers */
  head_ptr[0]=vbi->packetblob_markers[0];
  for(i=1;i<PACKETBLOBS;i++){
    head_ptr[i]=vbi->packetblob_markers[i]-vbi->packetblob_markers[i-1];
  }

  if(bm->avg_binacc)
    new_minmax_head=minmax_head=bm->avg_center;
  else
    new_minmax_head=minmax_head=head;

  /* the average tracking queue is updated first; its results (if it's
     in use) are taken into account by the min/max limiter (if min/max
     is in use) */
  if(bm->avg_binacc){
    unsigned long desired_center=bm->avg_centerdesired;
    if(eofflag)desired_center=0;

    /* update the avg head */
    for(i=0;i<bins;i++)
      bm->avg_binacc[i]+=LACING_ADJUST(head_ptr[i]);
    bm->avg_sampleacc+=ci->blocksizes[vb->W]>>1;
    bm->avg_centeracc+=ci->blocksizes[vb->W]>>1;

    if(bm->avg_sampleacc>bm->avg_sampledesired || eofflag){

      /* update the avg center */
      if(bm->avg_centeracc>desired_center){
	/* choose the new average floater */
	int samples=ci->blocksizes[vb->W]>>1;
	double upper=floater_interpolate(bm,vi,bi->queue_avgmax);
	double lower=floater_interpolate(bm,vi,bi->queue_avgmin);
	double new=PACKETBLOBS/2.,slew;
	int bin;
	
	if(upper<new)new=upper;
	if(lower>new)new=lower;
	
	slew=(new-bm->avgfloat)/samples*vi->rate;
	
	if(slew<bi->avgfloat_downslew_max)
	  new=bm->avgfloat+bi->avgfloat_downslew_max/vi->rate*samples;
	if(slew>bi->avgfloat_upslew_max)
	  new=bm->avgfloat+bi->avgfloat_upslew_max/vi->rate*samples;
	
	bm->avgfloat=new;
	/* apply the average floater to new blocks */
	bin=rint(bm->avgfloat);

	/*fprintf(stderr,"%d ",bin);*/
	
	while(bm->avg_centeracc>desired_center){
	  samples=ci->blocksizes[bm->queue_actual[bm->avg_center]&
				0x80000000UL?1:0]>>1;
	  
	  bm->queue_actual[bm->avg_center]|=bin;
	  
	  bm->avg_centeracc-=samples;
	  bm->avg_center++;
	  if(bm->avg_center>=bm->queue_size)bm->avg_center=0;
	}
	new_minmax_head=bm->avg_center;
	
      }
      
      /* update the avg tail if needed */
      while(bm->avg_sampleacc>bm->avg_sampledesired){
	int samples=
	  ci->blocksizes[bm->queue_actual[bm->avg_tail]&0x80000000UL?1:0]>>1;
	for(i=0;i<bm->queue_bins;i++)
	  bm->avg_binacc[i]-=LACING_ADJUST(bm->queue_binned[bins*bm->avg_tail+i]);
	bm->avg_sampleacc-=samples;
	bm->avg_tail++;
	if(bm->avg_tail>=bm->queue_size)bm->avg_tail=0;
      }
      
      
    }
  }else{
    /* if we're not using an average tracker, the 'float' is nailed to
       the avgfloat_initial value.  It needs to be set for the min/max
       to deal properly */
    long bin=PACKETBLOBS/2;
    bm->queue_actual[head]|=bin;
    new_minmax_head=next_head;
  }	
  
  /* update the min/max queues and enforce limits */
  if(bm->minmax_binstack){
    unsigned long sampledesired=eofflag?0:bm->minmax_sampledesired;
    
    /* add to stack recent */
    while(minmax_head!=new_minmax_head){
      unsigned int i;
      int samples=ci->blocksizes[bm->queue_actual[minmax_head]&
				0x80000000UL?1:0]>>1;
      int actual=bm->queue_actual[minmax_head]&0x7fffffffUL;

      for(i=0;i<(unsigned int)bins;i++){
	bm->minmax_binstack[bm->minmax_stackptr*bins*2+bins+i]+=
	  LACING_ADJUST(BINBYTES(bm,minmax_head,
				actual>i?actual:i));
	
	bm->minmax_binstack[bm->minmax_stackptr*bins*2+i]+=
	  LACING_ADJUST(BINBYTES(bm,minmax_head,
				actual<i?actual:i));
      }
      
      bm->minmax_posstack[bm->minmax_stackptr]=minmax_head; /* not one
							       past
							       like
							       typical */
      bm->minmax_limitstack[bm->minmax_stackptr]=0;
      bm->minmax_sampleacc+=samples;
      bm->minmax_acctotal+=
	LACING_ADJUST(BINBYTES(bm,minmax_head,actual));
      
      minmax_head++;
      if(minmax_head>=bm->queue_size)minmax_head=0;


    }
    
    /* check limits, enforce changes */
    if(bm->minmax_sampleacc>sampledesired){
      double bitrate=(double)(bm->minmax_acctotal*8)/
	bm->minmax_sampleacc*vi->rate;
      int limit=0;
      
      if((bi->queue_hardmax>0 && bitrate>bi->queue_hardmax) || 
	 (bi->queue_hardmin>0 && bitrate<bi->queue_hardmin)){
	int newstack;
	int stackctr;
	long bitsum=bm->minmax_acctotal*8;

	bitrate=(double)bitsum/bm->minmax_sampleacc*vi->rate;

	/* we're off rate.  Iteratively try out new hard floater
           limits until we find one that brings us inside.  Here's
           where we see the whole point of the limit stacks.  */
	if(bi->queue_hardmax>0 && bitrate>bi->queue_hardmax){
	  for(limit=-1;limit>-bins+1;limit--){
	    long bitsum=limit_sum(bm,limit)*8;
	    bitrate=(double)bitsum/bm->minmax_sampleacc*vi->rate;
	    if(bitrate<=bi->queue_hardmax)break;
	  }
	}else if(bitrate<bi->queue_hardmin){
	  for(limit=1;limit<bins-1;limit++){
	    long bitsum=limit_sum(bm,limit)*8;
	    bitrate=(double)bitsum/bm->minmax_sampleacc*vi->rate;
	    if(bitrate>=bi->queue_hardmin)break;
	  }
	  if(bitrate>bi->queue_hardmax)limit--;
	}

	/* trace the limit backward, stop when we see a lower limit */
	newstack=bm->minmax_stackptr-1;
	while(newstack>=0){
	  if(bm->minmax_limitstack[newstack]<limit)break;
	  newstack--;
	}
	
	/* update bit counter with new limit and replace any stack
           limits that have been replaced by our new lower limit */
	stackctr=bm->minmax_stackptr;
	while(stackctr>newstack){
	  bm->minmax_acctotal-=
	    LIMITBYTES(stackctr,bm->minmax_limitstack[stackctr]);
	  bm->minmax_acctotal+=LIMITBYTES(stackctr,limit);
	  
	  if(stackctr<bm->minmax_stackptr)
	    for(i=0;i<bins*2;i++)
	      bm->minmax_binstack[stackctr*bins*2+i]+=
		bm->minmax_binstack[(stackctr+1)*bins*2+i];

	  stackctr--;
	}
	stackctr++;
	bm->minmax_posstack[stackctr]=bm->minmax_posstack[bm->minmax_stackptr];
	bm->minmax_limitstack[stackctr]=limit;

	/* set up new blank stack entry */
	stackctr++;
	bm->minmax_stackptr=stackctr;
	memset(&bm->minmax_binstack[stackctr*bins*2],
	       0,
	       sizeof(*bm->minmax_binstack)*bins*2);
	bm->minmax_limitstack[stackctr]=0;
	bm->minmax_posstack[stackctr]=-1;
	
      }
    }
    
    /* remove from tail */
    while(bm->minmax_sampleacc>sampledesired){
      int samples=
	ci->blocksizes[bm->queue_actual[bm->minmax_tail]&0x80000000UL?1:0]>>1;
      int actual=bm->queue_actual[bm->minmax_tail]&0x7fffffffUL;

      for(i=0;i<bins;i++){
	bm->minmax_binstack[bins+i]-= /* always comes off the stack bottom */
	  LACING_ADJUST(BINBYTES(bm,bm->minmax_tail,
				actual>i?
				actual:i));
	bm->minmax_binstack[i]-= 
	  LACING_ADJUST(BINBYTES(bm,bm->minmax_tail,
				actual<i?
				actual:i));
      }

      if(bm->minmax_limitstack[0]>actual)
	actual=bm->minmax_limitstack[0];
      if(bins+bm->minmax_limitstack[0]<actual)
	actual=bins+bm->minmax_limitstack[0];
      
      bm->minmax_acctotal-=LACING_ADJUST(BINBYTES(bm,bm->minmax_tail,actual));
      bm->minmax_sampleacc-=samples;

      /* revise queue_actual to reflect the limit */
      bm->queue_actual[bm->minmax_tail]&=0x80000000UL;
      bm->queue_actual[bm->minmax_tail]|=actual;
      
      if(bm->minmax_tail==bm->minmax_posstack[0]){
	/* the stack becomes a FIFO; the first data has fallen off */
	memmove(bm->minmax_binstack,bm->minmax_binstack+bins*2,
		sizeof(*bm->minmax_binstack)*bins*2*bm->minmax_stackptr);
	memmove(bm->minmax_posstack,bm->minmax_posstack+1,
		sizeof(*bm->minmax_posstack)*bm->minmax_stackptr);
	memmove(bm->minmax_limitstack,bm->minmax_limitstack+1,
		sizeof(*bm->minmax_limitstack)*bm->minmax_stackptr);
	bm->minmax_stackptr--;
      }
      
      bm->minmax_tail++;
      if(bm->minmax_tail>=bm->queue_size)bm->minmax_tail=0;

    }
    
    
    bm->last_to_flush=bm->minmax_tail;
  }else{
    bm->last_to_flush=bm->avg_center;
  }
  if(eofflag)
    bm->last_to_flush=bm->queue_head;
  return(0);
}

int __fastcall vorbis_bitrate_flushpacket(vorbis_dsp_state *vd,ogg_packet *op){
  private_state         *b=vd->backend_state;
  bitrate_manager_state *bm=&b->bms;

  if(bm->queue_size==0){
    if(bm->queue_head==0)return(0);

    memcpy(op,bm->packets,sizeof(*op));
    bm->queue_head=0;

  }else{

    if(bm->next_to_flush==bm->last_to_flush)return(0);

    {
      long bin=bm->queue_actual[bm->next_to_flush]&0x7fffffff,i;
      long bins=bm->queue_bins;
      ogg_uint32_t *markers=bm->queue_binned+bins*bm->next_to_flush;
      long bytes=markers[bin];

      memcpy(op,bm->packets+bm->next_to_flush,sizeof(*op));

      /* we have [PACKETBLOBS] possible packets all squished together in
	 the buffer, in sequence.  count in to number [bin] */
      for(i=0;i<bin;i++)
	op->packet+=markers[i];
      op->bytes=bytes;
	
    }

    bm->next_to_flush++;
    if(bm->next_to_flush>=bm->queue_size)bm->next_to_flush=0;

  }

  return(1);
}
