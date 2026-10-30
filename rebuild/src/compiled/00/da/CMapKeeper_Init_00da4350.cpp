// CMapKeeper::Init() -- map-keeper entity script init: clear DoneIntro, put Me in "FACTION_MAP_KEEPER", reset ImprisonedFX to an empty script thing, make Me undamageable, not respond to hits and not force-pushable, add the "HUD_ORB_QUEST_CORE" minimap marker and the "ARCHAEOLOGIST" narrator
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA4350, 317 bytes (+ int3 padding). Entity-script slot 2 (Init).
// Callees: CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0, CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0, operator delete
//   0x00BFE9BC; CGameScriptInterface slots 350 MiniMapAddMarker (+0x578), 362 EntitySetWillBeUsingNarrator (+0x5a8),
//   515 EntitySetAsRespondToHit (+0x80c), 516 EntitySetAsDamageable (+0x810), 601 EntitySetInFaction (+0x964),
//   745 SetIsThingForcePushable (+0xba4).
// Layout = Ego_r.pdb CMapKeeper (44 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18,
//   DoneIntro +0x1c, ImprisonedFX +0x20.
// Shape: `ImprisonedFX = CScriptThing();` -- the default temporary (out-of-line CBaseClass ctor, vtable, zero PImp), the
//   implicit CScriptThing::operator= forwarding to an INLINE CCountedPointer::operator= that reads both source words into
//   locals before the info compare (GOTCHAS wave 24n), so the known-zero source folds into ebx stores inside the release
//   block; then the temporary's inline destructor.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    CScriptThing() { }
    ~CScriptThing() {}
    virtual void v01();
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CMapKeeper {
public:
    void*                 __vftable;          // +0x00
    CGameScriptInterface* Game;               // +0x04
    CScriptThing          Me;                 // +0x08
    void*                 ParentClass;        // +0x14 CQ_MinionClifftopChaseScript*
    void*                 PMasterData;        // +0x18
    bool                  DoneIntro;          // +0x1c
    CScriptThing          ImprisonedFX;       // +0x20
    void Init();
};

void CMapKeeper::Init()
{
    DoneIntro = false;
    Game->EntitySetInFaction(Me, "FACTION_MAP_KEEPER");
    ImprisonedFX = CScriptThing();
    Game->EntitySetAsDamageable(Me, false);
    Game->EntitySetAsRespondToHit(Me, false);
    Game->SetIsThingForcePushable(Me, false);
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    Game->EntitySetWillBeUsingNarrator(Me, "ARCHAEOLOGIST");
}