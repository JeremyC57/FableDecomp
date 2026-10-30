// CV_TravellingHeroesScript::GossipSetup(CCharString category, CCharString rumour, CCharString village,
// CCharString faction) -- register a gossip category with its rumour and village, and its faction (FACTION_GUILD
// expands to the guild apprentices, the guild servants and the villagers)
// Leaf: GossipSetup
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE07E0, 328 bytes (padded to 336), `ret 0x10` (four strings by value).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0, the string compare @ 0x00411570 (__fastcall,
//          same-TU stand-in); CGameScriptInterface slots 717 AddRumourCategory (+0xb34), 718 AddNewRumourToCategory
//          (+0xb38), 721 AddGossipVillage (+0xb44), 722 AddGossipFactionToCategory (+0xb48).
// Layout: interface at +0x40 (CQuestScriptBase model).
// Shape: CCharString::operator==(const char*) inline (an empty handle compares the literal with "", inlined as a
//        14-byte repe cmpsb); the callee destroys its four by-value strings at its end.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (same-TU stand-in of the landed body)
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
    bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};
namespace NScript {
class CV_TravellingHeroesScript : public CQuestScriptBase {
public:
    void GossipSetup(CCharString category, CCharString rumour, CCharString village, CCharString faction);
};
} // namespace NScript
using NScript::CV_TravellingHeroesScript;

void NScript::CV_TravellingHeroesScript::GossipSetup(CCharString category, CCharString rumour, CCharString village, CCharString faction)
{
    GSI->AddRumourCategory(category);
    GSI->AddNewRumourToCategory(category, rumour);
    GSI->AddGossipVillage(category, village);
    if (faction == "FACTION_GUILD") {
        GSI->AddGossipFactionToCategory(category, "FACTION_GUILD_APPRENTICES_GOOD");
        GSI->AddGossipFactionToCategory(category, "FACTION_GUILD_SERVANTS");
        GSI->AddGossipFactionToCategory(category, "FACTION_VILLAGERS");
    } else {
        GSI->AddGossipFactionToCategory(category, faction);
    }
}