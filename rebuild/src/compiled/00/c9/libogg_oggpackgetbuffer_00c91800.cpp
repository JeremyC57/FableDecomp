// libogg::oggpack_get_buffer(oggpack_buffer* b) -- bitwise.c: the packer's output buffer
// Leaf: @oggpack_get_buffer@4
// Flags: /TC /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C91800, LTCG __fastcall (b in ecx): `mov eax,[ecx+8] ; ret`.
// Library: libogg 1.1.1 src/bitwise.c compiled as C (/TC /Gr), statically linked into libpk. No PDB/RTTI: the name is the libogg
//   API name. The body is the bitwise.c text verbatim (gitlab.xiph.org xiph/ogg tag v1.1.1, bitwise.c lines 442-444).
// Identity (two sources): (1) the body returns oggpack_buffer.buffer (+8: endbyte +0, endbit +4, buffer +8); (2) its caller
//   vorbis_analysis 0x00C90CB0 (landed 2026-11-04e from the Xiph text) stores the result in op->packet next to oggpack_bytes
//   0x00C917B0, and it sits in bitwise.c order after oggpack_bytes.
//   Landing 2026-11-04e re-modelled this row: it had landed as an invented `DrawGetEnvironment(CWorld*)` (catalog
//   GetMember_dword0x8) and was bound from the old 0x00C90CB0 model as `?GetWidth@SSurface@@QAEKXZ`.
// Structures: the shared engine/Vorbis.h (oggpack_buffer).
// Original libogg bitwise.c; BSD-style terms in reference COPYING.
#include "engine/Vorbis.h"

unsigned char *oggpack_get_buffer(oggpack_buffer *b){
  return(b->buffer);
}