// NScript::CQ_BanditCampBossBattleScript::CGate3Guard::Init() -- the thing has no information, joins FACTION_TWINBLADE_CAMP_BANDITS and is not pushable by the hero
// Leaf: ?Init@CGate3Guard@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00d07410, 120 bytes, thiscall, plain `ret` (virtual, entity vtable slot 2).
// Identity (proven): the landed CGate3Guard::Alloc 0x00D07340 stores the entity vtable 0x012C8E9C whose slot 2 this is (Ego_r.pdb has ?Init@CGate3Guard@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ); entity vtable slot 2 is Init (Ego_r.pdb `?Init@<entity>@<quest>@NScript@@EAEXXZ`, private virtual).
// Callees: CGameScriptInterface (ghidra_out/cgsi_slots_aligned.tsv) slots 360 SetThingHasInformation(const CScriptThing&,
//   bool, bool, bool) (+0x5a0), 601 EntitySetInFaction(const CScriptThing&, const CCharString&) (+0x964) and 844
//   SetIsPushableByHero(CScriptThing, bool) (+0xd30); CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0
//   ("FACTION_TWINBLADE_CAMP_BANDITS" 0x012C8620).
// Shape (family L138): the by-value CScriptThing argument is the inline copy (CBaseClass vtable 0x01231710, then
//   CScriptThing's 0x01238C8C, PImp copied with the count bumped) built in the outgoing argument area.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18 (as the landed entity scripts).
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CCountBlock { long References; };
template <class T> class CCountedPointer { public:                                // CCountedPointer<CScriptThing>: {Data, Info}
    void* Data;
    CCountBlock* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->References; }
};
class CScriptThing : public CBaseClass { public:        // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                               // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    virtual ~CScriptThing();                            // 0x004AA840
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_BanditCampBossBattleScript { public:
    struct CGate3Guard {                                      // entity vtable 0x012C8E9C
        CGameScriptInterface* Game;                     // +0x04
        CScriptThing          Me;                       // +0x08
        CQ_BanditCampBossBattleScript* ParentClass;     // +0x14
        void*                 PMasterData;              // +0x18
        virtual void v0();
        virtual void Main();
    private:
        virtual void Init();                            // slot 2
    };
};
void CQ_BanditCampBossBattleScript::CGate3Guard::Init()
{
    Game->SetThingHasInformation(Me, false, true, false);
    Game->EntitySetInFaction(Me, "FACTION_TWINBLADE_CAMP_BANDITS");
    Game->SetIsPushableByHero(Me, false);
}
}