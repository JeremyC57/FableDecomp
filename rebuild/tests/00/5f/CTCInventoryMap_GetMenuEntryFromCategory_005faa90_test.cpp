// Behaviour test for CTCInventoryMap::GetMenuEntryFromCategory @ 0x005faa90: each logbook category asks the parent for its UI menu entry child.
#include <stdio.h>
#include <string.h>
class CCharString { public:
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
    CCharString(const CCharString& other) : Text(other.Text) {}
    ~CCharString() {}
};
class CHeroLogBook { public:
    class CHeroLogBookEntry { public:
        enum ECategory { };
    };
};
static char g_lastName[64];
static int g_calls = 0;
class CComponent { public:
    CComponent* Result;
    virtual ~CComponent() {}
    virtual CComponent* FindChild(CCharString name) { g_calls++; strcpy(g_lastName, name.Text); return Result; }
};
class CTCInventoryMap { public:
    CComponent* GetMenuEntryFromCategory(CHeroLogBook::CHeroLogBookEntry::ECategory category, CComponent* parent)
    {
        switch (category) {
        case 4: return parent->FindChild(CCharString("UI_MENU_ENTRY_LOGBOOK_OBJECTS"));
        case 5: return parent->FindChild(CCharString("UI_MENU_ENTRY_LOGBOOK_TOWNS"));
        case 6: return parent->FindChild(CCharString("UI_MENU_ENTRY_LOGBOOK_HERO"));
        case 7: return parent->FindChild(CCharString("UI_MENU_ENTRY_LOGBOOK_COMBAT"));
        case 0: return parent->FindChild(CCharString("UI_MENU_ENTRY_LOGBOOK_QUESTS"));
        case 1: return parent->FindChild(CCharString("UI_MENU_ENTRY_LOGBOOK_STORY"));
        default: return parent->FindChild(CCharString("UI_MENU_ENTRY_LOGBOOK_BASICS"));
        }
    }
};
int main()
{
    static const char* expect[10] = { "QUESTS", "STORY", "BASICS", "BASICS", "OBJECTS", "TOWNS", "HERO", "COMBAT", "BASICS", "BASICS" };
    CTCInventoryMap map; CComponent parent, child; parent.Result = &child;
    for (int i = 0; i < 10; ++i) {
        int category = i < 8 ? i : (i == 8 ? 8 : -1);
        char want[64]; sprintf(want, "UI_MENU_ENTRY_LOGBOOK_%s", expect[i]);
        g_lastName[0] = 0;
        CComponent* got = map.GetMenuEntryFromCategory((CHeroLogBook::CHeroLogBookEntry::ECategory)category, &parent);
        if (got != &child || strcmp(g_lastName, want) != 0 || g_calls != i + 1) { printf("005faa90_TEST FAIL\n"); return 1; }
    }
    parent.Result = 0;
    if (map.GetMenuEntryFromCategory((CHeroLogBook::CHeroLogBookEntry::ECategory)4, &parent) != 0) { printf("005faa90_TEST FAIL\n"); return 1; }
    printf("005faa90_TEST PASS\n");
    return 0;
}