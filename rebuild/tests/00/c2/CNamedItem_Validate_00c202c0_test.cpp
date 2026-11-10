// Behaviour test for CNamedItem::Validate @ 0x00C202C0: the item asks itself for its table (slot 5), then validates that table.
#include <cstdio>
static int g_order = 0, g_getTable = 0, g_validate = 0;
class CItemTable {
public:
    virtual unsigned long GetType() { return 0; }
    virtual long v01() { return -1; }
    virtual long v02() { return -1; }
    virtual void v03() { } virtual void v04() { } virtual void v05() { } virtual void v06() { } virtual void v07() { } virtual void v08() { }
    virtual void Validate() { g_validate = ++g_order; }
};
static CItemTable g_table;
struct CHashedItem {
    virtual void* v00() { return 0; }
    virtual void* v01() { return 0; }
    virtual const void* GetKey() { return 0; }
    virtual void Validate() { }
    virtual unsigned long GetKeyType() { return 2; }
    virtual CItemTable* GetTable() { g_getTable = ++g_order; return &g_table; }
    virtual ~CHashedItem() { }
};
struct CNamedItem : public CHashedItem {
    virtual void Validate();
};
void CNamedItem::Validate()
{
    GetTable()->Validate();
}
int main()
{
    CNamedItem item;
    CHashedItem* p = &item;
    p->Validate();
    if (g_getTable != 1 || g_validate != 2) { std::printf("00c202c0_TEST FAIL\n"); return 1; }
    std::printf("00c202c0_TEST PASS\n");
    return 0;
}