// libvorbis::vorbis_window(vorbis_dsp_state* v, int W) -- block.c: the encoder/decoder window curve for block size W (short 0 / long 1), or NULL when no window is set up
// Leaf: @vorbis_window@8
// Flags: /TC /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C90C80, 36 bytes, LTCG __fastcall (v in ecx, W in edx), the tail call to _vorbis_window_get
//   is a `jmp`.
// Library: libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909") lib/block.c compiled as C (/TC /Gr), statically linked into libpk.
//   No PDB/RTTI: the name is the libvorbis API name. The body is the 1.0.1 block.c text verbatim (gitlab.xiph.org xiph/vorbis tag
//   v1.0.1, block.c lines 962-970).
// Identity (two sources): (1) the body reads v->vi (+4) -> codec_setup (+0x1C) -> halfrate_flag (+0xE78) and v->backend_state (+0x68)
//   -> window[W] (+4 + W*4), returns 0 when `window[W]-1 < 0` and otherwise tail-calls _vorbis_window_get(window[W]-hs) 0x00C9EA70
//   (the window.c accessor, bound `@_vorbis_window_get@4` by the landed vorbis_synthesis_blockin 0x00C8FF30); (2) it sits in block.c
//   order after vorbis_synthesis_restart and before vorbis_analysis's object (analysis.c, 0x00C90CB0).
//   Landing 2026-11-04e re-modelled this row: it had landed as an invented `CPoolEntryTable_00c90c80::GetEntryAllocatorSize` with
//   the callee spelled `?LookupAllocatorSize@@YIKK@Z`.
// Structures: the shared engine/Vorbis.h (vorbis_dsp_state, vorbis_info, codec_setup_info, private_state).
// Original libvorbis block.c; BSD-style terms in reference COPYING.
#include <stdlib.h>
#include "engine/Vorbis.h"
float *_vorbis_window_get(int n);                                                      /* 0x00C9EA70 */

float *vorbis_window(vorbis_dsp_state *v,int W){
  vorbis_info *vi=v->vi;
  codec_setup_info *ci=vi->codec_setup;
  int hs=ci->halfrate_flag; 
  private_state *b=v->backend_state;

  if(b->window[W]-1<0)return NULL;
  return _vorbis_window_get(b->window[W]-hs);
}