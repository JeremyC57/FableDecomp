// NScript::CQ_GuildTrainingScript::GossipSetup() -- register the guild-training quest's seven rumour categories (Sweet Kid, Young Apprentice, Young Annoyance, Wannabe Hero, Complete Melee/Skill/Will) with their rumour text, the Guild village and the factions that gossip about them
// Leaf: ?GossipSetup@CQ_GuildTrainingScript@NScript@@QAEXXZ
// Respelled 2026-11-08 (chunk 853 evidence): was CGameflowScript::DeclareGossipCategories, which is 0x00CE6D00; this body sits in the
//   CQ_GuildTrainingScript block (ctor 0x00D3B390, RegisterMain 0x00D3BB50) and registers guild-training rumour categories; Ego_r.pdb /
//   FableWin.pdb ?GossipSetup@CQ_GuildTrainingScript@NScript@@QAEXXZ. Bytes unchanged (name only).
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D3B410, 1856 bytes (+ int3 padding), plain `ret`; no EH frame.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CGameScriptInterface slots 717
//   AddRumourCategory 0x008A9DB0, 718 AddNewRumourToCategory 0x008A9DD0, 721 AddGossipVillage 0x008A9E30, 722
//   AddGossipFactionToCategory 0x008A9E60 (all take const CCharString&; the literals convert implicitly, so both
//   temporaries are built before the first push, right to left).
// Layout: quest-script base CScriptBase (retail 0x40 bytes), CGameScriptInterface* at +0x40.
#include "fable_compiler.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
struct CScriptThingImpl;
struct CBase {                                          // vtable 0x01231710
    CBase() { }                                         // out-of-line copy 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
#include "engine/C3DVector.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CScriptGameResourceObjectBase : public CBaseIntelligentPointer { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    FABLE_FORCEINLINE ~CScriptGameResourceObjectBase() { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_GuildTrainingScript : public CScriptBase { public:
    CGameScriptInterface* GSI;                          // +0x40
    void GossipSetup();
};
}

void NScript::CQ_GuildTrainingScript::GossipSetup()
{
    GSI->AddRumourCategory("Sweet Kid");
    GSI->AddNewRumourToCategory("Sweet Kid", "TEXT_AI_GOSSIP_SWEET_KID");
    GSI->AddGossipVillage("Sweet Kid", "VILLAGE_GUILD_COMPLEX_INSIDE");
    GSI->AddGossipFactionToCategory("Sweet Kid", "FACTION_GUILD_SERVANTS");
    GSI->AddGossipFactionToCategory("Sweet Kid", "FACTION_VILLAGERS");
    GSI->AddGossipFactionToCategory("Sweet Kid", "FACTION_GUILD_APPRENTICES_GOOD");
    GSI->AddNewRumourToCategory("Young Apprentice", "TEXT_AI_GOSSIP_YOUNG_APPRENTICE");
    GSI->AddGossipVillage("Young Apprentice", "VILLAGE_GUILD_COMPLEX_INSIDE");
    GSI->AddGossipFactionToCategory("Young Apprentice", "FACTION_GUILD_SERVANTS");
    GSI->AddNewRumourToCategory("Young Annoyance", "TEXT_AI_GOSSIP_YOUNG_ANNOYANCE");
    GSI->AddGossipVillage("Young Annoyance", "VILLAGE_GUILD_COMPLEX_INSIDE");
    GSI->AddGossipFactionToCategory("Young Annoyance", "FACTION_VILLAGERS");
    GSI->AddRumourCategory("Wannabe Hero");
    GSI->AddNewRumourToCategory("Wannabe Hero", "TEXT_AI_GOSSIP_WANNABE_HERO");
    GSI->AddGossipVillage("Wannabe Hero", "VILLAGE_GUILD_COMPLEX_INSIDE");
    GSI->AddGossipFactionToCategory("Wannabe Hero", "FACTION_GUILD_APPRENTICES_GOOD");
    GSI->AddRumourCategory("Complete Melee");
    GSI->AddNewRumourToCategory("Complete Melee", "TEXT_AI_GOSSIP_MELEE_COMPLETE");
    GSI->AddGossipVillage("Complete Melee", "VILLAGE_GUILD_COMPLEX_INSIDE");
    GSI->AddGossipFactionToCategory("Complete Melee", "FACTION_GUILD_APPRENTICES_GOOD");
    GSI->AddRumourCategory("Complete Skill");
    GSI->AddNewRumourToCategory("Complete Skill", "TEXT_AI_GOSSIP_SKILL_COMPLETE");
    GSI->AddGossipVillage("Complete Skill", "VILLAGE_GUILD_COMPLEX_INSIDE");
    GSI->AddGossipFactionToCategory("Complete Skill", "FACTION_GUILD_APPRENTICES_GOOD");
    GSI->AddRumourCategory("Complete Will");
    GSI->AddNewRumourToCategory("Complete Will", "TEXT_AI_GOSSIP_WILL_COMPLETE");
    GSI->AddGossipVillage("Complete Will", "VILLAGE_GUILD_COMPLEX_INSIDE");
    GSI->AddGossipFactionToCategory("Complete Will", "FACTION_GUILD_APPRENTICES_GOOD");
}