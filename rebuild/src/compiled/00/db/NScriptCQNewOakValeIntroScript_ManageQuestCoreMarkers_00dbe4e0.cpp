// NScript::CQ_NewOakValeIntroScript::ManageQuestCoreMarkers() -- walk the hero through the Oakvale intro's quest-core minimap markers: father first; once the hero has 3+ gold and control, move the marker to the book trader (after the tutorial if it shows), once the sweets are given to Theresa, and once the chocolates are given back to the father
// Leaf: ManageQuestCoreMarkers
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DBE4E0, 944 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840 (out of line except on the gold-wait exit), ~CBaseClass
//   0x0099A2E0, operator delete 0x00BFE9BC; CGameScriptInterface slots 7 NewScriptFrame (+0x1c), 41
//   MsgIsTutorialClickedPast (+0xa4), 72 GetThingWithScriptName (+0x120, CScriptThing by value), 118 DisplayTutorial
//   (+0x1d8), 127 GetHeroGold (+0x1fc), 350 MiniMapAddMarker (+0x578), 352 MiniMapRemoveMarker (+0x580), 394
//   IsHeroControlledByPlayer (+0x628).
// Data: "NOVI_LiveFather" 0x012D8378, "NOVI_BookTrader" 0x012D82D8, "NOVI_Theresa" 0x012D8360, "HUD_ORB_QUEST_CORE"
//   0x01258AB0.
// Layout (retail = Ego_r PDB -0x14): GivenSweets +0x94, GivenTheresaChocs +0x95.
// Shape: the by-value CScriptThing model of the landed WatchForPickpocketing/CheckVictimHit; every wait is the rotated
//   script idiom; ~CScriptThing is __forceinline and `#pragma inline_depth(0)` opens after the first wait, so only
//   that wait's return expands the three destructors inline and every later exit calls 0x004AA840.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CCPPointerInfo { public:                                 // counted-pointer block (Ego_r.pdb)
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CScriptThing;
struct CCountedPointerScriptThing {                     // CCountedPointer<CScriptThing>
    CScriptThing*   Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointerScriptThing() : Data(0), Info(0) {}
    ~CCountedPointerScriptThing()
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

class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CCountedPointerScriptThing PImp;                    // +0x04
    virtual __forceinline ~CScriptThing() {}            // slot 0; out of line: 0x004AA840
    virtual void v001();    virtual void v002();    virtual void v003();    virtual void v004();    virtual void v005();    virtual void v006();    virtual void v007();    virtual void v008();
    virtual void v009();    virtual void v010();    virtual void v011();    virtual void v012();    virtual void v013();    virtual void v014();    virtual void v015();    virtual void v016();
    virtual void v017();
    virtual bool MsgIsKilledBy(const CCharString& killer) const    // slot 18 (+0x48)
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsKilledBy(killer);
    }
    virtual void v019();    virtual void v020();    virtual void v021();    virtual void v022();    virtual void v023();    virtual void v024();    virtual void v025();    virtual void v026();
    virtual void v027();    virtual void v028();    virtual void v029();    virtual void v030();    virtual void v031();    virtual void v032();    virtual void v033();    virtual void v034();
    virtual void v035();    virtual void v036();    virtual void v037();    virtual void v038();    virtual void v039();    virtual void v040();    virtual void v041();    virtual void v042();
    virtual void v043();    virtual void v044();    virtual void v045();    virtual void v046();    virtual void v047();    virtual void v048();    virtual void v049();    virtual void v050();
    virtual void v051();    virtual void v052();    virtual void v053();    virtual void v054();    virtual void v055();    virtual void v056();    virtual void v057();    virtual void v058();
    virtual void v059();    virtual void v060();    virtual void v061();    virtual void v062();    virtual void v063();    virtual void v064();    virtual void v065();    virtual void v066();
    virtual void v067();    virtual void v068();    virtual void v069();    virtual void v070();    virtual void v071();    virtual void v072();    virtual void v073();    virtual void v074();
    virtual void v075();    virtual void v076();    virtual void v077();
    virtual bool IsEqualTo(const CScriptThing* other) const;       // slot 78 (+0x138)
    bool operator==(const CScriptThing& other) const { return PImp.Data->IsEqualTo(other.PImp.Data); }
};

enum ETutorialCategory { TUTORIAL_CATEGORY_0x13 = 0x13 };   // value 0x13 (enumerator unnamed in the dumps)

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQ_NewOakValeIntroScript : public CScriptBase { public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[0x4c];
    bool                  GivenSweets;                  // +0x94
    bool                  GivenTheresaChocs;            // +0x95
    void ManageQuestCoreMarkers();
};

void CQ_NewOakValeIntroScript::ManageQuestCoreMarkers()
{
    CScriptThing father = GSI->GetThingWithScriptName("NOVI_LiveFather");
    CScriptThing bookTrader = GSI->GetThingWithScriptName("NOVI_BookTrader");
    CScriptThing theresa = GSI->GetThingWithScriptName("NOVI_Theresa");
    GSI->MiniMapAddMarker(father, "HUD_ORB_QUEST_CORE");
    while (GSI->GetHeroGold() <= 2) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
#pragma inline_depth(0)
    if (IsActiveThreadTerminating())
        return;
    while (!GSI->IsHeroControlledByPlayer()) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->MiniMapRemoveMarker(father);
    GSI->MiniMapAddMarker(bookTrader, "HUD_ORB_QUEST_CORE");
    if (GSI->DisplayTutorial(TUTORIAL_CATEGORY_0x13)) {
        if (IsActiveThreadTerminating())
            return;
        while (!GSI->MsgIsTutorialClickedPast()) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    while (!GivenSweets) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->MiniMapRemoveMarker(bookTrader);
    GSI->MiniMapAddMarker(theresa, "HUD_ORB_QUEST_CORE");
    while (!GivenTheresaChocs) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->MiniMapRemoveMarker(theresa);
    GSI->MiniMapAddMarker(father, "HUD_ORB_QUEST_CORE");
}
#pragma inline_depth()
}