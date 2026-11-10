// Behaviour test for 0x00c1f7b0 CKeyedItem::Validate: validates the table member once per call
// Self-contained: recording fakes stand in for the item table and the dispatcher's lists.
#include <stdio.h>

struct CItem;
class CItemTable {
public:
    int validated;
    CItemTable() : validated(0) { }
    virtual ~CItemTable() { }
    virtual void Validate() { ++validated; }
};

class CItemList {                                // recording fake of the PKCTRList<CItem> slots 8 / 9
public:
    int last; CItem* item;
    CItemList() : last(-1), item(0) { }
    virtual ~CItemList() { }
    virtual void Remove(CItem* p) { last = 8; item = p; }
    virtual void Add(CItem* p) { last = 9; item = p; }
};

struct CItemDispatcher : public CItemList {      // the id-keyed list (+0) and the name-hash index
    CItemList m_names;
};

struct CItem {
    CItemTable* m_pTable;
    explicit CItem(CItemTable* table) : m_pTable(table) { }
    virtual ~CItem() { }
    virtual CItemTable* GetTable() { return m_pTable; }
    virtual void Validate();
};

void CItem::Validate()
{
    GetTable()->Validate();
}

typedef CItem Item;

int main()
{
    int ok = 1;
    CItemTable table; Item item(&table);
    item.Validate(); item.Validate();
    if (table.validated != 2) ok = 0;
    if (!ok) { printf("00c1f7b0_TEST FAIL\n"); return 1; }
    printf("00c1f7b0_TEST PASS\n");
    return 0;
}