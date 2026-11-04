// libvorbis::vorbis_analysis(vorbis_block* vb, ogg_packet* op) -- analysis.c: encode one block through mapping 0 and, when asked, hand the packed bits out as an ogg_packet
// Leaf: @vorbis_analysis@8
// Flags: /TC /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C90CB0, 130 bytes, LTCG __fastcall (vb in ecx, op in edx).
// Library: libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909") lib/analysis.c compiled as C (/TC /Gr), statically linked into libpk.
//   No PDB/RTTI: the name is the libvorbis API name. The body is the 1.0.1 analysis.c text verbatim (gitlab.xiph.org xiph/vorbis tag
//   v1.0.1, analysis.c lines 32-64).
// Identity (two sources): (1) the body zeroes vb->glue_bits / time_bits / floor_bits / res_bits, calls oggpack_reset(&vb->opb)
//   0x00C912F0, then `_mapping_P[0]->forward(vb)` through the registry table 0x013A3DDC slot +0x0C (the address-taken
//   mapping0_forward 0x00C9EF90, landed 2026-11-04e), returns OV_EINVAL (-131) when vorbis_bitrate_managed 0x00C9CFD0 says the stream is
//   bitrate-managed, and fills op->packet / bytes / b_o_s / e_o_s / granulepos / packetno from oggpack_get_buffer 0x00C91800,
//   oggpack_bytes 0x00C917B0, vb->eofflag, vb->granulepos and vb->sequence; (2) analysis.c is the next object after block.c
//   (vorbis_window 0x00C90C80) and its callees are the bitwise.c and bitrate.c API.
//   Landing 2026-11-04e re-modelled this row: it had landed as an invented `InitializeDrawing(SVideoPlayer*, SDrawingInfo*)` with
//   `SSurface::Reset / GetWidth / GetHeight`, `SVideoPlayer::IsBusy` and `g_pPlayerBackend` stand-ins for these Xiph symbols.
// Structures: the shared engine/Vorbis.h (vorbis_block, oggpack_buffer, ogg_packet); vorbis_func_mapping (lib/backends.h layout:
//   forward is the fourth pointer, +0x0C) and the registry's `_mapping_P` table (lib/registry.c).
// Original libvorbis analysis.c; BSD-style terms in reference COPYING.
#include "engine/Vorbis.h"
#define OV_EINVAL -131
extern vorbis_func_mapping   *_mapping_P[];                                            /* 0x013A3DDC */
extern void  oggpack_reset(oggpack_buffer *b);                                         /* 0x00C912F0 */
extern unsigned char *oggpack_get_buffer(oggpack_buffer *b);                           /* 0x00C91800 */
extern long  oggpack_bytes(oggpack_buffer *b);                                         /* 0x00C917B0 */
extern int   vorbis_bitrate_managed(vorbis_block *vb);                                 /* 0x00C9CFD0 */

int vorbis_analysis(vorbis_block *vb, ogg_packet *op){
  int                   ret;

  vb->glue_bits=0;
  vb->time_bits=0;
  vb->floor_bits=0;
  vb->res_bits=0;

  /* first things first.  Make sure encode is ready */
  oggpack_reset(&vb->opb);
  
  /* we only have one mapping type (0), and we let the mapping code
     itself figure out what soft mode to use.  This allows easier
     bitrate management */

  if((ret=_mapping_P[0]->forward(vb)))
    return(ret);

  if(op){
    if(vorbis_bitrate_managed(vb))
      /* The app is using a bitmanaged mode... but not using the
         bitrate management interface. */
      return(OV_EINVAL);
    
    op->packet=oggpack_get_buffer(&vb->opb);
    op->bytes=oggpack_bytes(&vb->opb);
    op->b_o_s=0;
    op->e_o_s=vb->eofflag;
    op->granulepos=vb->granulepos;
    op->packetno=vb->sequence; /* for sake of completeness */
  }
  return(0);
}