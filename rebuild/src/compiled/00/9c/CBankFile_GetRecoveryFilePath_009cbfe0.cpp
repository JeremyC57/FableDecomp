// CBankFile::GetRecoveryFilePath() -- the bank file's path with its extension replaced by ".rec": split the open file's pathname and rebuild drive + directory + name + L".rec"
// Leaf: GetRecoveryFilePath
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CBFE0, length 205 bytes (+3 int3), ret 4 (hidden CWideString result).
// Callees: CWideString::CWideString() @ 0x0099AED0 (x3), the bank's disk file (+0x7c) virtual slot 11 (+0x2c, GetPathname,
// by value), SplitPath @ 0x00997620 (__fastcall: path in ecx, drive in edx, directory/name/extension on the stack),
// operator+(const CWideString&, const CWideString&) @ 0x0099BE70 and operator+(const CWideString&, const wchar_t*)
// @ 0x0099BF30 (both __fastcall, hidden result in ecx), CWideString::CWideString(const CWideString&) @ 0x0099B720,
// CWideString::~CWideString @ 0x0099B510. L".rec" @ 0x0129B52C.
// Shape: as the landed ReplacePathExtension 0x00997800 (three locals, null extension); the joined path is a NAMED local
// copied into the result (VC7.1 has no NRVO here); the two operator+ temporaries die at the end of the full expression.
// Layout: retail CBankFile BankFile_Data (CDiskFileWin32*) +0x7c (PDB +0x8c, retail -0x10).
// Layout (supersedes any local-model note above): engine/CBankFile.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CBankFile CBankFileDonor
#include "engine/CBankFile.h"
#undef CBankFile
class CWideString { public:
    void* PStringData;
    CWideString();                                 // 0x0099AED0
    CWideString(const CWideString& other);         // 0x0099B720
    ~CWideString();                                // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const unsigned short* b);   // 0x0099BF30 (wchar_t is unsigned short in this lane)
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension);   // 0x00997620

class CDiskFileWin32 { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual CWideString GetPathname() const;       // slot 11 (+0x2c)
};

class CBankFile : public CBankFileDonor {
public:
    CWideString GetRecoveryFilePath();
};

CWideString CBankFile::GetRecoveryFilePath()
{
    CWideString drive;
    CWideString directory;
    CWideString name;
    SplitPath(BankFile_Data->GetPathname(), &drive, &directory, &name, 0);
    CWideString path = drive + directory + name + L".rec";
    return path;
}