// CFlammableDef::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CFlammableDef inlined (the member's destructor, then the base chain down to the first user-written destructor), and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCFlammableDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E2869, 44 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity (proven): CFlammableDef::`scalar deleting destructor'; evidence: vtable 0x01242984 slot 0 (RTTI .?AVCFlammableDef@@ : CSubDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase)
// Label: the manifest's "`vector_deleting_destructor'" is propagated across byte-identical bodies (family J065: five deleting destructors of five unrelated classes).
// Callees: CFireEffectCreationSet::~CFireEffectCreationSet 0x004E0A1F (EffectCreationSet, +0x38), CResource::~CResource 0x009FC550 (after the inline user-written ~CDefPointeeBase stores 0x01230BA0), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CFlammableDef members at their PDB offsets (the map is the last member); retail size 0x4C from the def's size slot.
// Shape: no vtable store for CFlammableDef itself -- VC7.1 resets the vptr only in user-written destructors (agent952's finding, 0x006DF8C0); the stored
//   vtable 0x01230BA0 belongs to the first user-written (inline) base destructor. The deleting destructor is emitted with the vtable, i.e. in a TU
//   that constructs the class: MakeOne is that construction (a modelling device); the qualified explicit destructor calls make VC7.1 define the
//   implicit destructors before it generates the deleting destructor, so the chain inlines.
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy { _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { };   // STLport, 12 bytes
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                                              // STLport, 12 bytes (its _Rb_tree: header, count, compare)
public:
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
}   // namespace std
class CDefString {                                       // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
class CResourceList;
class CResource {                                        // Ego_r.pdb 0x1C (CIVCountedPointeeBase: vptr, IVRefCount)
public:
    CResource();                                         // 0x009FBEC0
    virtual ~CResource();                                // 0x009FC550
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0C
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
};
class CDefPointeeBase : public CResource {               // vtable 0x01230BA0
public:
    virtual ~CDefPointeeBase() { }                       // user-written, inline: stores 0x01230BA0, then ~CResource
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {           // vtable 0x01230BCC (implicit destructor)
public:
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
class CSubDefClassBase : public CDefClassBase {         // vtable 0x01231D54, Ego_r.pdb 0x25 (packed, no members, implicit destructor)
public:
    CSubDefClassBase();                                  // 0x0044C0C0
};
class CEngineGraphic {                                   // Ego_r.pdb 14 bytes, packed
public:
    CEngineGraphic() : BankIndex(0), AnimStep(0.0f), RenderSizeX(0.0f), AdditiveAlpha(false), Type(0) { }
    unsigned long BankIndex;                             // +0x00
    float         AnimStep;                              // +0x04
    float         RenderSizeX;                           // +0x08
    bool          AdditiveAlpha;                         // +0x0C
    unsigned char Type;                                  // +0x0D (EEngineGraphicType)
};
#pragma pack(pop)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CInitBaseClass : public CBaseClass {               // Ego_r.pdb 8 bytes, vtable 0x0129A7CC
public:
    CInitBaseClass();
    virtual ~CInitBaseClass();                           // 0x0099A410
    bool Valid;                                          // +0x04
};
template <class T> class CCompareDefString { };          // Ego_r.pdb CCompareDefString<long>, empty
class CFireEffectCreationSet : public CInitBaseClass {   // Ego_r.pdb 24 bytes, retail 20 (the STLport map 12), vtable 0x012421D4
public:
    CFireEffectCreationSet();
    virtual ~CFireEffectCreationSet();                   // 0x004E0A1F
    std::map<CDefString, long, CCompareDefString<long> > ContainmentMap;   // +0x08
};
class CFlammableDef : public CSubDefClassBase {          // vtable 0x01242984, retail 0x4C (size slot 20)
public:
    long  FireResistance;                                // +0x28
    float MaxDamageCaused;                               // +0x2C
    float FireDamagePerCycle;                            // +0x30
    float FireDamagePeriod;                              // +0x34
    CFireEffectCreationSet EffectCreationSet;            // +0x38
};
void* MakeOne()
{
    return new CFlammableDef;
}
void KillDefBase(CDefClassBase* p) { p->CDefClassBase::~CDefClassBase(); }
void KillSubDef(CSubDefClassBase* p) { p->CSubDefClassBase::~CSubDefClassBase(); }
void KillOne(CFlammableDef* p) { p->~CFlammableDef(); }