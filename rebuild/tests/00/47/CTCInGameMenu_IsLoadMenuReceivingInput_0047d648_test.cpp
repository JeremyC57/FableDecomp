// Behaviour test for CTCInGameMenu::IsLoadMenuReceivingInput (0x0047D648): walks every component's
// "UI_MENU_ENTRY_LOAD_CONFIRM" children; true when one is in state 5 or 3; the found list is always destroyed.
#include <cstdio>
#include <cstring>

class CCharString { public: const char* Text; CCharString(const char* text, long length = -1) : Text(text) { (void)length; } };
namespace NUISystem { class CComponent; }
struct SComponentListNode { SComponentListNode* m_pNext; SComponentListNode* m_pPrev; NUISystem::CComponent* m_data; };
static int g_lists = 0;
struct CComponentList {
    SComponentListNode* m_pNode; SComponentListNode head; SComponentListNode nodes[4]; int n;
    CComponentList() : n(0) { m_pNode = &head; head.m_pNext = head.m_pPrev = &head; ++g_lists; }
    ~CComponentList() { --g_lists; }
    void Add(NUISystem::CComponent* c) { SComponentListNode* x = &nodes[n++]; x->m_data = c; x->m_pNext = &head; x->m_pPrev = head.m_pPrev; head.m_pPrev->m_pNext = x; head.m_pPrev = x; }
};
namespace NUISystem {
class CComponent {
public:
    const char* Name; long State; CComponent* Kids[3]; int nKids;
    void FindChildrenByName(CCharString name, CComponentList& found)
    { for (int i = 0; i < nKids; ++i) if (std::strcmp(Kids[i]->Name, name.Text) == 0) found.Add(Kids[i]); }
    long GetState() { return State; }
};
}
struct CComponentVector {
    NUISystem::CComponent** m_pFirst; NUISystem::CComponent** m_pLast; NUISystem::CComponent** m_pEnd;
    NUISystem::CComponent** begin() const { return m_pFirst; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    NUISystem::CComponent* operator[](unsigned long n) const { return *(begin() + n); }
};
class CTCInGameMenu {
public:
    CComponentVector Components;
    bool IsLoadMenuReceivingInput() const;
};
bool CTCInGameMenu::IsLoadMenuReceivingInput() const
{
    for (unsigned long i = 0; i < Components.size(); ++i) {
        CComponentList found;
        Components[i]->FindChildrenByName(CCharString("UI_MENU_ENTRY_LOAD_CONFIRM", -1), found);
        for (SComponentListNode* node = found.m_pNode->m_pNext; node != found.m_pNode; node = node->m_pNext) {
            NUISystem::CComponent* child = node->m_data;
            if (child && (child->GetState() == 5 || child->GetState() == 3))
                return true;
        }
    }
    return false;
}
int main()
{
    using NUISystem::CComponent;
    CComponent a = { "UI_MENU_ENTRY_LOAD_CONFIRM", 1, {0, 0, 0}, 0 };
    CComponent b = { "UI_MENU_ENTRY_LOAD_CONFIRM", 2, {0, 0, 0}, 0 };
    CComponent other = { "OTHER", 3, {0, 0, 0}, 0 };
    CComponent screen = { "S", 0, {&other, &a, &b}, 3 };
    CComponent* list[1] = { &screen };
    CTCInGameMenu menu; menu.Components.m_pFirst = list; menu.Components.m_pLast = menu.Components.m_pEnd = list + 1;
    if (menu.IsLoadMenuReceivingInput() || g_lists != 0) { std::printf("0047d648_TEST FAIL\n"); return 1; }
    b.State = 3;
    if (!menu.IsLoadMenuReceivingInput() || g_lists != 0) { std::printf("0047d648_TEST FAIL\n"); return 1; }
    b.State = 0; a.State = 5;
    if (!menu.IsLoadMenuReceivingInput() || g_lists != 0) { std::printf("0047d648_TEST FAIL\n"); return 1; }
    std::printf("0047d648_TEST PASS\n");
    return 0;
}