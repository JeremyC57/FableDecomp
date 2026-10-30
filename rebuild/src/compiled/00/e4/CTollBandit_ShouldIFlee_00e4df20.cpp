// CTollBandit::Should_I_Flee(CScriptGameResourceObjectScriptedThingBase&) -- once the bandits flee, take scripting control of the bandit, run him to the escape marker until he is close to it, then fade him out and kill him
// Leaf: ?Should_I_Flee@CTollBandit@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E4DF20, 433 bytes (448 with padding), `ret 4`.
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, the handle's IsValid 0x00CD23B9 / Release
//   0x00CD2770 and virtuals MoveToPosition (slot 4), GetScriptThing (slot 12), IsPerformingScriptTask (slot 26),
//   IsDistanceFromThingToPositionOver 0x00CBE45C and FadeOutAndKillEntity 0x00CBE716 (`__fastcall`), ~CScriptThing
//   0x004AA840; through the interface (+4) NewScriptFrame (slot 7), StartScriptingEntity (slot 8); BanditEscapeMarker's
//   GetPos (CScriptThing slot 6, virtual on the member). Float 2.0f 0x0122DCDC.
// Layout: CTollBandit as Ego_r.pdb (Game +4, Me +8, ParentClass +0x14, BanditEscapeMarker +0x28); CV_BanditTollScript
//   BanditsFlee +0x4f (PDB - 0x14); CScriptDef TollBanditCloseToExit +0x43c, TollBanditTimeToFade +0x440 (retail; PDB +0x470/+0x474).
namespace NScript { class CPredicateBase; }
class C3DVector { public: float X, Y, Z; };
class CScriptThing { public:                                       // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing();                                // 0x004AA840
    virtual void t01() const; virtual void t02() const; virtual void t03() const; virtual void t04() const; virtual void t05() const;
    virtual const C3DVector& GetPos() const;                // slot 6 (+0x18)
    void* PImpData;
    void* PImpInfo;
};
enum EScriptEntityMoveType { MOVE_TYPE_WALK = 0, MOVE_TYPE_RUN = 1 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_4 = 4 };
class CScriptGameResourceObjectScriptedThingBase { public:         // vtable 0x0127094C, 0x10 bytes
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    virtual void s001(); virtual void s002(); virtual void s003();
    virtual void MoveToPosition(const C3DVector& pos, float speed, EScriptEntityMoveType type, bool a, bool b); // slot 4 (+0x10)
    virtual void s005(); virtual void s006(); virtual void s007(); virtual void s008(); virtual void s009();
    virtual void s010(); virtual void s011();
    virtual CScriptThing GetScriptThing() const;                                        // slot 12 (+0x30)
    virtual void s013(); virtual void s014(); virtual void s015(); virtual void s016(); virtual void s017(); virtual void s018(); virtual void s019();
    virtual void s020(); virtual void s021(); virtual void s022(); virtual void s023(); virtual void s024(); virtual void s025();
    virtual bool IsPerformingScriptTask() const;                                        // slot 26 (+0x68)
    void* m_pIntelligentPointer;
    void* PImpData;
    void* PImpInfo;
    bool IsValid() const;                                     // 0x00CD23B9
    void Release();                                         // 0x00CD2770
};
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, float seconds, bool kill, bool removeOnFade);   // 0x00CBE716
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptDef { public:
    unsigned char _pad[0x43c];
    float         TollBanditCloseToExit;                    // +0x43c
    float         TollBanditTimeToFade;                     // +0x440
};
extern CScriptDef* pGlobalScriptDef;                            // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CV_BanditTollScript {
    unsigned char _pad[0x4f];
    bool          BanditsFlee;                              // +0x4f
};
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CTollBandit : public CEntityScriptBase {
public:
    void*                 __vftable;                        // +0x00
    CGameScriptInterface* Game;                             // +0x04
    CScriptThing          Me;                               // +0x08
    CV_BanditTollScript*  ParentClass;                      // +0x14
    void*                 PMasterData;                      // +0x18
    long                  TollState;                        // +0x1c
    bool                  TollBanditOnlyCollectsToll;       // +0x20
    bool                  KilledReluctantTrader;            // +0x21
    long                  TollBanditPayMeDistance;          // +0x24
    CScriptThing          BanditEscapeMarker;               // +0x28
    void Should_I_Flee(CScriptGameResourceObjectScriptedThingBase& bandit);
};

void CTollBandit::Should_I_Flee(CScriptGameResourceObjectScriptedThingBase& bandit)
{
    if (!ParentClass->BanditsFlee)
        return;
    if (IsActiveThreadTerminating())
        return;
    if (bandit.IsValid())
        bandit.Release();
    while (!Game->StartScriptingEntity(Me, bandit, SCRIPT_AI_PRIORITY_4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    float closeToExit = pGlobalScriptDef->TollBanditCloseToExit;
    C3DVector exitPos = BanditEscapeMarker.GetPos();
    float range = closeToExit + 2.0f;
    while (IsDistanceFromThingToPositionOver(bandit.GetScriptThing(), exitPos, range)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        bandit.MoveToPosition(exitPos, closeToExit, MOVE_TYPE_RUN, false, true);
        while (bandit.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    FadeOutAndKillEntity(Me, pGlobalScriptDef->TollBanditTimeToFade, true, true);
}