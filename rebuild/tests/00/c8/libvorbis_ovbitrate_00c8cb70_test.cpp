// Behaviour test for libvorbis vorbisfile.c ov_bitrate (retail 0x00C8CB70): a copy over a model OggVorbis_File with two
// links -- OV_EINVAL before OPENED and past the last link, the whole-file bitrate (bits of every link over the total time,
// rounded), one seekable link's bitrate, and the unseekable answers (link 0 for any i; nominal, (upper+lower)/2, upper, OV_FALSE).
#include <cstdio>
#include <cstring>
#include <cmath>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
typedef long long ogg_int64_t;
#endif
typedef struct vorbis_info { int version; int channels; long rate; long bitrate_upper; long bitrate_nominal; long bitrate_lower; long bitrate_window; void *codec_setup; } vorbis_info;
typedef struct OggVorbis_File {
    void *datasource; int seekable; ogg_int64_t offset; ogg_int64_t end; unsigned char oy[28];
    int links; ogg_int64_t *offsets; ogg_int64_t *dataoffsets; long *serialnos; ogg_int64_t *pcmlengths;
    vorbis_info *vi; void *vc; ogg_int64_t pcm_offset; int ready_state;
} OggVorbis_File;
#define OV_FALSE  -1
#define OV_EINVAL -131
#define OPENED 2
#define rint(x) (floor((x)+0.5f))
#ifndef _MSC_VER
#include <cstdlib>
static inline float floorf_(float x) { return (float)std::floor((double)x); }
#define floorf floorf_
#endif
/* VC7.1 <math.h> gives C++ a float overload; g++ has it in <cmath> through std::floor(float) -- both round the float result */
static long rint_long(double x) { return (long)std::floor(x + 0.5f); }
static long rint_long_f(float x) { return (long)(float)std::floor((double)(x + 0.5f)); }

static double ov_time_total(OggVorbis_File *vf,int i){
  if(vf->ready_state<OPENED)return(OV_EINVAL);
  if(!vf->seekable || i>=vf->links)return(OV_EINVAL);
  if(i<0){
    double acc=0; int i;
    for(i=0;i<vf->links;i++) acc+=ov_time_total(vf,i);
    return(acc);
  }else{
    return((double)(vf->pcmlengths[i*2+1])/vf->vi[i].rate);
  }
}

static long ov_bitrate(OggVorbis_File *vf,int i){
  if(vf->ready_state<OPENED)return(OV_EINVAL);
  if(i>=vf->links)return(OV_EINVAL);
  if(!vf->seekable && i!=0)return(ov_bitrate(vf,0));
  if(i<0){
    ogg_int64_t bits=0;
    int i;
    float br;
    for(i=0;i<vf->links;i++)
      bits+=(vf->offsets[i+1]-vf->dataoffsets[i])*8;
    double seconds=ov_time_total(vf,-1);
    br = bits/seconds;
    return(rint_long_f(br));
  }else{
    if(vf->seekable){
      return(rint_long((vf->offsets[i+1]-vf->dataoffsets[i])*8/ov_time_total(vf,i)));
    }else{
      if(vf->vi[i].bitrate_nominal>0){
	return vf->vi[i].bitrate_nominal;
      }else{
	if(vf->vi[i].bitrate_upper>0){
	  if(vf->vi[i].bitrate_lower>0){
	    return (vf->vi[i].bitrate_upper+vf->vi[i].bitrate_lower)/2;
	  }else{
	    return vf->vi[i].bitrate_upper;
	  }
	}
	return(OV_FALSE);
      }
    }
  }
}
static int fail(const char* w){ std::printf("%s\n00c8cb70_TEST FAIL\n", w); return 1; }

int main()
{
    OggVorbis_File vf; vorbis_info vi[2];
    ogg_int64_t offsets[3] = { 0, 100000, 400000 }, dataoffsets[2] = { 4000, 110000 }, pcmlengths[4] = { 0, 44100 * 10, 0, 44100 * 5 };
    std::memset(&vf, 0, sizeof(vf)); std::memset(vi, 0, sizeof(vi));
    vi[0].rate = 44100; vi[1].rate = 44100;
    vf.links = 2; vf.offsets = offsets; vf.dataoffsets = dataoffsets; vf.pcmlengths = pcmlengths; vf.vi = vi; vf.seekable = 1;
    vf.ready_state = 1;
    if (ov_bitrate(&vf, 0) != OV_EINVAL) return fail("not opened");
    vf.ready_state = OPENED;
    if (ov_bitrate(&vf, 2) != OV_EINVAL) return fail("past last link");
    /* link 0: 96000 bytes in 10 s = 76800 bps; link 1: 290000 bytes in 5 s = 464000 bps; whole: 386000*8/15 = 205866.67 -> 205867 */
    if (ov_bitrate(&vf, 0) != 76800) return fail("link 0");
    if (ov_bitrate(&vf, 1) != 464000) return fail("link 1");
    if (ov_bitrate(&vf, -1) != 205867) return fail("whole file");
    /* unseekable: link 0 answers for every i, from the header hints */
    vf.seekable = 0;
    vi[0].bitrate_nominal = 128000;
    if (ov_bitrate(&vf, 1) != 128000 || ov_bitrate(&vf, 0) != 128000) return fail("nominal");
    vi[0].bitrate_nominal = 0; vi[0].bitrate_upper = 160000; vi[0].bitrate_lower = 96000;
    if (ov_bitrate(&vf, 0) != 128000) return fail("mean");
    vi[0].bitrate_lower = 0;
    if (ov_bitrate(&vf, 0) != 160000) return fail("upper");
    vi[0].bitrate_upper = 0;
    if (ov_bitrate(&vf, 0) != OV_FALSE) return fail("false");
    std::printf("00c8cb70_TEST PASS\n");
    return 0;
}