// CScytheMarker::Main() -- the scythe marker entity script: spawn the rival hero Scythe (CREATURE_RIVAL_HERO_SCYTHE) at the marker with the ScytheNearOracle script, then remove the marker
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E29F50, length 195 bytes (`ret`).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 and ~CCharString @ 0x0099EAE0 (the two names),
// CScriptThing slot 6 GetPos (+0x18), CGameScriptInterface slots 91 CreateCreature(const CCharString&, const C3DVector&,
// const CCharString&, bool) (CScriptThing by value) and 108 RemoveThing(const CScriptThing&, bool, bool), operator delete
// @ 0x00BFE9BC and CBaseClass::~CBaseClass @ 0x0099A2E0 (the inlined ~CScriptThing of the created creature).
// Layout = CEntityScriptBase {vptr, Game +4, CScriptThing Me +8} (+ ParentClass +0x14, PMasterData +0x18; PDB 28 bytes).
// Shape: the landed CSP_NymphTrigger::Main 0x00DED380 spawn (implicit CCharString conversions built before every other
// argument, the result a named local) without the wait loop; in this module ~CScriptThing is inline -- the CScriptThing
// vtable store (0x01238C8C), the PImp counted-pointer release (`--count; if (count == 0)` re-reading the block, the
// destroy member pointer on the item, delete, both words cleared) and the out-of-line CBaseClass destructor -- as in
// the landed CScriptThingFunctor_00e241b0 destructor.
#include <stddef.h>
void __cdecl operator delete(void* block);          // 0x00BFE9BC

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CCountedItem { };
struct SCountBlock {
    long m_references;                               // +0x00
    void (CCountedItem::*m_pfnDestroy)();            // +0x04 (called with the item as `this`)
    CCountedItem* m_pItem;                           // +0x08
};
class CGameScriptThing;
template <class T> class CCountedPointer { public:             // CCountedPointer<CGameScriptThing>
    CGameScriptThing* Data;                          // +0x00
    SCountBlock*      Info;                          // +0x04
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                (Info->m_pItem->*Info->m_pfnDestroy)();
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScytheMarker {
public:
    void*                 __vftable;                 // +0x00
    CGameScriptInterface* Game;                      // +0x04
    CScriptThing          Me;                        // +0x08
    void*                 ParentClass;               // +0x14
    void*                 PMasterData;               // +0x18
    void Main();
};

void CScytheMarker::Main()
{
    CScriptThing scythe = Game->CreateCreature("CREATURE_RIVAL_HERO_SCYTHE", Me.GetPos(), "ScytheNearOracle", false);
    Game->RemoveThing(Me, false, true);
}