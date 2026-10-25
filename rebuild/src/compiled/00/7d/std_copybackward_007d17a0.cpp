// std::__copy_backward(CShoveHaPennyRegionDef* first, CShoveHaPennyRegionDef* last, CShoveHaPennyRegionDef* result, const random_access_iterator_tag&, int*) -- STLport counted backward copy of shove-ha'penny region defs through the def's implicit assignment (VC7.1 unrolls the counted loop four times); returns the start of the output
// Leaf: ??$__copy_backward@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D17A0, 714 bytes, /Gr `__fastcall` (first ecx, last edx), `ret 0Ch` (result,
//   the tag reference and the _Distance* tag on the stack).
// Identity: called by vector<CShoveHaPennyRegionDef>::_M_fill_insert 0x007D24F0 (`push 0 ; push &tag ; push
//   old_finish` = __copy_backward_ptrs(.., __false_type) inline); the element is the 0x30-byte CShoveHaPennyRegionDef
//   of the sibling __copy 0x007D14B0. The catalog's "memmove_Entity_48byte_Backward" is descriptive.
// Shape: STLport _algobase.h `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last;
//   return __result;` with the implicit assignment inline (vptr skipped; six CResource words, PDefManager, GlobalIndex,
//   the three flag bits by read-modify-write, Start / Score); VC7.1 unrolls the call-free loop by four.
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

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template CShoveHaPennyRegionDef* __fastcall __copy_backward(CShoveHaPennyRegionDef*, CShoveHaPennyRegionDef*,
                                                            CShoveHaPennyRegionDef*, const random_access_iterator_tag&,
                                                            int*);
}