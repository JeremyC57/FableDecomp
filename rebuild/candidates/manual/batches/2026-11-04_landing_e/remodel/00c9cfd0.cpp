// libvorbis::vorbis_bitrate_managed(vorbis_block* vb) -- bitrate.c: 1 when the encoder's bitrate manager is queueing binned packets (managed mode), else 0
// Leaf: @vorbis_bitrate_managed@4
// Flags: /TC /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9CFD0, LTCG __fastcall (vb in ecx).
// Library: libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909") lib/bitrate.c compiled as C (/TC /Gr), statically linked into libpk.
//   No PDB/RTTI: the name is the libvorbis API name. The body is the 1.0.1 bitrate.c text verbatim (gitlab.xiph.org xiph/vorbis tag
//   v1.0.1, bitrate.c lines 171-178).
// Identity (two sources): (1) the body reads vb->vd (+0x40) -> backend_state (+0x68) -> bms.queue_binned (+0x50) and returns it as
//   a bool; (2) its callers are vorbis_analysis 0x00C90CB0 (OV_EINVAL when managed) and mapping0_forward 0x00C9EF90 (the
//   PACKETBLOBS loop bounds), both landed 2026-11-04e from the Xiph text and bound `@vorbis_bitrate_managed@4`.
//   Landing 2026-11-04e re-modelled this row: it had landed as an invented `S::M()` (catalog CPlayer::GetCurrentMode) and was bound
//   from the old 0x00C90CB0 model as `?IsBusy@SVideoPlayer@@QAEJXZ`.
// Structures: the shared engine/Vorbis.h (vorbis_block, vorbis_dsp_state, private_state, bitrate_manager_state).
// Original libvorbis bitrate.c; BSD-style terms in reference COPYING.
#include "engine/Vorbis.h"

int vorbis_bitrate_managed(vorbis_block *vb){
  vorbis_dsp_state      *vd=vb->vd;
  private_state         *b=vd->backend_state; 
  bitrate_manager_state *bm=&b->bms;

  if(bm->queue_binned)return(1);
  return(0);
}
