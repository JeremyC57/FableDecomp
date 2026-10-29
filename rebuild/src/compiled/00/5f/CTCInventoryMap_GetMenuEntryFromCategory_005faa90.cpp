// CTCInventoryMap::GetMenuEntryFromCategory(CHeroLogBook::CHeroLogBookEntry::ECategory category, NUISystem::CComponent* parent) -- find the logbook menu entry child (UI_MENU_ENTRY_LOGBOOK_<category>) of the given list component
// Leaf: ?GetMenuEntryFromCategory@CTCInventoryMap@@IAEPAVCComponent@NUISystem@@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@PAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FAA90, 272 bytes (jump table included), `ret 8`; `this` is never read.
// Identity: Ego_r.pdb has `?GetMenuEntryFromCategory@CTCInventoryMap@@IAE...` (protected) and, next in donor order,
//   `?GetMenuEntryFromCategoryPC@...` with the same signature. This body uses the UI_MENU_ENTRY_LOGBOOK_* strings, its twin
//   0x005FAC20 the PC_MENU_ENTRY_LOGBOOK_* strings, so this is the console/UI form and 0x005FAC20 the PC form. Its callers
//   0x006094EC / 0x00609543 sit in 0x00607330 CTCInventoryMap::ConstructLogbookList. The catalog gives both rows this label.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (`??0CCharString@@QAE@PBDJ@Z`, length -1), built straight in
//   the argument slot of the by-value parameter; NUISystem::CComponent::FindChild(CCharString) is virtual slot 3 (+0x0C)
//   (Ego_r `?FindChild@CComponent@NUISystem@@UAEPAV12@VCCharString@@@Z`; the landed CFrontEndScreen::Initialise 0x0054E4B0
//   calls it the same way). The callee destroys the by-value string.
// Shape: switch over the category with one `return parent->FindChild(...)` per case; the jump table spans 0..7, cases 2 and 3
//   share the BASICS body with the out-of-range default. Case bodies are laid out in source order 4,5,6,7,0,1,default.
//   ECategory enumerator names are not in any PDB name set, so the cases use the enum's integer values.
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};

class CHeroLogBook { public:
    class CHeroLogBookEntry { public:
        enum ECategory { };
    };
};

namespace NUISystem {
class CUIInterface { public:
    virtual ~CUIInterface();
};
class CComponent : public CUIInterface { public:
    virtual ~CComponent();
    virtual void Update(float time);                   // slot 1
    virtual void CUnprovenSlot2_0124608c();            // slot 2 (Draw)
    virtual CComponent* FindChild(CCharString name);   // slot 3 (+0x0C)
};
}

class CTCInventoryMap { public:
protected:
    NUISystem::CComponent* GetMenuEntryFromCategory(CHeroLogBook::CHeroLogBookEntry::ECategory category, NUISystem::CComponent* parent);
};

NUISystem::CComponent* CTCInventoryMap::GetMenuEntryFromCategory(CHeroLogBook::CHeroLogBookEntry::ECategory category, NUISystem::CComponent* parent)
{
    switch (category) {
    case 4:
        return parent->FindChild(CCharString("UI_MENU_ENTRY_LOGBOOK_OBJECTS"));
    case 5:
        return parent->FindChild(CCharString("UI_MENU_ENTRY_LOGBOOK_TOWNS"));
    case 6:
        return parent->FindChild(CCharString("UI_MENU_ENTRY_LOGBOOK_HERO"));
    case 7:
        return parent->FindChild(CCharString("UI_MENU_ENTRY_LOGBOOK_COMBAT"));
    case 0:
        return parent->FindChild(CCharString("UI_MENU_ENTRY_LOGBOOK_QUESTS"));
    case 1:
        return parent->FindChild(CCharString("UI_MENU_ENTRY_LOGBOOK_STORY"));
    default:
        return parent->FindChild(CCharString("UI_MENU_ENTRY_LOGBOOK_BASICS"));
    }
}