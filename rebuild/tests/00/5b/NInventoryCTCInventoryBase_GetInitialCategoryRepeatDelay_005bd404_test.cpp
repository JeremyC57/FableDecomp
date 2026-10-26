// test for 005bd404 CTCInventoryBase::GetInitialCategoryRepeatDelay
#include <stdio.h>
#include <string.h>
class CDefPointeeBase { public:
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                         // slot 1 (+4)
    mutable unsigned long IVRefCount;                    // +0x04
    __forceinline void Release() const { if (--IVRefCount == 0) Delete(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CInventoryDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x34];                          // +0x08
    float         InitialCategoryMovementRepeatDelay;    // +0x3c
    float         ContinuedCategoryMovementRepeatDelay;  // +0x40
    float         InitialItemMovementRepeatDelay;        // +0x44
    float         ContinuedItemMovementRepeatDelay;      // +0x48
};
namespace NInventory {
class CTCInventoryBase {
public:
    virtual float GetInitialCategoryRepeatDelay() const;
    unsigned char                     _pad04[0x64];      // +0x04
    CDefPointer<const CInventoryDef>  PDef;              // +0x68
};
float CTCInventoryBase::GetInitialCategoryRepeatDelay() const
{
    CDefPointer<const CInventoryDef> def(PDef);
    return def->InitialCategoryMovementRepeatDelay;
}
}

static int g_deleted;
CDefPointeeBase::~CDefPointeeBase() {}
void CDefPointeeBase::Delete() const { ++g_deleted; }
struct TestDef : CInventoryDef { };
int main()
{
    TestDef def; def.IVRefCount = 2; def.InitialCategoryMovementRepeatDelay = 0.25f; def.ContinuedCategoryMovementRepeatDelay = 9.0f;
    static unsigned char buf[sizeof(NInventory::CTCInventoryBase)];
    NInventory::CTCInventoryBase* tc = (NInventory::CTCInventoryBase*)buf;
    tc->PDef.Object = &def;
    int ok = tc->NInventory::CTCInventoryBase::GetInitialCategoryRepeatDelay() == 0.25f && def.IVRefCount == 2 && g_deleted == 0;
    def.IVRefCount = 0;
    ok &= tc->NInventory::CTCInventoryBase::GetInitialCategoryRepeatDelay() == 0.25f && g_deleted == 1 && def.IVRefCount == 0;
    ok &= (char*)&tc->PDef - (char*)tc == 0x68 && (char*)&def.InitialCategoryMovementRepeatDelay - (char*)(CDefPointeeBase*)&def == 0x3c;
    printf(ok ? "005bd404_TEST PASS\n" : "005bd404_TEST FAIL\n");
    return ok ? 0 : 1;
}