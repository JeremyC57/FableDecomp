// Behaviour test for libvorbis vorbisfile.c ov_read_float (retail 0x00C8BEE0): a copy of the float decode entry over a scripted
// decoder -- OV_EINVAL before the file is open, 0 at end of stream and errors passed through, the decoder's channel pointers
// handed back with the sample count clamped to `length`, the samples consumed, pcm_offset advanced and the current link reported.
#include <cstdio>
#include <cstring>
#include <cmath>
#define NULL 0
#define OV_EOF -2
#define OV_EINVAL -131
#define OPENED 2
#define STREAMSET 3
#define INITSET 4
typedef struct vorbis_info { int version; int channels; long rate; } vorbis_info;
typedef struct { int dummy; } vorbis_dsp_state;
typedef struct ogg_packet ogg_packet;
typedef struct OggVorbis_File { vorbis_dsp_state vd; long pcm_offset; int ready_state; int current_link; vorbis_info info; } OggVorbis_File;

/* model decoder: a scripted supply of PCM frames; each fetch either arms `gAvail` samples, reports a hole, or the end */
static float gPcm[2][16]; static float *gPcmPtr[2] = { gPcm[0], gPcm[1] };
static long gAvail = 0, gRead = 0; static int gFetches = 0, gFetchResult = 1, gArmOnFetch = 0;
static int host_is_big_endian(void){ return 0; }
static int vorbis_synthesis_pcmout(vorbis_dsp_state*, float ***pcm){ if (pcm) *pcm = gPcmPtr; return gAvail; }
static int _fetch_and_process_packet(OggVorbis_File *vf, ogg_packet*, int, int){ gFetches++; if (gFetchResult > 0) gAvail = gArmOnFetch; return gFetchResult; }
static vorbis_info *ov_info(OggVorbis_File *vf, int){ return &vf->info; }
static void vorbis_synthesis_read(vorbis_dsp_state*, int samples){ gRead += samples; gAvail -= samples; }
/* os.h vorbis_ftoi: round to nearest, ties to even (the x87 default rounding) */
#define vorbis_fpu_setround(vorbis_fpu_control) {}
#define vorbis_fpu_restore(vorbis_fpu_control) {}
typedef int vorbis_fpu_control;
static int vorbis_ftoi(double f){ double r = std::floor(f + 0.5); if (r - f == 0.5 && std::fmod(r, 2.0) != 0.0) r -= 1.0; return (int)r; }
static long ov_read(OggVorbis_File *vf,char *buffer,int length,
		    int bigendianp,int word,int sgned,int *bitstream){
  int i,j;
  int host_endian = host_is_big_endian();

  float **pcm;
  long samples;

  if(vf->ready_state<OPENED)return(OV_EINVAL);

  while(1){
    if(vf->ready_state==INITSET){
      samples=vorbis_synthesis_pcmout(&vf->vd,&pcm);
      if(samples)break;
    }

    /* suck in another packet */
    {
      int ret=_fetch_and_process_packet(vf,NULL,1,1);
      if(ret==OV_EOF)
	return(0);
      if(ret<=0)
	return(ret);
    }

  }

  if(samples>0){
  
    /* yay! proceed to pack data into the byte buffer */
    
    long channels=ov_info(vf,-1)->channels;
    long bytespersample=word * channels;
    vorbis_fpu_control fpu;
    if(samples>length/bytespersample)samples=length/bytespersample;

    if(samples <= 0)
      return OV_EINVAL;
    
    /* a tight loop to pack each size */
    {
      int val=0;
      if(word==1){
	int off=(sgned?0:128);
	vorbis_fpu_setround(&fpu);
	for(j=0;j<samples;j++)
	  for(i=0;i<channels;i++){
	    val=vorbis_ftoi(pcm[i][j]*128.f);
	    if(val>127)val=127;
	    else if(val<-128)val=-128;
	    *buffer++=val+off;
	  }
	vorbis_fpu_restore(fpu);
      }else{
	int off=(sgned?0:32768);
	
	if(host_endian==bigendianp){
	  if(sgned){
	    
	    vorbis_fpu_setround(&fpu);
	    for(i=0;i<channels;i++) { /* It's faster in this order */
	      float *src=pcm[i];
	      short *dest=((short *)buffer)+i;
	      for(j=0;j<samples;j++) {
		val=vorbis_ftoi(src[j]*32768.f);
		if(val>32767)val=32767;
		else if(val<-32768)val=-32768;
		*dest=val;
		dest+=channels;
	      }
	    }
	    vorbis_fpu_restore(fpu);
	    
	  }else{
	    
	    vorbis_fpu_setround(&fpu);
	    for(i=0;i<channels;i++) {
	      float *src=pcm[i];
	      short *dest=((short *)buffer)+i;
	      for(j=0;j<samples;j++) {
		val=vorbis_ftoi(src[j]*32768.f);
		if(val>32767)val=32767;
		else if(val<-32768)val=-32768;
		*dest=val+off;
		dest+=channels;
	      }
	    }
	    vorbis_fpu_restore(fpu);
	    
	  }
	}else if(bigendianp){
	  
	  vorbis_fpu_setround(&fpu);
	  for(j=0;j<samples;j++)
	    for(i=0;i<channels;i++){
	      val=vorbis_ftoi(pcm[i][j]*32768.f);
	      if(val>32767)val=32767;
	      else if(val<-32768)val=-32768;
	      val+=off;
	      *buffer++=(val>>8);
	      *buffer++=(val&0xff);
	    }
	  vorbis_fpu_restore(fpu);
	  
	}else{
	  int val;
	  vorbis_fpu_setround(&fpu);
	  for(j=0;j<samples;j++)
	    for(i=0;i<channels;i++){
	      val=vorbis_ftoi(pcm[i][j]*32768.f);
	      if(val>32767)val=32767;
	      else if(val<-32768)val=-32768;
	      val+=off;
	      *buffer++=(val&0xff);
	      *buffer++=(val>>8);
	  	}
	  vorbis_fpu_restore(fpu);  
	  
	}
      }
    }
    
    vorbis_synthesis_read(&vf->vd,samples);
    vf->pcm_offset+=samples;
    if(bitstream)*bitstream=vf->current_link;
    return(samples*bytespersample);
  }else{
    return(samples);
  }
}

static long ov_read_float(OggVorbis_File *vf,float ***pcm_channels,int length,
		   int *bitstream){

  if(vf->ready_state<OPENED)return(OV_EINVAL);

  while(1){
    if(vf->ready_state==INITSET){
      float **pcm;
      long samples=vorbis_synthesis_pcmout(&vf->vd,&pcm);
      if(samples){
	if(pcm_channels)*pcm_channels=pcm;
	if(samples>length)samples=length;
	vorbis_synthesis_read(&vf->vd,samples);
	vf->pcm_offset+=samples;
	if(bitstream)*bitstream=vf->current_link;
	return samples;

      }
    }

    /* suck in another packet */
    {
      int ret=_fetch_and_process_packet(vf,NULL,1,1);
      if(ret==OV_EOF)return(0);
      if(ret<=0)return(ret);
    }

  }
}

static int fail(const char* what) { std::printf("%s\n00c8bee0_TEST FAIL\n", what); return 1; }

static void reset(OggVorbis_File *vf){
    int i;
    memset(vf, 0, sizeof *vf); vf->ready_state = INITSET; vf->info.channels = 2; vf->current_link = 3;
    for (i = 0; i < 16; i++) { gPcm[0][i] = (float)(i - 8) / 8.f; gPcm[1][i] = (float)i * 0.03125f; }
    gPcm[0][15] = 2.f; gPcm[1][15] = -3.f;          /* out of range: clipped */
    gAvail = 0; gRead = 0; gFetches = 0; gFetchResult = 1; gArmOnFetch = 16;
}

int main()
{
    OggVorbis_File vf; float **pcm = 0; int bs = -1; long r;
    reset(&vf); vf.ready_state = OPENED - 1;
    if (ov_read_float(&vf, &pcm, 64, &bs) != OV_EINVAL) return fail("not open");
    reset(&vf); gFetchResult = OV_EOF;
    if (ov_read_float(&vf, &pcm, 64, &bs) != 0 || gFetches != 1) return fail("eof");
    reset(&vf); gFetchResult = -3;
    if (ov_read_float(&vf, &pcm, 64, &bs) != -3) return fail("hole");
    /* a fetch arms 16 samples: all 16 handed over as the decoder's own channel pointers, consumed, offset / link reported */
    reset(&vf);
    r = ov_read_float(&vf, &pcm, 64, &bs);
    if (r != 16 || pcm != gPcmPtr || gRead != 16 || gAvail != 0 || vf.pcm_offset != 16 || bs != 3 || gFetches != 1) return fail("full read");
    /* length clamps the sample count; the rest stays in the decoder */
    reset(&vf); gAvail = 16;
    if (ov_read_float(&vf, &pcm, 5, NULL) != 5 || gRead != 5 || gAvail != 11 || gFetches != 0 || vf.pcm_offset != 5) return fail("clamped read");
    /* a NULL channel pointer is allowed */
    reset(&vf); gAvail = 4;
    if (ov_read_float(&vf, NULL, 64, &bs) != 4) return fail("null pcm_channels");
    /* a stream not yet INITSET fetches first (the fetch sets up the decoder in the real file) */
    reset(&vf); vf.ready_state = STREAMSET; gAvail = 16;
    gFetchResult = OV_EOF;
    if (ov_read_float(&vf, &pcm, 64, &bs) != 0 || gFetches != 1) return fail("streamset fetch");
    std::printf("00c8bee0_TEST PASS\n");
    return 0;
}
