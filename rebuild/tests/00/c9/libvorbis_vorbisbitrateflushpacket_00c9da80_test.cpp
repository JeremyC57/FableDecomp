// Behaviour test for libvorbis bitrate.c vorbis_bitrate_flushpacket (retail 0x00C9DA80): a copy of the packet output over a model
// manager -- the unmanaged single buffered packet, then the managed queue where the block's chosen bin picks one of the PACKETBLOBS
// blobs (packet pointer advanced past the earlier blobs, length = that blob's bytes), up to last_to_flush, wrapping at queue_size.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
typedef unsigned int ogg_uint32_t;
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
typedef long long ogg_int64_t;
#endif
#define PACKETBLOBS 15
#define rint(x) (floor((x) + 0.5f))
typedef struct { long endbyte; int endbit; unsigned char *buffer; unsigned char *ptr; long storage; } oggpack_buffer;
typedef struct { unsigned char *packet; long bytes; long b_o_s; long e_o_s; ogg_int64_t granulepos; ogg_int64_t packetno; } ogg_packet;
typedef struct bitrate_manager_info { double queue_avg_time, queue_avg_center, queue_minmax_time, queue_hardmin, queue_hardmax, queue_avgmin, queue_avgmax, avgfloat_downslew_max, avgfloat_upslew_max; } bitrate_manager_info;
typedef struct { long blocksizes[2]; bitrate_manager_info bi; } codec_setup_info;
typedef struct vorbis_info { int version; int channels; long rate; long bitrate_upper; long bitrate_nominal; long bitrate_lower; long bitrate_window; void *codec_setup; } vorbis_info;
typedef struct vorbis_dsp_state { int analysisp; vorbis_info *vi; float **pcm; float **pcmret; int pcm_storage; int pcm_current; int pcm_returned; int preextrapolate; int eofflag; long lW; long W; long nW; long centerW; ogg_int64_t granulepos; ogg_int64_t sequence; ogg_int64_t glue_bits, time_bits, floor_bits, res_bits; void *backend_state; } vorbis_dsp_state;
typedef struct bitrate_manager_state {
  ogg_uint32_t *queue_binned; ogg_uint32_t *queue_actual; int queue_size; int queue_head; int queue_bins;
  long *avg_binacc; int avg_center; int avg_tail; ogg_uint32_t avg_centeracc; ogg_uint32_t avg_sampleacc; ogg_uint32_t avg_sampledesired; ogg_uint32_t avg_centerdesired;
  long *minmax_binstack; long *minmax_posstack; long *minmax_limitstack; long minmax_stackptr; long minmax_acctotal; int minmax_tail; ogg_uint32_t minmax_sampleacc; ogg_uint32_t minmax_sampledesired;
  int next_to_flush; int last_to_flush; double avgfloat; oggpack_buffer *packetbuffers; ogg_packet *packets;
} bitrate_manager_state;
typedef struct private_state { bitrate_manager_state bms; } private_state;   /* the model keeps only the manager */
typedef struct vorbis_block { float **pcm; oggpack_buffer opb; long lW; long W; long nW; int pcmend; int mode; int eofflag; ogg_int64_t granulepos; ogg_int64_t sequence; vorbis_dsp_state *vd; void *localstore; long localtop; long localalloc; long totaluse; void *reap; long glue_bits, time_bits, floor_bits, res_bits; void *internal; } vorbis_block;
typedef struct vorbis_block_internal { float **pcmdelay; float ampmax; int blocktype; ogg_uint32_t packetblob_markers[PACKETBLOBS]; } vorbis_block_internal;
static long oggpack_bytes(oggpack_buffer* b){ return(b->endbyte+(b->endbit+7)/8); }
static unsigned char* oggpack_get_buffer(oggpack_buffer* b){ return(b->buffer); }

static long BINBYTES(bitrate_manager_state *bm,long pos,long bin){
  int bins=bm->queue_bins;
  return(bm->queue_binned[pos*bins+bin]);
}

#define LIMITBYTES(pos,bin) (bm->minmax_binstack[(pos)*bins*2+((bin)+bins)])

static long LACING_ADJUST(long bytes){
  int addto=bytes/255+1;
  return(bytes+addto);
}

static int floater_interpolate(bitrate_manager_state *bm,vorbis_info *vi,
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
static long limit_sum(bitrate_manager_state *bm,int limit){
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

static int vorbis_bitrate_addblock(vorbis_block *vb){
  int i; 
  vorbis_block_internal *vbi=(vorbis_block_internal*)vb->internal;
  vorbis_dsp_state      *vd=vb->vd;
  private_state         *b=(private_state*)vd->backend_state; 
  bitrate_manager_state *bm=&b->bms;
  vorbis_info           *vi=vd->vi;
  codec_setup_info      *ci=(codec_setup_info*)vi->codec_setup;
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
	double newf=PACKETBLOBS/2.,slew;
	int bin;
	
	if(upper<newf)newf=upper;
	if(lower>newf)newf=lower;
	
	slew=(newf-bm->avgfloat)/samples*vi->rate;
	
	if(slew<bi->avgfloat_downslew_max)
	  newf=bm->avgfloat+bi->avgfloat_downslew_max/vi->rate*samples;
	if(slew>bi->avgfloat_upslew_max)
	  newf=bm->avgfloat+bi->avgfloat_upslew_max/vi->rate*samples;
	
	bm->avgfloat=newf;
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

static int vorbis_bitrate_flushpacket(vorbis_dsp_state *vd,ogg_packet *op){
  private_state         *b=(private_state*)vd->backend_state;
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

/* a manager as vorbis_bitrate_init builds it for the given queue size / bins, or an unmanaged one (queue_size 0, no bins) */
static void setup(bitrate_manager_state *bm, int size, int bins, int avg, int minmax){
  memset(bm,0,sizeof(*bm));
  if(size){
    bm->queue_size=size; bm->queue_bins=bins;
    bm->queue_binned=(ogg_uint32_t*)calloc(size*bins,sizeof(ogg_uint32_t));
    bm->queue_actual=(ogg_uint32_t*)calloc(size,sizeof(ogg_uint32_t));
    if(avg){ bm->avg_binacc=(long*)calloc(bins,sizeof(long)); bm->avg_sampledesired=avg; bm->avg_centerdesired=avg/2; }
    if(minmax){ bm->minmax_binstack=(long*)calloc((size+1)*bins*2,sizeof(long)); bm->minmax_posstack=(long*)calloc(size+1,sizeof(long)); bm->minmax_limitstack=(long*)calloc(size+1,sizeof(long)); bm->minmax_sampledesired=minmax; }
    bm->avgfloat=PACKETBLOBS/2.;
  }
  bm->packetbuffers=(oggpack_buffer*)calloc(size?size:1,sizeof(oggpack_buffer));
  bm->packets=(ogg_packet*)calloc(size?size:1,sizeof(ogg_packet));
}
/* a block whose packet is `bytes` bytes in blob 0, each further blob 10 bytes longer, with the marker list that implies */
static void block(vorbis_block *vb, vorbis_block_internal *vbi, vorbis_dsp_state *vd, unsigned char *buf, int bytes, int W, int seq, int eof){
  int i;
  memset(vb,0,sizeof(*vb)); memset(vbi,0,sizeof(*vbi));
  vb->vd=vd; vb->internal=vbi; vb->W=W; vb->eofflag=eof; vb->sequence=seq; vb->granulepos=seq*1024;
  vb->opb.buffer=vb->opb.ptr=buf; vb->opb.endbyte=bytes; vb->opb.storage=bytes+200;
  for(i=0;i<PACKETBLOBS;i++) vbi->packetblob_markers[i]=bytes+10*i;
}
static int fail(const char* what) { std::printf("%s\n00c9da80_TEST FAIL\n", what); return 1; }

int main()
{
    vorbis_info vi; codec_setup_info ci; vorbis_dsp_state vd; private_state ps; vorbis_block vb; vorbis_block_internal vbi; ogg_packet op;
    unsigned char buf[600]; int i;
    memset(&vi, 0, sizeof vi); memset(&ci, 0, sizeof ci); memset(&vd, 0, sizeof vd); memset(&ps, 0, sizeof ps);
    vi.rate = 44100; vi.codec_setup = &ci; ci.blocksizes[0] = 256; ci.blocksizes[1] = 2048;
    vd.vi = &vi; vd.backend_state = &ps;
    for (i = 0; i < 600; i++) buf[i] = (unsigned char)i;

    /* unmanaged: nothing queued -> 0; one queued -> the packet comes back and the queue empties */
    setup(&ps.bms, 0, 0, 0, 0);
    if (vorbis_bitrate_flushpacket(&vd, &op) != 0) return fail("unmanaged empty");
    block(&vb, &vbi, &vd, buf, 300, 0, 5, 1);
    vorbis_bitrate_addblock(&vb);
    if (vorbis_bitrate_flushpacket(&vd, &op) != 1) return fail("unmanaged flush");
    if (op.packet != buf || op.bytes != 300 || op.e_o_s != 1 || op.packetno != 5 || ps.bms.queue_head != 0) return fail("unmanaged packet");
    if (vorbis_bitrate_flushpacket(&vd, &op) != 0) return fail("unmanaged drained");

    /* managed: packets come out up to last_to_flush; the bin chosen for a block selects the blob (offset by the earlier blobs'
       marker bytes, length = that blob's bytes) */
    setup(&ps.bms, 8, PACKETBLOBS, 0, 0);
    block(&vb, &vbi, &vd, buf, 120, 0, 1, 0); vorbis_bitrate_addblock(&vb);
    block(&vb, &vbi, &vd, buf + 300, 100, 1, 2, 0); vorbis_bitrate_addblock(&vb);
    ps.bms.queue_actual[0] = 3;                 /* force blob 3 for the first block: skips 120 + 10 + 10 bytes, length 10 */
    ps.bms.last_to_flush = 2;
    if (vorbis_bitrate_flushpacket(&vd, &op) != 1) return fail("managed flush 1");
    if (op.packet != buf + 140 || op.bytes != 10 || op.packetno != 1 || ps.bms.next_to_flush != 1) return fail("managed blob 3");
    if (vorbis_bitrate_flushpacket(&vd, &op) != 1) return fail("managed flush 2");
    if (op.packet != buf + 300 + 100 + 10 * 6 || op.bytes != 10 || op.packetno != 2 || ps.bms.next_to_flush != 2) return fail("managed blob 7");
    if (vorbis_bitrate_flushpacket(&vd, &op) != 0) return fail("managed drained");
    /* the flush index wraps at queue_size */
    ps.bms.next_to_flush = 7; ps.bms.last_to_flush = 0; ps.bms.queue_actual[7] = 0;
    if (vorbis_bitrate_flushpacket(&vd, &op) != 1 || ps.bms.next_to_flush != 0) return fail("wrap");
    std::printf("00c9da80_TEST PASS\n");
    return 0;
}