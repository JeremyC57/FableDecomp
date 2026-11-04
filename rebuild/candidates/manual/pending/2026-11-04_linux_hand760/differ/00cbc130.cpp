// std::vector<SMusicTrack>::_M_insert_overflow(SMusicTrack* position, const SMusicTrack& x, const __false_type&, unsigned long fillLen, bool atEnd) -- STLport growth path of the music manager's 24-byte track vector: allocate old + max(old, fillLen) elements, copy the head, construct the fill, copy the tail unless inserting at the end, destroy + free the old block and adopt the new one
// Grade: DIFFER(287v284) -- dl=47 (try b: the shared STLport _M_insert_overflow explicitly instantiated, out-of-line __fastcall __uninitialized_copy (new FABLE_STLP_OUTOFLINE_UNINIT_COPY) and _fill_n; was 291v284): fillLength in edi and the new finish in ebx (retail ebx / edi, fillLength loaded at entry), the element copy follows
// Leaf: _M_insert_overflow
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBC130, 284 bytes, `ret 14h` (an ICF-shared body; the catalog label CMusicEntry::LoadFromScript is
//   propagated: the push_back sites 0x00CB85CC / 0x00CBC7FE call it).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, the __fastcall __uninitialized_copy 0x00CB9B00 and __uninitialized_fill_n 0x00CB9B50
//   (out-of-line COMDATs: FABLE_STLP_OUTOFLINE_UNINIT_COPY / _FILL_N), CCharString copy ctor 0x0099EC30 / dtor 0x0099EAE0.
// Body: the shared STLport 4.6 vector::_M_insert_overflow (stlport/fable_containers.h), explicitly instantiated.
// Layout: element 0x18 bytes: CMusicEntry (Ego_r.pdb: ID, Filename CDefString, Gain, CacheToHD; 16 bytes), the name CCharString +0x10,
//   four uncopied bytes +0x14 (the copy ctor copies the entry and the name only). No engine header: local model.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CDefString.h"
struct CMusicEntry {
    unsigned long ID;                                   // +0x00
    CDefString    Filename;                             // +0x04
    float         Gain;                                 // +0x08
    bool          CacheToHD;                            // +0x0c
};
struct SMusicTrack {
    CMusicEntry   Entry;                                // +0x00
    CCharString   Name;                                 // +0x10
    unsigned long Spare;                                // +0x14 (not copied)
    SMusicTrack(const SMusicTrack& other) : Entry(other.Entry), Name(other.Name) { }
};
template void std::vector<SMusicTrack>::_M_insert_overflow(SMusicTrack*, const SMusicTrack&, const std::__false_type&, std::vector<SMusicTrack>::size_type, bool);
