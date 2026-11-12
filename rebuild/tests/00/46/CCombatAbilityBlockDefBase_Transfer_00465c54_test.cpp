// Behaviour test for CCombatAbilityBlockDefBase::Transfer (0x00465C54): a model of the save/load round trip of the block data,
// the valid weapon set going through a temporary vector.
#include <cstdio>
#include <set>
#include <vector>
struct Context {
    int Mode; std::vector<float> floats; std::vector<bool> bools; std::vector<std::vector<int> > vecs; int dataTransfers;
    unsigned fi, bi, vi;
    Context(int mode) : Mode(mode), dataTransfers(0), fi(0), bi(0), vi(0) {}
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(float& v) { if (IsSaving()) floats.push_back(v); else v = floats[fi++]; }
    void Transfer(bool& v) { if (IsSaving()) bools.push_back(v); else v = bools[bi++]; }
    void TransferData() { ++dataTransfers; }
    void TransferVector(std::vector<int>& v) { if (IsSaving()) vecs.push_back(v); else v = vecs[vi++]; }
};
struct BlockDef {
    float BlockAngle; bool BlockCanBeBroken; bool ForceRecoilWhenBlocking; std::set<int> ValidBlockWeaponTypes;
    void Transfer(Context& context)
    {
        context.Transfer(BlockAngle);
        context.Transfer(BlockCanBeBroken);
        context.Transfer(ForceRecoilWhenBlocking);
        context.TransferData();
        if (context.IsSaving()) {
            std::vector<int> types;   // the range constructor, spelled as a loop for the test's library
            for (std::set<int>::const_iterator it = ValidBlockWeaponTypes.begin(); it != ValidBlockWeaponTypes.end(); ++it)
                types.push_back(*it);
            context.TransferVector(types);
        } else {
            std::vector<int> types;
            context.TransferVector(types);
            std::set<int> loaded;
            for (unsigned i = 0; i < types.size(); ++i)
                loaded.insert(types[i]);
            ValidBlockWeaponTypes = loaded;
        }
    }
};
int main()
{
    BlockDef a; a.BlockAngle = 45.0f; a.BlockCanBeBroken = true; a.ForceRecoilWhenBlocking = false;
    a.ValidBlockWeaponTypes.insert(3); a.ValidBlockWeaponTypes.insert(1); a.ValidBlockWeaponTypes.insert(2);
    Context save(3); a.Transfer(save);
    if (save.vecs.size() != 1 || save.vecs[0].size() != 3 || save.vecs[0][0] != 1 || save.dataTransfers != 1) { std::printf("00465c54_TEST FAIL\n"); return 1; }
    Context load(2); load.floats = save.floats; load.bools = save.bools; load.vecs = save.vecs;
    BlockDef b; b.BlockAngle = 0; b.BlockCanBeBroken = false; b.ForceRecoilWhenBlocking = true; b.ValidBlockWeaponTypes.insert(9);
    b.Transfer(load);
    if (b.BlockAngle != 45.0f || !b.BlockCanBeBroken || b.ForceRecoilWhenBlocking || b.ValidBlockWeaponTypes != a.ValidBlockWeaponTypes)
        { std::printf("00465c54_TEST FAIL\n"); return 1; }
    std::printf("00465c54_TEST PASS\n");
    return 0;
}