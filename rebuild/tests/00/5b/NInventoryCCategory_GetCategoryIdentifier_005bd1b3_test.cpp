// behaviour test for 0x005BD1B3
#include <stdio.h>
static int g_deletes = 0;
enum EInventoryCategoryIdentifier { INVENTORY_CATEGORY_NONE };
class CDefPointeeBase { public:
    virtual ~CDefPointeeBase() {}
    virtual void Delete() const { ++g_deletes; }
    unsigned long IVRefCount;                            // +0x04
    __forceinline void Release() { if (--IVRefCount == 0) Delete(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CInventoryCategoryDef : public CDefPointeeBase { public:
    unsigned char                _pad08[0x48];            // +0x08
    EInventoryCategoryIdentifier CategoryIdentifier;      // +0x50
};
namespace NInventory {
class CCategory {
public:
    unsigned char               PAssociatedThing[0x8];    // +0x00 CIntelligentPointer<CThing>
    unsigned char               ItemsInCategory[0xc];     // +0x08 STLport vector<CItem>
    CDefPointer<CInventoryCategoryDef> PDef;              // +0x14
    EInventoryCategoryIdentifier GetCategoryIdentifier() const;
};
EInventoryCategoryIdentifier CCategory::GetCategoryIdentifier() const
{
    CDefPointer<CInventoryCategoryDef> def(PDef);
    return def->CategoryIdentifier;
}
}

int main()
{
    CInventoryCategoryDef def; def.IVRefCount = 1; def.CategoryIdentifier = (EInventoryCategoryIdentifier)5;
    static long buf[8]; NInventory::CCategory& c = *(NInventory::CCategory*)buf; c.PDef.Object = &def;
    int ok = c.GetCategoryIdentifier() == 5 && def.IVRefCount == 1 && g_deletes == 0
          && (char*)&def.CategoryIdentifier - (char*)&def == 0x50 && (char*)&c.PDef - (char*)&c == 0x14;
    c.PDef.Object = 0;
    printf(ok ? "005bd1b3_TEST PASS\n" : "005bd1b3_TEST FAIL\n");
    return ok ? 0 : 1;
}