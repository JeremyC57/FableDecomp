// behaviour test for 0x0044C1DC CCarrySlotDef::CCarrySlotDef() (model: the CDefString members start unset)
#include <cstdio>
#include <cstring>
#include <new>
class CDefString { public: int TablePos; CDefString() : TablePos(-1) { } };
struct Def { CDefString DummyPosName; int DummyPosIndex; bool PrimarySlot; CDefString SecondaryDummyPosName; };
int main()
{
    static double raw[32]; std::memset(raw, 0x11, sizeof(raw));
    Def* d = new (raw) Def;
    int ok = 1;
    if (d->DummyPosName.TablePos != -1) ok = 0;
    if (d->SecondaryDummyPosName.TablePos != -1) ok = 0;
    if (!ok) { std::printf("0044c1dc_TEST FAIL\n"); return 1; }
    std::printf("0044c1dc_TEST PASS\n");
    return 0;
}