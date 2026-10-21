#include <cstdio>
#include <cstring>
#include <string>
// CWideString stand-in: the test keeps the text narrow (g++'s wchar_t is 4 bytes); every operation the function uses
// (Contains, concatenation, equality, ToCharString) behaves as on the 16-bit original
struct CCharString {
    std::string S;
    CCharString() { }
    CCharString(const char* text) : S(text) { }
    long GetLength() const { return (long)S.size(); }
    operator const char*() const { return S.c_str(); }
};
struct CWideString {
    std::string S;
    CWideString() { }
    CWideString(const char* text) : S(text) { }
    bool Contains(const CWideString& part) const { return S.find(part.S) != std::string::npos; }
    CCharString ToCharString() const { return CCharString(S.c_str()); }
    bool operator==(const CWideString& other) const { return S == other.S; }
};
static CWideString operator+(const CWideString& a, const CWideString& b) { CWideString r; r.S = a.S + b.S; return r; }
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s\n", addr, what); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CUserProfileManager::GetSaveDisplayNameFromFileName maps the autosave file to its display name, a
// manual save (name starting with "Save" or containing " - Save") to the slot's display name, anything else to itself
struct CCharStringOps {
    static bool StartsWith(const CCharString& text, const CCharString& prefix) { return std::strncmp(text, prefix, prefix.GetLength()) == 0; }
};
struct CUserProfileManager {
    CWideString GetManualSaveBaseFileName() const { return "Save"; }
    CWideString GetAutoSaveFileName() const { return "AutoSave"; }
    CWideString GetAutoSaveDisplayName() const { return "Auto Save"; }
    CWideString GetManualSaveDisplayName(long slot) const { char b[32]; std::sprintf(b, "Manual Save %ld", slot); return b; }
    CWideString GetSaveDisplayNameFromFileName(const CWideString& fileName, long slot) const;
};
CWideString CUserProfileManager::GetSaveDisplayNameFromFileName(const CWideString& fileName, long slot) const
{
    if (fileName == GetAutoSaveFileName())
        return GetAutoSaveDisplayName();
    bool manual = CCharStringOps::StartsWith(fileName.ToCharString(), GetManualSaveBaseFileName().ToCharString())
        || fileName.Contains(CWideString(" - ") + GetManualSaveBaseFileName());
    if (manual)
        return GetManualSaveDisplayName(slot);
    return fileName;
}
int main()
{
    CUserProfileManager manager;
    CHECK("00407e10", manager.GetSaveDisplayNameFromFileName("AutoSave", 3).S == "Auto Save");
    CHECK("00407e10", manager.GetSaveDisplayNameFromFileName("Save02", 2).S == "Manual Save 2");
    CHECK("00407e10", manager.GetSaveDisplayNameFromFileName("Oakvale - Save 7", 7).S == "Manual Save 7");
    CHECK("00407e10", manager.GetSaveDisplayNameFromFileName("MySave", 1).S == "MySave");
    CHECK("00407e10", manager.GetSaveDisplayNameFromFileName("AutoSave2", 1).S == "AutoSave2");
    std::printf("00407e10_TEST PASS\n");
    return 0;
}