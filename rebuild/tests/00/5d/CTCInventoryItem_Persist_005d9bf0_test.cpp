// Behaviour test for 0x005D9BF0 CTCInventoryItem::Persist
#include <cstdio>
class CPersistContext { public:
    long Mode; unsigned long long stored; bool loaded;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char*, unsigned long long& v, const unsigned long long&) { if (IsSaving()) stored = v; else v = stored; }
};
class CThing { public: unsigned long long UID; };
struct CInvBase { CThing* ParentThing; };
struct CItem {
    CInvBase* PInventory; unsigned long long InventoryUIDToAttachTo;
    void Persist(CPersistContext* context)
    {
        if (context->IsSaving()) {
            unsigned long long uid = 0;
            if (PInventory) uid = PInventory->ParentThing->UID;
            context->Transfer("InventoryUID", uid, 0);
        } else
            context->Transfer("InventoryUID", InventoryUIDToAttachTo, 0);
    }
};
int main()
{
    CThing t = { 0x1234567890ULL }; CInvBase b = { &t };
    CItem item = { &b, 0 }; CPersistContext c = { 1, 0, false };
    item.Persist(&c);
    bool ok = c.stored == 0x1234567890ULL;
    CItem loose = { 0, 7 }; c.Mode = 3; loose.Persist(&c); if (c.stored != 0) ok = false;
    c.Mode = 2; c.stored = 99; loose.Persist(&c); if (loose.InventoryUIDToAttachTo != 99) ok = false;
    if (!ok) { std::printf("005d9bf0_TEST FAIL\n"); return 1; }
    std::printf("005d9bf0_TEST PASS\n");
    return 0;
}