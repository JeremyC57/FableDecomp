// CAS_Buddy::Init() -- ambush scam buddy setup: ally with the hero both ways, allow following the hero, clear the three approach/speech flags, mark the buddy as having information, remember its start position and make it wander around it (min 0, max from the script def) in scripting state group 4
// Leaf: ?Init@CAS_Buddy@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E2BFA0, length 345 bytes (plain `ret`; vtable slot 2 = Init of the entity
//          script contract).
// Callees: the game interface's slots 70 GetHero, 599 EntitySetThingAsAllyOfThing, 235 EntitySetAsAllowedToFollowHero, 360
//          SetThingHasInformation, 763 SetWanderCentrePoint, 766 SetWanderMinDistance, 769 SetWanderMaxDistance, 784
//          SetScriptingStateGroup (cgsi_slots_aligned.tsv); CScriptThing slot 6 GetPos on Me; data: pGlobalScriptDef @ 0x0143E90C
//          (+0x298 the buddy wander distance). Vtables 0x01231710 (CBaseClass) / 0x01238C8C (CScriptThing) stored by the
//          inline copy constructor of each by-value CScriptThing argument.
// Shape: the four by-value CScriptThing arguments are built in the call area (base vtable, derived vtable, Data, Info with
//          the null-tested AddRef) after the scalar argument push; the by-value C3DVector is copied through its user copy
//          constructor; the start position is assigned member-wise from GetPos; the three flags are cleared from one zero
//          register between the argument pushes of SetThingHasInformation.
// Layout: Ego_r.pdb CAS_Buddy (44 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
//          PMasterData +0x18, BuddyHeroApproachComplete +0x1c, BuddyHeroApproachSpeechComplete +0x1d,
//          BuddyFollowSpeechComplete +0x1e, BuddyStartPos +0x1f (unaligned C3DVector: pack(1) on the script only).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
};

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
class CScriptThing;
template <class T> class CCountedPointer { public:                                // CCountedPointer<CScriptThing>
    CScriptThing*   Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptDef { public:
    unsigned char _pad[0x298];
    float AS_BuddyWanderDistance;                        // +0x298
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

struct CV_AmbushScamScript;
struct CQ_SunnyvaleMasterData;

#pragma pack(push, 1)
struct CAS_Buddy {
    void* __vftable;                                     // +0x00
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing Me;                                     // +0x08
    CV_AmbushScamScript* ParentClass;                    // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                 // +0x18
    bool BuddyHeroApproachComplete;                      // +0x1c
    bool BuddyHeroApproachSpeechComplete;                // +0x1d
    bool BuddyFollowSpeechComplete;                      // +0x1e
    C3DVector BuddyStartPos;                             // +0x1f
    void Init();
};
#pragma pack(pop)

void CAS_Buddy::Init()
{
    Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
    Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
    Game->EntitySetAsAllowedToFollowHero(Me, true);
    BuddyHeroApproachComplete = false;
    BuddyHeroApproachSpeechComplete = false;
    BuddyFollowSpeechComplete = false;
    Game->SetThingHasInformation(Me, true, false, false);
    BuddyStartPos = Me.GetPos();
    Game->SetWanderCentrePoint(Me, BuddyStartPos);
    Game->SetWanderMinDistance(Me, 0.0f);
    Game->SetWanderMaxDistance(Me, pGlobalScriptDef->AS_BuddyWanderDistance);
    Game->SetScriptingStateGroup(Me, (EScriptingStateGroups)4);
}