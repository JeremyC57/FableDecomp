#include <cstdio>
// Behaviour of CTCInventoryTrade::DeleteComponents (0x0059C8AE).
static long g_seq[8]; static long g_n = 0;
struct CComponent { virtual ~CComponent() { g_seq[g_n++] = 1; } };
struct CFrontEndManager {
    virtual void Slot29() { g_seq[g_n++] = 3; }
    virtual void Slot37() { g_seq[g_n++] = 2; }
    static CFrontEndManager* GetInstance() { static CFrontEndManager s; return &s; }
};
struct CNodeList { void clear() { g_seq[g_n++] = 4; } };
CNodeList g_frontEndNodeList;
struct CRetailVector { CComponent** _First; CComponent** _Last; CComponent** _End; void pop_back() { --_Last; } };
struct CTCInventoryBase_Retail { CRetailVector Components; };
class CTCInventoryTrade : public CTCInventoryBase_Retail { public:
    CComponent* PCurrentOpenTradeMenu;
    void DeleteComponents()
    {
        if (PCurrentOpenTradeMenu) {
            delete PCurrentOpenTradeMenu;
            Components.pop_back();
            PCurrentOpenTradeMenu = 0;
        }
        CFrontEndManager::GetInstance()->Slot37();
        CFrontEndManager::GetInstance()->Slot29();
        g_frontEndNodeList.clear();
    }
};
int main()
{
    CComponent* storage[4];
    CTCInventoryTrade t; t.PCurrentOpenTradeMenu = new CComponent;
    storage[0] = 0; storage[1] = t.PCurrentOpenTradeMenu;
    t.Components._First = storage; t.Components._Last = storage + 2; t.Components._End = storage + 4;
    t.DeleteComponents();
    int bad = (g_n != 4 || g_seq[0] != 1 || g_seq[1] != 2 || g_seq[2] != 3 || g_seq[3] != 4 || t.PCurrentOpenTradeMenu != 0 || t.Components._Last != storage + 1);
    g_n = 0; t.DeleteComponents();
    bad |= (g_n != 3 || g_seq[0] != 2 || t.Components._Last != storage + 1);
    if (bad) { std::printf("0059c8ae_TEST FAIL\n"); return 1; }
    std::printf("0059c8ae_TEST PASS\n");
    return 0;
}