// std::__copy(CShoveHaPennyRegionDef* first, CShoveHaPennyRegionDef* last, CShoveHaPennyRegionDef* result, const random_access_iterator_tag&, int*) -- STLport counted forward copy of shove-ha'penny region defs through the def's implicit assignment (VC7.1 unrolls the counted loop four times); returns the end of the output
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D14B0, 752 bytes, /Gr `__fastcall` (first ecx, last edx), `ret 0Ch` (result,
//   the tag reference and the _Distance* tag on the stack).
// Identity: the element is CShoveHaPennyRegionDef (0x30 bytes: the divide-by-48 multiply, the vtable 0x0124383C its
//   copy constructor stores in the sibling vector helpers 0x007D22F0 / 0x007D2370 / 0x007D23B0, Ego_r.pdb Start / Score
//   at +0x28 / +0x2C); the CTavernGameShoveHaPennyDef::Regions vector's erase/assign copy. The catalog's
//   "memcpy_Entity_48byte_Blocks" is descriptive.
// Shape: STLport _algobase.h `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first;
//   ++__first; ++__result; } return __result;` with the implicit assignment inline (the vptr is not copied; the six
//   CResource words, PDefManager, GlobalIndex, each flag bit by read-modify-write, then Start / Score through integer
//   registers); the call-free body lets VC7.1 unroll the loop by four (`(n - 4) / 4 + 1` passes, then the remainder).
// Layout (Ego_r.pdb names, retail offsets): CResource root (vptr with the virtual destructor 0x009FC550 behind slot
//   0's deleting destructor; six words +0x04..+0x18); CDefPointeeBase (vtable 0x01230BA0, an empty user destructor:
//   its vtable store is the only one a by-value destruction keeps); CDefClassBase, pack(1), 0x25 bytes: PDefManager
//   +0x1C, GlobalIndex +0x20, the Setup / Template / DefaultValsApplied bits at +0x24, an out-of-line user copy
//   constructor 0x0046644C and the implicit assignment; CShoveHaPennyRegionDef (0x30, vtable 0x0124383C): Start
//   +0x28, Score +0x2C, implicit copy constructor / assignment / destructor.
class CResourceList;
class CResource {                                          // 28 bytes
public:
    virtual ~CResource();                                  // 0x009FC550 (labelled "~CMemoryBlock")
    long           m_refCount;                             // +0x04
    CResourceList* ResourceList;                           // +0x08
    CResource*     PrevResource;                           // +0x0C
    CResource*     NextResource;                           // +0x10
    unsigned long  ResourceSize;                           // +0x14
    unsigned long  LastUsedFrame;                          // +0x18
};
class CDefPointeeBase : public CResource {                 // vtable 0x01230BA0
public:
    ~CDefPointeeBase() { }
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {             // 0x25 bytes
public:
    CDefinitionManager* PDefManager;                       // +0x1C
    unsigned long       GlobalIndex;                       // +0x20
    unsigned char       Setup : 1;                         // +0x24 bit 0
    unsigned char       Template : 1;                      // +0x24 bit 1
    unsigned char       DefaultValsApplied : 1;            // +0x24 bit 2
    CDefClassBase(const CDefClassBase& other);             // 0x0046644C
};
#pragma pack(pop)
class CShoveHaPennyRegionDef : public CDefClassBase {      // 0x30 bytes, vtable 0x0124383C
public:
    float Start;                                           // +0x28
    float Score;                                           // +0x2C
};

namespace std {
struct random_access_iterator_tag { };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template CShoveHaPennyRegionDef* __fastcall __copy(CShoveHaPennyRegionDef*, CShoveHaPennyRegionDef*,
                                                   CShoveHaPennyRegionDef*, const random_access_iterator_tag&, int*);
}