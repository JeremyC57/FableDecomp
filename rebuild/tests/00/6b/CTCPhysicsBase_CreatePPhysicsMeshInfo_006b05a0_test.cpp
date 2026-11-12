// Behaviour test for 0x006B05A0 CTCPhysicsBase::CreatePPhysicsMeshInfo: the same decision logic over a small model (a mesh bank
// with static and animated mesh infos, an optional physics mesh per info, an optional physics def) with recording setters.
#include <cstdio>

struct PhysicsMeshInfo { unsigned long Index; };
struct MeshInfo {
    const PhysicsMeshInfo* physics;
    bool HasPhysicsMesh() const { return physics != 0; }
    const PhysicsMeshInfo* PeekPhysicsMesh() const { return physics; }
};
struct MeshBank {
    const MeshInfo* statics[4];
    const MeshInfo* animated[4];
    mutable int staticLookups, animatedLookups;
    const MeshInfo* PeekStaticMesh(unsigned long i) const { ++staticLookups; return statics[i]; }
    const MeshInfo* PeekAnimatedMesh(unsigned long i) const { ++animatedLookups; return animated[i]; }
};
struct Graphic { unsigned long BankIndex; unsigned char Type; };
struct PhysicsDef { long InteractionFlags; };

struct Physics {
    const MeshBank* bank;
    const PhysicsDef* def;
    long meshIndexSet; int meshIndexCalls;
    long flagsSet;     int flagsCalls;
    void SetPhysicsMeshBankIndex(long index) { meshIndexSet = index; ++meshIndexCalls; }
    void SetInteractionFlags(long flags) { flagsSet = flags; ++flagsCalls; }
    void CreatePPhysicsMeshInfo(const Graphic& graphic)
    {
        if (graphic.Type == 5) {
            const MeshInfo* info = bank->PeekStaticMesh(graphic.BankIndex);
            if (info->HasPhysicsMesh()) {
                SetPhysicsMeshBankIndex(info->PeekPhysicsMesh()->Index);
                return;
            }
            if (def != 0)
                SetInteractionFlags(def->InteractionFlags);
        } else if (graphic.Type == 4) {
            const MeshInfo* info = bank->PeekAnimatedMesh(graphic.BankIndex);
            if (info->HasPhysicsMesh()) {
                SetPhysicsMeshBankIndex(info->PeekPhysicsMesh()->Index);
                return;
            }
            if (def != 0)
                SetInteractionFlags(def->InteractionFlags);
        }
    }
};

int main()
{
    PhysicsMeshInfo crate = { 17 }, door = { 42 };
    MeshInfo staticWith = { &crate }, staticWithout = { 0 }, animWith = { &door }, animWithout = { 0 };
    MeshBank bank = { { &staticWith, &staticWithout, 0, 0 }, { &animWithout, &animWith, 0, 0 }, 0, 0 };
    PhysicsDef def = { 0x1234 };
    Physics p = { &bank, &def, -1, 0, -1, 0 };
    bool ok = true;
    Graphic g = { 0, 5 };
    p.CreatePPhysicsMeshInfo(g);                       // static mesh with a physics mesh: its bank index
    ok = ok && p.meshIndexSet == 17 && p.meshIndexCalls == 1 && p.flagsCalls == 0;
    g.BankIndex = 1;
    p.CreatePPhysicsMeshInfo(g);                       // static mesh without one: the def's interaction flags
    ok = ok && p.meshIndexCalls == 1 && p.flagsSet == 0x1234 && p.flagsCalls == 1;
    ok = ok && bank.staticLookups == 2 && bank.animatedLookups == 0;
    g.Type = 4;
    p.CreatePPhysicsMeshInfo(g);                       // animated mesh with a physics mesh
    ok = ok && p.meshIndexSet == 42 && p.meshIndexCalls == 2 && p.flagsCalls == 1;
    g.BankIndex = 0; p.def = 0;
    p.CreatePPhysicsMeshInfo(g);                       // animated mesh without one and no def: nothing
    ok = ok && p.meshIndexCalls == 2 && p.flagsCalls == 1;
    ok = ok && bank.staticLookups == 2 && bank.animatedLookups == 2;
    g.Type = 3; p.def = &def;
    p.CreatePPhysicsMeshInfo(g);                       // any other graphic type: no lookup, no setter
    g.Type = 6;
    p.CreatePPhysicsMeshInfo(g);
    ok = ok && p.meshIndexCalls == 2 && p.flagsCalls == 1 && bank.staticLookups == 2 && bank.animatedLookups == 2;
    if (!ok) { std::printf("006b05a0_TEST FAIL\n"); return 1; }
    std::printf("006b05a0_TEST PASS\n");
    return 0;
}