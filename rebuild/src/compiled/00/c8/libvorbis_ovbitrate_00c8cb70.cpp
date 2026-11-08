// libvorbis::ov_bitrate(OggVorbis_File* vf, int i) -- vorbisfile.c: average bitrate of logical bitstream i -- OV_EINVAL before the file is open or past the last link; an unseekable file answers for link 0; i < 0 sums the byte lengths of every link and divides by the total playing time; a seekable link divides its own byte length by its time (rint = floor(x + .5)); an unseekable link reports the nominal bitrate, else the mean of upper and lower (or upper alone), else OV_FALSE
// Leaf: @ov_bitrate@8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8CB70, 382 bytes, `__fastcall` (vf in ecx, i in edx), plain `ret`, returns long.
//   Callees: __allmul 0x00BFEB50 (the ogg_int64_t `*8`), ov_time_total 0x00C8AAF0 (`@ov_time_total@8`, landed), floor through
//   the IAT [0x01440100] (os.h rint), __ftol2 0x00BFEA70 (the long conversion, tail-jumped after each epilogue). Constants:
//   float .5 0x0122F59C (the float `br + .5f`), double .5 0x012316F0 (the double expression `+ .5f`).
// Identity (two sources): (1) the body is libvorbis 1.0.1 vorbisfile.c ov_bitrate -- OV_EINVAL (-131) on ready_state +0x58 <
//   OPENED and on i >= links +0x34, the `!seekable && i!=0 -> ov_bitrate(vf,0)` tail recursion VC7.1 turns into the
//   `xor esi,esi; jmp` loop, the i < 0 sum `bits += (offsets[i+1]-dataoffsets[i])*8` kept in a 64-bit frame slot and converted
//   through `fild qword` into the FLOAT `br` (hence the dword .5), the seekable link's double expression (qword .5), and the
//   nominal / (upper+lower)/2 / upper / OV_FALSE chain on vi[i] (0x20-byte vorbis_info); (2) vorbisfile.c file order:
//   ov_bitrate is the one ov_* entry between ov_test_callbacks 0x00C8CB20 and the landed ov_bitrate_instant 0x00C8A960 /
//   ov_raw_total 0x00C8AA00 / ov_time_total 0x00C8AAF0 family (the libpk LTCG link reordered vorbisfile.c's functions).
//   The catalog name "Audio_GetSamplePosition" is a descriptive guess.
// Shape (probe-proven, see NOTES.md): the plain 1.0.1 text lets VC7.1 cross-jump the two `rint` tails into one
//   (354 bytes); retail keeps both tails because the two long conversions differ: libpk compiles vorbisfile.c as C++, so
//   `floor((br)+0.5f)` on the FLOAT br resolves to VC7.1 <math.h>'s C++ overload `float floor(float)` -> `floorf` ->
//   `(float)floor((double)x)` (the two x87 no-op conversions make the tails differ at the tuple level, no bytes), while the
//   seekable link's double expression takes the double floor. The overload chain is written out here exactly as math.h
//   declares it, over the dllimport `floor` the row needs for the IAT call. The remaining residue was the operand order of
//   `lea ecx,[esi+eax]` (&vf->vi[i]); naming the total time (`double seconds`) before the division restores it with no
//   other byte change (five other one-tuple perturbations did not), so the time local is kept: the libpk vorbisfile.c is a
//   modified copy and this is the one spelling of that line that matches. Every vorbisfile.c entry point is extern "C"
//   __fastcall (the TU is /Gr); the structures are the landed ov_bitrate_instant TU's (libpk's OggVorbis_File without
//   ov_callbacks, 0x2C0 bytes).
// Original libvorbis vorbisfile.c; BSD-style terms in reference COPYING.
// ---- libogg 1.1 / libvorbis 1.0.1 (20030909) public structures, as statically linked into libpk ----
typedef __int64 ogg_int64_t;

typedef struct {
    long endbyte;
    int endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long storage;
} oggpack_buffer;

typedef struct {
    unsigned char* data;
    int storage;
    int fill;
    int returned;
    int unsynced;
    int headerbytes;
    int bodybytes;
} ogg_sync_state;

typedef struct {
    unsigned char* body_data;
    long body_storage;
    long body_fill;
    long body_returned;
    int* lacing_vals;
    ogg_int64_t* granule_vals;
    long lacing_storage;
    long lacing_fill;
    long lacing_packet;
    long lacing_returned;
    unsigned char header[282];
    int header_fill;
    int e_o_s;
    int b_o_s;
    long serialno;
    long pageno;
    ogg_int64_t packetno;
    ogg_int64_t granulepos;
} ogg_stream_state;

typedef struct vorbis_info {
    int version;
    int channels;
    long rate;
    long bitrate_upper;
    long bitrate_nominal;
    long bitrate_lower;
    long bitrate_window;
    void* codec_setup;
} vorbis_info;

typedef struct vorbis_comment {
    char** user_comments;
    int* comment_lengths;
    int comments;
    char* vendor;
} vorbis_comment;

typedef struct vorbis_dsp_state {
    int analysisp;
    vorbis_info* vi;
    float** pcm;
    float** pcmret;
    int pcm_storage;
    int pcm_current;
    int pcm_returned;
    int preextrapolate;
    int eofflag;
    long lW;
    long W;
    long nW;
    long centerW;
    ogg_int64_t granulepos;
    ogg_int64_t sequence;
    ogg_int64_t glue_bits;
    ogg_int64_t time_bits;
    ogg_int64_t floor_bits;
    ogg_int64_t res_bits;
    void* backend_state;
} vorbis_dsp_state;

typedef struct vorbis_block {
    float** pcm;
    oggpack_buffer opb;
    long lW;
    long W;
    long nW;
    int pcmend;
    int mode;
    int eofflag;
    ogg_int64_t granulepos;
    ogg_int64_t sequence;
    vorbis_dsp_state* vd;
    void* localstore;
    long localtop;
    long localalloc;
    long totaluse;
    struct alloc_chain* reap;
    long glue_bits;
    long time_bits;
    long floor_bits;
    long res_bits;
    void* internal;
} vorbis_block;

// libpk's private vorbisfile: no ov_callbacks member (reads go through the owner's stream reader), 0x2C0 bytes.
typedef struct OggVorbis_File {
    void* datasource;                      // +0x000
    int seekable;                          // +0x004
    ogg_int64_t offset;                    // +0x008
    ogg_int64_t end;                       // +0x010
    ogg_sync_state oy;                     // +0x018
    int links;                             // +0x034
    ogg_int64_t* offsets;                  // +0x038
    ogg_int64_t* dataoffsets;              // +0x03c
    long* serialnos;                       // +0x040
    ogg_int64_t* pcmlengths;               // +0x044
    vorbis_info* vi;                       // +0x048
    vorbis_comment* vc;                    // +0x04c
    ogg_int64_t pcm_offset;                // +0x050
    int ready_state;                       // +0x058
    long current_serialno;                 // +0x05c
    int current_link;                      // +0x060
    double bittrack;                       // +0x068
    double samptrack;                      // +0x070
    ogg_stream_state os;                   // +0x078
    vorbis_dsp_state vd;                   // +0x1e0
    vorbis_block vb;                       // +0x250
} OggVorbis_File;
// vorbisfile.c 1.0.1 (libpk's modified copy) is built /Gr: every entry point is extern "C" __fastcall.
#define OV_FALSE  -1
#define OV_EOF    -2
#define OV_EREAD  -128
#define OV_EINVAL -131
#define OPENED 2
extern "C" __declspec(dllimport) double __cdecl floor(double);               // IAT [0x01440100]
/* VC7.1 <math.h> C++ overloads (the library is C++ compiled, so `floor(float)` resolves to these): */
inline float floorf(float _X)
        {return ((float)floor((double)_X)); }
inline float __cdecl floor(float _X)
        {return (floorf(_X)); }
#define rint(x)   (floor((x)+0.5f))                                           // os.h (MSVC branch)

extern "C" double __fastcall ov_time_total(OggVorbis_File* vf, int i);      // 0x00C8AAF0

extern "C" long __fastcall ov_bitrate(OggVorbis_File *vf,int i){
  if(vf->ready_state<OPENED)return(OV_EINVAL);
  if(i>=vf->links)return(OV_EINVAL);
  if(!vf->seekable && i!=0)return(ov_bitrate(vf,0));
  if(i<0){
    ogg_int64_t bits=0;
    int i;
    float br;
    for(i=0;i<vf->links;i++)
      bits+=(vf->offsets[i+1]-vf->dataoffsets[i])*8;
    /* This once read: return(rint(bits/ov_time_total(vf,-1)));
     * gcc 3.x on x86 miscompiled this at optimisation level 2 and above,
     * so this is slightly transformed to make it work.
     */
    double seconds=ov_time_total(vf,-1);
    br = bits/seconds;
    return(rint(br));
  }else{
    if(vf->seekable){
      /* return the actual bitrate */
      return(rint((vf->offsets[i+1]-vf->dataoffsets[i])*8/ov_time_total(vf,i)));
    }else{
      /* return nominal if set */
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