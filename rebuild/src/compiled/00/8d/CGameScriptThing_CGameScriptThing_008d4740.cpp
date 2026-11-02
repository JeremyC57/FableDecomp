// CGameScriptThing::~CGameScriptThing() -- implicit script-thing handle destructor: destroy DataString, Name and PThing, store the CScriptThing vtable (inline ~CScriptThing: release PImp), then ~CBaseClass
// Leaf: ??1CGameScriptThing@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D4740, int3 padding before and after, thiscall, tail `jmp ~CBaseClass` 0x0099A2E0.
// Identity (proven, two sources): (1) its sole caller is the scalar deleting destructor 0x008D4720, slot 0 of vtable 0x012852CC
//   (RTTI .?AVCGameScriptThing@@ : CScriptThing : CBaseClass), and it stores the CScriptThing vtable 0x01238C8C before the tail;
//   (2) Ego_r.pdb 0x0090B420 / FableWin.pdb 0x02AFE830 ??1CGameScriptThing@@UAE@XZ. No 0x012852CC store: compiler-generated.
// Callees: ??1CCharString@@QAE@XZ 0x0099EAE0 (DataString +0x24, Name +0x20), ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10 (PThing
//   +0x0C), operator delete 0x00BFE9BC (PImp), ??1CBaseClass@@UAE@XZ 0x0099A2E0 (tail).
// Shape: members in reverse order (Pos / HomePos / AngleXY / World trivial), then CScriptThing's empty inline dtor (engine/
//   CScriptThing.h contract) and its by-value CCountedPointer PImp (engine/CCountedPointer.h: DeleteFunc, operator delete 0x00BFE9BC). The private PDB ctor CGameScriptThing(CWorld&) is defined (not graded) to emit the vtable, ??_G
//   (RELOCATION_MATCH vs 0x008D4720) and this ??1.
// Layout: engine/CGameScriptThing.h FABLE_RETAIL_CGAMESCRIPTTHING (chunk 715 overlay: Ego_r.pdb members, sizeof 0x48; the default
//   branch flattens CScriptThing and the intelligent pointer). CCharString / C3DVector / CIntelligentPointer are local (the overlay's
//   includer contract).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() { }

class CCharStringData;
class CCharString { public:
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};
class C3DVector { public: float X; float Y; float Z; };
class CBaseObject;
class CBaseObjectPointer;
#pragma pack(push, 1)
class CBaseIntelligentPointer {                         // vtable 0x0129C95C (vptr + PData, 8 bytes; 1-aligned in retail, chunk 706 model)
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10 ??1CBaseIntelligentPointer@@UAE@XZ
    CBaseObject* GetPItem() const;                      // 0x00A01B50 ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
private:
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {    // FableWin member set: Get / Set / -> / *
public:
    T* Get() const { return static_cast<T*>(GetPItem()); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
};
#pragma pack(pop)

#define FABLE_RETAIL_CGAMESCRIPTTHING
#include "engine/CGameScriptThing.h"

CGameScriptThing::CGameScriptThing(CWorld& world)
    : World(&world)
{
}