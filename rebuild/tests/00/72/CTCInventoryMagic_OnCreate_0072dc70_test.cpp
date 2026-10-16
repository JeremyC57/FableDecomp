#include <cstdio>
// Behaviour model of CTCInventoryMagic::OnCreate (0x0072DC70): add-ref the owner def around GetSubDef, set the inventory def index, release both.
struct Def { long refs; long InventoryDefIndex; };
struct Owner { long refs; Def sub; void GetSubDef(Def** out) { ++sub.refs; *out = &sub; } };
static long g_index = -1;
static void OnCreate(Owner* owner)
{
    Def* out = 0;
    if (owner) ++owner->refs;
    owner->GetSubDef(&out);
    if (owner && --owner->refs == 0) owner->refs = -99;
    g_index = out->InventoryDefIndex;
    --out->refs;
}
int main()
{
    Owner o = { 1, { 1, 7 } };
    OnCreate(&o);
    if (g_index != 7 || o.refs != 1 || o.sub.refs != 1) { std::printf("0072dc70_TEST FAIL\n"); return 1; }
    std::printf("0072dc70_TEST PASS\n"); return 0;
}