// behaviour test for 0x005D9030
#include <stdio.h>
#include <string.h>
static int g_deletes = 0;
enum ETCInterfaceType { TC_INTERFACE_NONE };
struct CDefPointeeBase {
    virtual ~CDefPointeeBase() {}
    virtual void Delete() const { ++g_deletes; }
    unsigned long IVRefCount;                            // +0x04
    __forceinline void Release() { if (--IVRefCount == 0) Delete(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CInventoryItemDef : CDefPointeeBase {
    unsigned char    _pad08[0x48];                       // +0x08
    ETCInterfaceType InventoryType;                      // +0x50
};
class CTCInventoryItem {
public:
    unsigned char                  _pad00[0xc];          // +0x00 CTCBase
    CDefPointer<CInventoryItemDef> PDef;                 // +0x0c
    ETCInterfaceType GetInventoryType() const;
};
ETCInterfaceType CTCInventoryItem::GetInventoryType() const
{
    CDefPointer<CInventoryItemDef> def(PDef);
    return def->InventoryType;
}

int main()
{
    CInventoryItemDef def; def.IVRefCount = 1; def.InventoryType = (ETCInterfaceType)0x48;
    static long buf[8]; CTCInventoryItem& tc = *(CTCInventoryItem*)buf; tc.PDef.Object = &def;
    int ok = tc.GetInventoryType() == 0x48 && def.IVRefCount == 1 && g_deletes == 0;
    def.IVRefCount = 0; tc.PDef.Object = 0;
    printf(ok ? "005d9030_TEST PASS\n" : "005d9030_TEST FAIL\n");
    return ok ? 0 : 1;
}