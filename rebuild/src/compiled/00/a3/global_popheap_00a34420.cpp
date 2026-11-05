// _global::__pop_heap(SKeyedPair* first, SKeyedPair* last, SKeyedPair* result, SKeyedPair value, CKeyPairCompareLess comp, long*) -- SGI pop_heap core for a heap of (unsigned long key, named counted pointer) pairs: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A34420, length 159 bytes (`ret 1Ch`: result, the 0x10-byte value, the comparator and the
// distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the manifest's "CGameCameraManager::PushCameraMode" is propagated and wrong; the callee 0x00A33A70 carries the manifest name
// `_Adjust_heap<std::pair<unsigned long,CCountedPointer<CFaction> >*,...,CKeyPairCompareLess<...>>`, so this is the matching __pop_heap (byte-identical twin 0x00A71B40).
// Element: 0x10 bytes = the key + a 12-byte {CCharString, CCountedPointer<CFaction>} record whose assignment is the out-of-line
// 0x00A32960 (label "CScriptGameResourceObjectScriptedThingBase::operator=": CCharString::operator= then the counted-pointer assignment;
// byte-identical to the pair<CCharString, CCountedPointer> assignment 0x00CBA700). The manifest's 12-byte pair<key, CCountedPointer> does
// not fit the 16-byte stride (`sar 4`) or the string copy, so the record is modelled from the bytes.
// Callees: the record's operator= @ 0x00A32960, CCharString::CCharString(const CCharString&) @ 0x0099EC30, __adjust_heap @ 0x00A33A70,
// the count block's destroy function, operator delete @ 0x00BFE9BC, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the pair's IMPLICIT assignment (key inline, record out of line) and copy constructor (string out of line, counted pointer
// inline: copy both words, add-ref through the re-read block) are inlined; the by-value `value` is destroyed by this callee: the
// counted pointer's destructor (release with the block re-read, then object and block zero stores) and the string's.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0 (inside the record assignment)
};

#include "engine/CCountedPointer.h"


class CFaction;

struct SNamedPointer {                                  // 12 bytes
    CCharString                 Name;                   // +0x00
    CCountedPointer<CFaction>    Pointer;                // +0x04
    SNamedPointer& operator=(const SNamedPointer& other);   // 0x00A32960 (out of line)
};

struct SKeyedPair {                                     // std::pair<unsigned long, SNamedPointer>, 0x10 bytes
    unsigned long first;                                      // +0x00
    SNamedPointer second;                               // +0x04
};

struct CKeyPairCompareLess { };                         // empty, passed by value

void __fastcall __adjust_heap(SKeyedPair* first, long holeIndex, long len, SKeyedPair value,
                              CKeyPairCompareLess comp);   // 0x00A33A70

void __fastcall __pop_heap(SKeyedPair* first, SKeyedPair* last, SKeyedPair* result, SKeyedPair value,
                           CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}