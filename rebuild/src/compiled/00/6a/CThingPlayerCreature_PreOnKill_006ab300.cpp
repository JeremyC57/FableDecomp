// CThingPlayerCreature::PreOnKill() -- for the main player: use a resurrection item from the inventory when one is there, else open the quest-completion UI in "death" mode; either way the kill is vetoed
// Leaf: PreOnKill
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AB300, length 302 bytes (+ int3 padding).
// Callees: CThing::GetPlayer() const @ 0x004C7A30, CPlayer::IsMainPlayer @ 0x00487520, CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (interfaces 0x11 = CTCInventoryBase, bit 17 of +0x20; 0xe1 = CTCResurrectionItem and
// 0xfc = CTCQuestCompletionUI, bits 1 / 28 of +0x3c), CTCInventoryBase::GetResurrectionItemInInventory @ 0x005BF77D,
// CTCResurrectionItem::OnItemUsed(CThingCreatureBase&) @ 0x007F04C0 (landed), CWideString(const wchar_t*) @ 0x0099B6B0 /
// ~CWideString @ 0x0099B510, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
// CTCQuestCompletionUI::Activate(CompletionType, const CCharString&, bool, const CWideString&, bool) @ 0x005E4740.
// Strings: 0x0122D70C L"", 0x0122D70E "".
// Layout: CThing interface bitset +0x20, map +0x44, Dead bit +0x91 (retail, local model).
// Shape: the inventory lookup is the plain entry-pick GetTC used straight as `this`; the resurrection item and the UI are
// `T* p = 0; if (HasTC(k)) p = GetTC(k);` (the zero preload); both Activate strings are implicit conversion
// temporaries (wide first, by right-to-left evaluation); a same-TU LowerBound stand-in keeps every key in one frame word;
// the main-player test wraps the body so the `return true` exit is laid out last.
typedef unsigned short wchar_t;                                // VC7.1 without /Zc:wchar_t
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CPlayer {
public:
    bool IsMainPlayer() const;                                 // 0x00487520
};

class CThingCreatureBase : public CThing { };
class CThingPlayerCreature : public CThingCreatureBase {
public:
    bool PreOnKill();
};
class CTCInventoryBase {
public:
    CThing* GetResurrectionItemInInventory();                  // 0x005BF77D
};
class CTCResurrectionItem {
public:
    void OnItemUsed(CThingCreatureBase& creature);             // 0x007F04C0
};
class CTCQuestCompletionUI {
public:
    enum CompletionType { COMPLETION_TYPE_3 = 3 };   // nested: Ego_r/FableWin ?Activate@CTCQuestCompletionUI@@QAEXW4CompletionType@1@...
    void Activate(CompletionType type, const CCharString& questName, bool a, const CWideString& text, bool b);   // 0x005E4740
};

extern const wchar_t g_emptyWideString_0122d70c[];             // 0x0122D70C (L"")
extern const char g_emptyString_0122d70e[];                    // 0x0122D70E ("")

bool CThingPlayerCreature::PreOnKill()
{
    if (GetPlayer().IsMainPlayer()) {
        if (HasTC(TCI_INVENTORY)) {
            CThing* item = ((CTCInventoryBase*)(&GetTC(TCI_INVENTORY)))->GetResurrectionItemInInventory();
            if (item && !item->IsDead()) {
                CTCResurrectionItem* resurrection = 0;
                if (item->HasTC(TCI_RESURRECTION_ITEM))
                    resurrection = (CTCResurrectionItem*)((CTCResurrectionItem*)&item->GetTC(TCI_RESURRECTION_ITEM));
                resurrection->OnItemUsed(*this);
                return false;
            }
        }
        CTCQuestCompletionUI* ui = 0;
        if (HasTC(TCI_QUEST_COMPLETION_UI))
            ui = (CTCQuestCompletionUI*)(&GetTC(TCI_QUEST_COMPLETION_UI));
        ui->Activate(CTCQuestCompletionUI::COMPLETION_TYPE_3, g_emptyString_0122d70e, true, g_emptyWideString_0122d70c, true);
        return false;
    }
    return true;
}