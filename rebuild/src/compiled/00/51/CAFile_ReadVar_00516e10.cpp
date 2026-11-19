// CAFile::ReadVar<unsigned char>(unsigned char& var, CASuspendableProcess* psuspendable) -- reads one unsigned char from the file; when the file needs aligned reads,
//   through a zeroed byte buffer rounded up to the alignment
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00516E10, length 111 bytes.
// Leaf: ??$ReadVar@E@CAFile@@QAEXAAEPAVCASuspendableProcess@@@Z
// Identity: Ego_r.pdb 0x004E6970 / FableWin.pdb 0x01CCE070 ??$ReadVar@E@CAFile@@QAEXAAEPAVCASuspendableProcess@@@Z (the variable by
//   reference; the one 1-byte instance in both PDBs). NF93 (q11297): ported from a local
//   CAFile model (13 placeholder slots, ReadVar(T*)) and a local CByteVector (an in-TU vector constructor) onto engine/CAFile.h's ReadVar
//   body and engine/CArray.h; the buffer is CArray<unsigned char> (FableWin ??0?$CArray@E@@QAE@I@Z 0x00514E20; Ego_d types ReadVar's
//   buffer CArray<unsigned char>).
// Callees: CAFile virtual slot 13 (+0x34) GetAlignment (0x00401660 for CDiskFileWin32), slot 3 (+0x0c) Read(void*, long,
//   CASuspendableProcess*) (0x0099A960), CArray<unsigned char>(unsigned int) 0x00514E20, free 0x00BFEA14 (the inline destructor).
// Shape: GetAlignment() is called twice (the guard `<= 1` is signed, the rounding `((sizeof(T) + a - 1) / a) * a` unsigned: `div` +
//   `imul`); the buffer is a 12-byte frame local whose inline destructor frees its start without a null test (the fitted
//   ~_Vector_base). The size constructor is out of line with its body visible (FABLE_RETAIL_CARRAY_SIZE_CTOR_VISIBLE_NOINLINE): VC7.1 then
//   knows it does not let the buffer's address escape, so the start survives the virtual Read call in edi.
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_SIZE_CTOR_VISIBLE_NOINLINE
#include "engine/CAFile.h"

template void CAFile::ReadVar<unsigned char>(unsigned char& var, CASuspendableProcess* psuspendable);
