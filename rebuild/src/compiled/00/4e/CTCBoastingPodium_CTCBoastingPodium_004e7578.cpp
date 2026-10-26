// CTCBoastingPodium::~CTCBoastingPodium() -- the compiler-generated (implicit) destructor, kept out of line: release the def pointer PDef (+0x28), destroy BoastingPositions (+0xC), then the base destructor CTCBase::~CTCBase
// Leaf: ??1CTCBoastingPodium@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E7578, 40 bytes (+ int3 padding), thiscall, no return value; size-optimised TU (odd entry,
//   `and dword ptr [esi+28h],0`); reached only from its scalar deleting destructor 0x004E755C.
// Identity (proven): CTCBoastingPodium::~CTCBoastingPodium(); evidence: its only caller is the scalar deleting destructor 0x004E755C in slot 0 of vtable 0x0124346C, whose RTTI is .?AVCTCBoastingPodium@@ : CTCBase : CBaseClassNonCopyable : CBaseClass; the PDB name sets hold ??1CTCBoastingPodium@@UAE@XZ; the released and destroyed offsets +0x28 / +0xC are Ego_r.pdb CTCBoastingPodium::PDef / BoastingPositions (rebased: one 16-byte PDB container precedes PDef), and the tail jump is the base's destructor.
// Label: the manifest's "~CCreatureAction_FireMissileWeapon" is propagated across byte-identical bodies (family J081: five implicit destructors of five
//   classes, identical up to member offsets and callees; retail has no identical-code folding, so each address is one function).
// Callees: CIVCountedPointeeBase::Delete() const (the def's vtable slot 1, on the last release); CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess<long, CIntelligentPointer<CThing> > >::~CVectorMap 0x004E34EC (FableWin.pdb ??1?$CVectorMap@JV?$CIntelligentPointer@VCThing@@@@V?$CKeyPairCompareLess@JV?$CIntelligentPointer@VCThing@@@@@@@@QAE@XZ); CTCBase::~CTCBase 0x00686830 (tail jmp; ??1CTCBase@@UAE@XZ, the PDB-confirmed virtual spelling).
// Layout: Ego_r.pdb CTCBoastingPodium members rebased after the 0x10-byte retail CVectorMap (PDef Ego_r +0x2C -> retail +0x28, as the body's release shows).
// Shape: the J072 model (no vtable store -- VC7.1 resets the vptr only in user-written destructors; members destroyed in reverse
//   declaration order, the last one the tail `jmp`) with a CDefPointer member whose inline destructor is the IV release
//   (`p = Object; if (p) { if (--p->IVRefCount == 0) p->Delete(); Object = 0; }`, agent952 H021 / J075), `__forceinline` because
//   under /Os VC7.1 keeps the class-body destructor out of line (grade 1: a call, 27 bytes) and without /Os the clear is a 7-byte
//   `mov` (grade 2, 43 bytes); retail has both the size-TU `and` and the inlined release. KillOne's explicit
//   destructor call is a modelling device that makes VC7.1 emit the implicit destructor out of line.
#pragma optimize("s", on)
#pragma pack(push, 1)
class CBaseIntelligentPointer {                          // 8 bytes, 1-aligned in retail
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    void* PData;                                         // CBaseObjectPointer*
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
#pragma pack(pop)
class CIVCountedPointeeBase {                            // Ego_r.pdb 8 bytes: vptr, IVRefCount
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                         // slot 1 (FableWin.pdb ?Delete@CIVCountedPointeeBase@@UBEXXZ)
    unsigned long IVRefCount;                            // +0x04
};
class CResourceList;
class CResource : public CIVCountedPointeeBase {         // Ego_r.pdb 0x1C
public:
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0C
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
};
class CDefPointeeBase : public CResource { };            // Ego_r.pdb 0x1C, no members of its own
template <class T> class CDefPointer {                   // Ego_r.pdb 4 bytes
public:
    __forceinline ~CDefPointer()                         // the IV release, inline (FableWin.pdb has it out of line, debug)
    {
        CDefPointeeBase* p = Object;
        if (p) {
            if (--p->IVRefCount == 0)
                p->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                             // +0x0
};
namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {            // STLport 4.6, 12 bytes
public:
    ~vector();                                                  // out of line in retail
};
}   // namespace std
class CThing;
class CTCBase {                                          // retail 0x0C bytes (agent951 G056 / J079 model)
public:
    CTCBase(CThing& thing);                              // 0x00686800
    virtual ~CTCBase();                                  // 0x00686830 (user-written: stores its vtable)
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
};
template <class K, class V> class CKeyPairCompareLess { };
template <class T> class CArray : public std::vector<T> { };   // 12 bytes in retail (16 in Ego_r.pdb)
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    ~CVectorMap();                                       // 0x004E34EC for <long, CIntelligentPointer<CThing>, CKeyPairCompareLess<...> >
    CArray<std::pair<K, V> > Data;                       // +0x00
    TCompare                 Compare;                    // +0x0C
    bool                     Dirty;                      // +0x0D
};
class CBoastingPodiumDef;
class CTCBoastingPodium : public CTCBase {               // vtable 0x0124346C, Ego_r.pdb 0x30 bytes, retail 0x2C (16-byte CVectorMap)
public:
    CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess<long, CIntelligentPointer<CThing> > > BoastingPositions;   // +0x0C
    long                                  LastPositionsWithBoastCount;   // +0x1C (Ego_r +0x20)
    unsigned long                         FrameBoastListFrom;            // +0x20 (Ego_r +0x24)
    bool                                  BoastingActive;                // +0x24 (Ego_r +0x28)
    CDefPointer<const CBoastingPodiumDef> PDef;                          // +0x28 (Ego_r +0x2C)
};
void KillOne(CTCBoastingPodium* p) { p->CTCBoastingPodium::~CTCBoastingPodium(); }