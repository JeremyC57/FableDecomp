#include <cstdio>
#include <cstring>
// Behaviour model of CBankFile::BeginUpdateEntries: wide strings are modelled as small char buffers.
class CWideString { public:
    char s[64];
    CWideString() { s[0] = 0; }
    CWideString(const char* t) { std::strcpy(s, t); }
    long GetLength() const { return (long)std::strlen(s); }
};
CWideString operator+(const CWideString& a, const CWideString& b) { CWideString r(a.s); std::strcat(r.s, b.s); return r; }
CWideString operator+(const CWideString& a, const char* b) { CWideString r(a.s); std::strcat(r.s, b); return r; }
void SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension)
{
    // "C:\dir\file.bnk" -> "C:", "\dir\", "file", ".bnk"
    const char* p = path.s; const char* slash = std::strrchr(p, '\\'); const char* dot = std::strrchr(p, '.');
    std::strncpy(drive->s, p, 2); drive->s[2] = 0;
    std::strncpy(directory->s, p + 2, slash - p - 1); directory->s[slash - p - 1] = 0;
    std::strncpy(name->s, slash + 1, dot - slash - 1); name->s[dot - slash - 1] = 0;
    std::strcpy(extension->s, dot);
}
static char g_log[256];
class CDiskFileWin32 { public:
    long m_length; long m_seek; long m_access; long m_lengthCalls;
    CDiskFileWin32() : m_length(1000), m_seek(-1), m_access(0), m_lengthCalls(0) {}
    void Open(const CWideString&, long access, unsigned long) { m_access = access; }
    void Seek(long position) { m_seek = position; }
    long GetLength() { ++m_lengthCalls; return m_length; }
};
class CBankFile { public:
    CDiskFileWin32* BankFile;
    CWideString Filename;
    bool RetailMode, WrittenToFile, Solid, UpdatingFlag;
    unsigned int Alignment;
    CWideString CacheName;
    unsigned int m_zeros; int m_infoWrites;
    unsigned int Align(unsigned int size) const { return (Alignment + size - 1) / Alignment * Alignment; }
    void WriteZeros(unsigned int count) { m_zeros = count; }
    void WriteFileInformation(bool) { ++m_infoWrites; }
    void Open(const CWideString& path) { CacheName = path; }
    void BeginUpdateEntries()
    {
        if (RetailMode) {
            BankFile = new CDiskFileWin32;
            BankFile->Open(Filename, 2, 4);
            WrittenToFile = true;
            long length = BankFile->GetLength();
            BankFile->Seek(length);
            WriteZeros(Align(length) - length);
            BankFile->GetLength();
            Solid = true;
            UpdatingFlag = true;
            return;
        }
        if (CacheName.GetLength() <= 0) {
            CWideString drive, directory, name, extension;
            SplitPath(Filename, &drive, &directory, &name, &extension);
            Open(drive + directory + name + ".cks");
        }
        WriteFileInformation(false);
        UpdatingFlag = true;
    }
};
int main()
{
    CBankFile a; std::memset(&a, 0, sizeof(a));
    std::strcpy(a.Filename.s, "C:\\data\\text.bnk"); a.RetailMode = true; a.Alignment = 256;
    a.BeginUpdateEntries();
    if (!a.BankFile || a.BankFile->m_access != 2 || a.BankFile->m_seek != 1000 || a.m_zeros != 24 || a.BankFile->m_lengthCalls != 2
        || !a.WrittenToFile || !a.Solid || !a.UpdatingFlag || a.m_infoWrites != 0) { std::printf("009ce8d0_TEST FAIL\n"); return 1; }
    delete a.BankFile;
    CBankFile b; std::memset(&b, 0, sizeof(b));
    std::strcpy(b.Filename.s, "C:\\data\\text.bnk");
    b.BeginUpdateEntries();
    if (std::strcmp(b.CacheName.s, "C:\\data\\text.cks") != 0 || b.m_infoWrites != 1 || !b.UpdatingFlag || b.Solid) { std::printf("009ce8d0_TEST FAIL\n"); return 1; }
    std::strcpy(b.CacheName.s, "keep");
    b.BeginUpdateEntries();
    if (std::strcmp(b.CacheName.s, "keep") != 0 || b.m_infoWrites != 2) { std::printf("009ce8d0_TEST FAIL\n"); return 1; }
    std::printf("009ce8d0_TEST PASS\n");
    return 0;
}