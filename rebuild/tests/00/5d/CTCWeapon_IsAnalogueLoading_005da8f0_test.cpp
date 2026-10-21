// test for 005da8f0 CTCWeapon::IsAnalogueLoading
#include <stdio.h>
struct CDefPointeeBase {
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                         // slot 1 (+4)
    unsigned long IVRefCount;                            // +0x04
    __forceinline void Release() { if (--IVRefCount == 0) Delete(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CWeaponDef : CDefPointeeBase {
    unsigned char _pad08[0x1c];                          // +0x08
    unsigned char _pad24[0x6c];                          // +0x24
    bool          UseAnalogueLoading;                    // +0x90
};
class CTCWeapon {
public:
    unsigned char           _pad00[0xc];                 // +0x00 CTCBase
    CDefPointer<CWeaponDef> WeaponDef;                   // +0x0c
    bool IsAnalogueLoading() const;
};
bool CTCWeapon::IsAnalogueLoading() const
{
    CDefPointer<CWeaponDef> def(WeaponDef);
    return def->UseAnalogueLoading;
}

static int g_deletes;
CDefPointeeBase::~CDefPointeeBase() {}
void CDefPointeeBase::Delete() const { ++g_deletes; }
int main()
{
    CWeaponDef def; def.IVRefCount = 1; def.UseAnalogueLoading = true;
    static long buf[8]; CTCWeapon& tc = *(CTCWeapon*)buf; tc.WeaponDef.Object = &def;
    int ok = tc.IsAnalogueLoading() == true && def.IVRefCount == 1 && g_deletes == 0;
    def.UseAnalogueLoading = false;
    ok &= tc.IsAnalogueLoading() == false;
    def.IVRefCount = 0;
    ok &= tc.IsAnalogueLoading() == false && g_deletes == 1;
    ok &= (char*)&def.UseAnalogueLoading - (char*)(CDefPointeeBase*)&def == 0x90;
    tc.WeaponDef.Object = 0;
    printf(ok ? "005da8f0_TEST PASS\n" : "005da8f0_TEST FAIL\n");
    return ok ? 0 : 1;
}