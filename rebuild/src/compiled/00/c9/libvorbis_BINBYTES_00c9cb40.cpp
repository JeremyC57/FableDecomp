// libvorbis::BINBYTES(bitrate_manager_state* bm, long pos, long bin) -- bitrate.c: the byte count queued for packet slot `pos` in bitrate bin `bin` (queue_binned[pos * queue_bins + bin])
// Shape (retry1): parked as an "LTCG custom convention" gap (bm in eax, bin in edx, pos on the stack, `ret 4`); this is libvorbis
//   1.0.1 bitrate.c's static BINBYTES. As a same-TU `static __declspec(noinline)` with a same-TU caller (stand-in for
//   vorbis_bitrate_addblock, which calls it at 0x00C9D553.. and feeds LACING_ADJUST 0x00C9CB60) VC7.1 /O2 picks that private
//   convention itself.
// Leaf: @BINBYTES@12
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9CB40, 18 bytes. Layout: libvorbis 1.0.1 bitrate.h bitrate_manager_state
//   {queue_binned +0x00, queue_actual +0x04, queue_size +0x08, queue_head +0x0C, queue_bins +0x10, ...}.
// Label correction: the catalog's CTopologySizeGrid::Hora is propagated; the parked struct is bitrate_manager_state.
// Original libvorbis bitrate.c; BSD-style terms in reference COPYING.
extern "C" {
typedef unsigned int ogg_uint32_t;
typedef struct bitrate_manager_state {
  ogg_uint32_t  *queue_binned;
  ogg_uint32_t  *queue_actual;
  int            queue_size;
  int            queue_head;
  int            queue_bins;
} bitrate_manager_state;

static __declspec(noinline) long BINBYTES(bitrate_manager_state *bm,long pos,long bin){
  int bins=bm->queue_bins;
  return(bm->queue_binned[pos*bins+bin]);
}

long LACING_ADJUST(long bytes);   // 0x00C9CB60

/* Same-TU caller (stand-in for vorbis_bitrate_addblock's bin sums). Not graded. */
long BINBYTES_caller(bitrate_manager_state *bm,long pos,long bins,long *acc){
  long i;
  for(i=0;i<bins;i++)
    acc[i]+=LACING_ADJUST(BINBYTES(bm,pos,i));
  return acc[0];
}
}