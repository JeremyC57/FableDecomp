// CBlurredAttitudesDef::~CBlurredAttitudesDef() -- implicit destructor of the blurred-attitudes definition: free the Attitudes vector storage, then the inline parent-def bases (SubDefInfoMap storage, the CDefPointeeBase vptr) and ~CResource
// Leaf: ??1CBlurredAttitudesDef@@UAE@XZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067C370, 49 bytes, thiscall, `jmp` tail; no call or address reference (an
//   unreferenced COMDAT: VC7.1 emits the implicit `??1` when the TU destroys the object by name; retail kept it).
// Identity (proven): (1) the scalar deleting destructor 0x0067C520 -- slot 0 of vtable 0x0125B8AC, whose RTTI is
//   .?AVCBlurredAttitudesDef@@ (rebuild/manifest/rtti-vtables.tsv; slots 18-20 Transfer 0x0067CDB0 / Copy 0x0067C980 /
//   GetSizeofClass 0x0067C510) -- inlines exactly this member sequence (free [+0x3C], free [+0x29], CDefPointeeBase vptr
//   0x01230BA0, ~CResource) before its flag test; (2) Ego_r.pdb CBlurredAttitudesDef = CParentDefClassBase + vector<
//   EOpinionAttitudeType> Attitudes (retail +0x3C: CParentDefClassBase is 0x39 bytes packed, the vector 4-aligned), the
//   layout the landed ctor 0x0067C4D0 (stores vtable 0x0125B8AC) zero-initialises; (3) it sits in the CBlurredAttitudesDef /
//   COpinionReactionManagerDef TU next to GetSizeofClass 0x0067C3B0 / the ctor 0x0067C4D0. The catalog label
//   CCompressedMemoryDataInputStream::~CCompressedMemoryDataInputStream is propagated (twin: ~CControlsDef 0x004304A0).
// Callees: free 0x00BFEA14 (x2, through the STLport allocator's deallocate), ~CResource 0x009FC550.
// Shape: every destructor below CDefPointeeBase is compiler-generated and inlined: no CBlurredAttitudesDef or
//   CParentDefClassBase vtable store, the inline ~CDefClassBase store dead behind the inline ~CDefPointeeBase one (the one
//   retail keeps), the two STLport ~_Vector_base frees, and `jmp ~CResource`. The helper below (not a retail function)
//   destroys the def by name so VC7.1 emits the `??1` COMDAT.
// Layout: CResource / CDefPointeeBase / CDefClassBase from the shared retail headers (engine/CDefClassBase.h opt-in).
//   CParentDefClassBase is declared here with the layout of engine/CParentDefClassBase.h (0x39 bytes under the header's
//   own pack(1): CDefString InstantiationName +0x25, CVectorMap SubDefInfoMap +0x29) but WITHOUT its out-of-line
//   `virtual ~CParentDefClassBase();` -- with the header the body calls 0x00430300 instead of inlining it (probe 1,
//   27v49); retail's ~CParentDefClassBase stores no vtable of its own, i.e. it is the implicit one, and /O2 TUs inline
//   it (here and in ~CControlsDef 0x004304A0) while /Os TUs call its COMDAT 0x00430300. Header debt in NOTES.md.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                        // : CDefPointeeBase (vtable 0x01230BA0) : CResource (~CResource 0x009FC550)
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
class CDefString {                                       // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
#include "engine/CVectorMap.h"
enum EOpinionAttitudeType { };                   // global enum (Ego_r W4EOpinionAttitudeType@@); its values do not reach the bytes

#pragma pack(push, 1)                                    // as engine/CParentDefClassBase.h (retail 0x39 bytes)
class CParentDefClassBase : public CDefClassBase {       // vtable 0x01230CBC
public:
    virtual CDefString GetInstantiationName() const;                     // slot 10 0x004303B0
    virtual void SetInstantiationName(CDefString);                       // slot 11 0x004303C0
    virtual void AddSubDef(const CCharString&, long, long);              // slot 12 0x009B00D0
    virtual void RemoveSubDef(const CCharString&);                       // slot 13 0x009B0170
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             // slot 15 0x009AD510
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; // slot 14 0x009AD4A0
    virtual bool IsParentDef() const;                                    // slot 16 0x004303D0
    virtual void InternalSaveBinary(CDataOutputStream&);                 // slot 21 0x009B0200
    virtual void InternalLoadBinary(CDataInputStream&);                  // slot 22 0x009AED70
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             // slot 24 0x004303E0
    CDefString InstantiationName;                                        // +0x25
    CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)

class CBlurredAttitudesDef : public CParentDefClassBase {   // vtable 0x0125B8AC
public:
    std::vector<EOpinionAttitudeType> Attitudes;         // +0x3c
    virtual void Transfer(CPersistContext&);             // slot 18 0x0067CDB0
    virtual void Copy(const CDefClassBase*);             // slot 19 0x0067C980
    virtual unsigned long GetSizeofClass();              // slot 20 0x0067C510
};

void TUHarness_DestroyCBlurredAttitudesDef(CBlurredAttitudesDef* def)   // emission helper, not a retail function
{
    def->CBlurredAttitudesDef::~CBlurredAttitudesDef();
}