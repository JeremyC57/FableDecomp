// NGameText::CGroupEntry::~CGroupEntry() -- implicit destructor: release PFileData (+0x28), free the Entries block (+0x1C), then ~CResource (tail jump)
// Leaf: ??1CGroupEntry@NGameText@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CAE60, 76 bytes (int3 padding 0x009CAEAC..AF): push esi ; mov esi,ecx ; the inline
//   ~CCountedPointer on +0x28 (Info +0x2C: dec / cmp / DeleteFunc / delete Info ; Data = 0, Info = 0) ; the inline STLport
//   ~vector<long> on +0x1C (`if (_M_start) free(_M_start)`) ; mov ecx,esi ; pop esi ; jmp ~CResource 0x009FC550. No vtable store:
//   the destructor is compiler-generated.
// Identity (proven, two sources): (1) the only caller is the scalar deleting dtor 0x009CAE40, slot 0 of vtable 0x0129B478 whose RTTI
//   is .?AVCGroupEntry@NGameText@@ : CResource : CIVCountedPointeeBase (rtti-vtables.tsv / rtti_bases.tsv); (2) Ego_r.pdb
//   ??1CGroupEntry@NGameText@@UAE@XZ (0x00B186B0, beside ??_G 0x00B18820) and the Ego_r member list Entries (CArray<long>),
//   PFileData (CCountedPointer<CBankFileAsyncData>), Owner (const CDataBankGroupEntry*), released here in reverse order. The catalog
//   label `_Dest_val<...C3DSkeletalMorphTarget...>` is propagated (NAME_FIXES.tsv).
// Callees: the count block's DeleteFunc (`mov ecx,[eax+8] ; call [eax+4]`), operator delete 0x00BFE9BC (??3@YAXPAX@Z), free 0x00BFEA14
//   (allocator<long>::deallocate), CResource::~CResource 0x009FC550 (??1CResource@@UAE@XZ, tail jump).
// Layout: engine/CResource.h retail opt-in (FABLE_RETAIL_DEF_CLASS_BASE: CResource : CIVCountedPointeeBase, sizeof 0x1C); retail
//   members = Ego_r - 4 past the 12-byte STLport vector: Entries +0x1C (Ego_r +0x1C, 0x10 there), PFileData +0x28, Owner +0x30;
//   sizeof 0x34. CArray<T> is Lionhead's CArray over STLport 4.6 vector (_Vector_base + _STLP_alloc_proxy, as the landed 0x00B18780);
//   CCountedPointer is engine/CCountedPointer.h (its own Data-then-Info dtor body).
// Shape: the class declares no destructor; DestroyOne (a destruction by name, ungraded) makes VC7.1 emit the out-of-line implicit ??1.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
#include "engine/CArray.h"

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CResource.h"
#include "engine/CCountedPointer.h"
class CBankFileAsyncData;

namespace NGameText {
class CDataBankGroupEntry;
class CGroupEntry : public CResource {                  // vtable 0x0129B478; retail sizeof 0x34
public:
    CGroupEntry(CCountedPointer<CBankFileAsyncData>& fileData, const CDataBankGroupEntry* owner);   // Ego_r ??0 0x00B15D40
    CArray<long>                        Entries;        // +0x1C
    CCountedPointer<CBankFileAsyncData> PFileData;      // +0x28
    const CDataBankGroupEntry*          Owner;          // +0x30
};
} // namespace NGameText
FABLE_STATIC_ASSERT(sizeof(NGameText::CGroupEntry) == 0x34);
FABLE_STATIC_ASSERT(offsetof(NGameText::CGroupEntry, Entries) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(NGameText::CGroupEntry, PFileData) == 0x28);

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1 (chunk 656/657).
void DestroyOne(NGameText::CGroupEntry* entry)
{
    entry->NGameText::CGroupEntry::~CGroupEntry();
}