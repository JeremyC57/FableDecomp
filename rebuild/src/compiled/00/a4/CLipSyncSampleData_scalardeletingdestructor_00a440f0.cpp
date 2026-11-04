// CLipSyncSampleData::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the inline destructor destroys the CLipSyncFrameData range of its vector through the out-of-line __destroy_aux and frees the storage, then the object is freed when bit 0 of the flags is set
// Leaf: ??_GCLipSyncSampleData@@QAEPAXI@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A440F0, 55 bytes (+ int3 padding to 64), thiscall `ret 4`, returns this.
// Identity (proven): CLipSyncSampleData::`scalar deleting destructor'; evidence: its callee 0x00A43F80 is the landed __destroy_aux over 12-byte vectors of 12-byte CCharString-led records, called with the same inline body by the CCountedPointer deleter 0x00A44130 (before `operator delete`); that deleter is stored only by CCountedPointer::Reset 0x00A44170, called from the landed CLipSyncDataBankEntry::CData::BuildLipSync 0x00A42EA0 with a `new` 0x10-byte CLipSyncSampleData (its callee CLipSyncSampleData::ReadFromFile 0x00A495F0 also calls 0x00A43F80); Ego_r.pdb CLipSyncSampleData is CArray<CLipSyncFrameData> FrameData + float FramesPerSecond, CLipSyncFrameData one CArray<CLipSyncPhoneme>
// Label: the manifest's "`scalar_deleting_destructor'" is propagated (family H064: six deleting destructors that differ only in the
//   __destroy_aux they call; retail has no identical-code folding, so each address is one class's COMDAT). No call or
//   data reference reaches this address: VC7.1 emits the ??_G COMDAT for every `delete p` and explicit `p->~T()` of a
//   class with a destructor (checked with a two-line probe), inlines it at the site, and the retail link kept it.
// Callees: std::__destroy_aux<CLipSyncFrameData*> @ 0x00A43F80 (`__fastcall` first/last, the empty __false_type tag by reference,
//   homed on the flags argument's slot: `lea eax,[esp+8]`), free 0x00BFEA14 (the null-tested
//   ~_Vector_base deallocate), operator delete 0x00BFE9BC.
// Layout: local models (no engine header for these classes); STLport 4.6 vector = 12 bytes.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* block);                      // 0x00BFE9BC

#include "engine/CArray.h"

class CLipSyncPhoneme {                               // 12 bytes, led by a CCharString
public:
    unsigned char Data[0xC];
    ~CLipSyncPhoneme();
};
class CLipSyncFrameData {                             // 12 bytes in retail (Ego_r.pdb: one CArray<CLipSyncPhoneme> Phonemes)
public:
    CArray<CLipSyncPhoneme> Phonemes;
};
class CLipSyncSampleData {                            // 0x10 bytes in retail
public:
    CArray<CLipSyncFrameData> FrameData;               // +0x0
    float FramesPerSecond;                              // +0xC
};

// Same-TU stand-in for the out-of-line __destroy_aux 0x00A43F80: a plain function above the caller under auto_inline(off)
// that never touches the tag, so VC7.1 homes the empty tag temporary on the live flags slot as retail does (a declared-only
// callee opens a `push ecx` frame word; a template stand-in is inlined despite the pragma -- GOTCHAS 2026-10-20).
namespace std {
#pragma auto_inline(off)
void __fastcall __destroy_aux(CLipSyncFrameData* __first, CLipSyncFrameData* __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(__first);
}
#pragma auto_inline(on)
}

// The CCountedPointer<CLipSyncSampleData> deleter (retail 0x00A44130): the delete that emits the COMDAT.
void __fastcall DeleteLipSyncSampleData_00a440f0(CLipSyncSampleData* p) { delete p; }