// behaviour test for 0x00AE85F0 CParticleEmitterDatabase::CreateSymbolMap: clear, add every emitter's upper-cased name with its handle, finalise
#include <cstdio>
#include <cstring>
#include <cctype>
struct Sym { char name[16]; int value; };
struct CSymbolMap {
    Sym syms[8]; int n; bool finalised;
    void Clear() { n = 0; finalised = false; }
    void AddSymbolDirty(const char* prefix, const char* name, int value)
    { std::strcpy(syms[n].name, prefix); std::strcat(syms[n].name, name); syms[n].value = value; ++n; }
    void Finalise() { finalised = true; }
};
struct CEmitter { char Name[16]; };
struct SEntry { int first; CEmitter* second; };
struct CDb {
    CSymbolMap SymbolMap; SEntry* first; SEntry* last;
    void CreateSymbolMap()
    {
        SymbolMap.Clear();
        for (SEntry* it = first; it != last; ++it) {
            char name[16]; std::strcpy(name, it->second->Name);
            for (char* p = name; *p; ++p) *p = (char)std::toupper((unsigned char)*p);
            SymbolMap.AddSymbolDirty("", name, it->first);
        }
        SymbolMap.Finalise();
    }
};
int main()
{
    CEmitter a = { "smoke" }, b = { "Fire" };
    SEntry e[2] = { { 3, &a }, { 7, &b } };
    CDb db; db.SymbolMap.n = 5; db.first = e; db.last = e + 2;
    db.CreateSymbolMap();
    if (db.SymbolMap.n != 2 || !db.SymbolMap.finalised || std::strcmp(db.SymbolMap.syms[0].name, "SMOKE") != 0 ||
        db.SymbolMap.syms[1].value != 7) { std::printf("00ae85f0_TEST FAIL\n"); return 1; }
    std::printf("00ae85f0_TEST PASS\n");
    return 0;
}