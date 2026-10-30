// CM_BodyGuardRespawn::Main() -- bodyguard respawn marker: find which of the four bodyguards this marker stands for (from its data string); if that bodyguard is dead (status 1) and its respawn timer has run out, respawn the matching CREATURE_BODY_GUARD* creature here as "BodyGuard", tag it with its _EVIL1/_EVIL2/_GOOD1/_GOOD2 suffix and mark it alive
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E5E0C0, length 360 bytes + a 4-entry jump table (0x00E5E228).
// Callees: CScriptThing slots 3 GetDataString, 4 SetDataString (0x004AA930, inlined on the local), 6 GetPos;
//   CV_BodyGuardScript's index-from-data-string helper @ 0x00E5E240 (manifest "GetAlignmentFromCode"), CCharString()
//   @ 0x0099E4B0, operator=(const char*) @ 0x0099EFE0, CCharString(const char*, long) @ 0x0099EBF0, ~CCharString
//   @ 0x0099EAE0, ~CScriptThing @ 0x004AA840, IsDone @ 0x00F35B30; the game script interface global @ 0x0143E8F8 slot 90
//   GetTimer and Game's slot 91 CreateCreature (CScriptThing by value). Strings: "CREATURE_BODY_GUARD_WEAK" @ 0x012578E0,
//   "CREATURE_BODY_GUARD" @ 0x012578FC, "CREATURE_BODY_GUARD_GOOD_WEAK" @ 0x012578A4, "CREATURE_BODY_GUARD_GOOD"
//   @ 0x012578C4, "_EVIL1" @ 0x012E706C, "_EVIL2" @ 0x012E7064, "_GOOD1" @ 0x012E705C, "_GOOD2" @ 0x012E7054,
//   "BodyGuard" @ 0x012CB3F0.
// Layout: Ego_r.pdb CM_BodyGuardRespawn (28 bytes): entity-script base {vptr, Game +4, CScriptThing Me +8}, ParentClass
//   (CV_BodyGuardScript*) +0x14. Retail CV_BodyGuardScript sits 0x18 bytes below the PDB here: HenchmanDeadRespawnTimer
//   (CTimer[4], one timer index each) at +0x74 and HenchmanGlobalStatus (long[4]) at +0x84 (PDB +0x8C/+0x9C), so the
//   quest is modelled locally.
// Shape: the two strings are default-constructed and assigned per case (the suffix assignment is tail-merged across the
//   cases); the switch's default exits through the strings' destructors.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class C3DVector { public: float x, y, z; };
class CCPPointerInfo;
class CScriptThing { public:                              // vtable 0x01238C8C, 12 bytes
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CCharString GetDataString() const;     // slot 3 (+0x0c)
    virtual void SetDataString(const CCharString& text)   // slot 4 (+0x10), 0x004AA930
    {
        if (PImp_Data)
            PImp_Data->SetDataString(text);
    }
    virtual void v005();
    virtual const C3DVector& GetPos() const;       // slot 6 (+0x18)
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
    virtual ~CScriptThing();                               // 0x004AA840
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
struct CV_BodyGuardScript {
    unsigned char _pad[0x74];
    CTimer        HenchmanDeadRespawnTimer[4];     // +0x74
    long          HenchmanGlobalStatus[4];         // +0x84
    long GetHenchmanIndex(const CCharString& code);    // 0x00E5E240
};
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CM_BodyGuardRespawn : public CEntityScriptBase {
public:
    void*                   __vftable;             // +0x00
    CGameScriptInterface*   Game;                  // +0x04
    CScriptThing            Me;                    // +0x08
    CV_BodyGuardScript*     ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
    void Main();
};

void CM_BodyGuardRespawn::Main()
{
    long index = ParentClass->GetHenchmanIndex(Me.GetDataString());
    if (ParentClass->HenchmanGlobalStatus[index] != 1)
        return;
    if (ParentClass->HenchmanDeadRespawnTimer[index].GetTime() != 0)
        return;
    if (IsActiveThreadTerminating())
        return;
    CCharString def;
    CCharString suffix;
    switch (index) {
    case 0:
        def = "CREATURE_BODY_GUARD_WEAK";
        suffix = "_EVIL1";
        break;
    case 1:
        def = "CREATURE_BODY_GUARD";
        suffix = "_EVIL2";
        break;
    case 2:
        def = "CREATURE_BODY_GUARD_GOOD_WEAK";
        suffix = "_GOOD1";
        break;
    case 3:
        def = "CREATURE_BODY_GUARD_GOOD";
        suffix = "_GOOD2";
        break;
    default:
        return;
    }
    CScriptThing bodyguard = Game->CreateCreature(def, Me.GetPos(), "BodyGuard", false);
    bodyguard.SetDataString(suffix);
    ParentClass->HenchmanGlobalStatus[index] = 0;
}