// CGameflowScript::DeclareGossipCategories() -- register the gameflow's seven rumour categories (Sweet Kid, Young Apprentice, Young Annoyance, Wannabe Hero, Complete Melee/Skill/Will) with their rumour text, the Guild village and the factions that gossip about them
// Leaf: DeclareGossipCategories
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D3B410, 1856 bytes (+ int3 padding), plain `ret`; no EH frame.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CGameScriptInterface slots 717
//   AddRumourCategory 0x008A9DB0, 718 AddNewRumourToCategory 0x008A9DD0, 721 AddGossipVillage 0x008A9E30, 722
//   AddGossipFactionToCategory 0x008A9E60 (all take const CCharString&; the literals convert implicitly, so both
//   temporaries are built before the first push, right to left).
// Layout: quest-script base CScriptBase (retail 0x40 bytes), CGameScriptInterface* at +0x40.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __forceinline ~CCountedPointer()
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
struct CScriptThingImpl;
struct CBase {                                          // vtable 0x01231710
    CBase() { }                                         // out-of-line copy 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
class C3DVector { public: float X, Y, Z; };
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBase(), PImp(other.PImp) { }
    ~CScriptThing();                                    // 0x004AA840
};
struct CBaseIntelligentPointer {                        // vtable 0x0129A7D8
    void* m_pIntelligentPointer;                        // +0x04
    CBaseIntelligentPointer();                          // 0x0099A380
    virtual ~CBaseIntelligentPointer();                 // 0x0099A430
};
class CScriptGameResourceObjectBase : public CBaseIntelligentPointer { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
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
struct CGameflowScript : CScriptBase {
    CGameScriptInterface* GSI;                          // +0x40
    void DeclareGossipCategories();
};

void CGameflowScript::DeclareGossipCategories()
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