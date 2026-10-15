#include <cstdio>
#include <cstring>
// Behaviour test for CTCInventory::IsQuitMenuReceivingInput (retail 0x005826FA).
static int g_lookups = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* text, long length = -1) : m_pData(text) { (void)length; }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { }
    ~CCharString() { }
};
struct CInputReceiver {
    bool m_receiving;
    long m_lastLayer;
    virtual bool IsReceivingInput(long inputLayer) { m_lastLayer = inputLayer; return m_receiving; }
    virtual ~CInputReceiver() { }
};
struct CMenuEntryOwner { virtual ~CMenuEntryOwner() { } };
struct CMenuEntry : CMenuEntryOwner, CInputReceiver { };
struct CUIComponent {
    CMenuEntry* m_quit;
    virtual CMenuEntry* FindEntry(CCharString name)
    {
        ++g_lookups;
        return std::strcmp(name.m_pData, "PC_MENU_ENTRY_QUIT") == 0 ? m_quit : 0;
    }
    virtual ~CUIComponent() { }
};
struct CComponentVector {
    CUIComponent** first; CUIComponent** last; CUIComponent** end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CUIComponent*& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventory {
    CComponentVector Components;
    bool IsQuitMenuReceivingInput();
};
bool CTCInventory::IsQuitMenuReceivingInput()
{
    for (unsigned long i = 0; i < Components.size(); ++i) {
        CMenuEntry* entry = Components[i]->FindEntry(CCharString("PC_MENU_ENTRY_QUIT"));
        if (entry && entry->IsReceivingInput(2))
            return true;
    }
    return false;
}
static int Fail() { std::printf("005826fa_TEST FAIL\n"); return 1; }
int main()
{
    CMenuEntry entry; entry.m_receiving = false; entry.m_lastLayer = 0;
    CUIComponent a; a.m_quit = 0;
    CUIComponent b; b.m_quit = &entry;
    CUIComponent* comps[2] = { &a, &b };
    CTCInventory inv;
    inv.Components.first = comps; inv.Components.last = comps + 2; inv.Components.end_of_storage = comps + 2;
    if (inv.IsQuitMenuReceivingInput() || g_lookups != 2 || entry.m_lastLayer != 2) return Fail();
    entry.m_receiving = true;
    if (!inv.IsQuitMenuReceivingInput() || g_lookups != 4) return Fail();
    inv.Components.last = comps + 1;
    if (inv.IsQuitMenuReceivingInput()) return Fail();
    std::printf("005826fa_TEST PASS\n");
    return 0;
}