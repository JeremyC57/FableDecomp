// CArray<unsigned char>::CArray(unsigned int n) -- the zero-filled size constructor: allocate n bytes and zero them
// Leaf: ??0?$CArray@E@@QAE@I@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00514E20, 77 bytes, thiscall, `ret 4`.
// Identity (NF93 q11297, from the egod e13-audit claim): FableWin.pdb ??0?$CArray@E@@QAE@I@Z (0x01CD3B90; it also lists the separate
//   ??0?$vector@EV?$allocator@E@std@@@std@@QAE@I@Z, retail 0x005136B0, byte-identical: two COMDATs, two names, no ICF). Ego_d types the
//   buffers of this address's callers CArray<unsigned char> (CDataOutputStream::WriteCompressed work_mem / compressed_data,
//   C3DPrimitive2::LoadVertexBuffer src_buffer, CAFile::ReadVar<int> buffer: the engine/CAFile.h ReadVar body, FableWin
//   ??$ReadVar@K@CAFile@@...) while the 0x005136B0 callers (CTexture::LoadFromDataStream*) use std::vector<unsigned char>; the landed
//   0x00BFC880 already binds this spelling. Was the stand-in std::vector<unsigned char>(n) explicit instantiation (the 0x005136B0 name).
// Callees: malloc 0x00BFEA0E (STLport allocator<unsigned char>::allocate, skipped for n == 0).
// Shape: engine/CArray.h's one-argument constructor forwards to STLport's vector(n) (FABLE_RETAIL_CARRAY): _Vector_base(n, a) zeroes
//   the three words, allocates, sets _M_end_of_storage = start + n; then _M_finish = uninitialized_fill_n(start, n, 0), a memset
//   (inlined /Oi as rep stosd + rep stosb). Emitted through an explicit instantiation of the constructor.
#define FABLE_RETAIL_CARRAY
#include "engine/CArray.h"

template CArray<unsigned char>::CArray(unsigned int n);
