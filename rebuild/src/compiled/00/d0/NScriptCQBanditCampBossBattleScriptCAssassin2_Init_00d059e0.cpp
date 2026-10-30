// NScript::CQ_BanditCampBossBattleScript::CAssassin2::Init() -- the thing has no information, joins FACTION_HERO and is not pushable by the hero
// Leaf: ?Init@CAssassin2@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00d059e0, 120 bytes, thiscall, plain `ret` (virtual, entity vtable slot 2).
// Identity (proven): the landed CAssassin2::Alloc 0x00D05910 stores the entity vtable 0x012C8D28 whose slot 2 this is (Ego_r.pdb has ?Init@CAssassin2@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ); entity vtable slot 2 is Init (Ego_r.pdb `?Init@<entity>@<quest>@NScript@@EAEXXZ`, private virtual).
// Callees: CGameScriptInterface (ghidra_out/cgsi_slots_aligned.tsv) slots 360 SetThingHasInformation(const CScriptThing&,
//   bool, bool, bool) (+0x5a0), 601 EntitySetInFaction(const CScriptThing&, const CCharString&) (+0x964) and 844
//   SetIsPushableByHero(CScriptThing, bool) (+0xd30); CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0
//   ("FACTION_HERO" 0x0126C3A4).
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_BanditCampBossBattleScript { public:
    struct CAssassin2 {                                      // entity vtable 0x012C8D28
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
void CQ_BanditCampBossBattleScript::CAssassin2::Init()
{
    Game->SetThingHasInformation(Me, false, true, false);
    Game->EntitySetInFaction(Me, "FACTION_HERO");
    Game->SetIsPushableByHero(Me, false);
}
}