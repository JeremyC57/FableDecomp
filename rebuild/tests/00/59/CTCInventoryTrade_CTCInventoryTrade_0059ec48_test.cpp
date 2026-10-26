// Behaviour test for CTCInventoryTrade::~CTCInventoryTrade (0x0059EC48): every owned component deleted, the list emptied,
// members destroyed in reverse order, then the bases.
#include <cstdio>

static int g_deleted = 0;
static char g_order[16];
static int g_n = 0;
struct CComponent { virtual ~CComponent() { ++g_deleted; } };
struct CComponentVector {
    CComponent* m_items[4];
    unsigned long m_count;
    unsigned long size() const { return m_count; }
    CComponent*& operator[](unsigned long i) { return m_items[i]; }
    void clear() { m_count = 0; }
};
struct CMember { char Tag; CMember(char t) : Tag(t) { } ~CMember() { g_order[g_n++] = Tag; } };
struct CTCInventoryBase {
    CComponentVector Components;
    virtual ~CTCInventoryBase() { g_order[g_n++] = 'B'; }
};
struct CBase { virtual ~CBase() { g_order[g_n++] = 'C'; } };
class CTCInventoryTrade : public CTCInventoryBase, public CBase { public:
    CMember First;
    CMember Last;
    CTCInventoryTrade() : First('1'), Last('2') { }
    virtual ~CTCInventoryTrade()
    {
        for (unsigned long i = 0; i < Components.size(); ++i)
            delete Components[i];
        Components.clear();
    }
};

int main()
{
    CTCInventoryTrade* t = new CTCInventoryTrade;
    t->Components.m_items[0] = new CComponent;
    t->Components.m_items[1] = 0;                 // null entries are skipped by delete
    t->Components.m_items[2] = new CComponent;
    t->Components.m_count = 3;
    CTCInventoryBase* b = t;
    delete b;
    g_order[g_n] = 0;
    bool ok = g_deleted == 2 && g_order[0] == '2' && g_order[1] == '1' && g_order[2] == 'C' && g_order[3] == 'B' && g_n == 4;
    if (!ok) { std::printf("0059ec48_TEST FAIL\n"); return 1; }
    std::printf("0059ec48_TEST PASS\n");
    return 0;
}