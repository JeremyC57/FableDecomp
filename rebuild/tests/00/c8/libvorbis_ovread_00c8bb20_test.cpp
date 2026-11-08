// Behaviour test for libvorbis vorbisfile.c ov_read (retail 0x00C8BB20): a copy of the PCM decode entry over a scripted decoder --
// OV_EINVAL before the file is open, 0 at end of stream and errors passed through, then packing of whole frames only into 16-bit
// signed / unsigned host-order, big-endian byte pairs and 8-bit signed / unsigned samples with clipping, the decoder told how many
// samples were taken, pcm_offset advanced and the current link reported.
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

static int fail(const char* what) { std::printf("%s\n00c8bb20_TEST FAIL\n", what); return 1; }

static void reset(OggVorbis_File *vf){
    int i;
    memset(vf, 0, sizeof *vf); vf->ready_state = INITSET; vf->info.channels = 2; vf->current_link = 3;
    for (i = 0; i < 16; i++) { gPcm[0][i] = (float)(i - 8) / 8.f; gPcm[1][i] = (float)i * 0.03125f; }
    gPcm[0][15] = 2.f; gPcm[1][15] = -3.f;          /* out of range: clipped */
    gAvail = 0; gRead = 0; gFetches = 0; gFetchResult = 1; gArmOnFetch = 16;
}

int main()
{
    OggVorbis_File vf; char buf[128]; int bs = -1, i, j; long r;
    /* not open */
    reset(&vf); vf.ready_state = OPENED - 1;
    if (ov_read(&vf, buf, 64, 0, 2, 1, &bs) != OV_EINVAL) return fail("not open");
    /* end of stream / error passed through */
    reset(&vf); gFetchResult = OV_EOF;
    if (ov_read(&vf, buf, 64, 0, 2, 1, &bs) != 0 || gFetches != 1) return fail("eof");
    reset(&vf); gFetchResult = -3;
    if (ov_read(&vf, buf, 64, 0, 2, 1, &bs) != -3) return fail("hole");
    /* 16-bit signed host order (as COggVorbisStream::Read): 64 bytes = 16 frames of 2 channels, interleaved, clipped */
    reset(&vf);
    r = ov_read(&vf, buf, 64, 0, 2, 1, &bs);
    if (r != 64 || gFetches != 1 || gRead != 16 || gAvail != 0 || vf.pcm_offset != 16 || bs != 3) return fail("16-bit signed return");
    for (j = 0; j < 16; j++) for (i = 0; i < 2; i++) {
        short want = (short)vorbis_ftoi(gPcm[i][j] * 32768.f); if (gPcm[i][j] * 32768.f > 32767.f) want = 32767; if (gPcm[i][j] * 32768.f < -32768.f) want = -32768;
        if (((short*)buf)[j * 2 + i] != want) return fail("16-bit signed samples");
    }
    /* a short buffer takes whole frames only: 10 bytes = 2 frames, 4 bytes consumed per frame */
    reset(&vf); gAvail = 16;
    if (ov_read(&vf, buf, 10, 0, 2, 1, &bs) != 8 || gRead != 2 || gAvail != 14 || gFetches != 0) return fail("partial frames");
    /* 16-bit unsigned: offset 32768 */
    reset(&vf); gAvail = 16;
    if (ov_read(&vf, buf, 64, 0, 2, 0, &bs) != 64) return fail("16-bit unsigned return");
    for (j = 0; j < 16; j++) for (i = 0; i < 2; i++) {
        int v = vorbis_ftoi(gPcm[i][j] * 32768.f); if (v > 32767) v = 32767; if (v < -32768) v = -32768;
        if (((unsigned short*)buf)[j * 2 + i] != (unsigned short)(v + 32768)) return fail("16-bit unsigned samples");
    }
    /* 16-bit big endian (host is little): high byte first */
    reset(&vf); gAvail = 16;
    if (ov_read(&vf, buf, 64, 1, 2, 1, &bs) != 64) return fail("big endian return");
    for (j = 0; j < 16; j++) for (i = 0; i < 2; i++) {
        int v = vorbis_ftoi(gPcm[i][j] * 32768.f); if (v > 32767) v = 32767; if (v < -32768) v = -32768;
        if ((unsigned char)buf[(j * 2 + i) * 2] != (unsigned char)((v >> 8) & 0xff) || (unsigned char)buf[(j * 2 + i) * 2 + 1] != (unsigned char)(v & 0xff)) return fail("big endian bytes");
    }
    /* 8-bit signed and unsigned (offset 128), clipped to -128..127 */
    reset(&vf); gAvail = 16;
    if (ov_read(&vf, buf, 32, 0, 1, 1, &bs) != 32) return fail("8-bit signed return");
    for (j = 0; j < 16; j++) for (i = 0; i < 2; i++) {
        int v = vorbis_ftoi(gPcm[i][j] * 128.f); if (v > 127) v = 127; if (v < -128) v = -128;
        if ((signed char)buf[j * 2 + i] != v) return fail("8-bit signed samples");
    }
    reset(&vf); gAvail = 16;
    if (ov_read(&vf, buf, 32, 0, 1, 0, NULL) != 32) return fail("8-bit unsigned return");
    for (j = 0; j < 16; j++) for (i = 0; i < 2; i++) {
        int v = vorbis_ftoi(gPcm[i][j] * 128.f); if (v > 127) v = 127; if (v < -128) v = -128;
        if ((unsigned char)buf[j * 2 + i] != (unsigned char)(v + 128)) return fail("8-bit unsigned samples");
    }
    /* a buffer smaller than one frame: OV_EINVAL */
    reset(&vf); gAvail = 16;
    if (ov_read(&vf, buf, 3, 0, 2, 1, &bs) != OV_EINVAL) return fail("sub-frame buffer");
    std::printf("00c8bb20_TEST PASS\n");
    return 0;
}