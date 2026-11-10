// Behaviour test for CIntelligentPointer_00c1fe00::Validate @ 0x00C1FE90: the item asks itself for its table (slot 5), then validates that table.
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
struct CPointerBase_00c1fe00 {
    virtual void* v00() { return 0; }
    virtual void* v01() { return 0; }
    virtual const void* GetKey() { return 0; }
    virtual void Validate() { }
    virtual unsigned long GetKeyType() { return 2; }
    virtual CItemTable* GetTable() { g_getTable = ++g_order; return &g_table; }
    virtual ~CPointerBase_00c1fe00() { }
};
struct CIntelligentPointer_00c1fe00 : public CPointerBase_00c1fe00 {
    virtual void Validate();
};
void CIntelligentPointer_00c1fe00::Validate()
{
    GetTable()->Validate();
}
int main()
{
    CIntelligentPointer_00c1fe00 item;
    CPointerBase_00c1fe00* p = &item;
    p->Validate();
    if (g_getTable != 1 || g_validate != 2) { std::printf("00c1fe90_TEST FAIL\n"); return 1; }
    std::printf("00c1fe90_TEST PASS\n");
    return 0;
}