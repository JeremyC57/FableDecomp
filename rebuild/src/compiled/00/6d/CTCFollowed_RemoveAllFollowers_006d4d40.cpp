// CTCFollowed::RemoveAllFollowers() -- empty the follower and AI-follower lists and refresh the player GUI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D4D40, length 34 bytes.
// Callees: list<CIntelligentPointer<CThing>>::clear() @ 0x004B8050 and list<CIntelligentPointer<CThingAICreature>>::clear()
// @ 0x00510D40 (the landed CRetailList clear bodies: destroy each value through its virtual destructor, free the node,
// relink the sentinel), CPlayerGui::UpdateMiniMapMarkers(bool) @ 0x004378C0 on the game UI singleton @ 0x013B8790 (landed naming of
// 0x0089EEC0).
// Layout: PDB CTCFollowed (56 bytes): Followers +0xc, AIFollowers +0x18 -- retail lists are a single sentinel pointer
// (PDB 12 bytes), so AIFollowers sits at +0x10.
// Shape: two out-of-line clears on member addresses (`lea ecx,[esi+0Ch]`, `lea ecx,[esi+10h]`), then the global loaded
// into ecx BEFORE the `push 0` (the call cannot be a tail jump: it pops an argument this function does not have).
#include "engine/CTCBase.h"

struct CFollowerList {                           // retail std::list<CIntelligentPointer<CThing>>: one sentinel pointer
    void* head;
    void clear();                                // 0x004B8050
};

struct CAIFollowerList {                         // retail std::list<CIntelligentPointer<CThingAICreature>>
    void* head;
    void clear();                                // 0x00510D40
};

class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool flag);        // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                     // 0x013B8790

class CTCFollowed : public CTCBase { public:
    CFollowerList   Followers;                   // +0x0c
    CAIFollowerList AIFollowers;                 // +0x10
    void RemoveAllFollowers();
};

void CTCFollowed::RemoveAllFollowers()
{
    Followers.clear();
    AIFollowers.clear();
    g_pPlayerGui->UpdateMiniMapMarkers(false);
}