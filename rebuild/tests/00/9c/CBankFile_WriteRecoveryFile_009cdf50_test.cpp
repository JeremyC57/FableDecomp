// Behaviour test for 0x009CDF50 CBankFile::WriteRecoveryFile: "BREC", three ULONGs, then the entry list, into the recovery file
#include <cstdio>
#include <cstring>
static unsigned char gOut[64]; static int gLen = 0; static int gOpenAccess = 0, gOpenFlags = 0, gClosed = 0;
class CWideString { public: const char* Text; CWideString() : Text("") {} CWideString(const char* t) : Text(t) {} };
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CAFile { public: virtual ~CAFile() {} virtual void Open(const CWideString&, FileAccessType, unsigned long) = 0; virtual void Close() = 0; };
class CDiskFileWin32 : public CAFile {
public:
    CDiskFileWin32() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString& p, FileAccessType a, unsigned long f) { Pathname = p; gOpenAccess = a; gOpenFlags = (int)f; Opened = true; }
    virtual void Close() { if (Opened) ++gClosed; Opened = false; }
    bool Opened; CWideString Pathname;
};
class CDataOutputStream { public:
    virtual ~CDataOutputStream() {}
    virtual void Write(const void* d, long n) { std::memcpy(gOut + gLen, d, n); gLen += n; }
    void WriteULONG(unsigned long v) { unsigned int x = (unsigned int)v; Write(&x, 4); }
};
class CFileDataOutputStream : public CDataOutputStream { public:
    CFileDataOutputStream(CAFile& f) : File(&f) {}
    CAFile* File;
};
class CBankFile {
public:
    unsigned long Entries;
    unsigned long InternalWriteEntryList(CDataOutputStream& s) { s.WriteULONG(Entries); return 4; }
    CWideString GetRecoveryFilePath() { return CWideString("bank.rec"); }
    void WriteRecoveryFile(unsigned long a, unsigned long b, unsigned long c)
    {
        CWideString path = GetRecoveryFilePath();
        CDiskFileWin32 file;
        file.Open(path, FILE_ACCESS_WRITE, 4);
        CFileDataOutputStream stream(file);
        char id[5] = "BREC";
        stream.Write(id, 4);
        stream.WriteULONG(a);
        stream.WriteULONG(b);
        stream.WriteULONG(c);
        InternalWriteEntryList(stream);
    }
};
int main()
{
    CBankFile bank; bank.Entries = 7;
    bank.WriteRecoveryFile(1, 2, 3);
    unsigned int w[4]; std::memcpy(w, gOut + 4, 16);
    if (gLen != 20 || std::memcmp(gOut, "BREC", 4) != 0 || w[0] != 1 || w[1] != 2 || w[2] != 3 || w[3] != 7
        || gOpenAccess != 4 || gOpenFlags != 4 || gClosed != 1) { std::printf("009cdf50_TEST FAIL\n"); return 1; }
    std::printf("009cdf50_TEST PASS\n");
    return 0;
}