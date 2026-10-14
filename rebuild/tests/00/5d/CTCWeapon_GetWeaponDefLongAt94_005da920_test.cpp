#include <cstdio>
struct CWeaponDef_Retail { long m_refCount; long m_value94; int m_destroyed; void AddRef() { ++m_refCount; } void Destroy() { ++m_destroyed; } void Release() { if (--m_refCount == 0) Destroy(); } };
struct CDefRef { CWeaponDef_Retail* m_p; CDefRef(CWeaponDef_Retail* p) : m_p(p) { if (m_p) m_p->AddRef(); } ~CDefRef() { m_p->Release(); } CWeaponDef_Retail* operator->() const { return m_p; } };
struct CDefPointer { CWeaponDef_Retail* m_pObject; CDefRef operator->() const { return CDefRef(m_pObject); } };
struct CTCWeapon_Retail { unsigned char _pad[0xC]; CDefPointer WeaponDef; long GetWeaponDefLongAt94() { return WeaponDef->m_value94; } };

int main()
{
    CWeaponDef_Retail def = { 2, 31, 0 }; CTCWeapon_Retail w; w.WeaponDef.m_pObject = &def;
    if (w.GetWeaponDefLongAt94() != 31 || def.m_refCount != 2 || def.m_destroyed != 0) { std::printf("005da920_TEST FAIL\n"); return 1; }
    def.m_refCount = 0; if (w.GetWeaponDefLongAt94() != 31 || def.m_destroyed != 1) { std::printf("005da920_TEST FAIL\n"); return 1; }
    std::printf("005da920_TEST PASS\n");
    return 0;
}