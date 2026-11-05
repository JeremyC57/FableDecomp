// CCutsceneDef::~CCutsceneDef() -- implicit definition destructor: destroy the eight vector<CCharString> members in reverse order (Answer1 .. Macro: each element's ~CCharString, then free the storage), free the SubDefInfoMap vector storage (+0x29), then the inline CDefClassBase / CDefPointeeBase levels (CDefPointeeBase vtable) and ~CResource
// Leaf: ??1CCutsceneDef@@UAE@XZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F29DC0, 382 bytes, thiscall, tail `jmp 0x009FC550` (~CResource).
// Identity (proven): its only caller is 0x00F29DA3 inside the scalar deleting destructor 0x00F29DA0, slot 0 of vtable
//   0x012FB6E0 -- the vtable the landed CCutsceneDef::CCutsceneDef 0x00F29D00 stores (Ego_r.pdb ??0CCutsceneDef@@QAE@XZ). The
//   vtable's slots 10-16 / 21 / 22 / 24 are CParentDefClassBase's (0x004303B0 .. 0x004303E0) and slots 18-20 are the
//   CCutsceneDef overrides Transfer 0x00F2A1D0, Copy 0x00F29FF0, GetSizeofClass 0x00F29D90, so the retail TU order is
//   GetSizeofClass, ctor, ??_G, ??1, Copy, Transfer -- the Ego_r.pdb CCutsceneDef set (??0, ??1CCutsceneDef@@UAE@XZ, ??_E,
//   Alloc, Transfer, ??4, Copy) and FableWin.pdb's (??0, Copy, GetSizeofClass, Transfer, ??4, ??_E, ??1). The member offsets
//   are Ego_r.pdb's eight `vector<CCharString>` members Macro, SkipCond, Lights, LightScene, SetupCond, Sound, Answer0, Answer1
//   at retail +0x3C..+0x90 (12-byte STLport vectors after the 0x39-byte packed CParentDefClassBase), destroyed last-first.
//   The catalog label "CTCInventoryMap::ResetPrimitiveHandles" is propagated and wrong (the 0x00F29DA0 ??_G is landed as a
//   CLandscapeBackgroundPatch `vector deleting destructor` stand-in calling this row as @CLandscapeBackgroundPatch_dtor_00f29dc0@4).
// Callees: CCharString::~CCharString 0x0099EAE0 (per element), free 0x00BFEA14 (STLport allocator<T>::deallocate),
//   CResource::~CResource 0x009FC550.
// Shape: CCutsceneDef and CParentDefClassBase declare no destructor (VC7.1 drops their dead vtable stores; only the
//   CDefPointeeBase store of the inline ~CDefClassBase remains); each STLport ~vector<CCharString> keeps start/finish in
//   edi/ebx for the _Destroy loop and ~_Vector_base re-reads _M_start for the null-tested free; the SubDefInfoMap's
//   pair elements are trivially destructible (__true_type: no loop). Model copied from the landed ~CCardDef 0x007D7950.
// Layout: CDefClassBase from engine/CDefClassBase.h (retail model, packed, 0x25); CParentDefClassBase (InstantiationName
//   CDefString +0x25, SubDefInfoMap CVectorMap +0x29, 0x39 bytes) is modelled locally as in ~CCardDef: engine/
//   CParentDefClassBase.h declares ~CParentDefClassBase out of line (0x00430300), and this body inlines it.
//   The TU emitter is the PDB default constructor (Ego_r ??0CCutsceneDef@@QAE@XZ, retail 0x00F29D00) with an empty body:
//   it emits the vtable, the ??_G and this implicit ??1 (the constructor body itself is the separate landed row).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CCharString {
public:
    ~CCharString();                                        // 0x0099EAE0
private:
    void* PStringData;
};

class CDefString { public:
    long TablePos;                                         // +0x00
    CDefString() : TablePos(-1) { }
};

#include "engine/CVectorMap.h"


#pragma pack(push, 1)                                      // retail: the def chain is packed (CDefString at +0x25)
class CParentDefClassBase : public CDefClassBase {         // vtable 0x01230CBC
public:
    CParentDefClassBase();                                 // 0x00430370
    CDefString InstantiationName;                          // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >
        SubDefInfoMap;                                     // +0x29
};
#pragma pack(pop)

class CCutsceneDef : public CParentDefClassBase {          // vtable 0x012FB6E0
public:
    CCutsceneDef();                                        // 0x00F29D00 (Ego_r ??0CCutsceneDef@@QAE@XZ)
    virtual void Transfer(CPersistContext& context);       // slot 18 0x00F2A1D0 (Ego_r ?Transfer@CCutsceneDef@@UAEXAAVCPersistContext@@@Z)
    virtual void Copy(const CDefClassBase* other);         // slot 19 0x00F29FF0 (Ego_r ?Copy@CCutsceneDef@@UAEXPBVCDefClassBase@@@Z)
    virtual unsigned long GetSizeofClass();                // slot 20 0x00F29D90 (FableWin ?GetSizeofClass@CCutsceneDef@@UAEKXZ)
    std::vector<CCharString> Macro;                        // +0x3c
    std::vector<CCharString> SkipCond;                     // +0x48
    std::vector<CCharString> Lights;                       // +0x54
    std::vector<CCharString> LightScene;                   // +0x60
    std::vector<CCharString> SetupCond;                    // +0x6c
    std::vector<CCharString> Sound;                        // +0x78
    std::vector<CCharString> Answer0;                      // +0x84
    std::vector<CCharString> Answer1;                      // +0x90
};

CCutsceneDef::CCutsceneDef()
{
}