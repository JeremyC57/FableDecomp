#include <cstdio>
#include <cstring>
// Behaviour copy of C3DMeshMaterial::InitialiseFromFileData (0x00ABF840).
struct Bank {
    bool noFilenameCopy; int lookups; int lastType;
    unsigned long FindIndexByFilename(const char* name, long type) { ++lookups; lastType = (int)type; return (unsigned long)std::strlen(name) * 10 + (unsigned long)type; }
};
struct Filenames { const char* names[4]; };
struct Chunk { long MaterialIndex; const char* Name; Filenames* Maps; unsigned long MapFlags, SelfIllumination; bool f[4]; };
struct Material {
    long MaterialIndex; const char* Name; long DecalID; unsigned long Index[4]; unsigned long MapFlags, SelfIllumination; bool f[4];
    Filenames* Copy;
    void Init(const Chunk& c, Bank* bank) {
        MaterialIndex = c.MaterialIndex; Name = c.Name; DecalID = 0; SelfIllumination = c.SelfIllumination;
        for (int i = 0; i < 4; ++i) f[i] = c.f[i];
        MapFlags = c.MapFlags;
        if (c.f[3]) { Index[0] = Index[1] = Index[2] = Index[3] = 0; return; }
        Index[0] = bank->FindIndexByFilename(c.Maps->names[0], 0);
        Index[1] = bank->FindIndexByFilename(c.Maps->names[1], 2);
        Index[2] = bank->FindIndexByFilename(c.Maps->names[2], 0);
        Index[3] = bank->FindIndexByFilename(c.Maps->names[3], 0);
        if (bank == 0 || !bank->noFilenameCopy) { delete Copy; Copy = new Filenames(*c.Maps); }
    }
};
int main() {
    Filenames maps = { { "a", "bb", "ccc", "" } };
    Chunk c = { 5, "mat", &maps, 3, 4, { true, false, true, false } };
    Bank bank = { false, 0, 0 };
    Material m; m.Copy = 0; m.DecalID = 9; m.Init(c, &bank);
    bool ok = m.MaterialIndex == 5 && m.DecalID == 0 && m.Index[0] == 10 && m.Index[1] == 22 && m.Index[2] == 30 && m.Index[3] == 0
        && m.Copy != 0 && m.Copy->names[2] == maps.names[2] && m.MapFlags == 3 && m.f[2] && bank.lookups == 4;
    Bank strip = { true, 0, 0 }; Material n; n.Copy = 0; n.Init(c, &strip);
    ok = ok && n.Copy == 0 && strip.lookups == 4;
    c.f[3] = true; Material o; o.Copy = 0; o.Index[1] = 77; o.Init(c, &bank);
    ok = ok && o.Index[1] == 0 && o.Copy == 0 && bank.lookups == 4 && o.f[3];
    delete m.Copy;
    if (!ok) { std::printf("00abf840_TEST FAIL\n"); return 1; }
    std::printf("00abf840_TEST PASS\n"); return 0;
}
