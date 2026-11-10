// Behaviour test for 0x00c1f940 CVtable_012b34e0::Attach: adds the item to the id-keyed list only
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
    virtual void Attach(CItemDispatcher* dispatcher);
};

void CItem::Attach(CItemDispatcher* dispatcher)
{
    dispatcher->Add(this);
}

typedef CItem Item;

int main()
{
    int ok = 1;
    CItemTable table; Item item(&table); CItemDispatcher d;
    item.Attach(&d);
    if (d.last != 9 || d.item != &item || d.m_names.last != -1) ok = 0;
    if (!ok) { printf("00c1f940_TEST FAIL\n"); return 1; }
    printf("00c1f940_TEST PASS\n");
    return 0;
}