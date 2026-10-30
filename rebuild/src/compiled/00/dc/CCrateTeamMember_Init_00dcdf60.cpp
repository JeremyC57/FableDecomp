// CCrateTeamMember::Init() -- sort the member into the bandit team (script name "BanditTeamMember") or the guard team, set its faction (friendly to the hero when its team is the hero's, otherwise hostile with a red minimap orb, no emote icon and, for bandits, not turncoatable) and its data string, join its team manager (member count, state-0 counter), reset both AI states to 2 and clear the patrol target
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCDF60, 710 bytes (720 with padding), plain `ret`.
// Callees: CScriptThing::GetName (slot 1, +4, 0x004AA8B0) and SetDataString (slot 4, +0x10, 0x004AA930) on the member's own
//   thing, Compare 0x00411570 (`__fastcall`, the inlined CCharString equality), CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, and through the
//   game script interface at +4: GetHero (slot 70), MiniMapAddMarker (350), EntitySetAsDisplayingEmoteIcon (530),
//   EntitySetThingAsAllyOfThing (599), EntitySetInFaction (601), SetIsThingTurncoatable (744).
// Layout: Ego_r.pdb CCrateTeamMember (landed 0x00DCEC70 GoOnPatrol): script interface +4, own CScriptThing +8, ParentClass
//   +0x14, CurrentAIState +0x1c, PreviousAIState +0x20, TeamID +0x24, MemberState +0x28, MyTeam +0x2c, ThingToPatrolTo +0x30;
//   CQ_OrchardFarmRaidScript (Ego_r.pdb - 0x18): HeroTeam +0x78, Teams[2] (64-byte CCrateTeamManager) +0x8c.
// Shape: the name test is the inlined CCharString == literal (null block: the strcmp intrinsic against "" as `repe cmpsb`
//   branching on its flags; else Compare and `? false : true`, whose byte is homed in the frame); TeamID is stored on both
//   arms and reloaded; inside the bandit arm `TeamID == HeroTeam` folds to `cmp [HeroTeam],1`; every faction / marker /
//   data string is an implicit CCharString temporary built before the interface is loaded (the shared `push -1` is hoisted
//   above the branch); the interface vtable is cached across GetHero(); MemberState = 0 folds the counter index; the patrol
//   target is cleared by assigning a default CScriptThing temporary through the inline counted-pointer operator=.
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare(PData, text) ? false : true; }
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CCrateTeamManager {                                  // Ego_r.pdb 64 bytes
    long StateCounter[6];                                   // +0x00
    long MemberCount;                                       // +0x18
    unsigned char _rest[0x40 - 0x1c];                       // reinforcement timer, max members, drop pos, carrier, enemy team
};
struct CQ_OrchardFarmRaidScript {
    unsigned char     _pad[0x78];
    long              HeroTeam;                             // +0x78
    unsigned char     _pad7c[0x8c - 0x7c];
    CCrateTeamManager Teams[2];                             // +0x8c
};
struct CCrateTeamMember {                                   // Ego_r.pdb: 60 bytes
    virtual void v00();
    CGameScriptInterface*      m_pScript;                   // +0x04
    CScriptThing               m_thing;                     // +0x08: this member's own thing
    CQ_OrchardFarmRaidScript*  ParentClass;                 // +0x14
    void*                      PMasterData;                 // +0x18
    long                       CurrentAIState;              // +0x1c
    long                       PreviousAIState;             // +0x20
    long                       TeamID;                      // +0x24
    long                       MemberState;                 // +0x28
    CCrateTeamManager*         MyTeam;                      // +0x2c
    CScriptThing               ThingToPatrolTo;             // +0x30
    void Init();
};

void CCrateTeamMember::Init()
{
    if (m_thing.GetName() == "BanditTeamMember")
        TeamID = 1;
    else
        TeamID = 0;
    if (TeamID == 1) {
        if (TeamID == ParentClass->HeroTeam) {
            m_pScript->EntitySetInFaction(m_thing, "FACTION_BANDITS_FRIENDLY");
            m_pScript->EntitySetThingAsAllyOfThing(m_thing, m_pScript->GetHero());
        } else {
            m_pScript->EntitySetInFaction(m_thing, "FACTION_BANDITS");
            m_pScript->MiniMapAddMarker(m_thing, "HUD_ORB_RED_SMALL");
            m_pScript->SetIsThingTurncoatable(m_thing, false);
            m_pScript->EntitySetAsDisplayingEmoteIcon(m_thing, false);
        }
        m_thing.SetDataString("BANDIT");
    } else {
        if (TeamID == ParentClass->HeroTeam) {
            m_pScript->EntitySetInFaction(m_thing, "FACTION_VILLAGERS");
            m_pScript->EntitySetThingAsAllyOfThing(m_thing, m_pScript->GetHero());
        } else {
            m_pScript->EntitySetInFaction(m_thing, "FACTION_GUARDS_ENEMY");
            m_pScript->MiniMapAddMarker(m_thing, "HUD_ORB_RED_SMALL");
            m_pScript->EntitySetAsDisplayingEmoteIcon(m_thing, false);
        }
        m_thing.SetDataString("GUARD");
    }
    MyTeam = &ParentClass->Teams[TeamID];
    ++MyTeam->MemberCount;
    CurrentAIState = 2;
    PreviousAIState = 2;
    MemberState = 0;
    ++MyTeam->StateCounter[MemberState];
    ThingToPatrolTo = CScriptThing();
}