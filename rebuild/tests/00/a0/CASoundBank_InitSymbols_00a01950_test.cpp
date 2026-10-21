#include <cstdio>
#include <cstring>
// Behaviour copy of CASoundBank::InitSymbols (0x00A01950): path split/concat, map creation, binary or text load, optional save.
struct Str { char s[64]; };
static void SplitPath(const char* path, Str* drive, Str* dir, Str* name) {
    const char* colon = std::strchr(path, ':'); const char* slash = std::strrchr(path, '/'); const char* dot = std::strrchr(path, '.');
    std::strncpy(drive->s, path, colon ? colon - path + 1 : 0); drive->s[colon ? colon - path + 1 : 0] = 0;
    const char* d0 = colon ? colon + 1 : path; std::strncpy(dir->s, d0, slash - d0 + 1); dir->s[slash - d0 + 1] = 0;
    std::strncpy(name->s, slash + 1, dot - slash - 1); name->s[dot - slash - 1] = 0;
}
struct Map { int binaryLoads; int textLoads; int saves; char lastPath[64]; const char* tag; };
struct Bank {
    Map* map; bool loadBinary, saveBinary; int progress; int created;
    void InitSymbols(const char* path) {
        Str drive, dir, name; SplitPath(path, &drive, &dir, &name);
        char symbolPath[64]; std::strcpy(symbolPath, drive.s); std::strcat(symbolPath, dir.s); std::strcat(symbolPath, name.s); std::strcat(symbolPath, ".");
        ++progress;
        ++created;
        if (loadBinary) { ++map->binaryLoads; std::strcpy(map->lastPath, symbolPath); }
        else {
            ++map->textLoads; std::strcpy(map->lastPath, path); map->tag = "";
            if (saveBinary) { ++map->saves; std::strcpy(map->lastPath, symbolPath); }
        }
    }
};
int main()
{
    Map m = { 0, 0, 0, "", 0 };
    Bank b = { &m, true, false, 0, 0 }; b.InitSymbols("c:/data/sound/bank.txt");
    if (m.binaryLoads != 1 || std::strcmp(m.lastPath, "c:/data/sound/bank.") != 0 || b.progress != 1 || b.created != 1) { std::printf("00a01950_TEST FAIL\n"); return 1; }
    Bank t = { &m, false, false, 0, 0 }; t.InitSymbols("c:/data/sound/bank.txt");
    if (m.textLoads != 1 || m.saves != 0 || std::strcmp(m.lastPath, "c:/data/sound/bank.txt") != 0 || std::strcmp(m.tag, "") != 0) { std::printf("00a01950_TEST FAIL\n"); return 1; }
    Bank s = { &m, false, true, 0, 0 }; s.InitSymbols("c:/data/sound/bank.txt");
    if (m.textLoads != 2 || m.saves != 1 || std::strcmp(m.lastPath, "c:/data/sound/bank.") != 0) { std::printf("00a01950_TEST FAIL\n"); return 1; }
    std::printf("00a01950_TEST PASS\n"); return 0;
}