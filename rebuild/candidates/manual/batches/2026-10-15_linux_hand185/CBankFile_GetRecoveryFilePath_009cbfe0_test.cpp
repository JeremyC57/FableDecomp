// Behaviour test for 009cbfe0: CBankFile::GetRecoveryFilePath -- drive + dir + name + ".rec" from the split pathname.
#include <cstdio>
#include <cstring>
struct TStr { char s[128]; TStr() { s[0] = 0; } TStr(const char* t) { std::strcpy(s, t); } };
static TStr operator+(const TStr& a, const TStr& b) { TStr r(a.s); std::strcat(r.s, b.s); return r; }
static TStr operator+(const TStr& a, const char* b) { TStr r(a.s); std::strcat(r.s, b); return r; }
static void SplitPath(const TStr& path, TStr* drive, TStr* dir, TStr* name, TStr* ext)
{
    const char* p = path.s; const char* slash = std::strrchr(p, '\\'); const char* dot = std::strrchr(p, '.');
    std::strncpy(drive->s, p, 2); drive->s[2] = 0;
    size_t dl = (size_t)(slash + 1 - (p + 2)); std::strncpy(dir->s, p + 2, dl); dir->s[dl] = 0;
    size_t nl = (size_t)(dot - (slash + 1)); std::strncpy(name->s, slash + 1, nl); name->s[nl] = 0;
    if (ext) std::strcpy(ext->s, dot);
}
struct TFile { TStr GetPathname() const { return TStr("C:\\Data\\Levels\\Fable.big"); } };
struct TBank {
    TFile* BankFile_Data;
    TStr GetRecoveryFilePath()
    {
        TStr drive, directory, name;
        SplitPath(BankFile_Data->GetPathname(), &drive, &directory, &name, 0);
        TStr path = drive + directory + name + ".rec";
        return path;
    }
};
int main()
{
    TFile f; TBank b; b.BankFile_Data = &f;
    TStr r = b.GetRecoveryFilePath();
    if (std::strcmp(r.s, "C:\\Data\\Levels\\Fable.rec") != 0) { std::printf("009cbfe0_TEST FAIL\n"); return 1; }
    std::printf("009cbfe0_TEST PASS\n");
    return 0;
}
