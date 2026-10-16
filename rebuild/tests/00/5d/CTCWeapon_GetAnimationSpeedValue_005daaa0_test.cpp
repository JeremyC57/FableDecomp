#include <cstdio>
struct CAug { float m; float Get() const { return m; } };
struct CThing { unsigned int bits[9]; CAug* aug; bool GetInterface(CAug** o, long t) { if (bits[t >> 5] & (1u << (t & 31))) { *o = aug; return true; } return false; } };
struct CWeaponDef { long RefCount; float AnimationSpeed; };
struct CTCWeapon {
    CThing* ParentThing; CWeaponDef* def;
    float Get() const { ++def->RefCount; float speed = def->AnimationSpeed; --def->RefCount; CAug* a; if (ParentThing->GetInterface(&a, 0xbf)) return speed * a->Get(); return speed; }
};
int main()
{
    CAug aug = { 2.0f }; CThing t = { {0}, &aug }; CWeaponDef d = { 1, 1.5f }; CTCWeapon w = { &t, &d };
    if (w.Get() != 1.5f) { std::printf("005daaa0_TEST FAIL\n"); return 1; }
    t.bits[5] = 0x80000000u;
    if (w.Get() != 3.0f || d.RefCount != 1) { std::printf("005daaa0_TEST FAIL\n"); return 1; }
    std::printf("005daaa0_TEST PASS\n");
    return 0;
}