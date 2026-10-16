#include <cstdio>
#include <cstring>
// Model: weapon def with ref count and created-shot index; default lookup by name.
static int g_destroyed = 0;
struct CWeaponDef { long RefCount; long CreatedShotDefIndex; void Destroy() { ++g_destroyed; } void Release() { if (--RefCount == 0) Destroy(); } };
struct CDefPointer { CWeaponDef* Object; CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; } CDefPointer(CWeaponDef* p) : Object(p) {} ~CDefPointer() {} };
static long Lookup(const char* n) { return std::strcmp(n, "SHOT_CROSSBOW_BOLT") == 0 ? 77 : -1; }
struct CTCWeapon {
    CDefPointer WeaponDef;
    CTCWeapon(CWeaponDef* d) : WeaponDef(d) {}
    long Get() const {
        { CDefPointer p(WeaponDef); long v = p.Object->CreatedShotDefIndex; p.Object->Release(); if (v) { CDefPointer q(WeaponDef); long r = q.Object->CreatedShotDefIndex; q.Object->Release(); return r; } }
        return Lookup("SHOT_CROSSBOW_BOLT");
    }
};
int main()
{
    CWeaponDef a = { 1, 12 }; CWeaponDef b = { 1, 0 };
    CTCWeapon wa(&a), wb(&b);
    if (wa.Get() != 12 || wb.Get() != 77 || a.RefCount != 1 || b.RefCount != 1 || g_destroyed != 0) { std::printf("005da750_TEST FAIL\n"); return 1; }
    std::printf("005da750_TEST PASS\n");
    return 0;
}