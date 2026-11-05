// CTCFollowed::RemoveAllFollowers() -- empty the follower and AI-follower lists and refresh the player GUI
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
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

#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool flag);        // 0x004378C0
};

class CTCFollowed : public CTCBase { public:
    CFollowerList   Followers;                   // +0x0c
    CAIFollowerList AIFollowers;                 // +0x10
    void RemoveAllFollowers();
};

void CTCFollowed::RemoveAllFollowers()
{
    Followers.clear();
    AIFollowers.clear();
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UpdateMiniMapMarkers(false);
}