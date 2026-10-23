// Grade: DIFFER(553v553) -- instructions identical; only the frame slot order of the six CWideString locals differs (retail pattern,subPattern,dir,baseName,extension,path ascending; ours the reverse by scope)
// CWorld::GetPathNameForFileName(const CWideString& fileName) const -- search every top-level directory of U:\ for a file whose name (without extension, and with an empty extension) equals fileName; return the found entry, else the empty string
// Leaf: ?GetPathNameForFileName@CWorld@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049EF50, 553 bytes, `ret 8` (/GS cookie, `and esp,-8`; FableWin.pdb
//   ?GetPathNameForFileName@CWorld@@QBE?AVCWideString@@ABV2@@Z).
// Callees: CWideString(const wchar16*) 0x0099B6B0, CWideString() 0x0099AED0, CWideString(const CWideString&) 0x0099B720,
//   ~CWideString 0x0099B510, operator+(const CWideString&, const wchar16*) 0x0099BF30, FindFirstFile 0x00999760 /
//   FindNextFile 0x00999850 (__fastcall pattern, finder, name buffer), IsPathFile 0x00999A30, SplitPath 0x00997620,
//   wcscmp 0x00BFE94C, MemCmp_Unsigned16 0x00403C60; the empty CWideString global 0x013BCA24.
//   Literals L"U:\\*" 0x01238E10, L"\\*.*" 0x01238E04, L"" 0x0122D70C.
typedef unsigned short wchar16;
extern "C" int __cdecl wcscmp(const wchar16* a, const wchar16* b);   // 0x00BFE94C
int __cdecl MemCmp_Unsigned16(const wchar16* lhs, const wchar16* rhs, unsigned long count);   // 0x00403C60
struct CWideStringData {                                // STLport basic_string<wchar_t> + NoRefs
    wchar16* _M_start;
    wchar16* _M_finish;
    wchar16* _M_end_of_storage;
    long     NoRefs;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};
struct CWideString {
    CWideStringData* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const wchar16* text);                   // 0x0099B6B0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    __forceinline bool operator==(const wchar16* text) const
    {
        if (PStringData == 0)
            return wcscmp(L"", text) == 0;
        return PStringData->size() == 0 && MemCmp_Unsigned16(PStringData->_M_start, text, 0) == 0;
    }
    __forceinline bool operator==(const CWideString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        const wchar16* a = PStringData->_M_start;
        const wchar16* b = other.PStringData->_M_start;
        unsigned long n = (unsigned long)(PStringData->_M_finish - a);
        return n == (unsigned long)(other.PStringData->_M_finish - b) && MemCmp_Unsigned16(a, b, n) == 0;
    }
};
extern const CWideString g_EmptyWideString;             // 0x013BCA24
CWideString __fastcall operator+(const CWideString& left, const wchar16* right);   // 0x0099BF30
struct CFileFinder { void* Handle; unsigned long Flags; };
bool __fastcall FindFirstFile(const CWideString& pattern, CFileFinder* finder, wchar16* fileName);   // 0x00999760
bool __fastcall FindNextFile(const CWideString& pattern, CFileFinder* finder, wchar16* fileName);    // 0x00999850
bool __fastcall IsPathFile(const CWideString& path);   // 0x00999A30
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension);   // 0x00997620

struct CWorld {
    CWideString GetPathNameForFileName(const CWideString& fileName) const;
};

CWideString CWorld::GetPathNameForFileName(const CWideString& fileName) const
{
    CWideString pattern(L"U:\\*");
    CFileFinder finder;
    wchar16 dirName[264];
    if (FindFirstFile(pattern, &finder, dirName)) {
        do {
            CWideString dir(dirName);
            if (!IsPathFile(dir)) {
                CWideString subPattern = dir + L"\\*.*";
                CFileFinder subFinder;
                wchar16 name[264];
                if (FindFirstFile(subPattern, &subFinder, name)) {
                    do {
                        CWideString path(name);
                        CWideString baseName;
                        CWideString extension;
                        SplitPath(path, 0, 0, &baseName, &extension);
                        if (extension.PStringData == 0) {
                            if (wcscmp(L"", L"") != 0)
                                continue;
                        } else if (extension.PStringData->size() != 0
                                   || MemCmp_Unsigned16(extension.PStringData->_M_start, L"", 0) != 0)
                            continue;
                        if (baseName == fileName)
                            return CWideString(name);
                    } while (FindNextFile(subPattern, &subFinder, name));
                }
            }
        } while (FindNextFile(pattern, &finder, dirName));
    }
    return g_EmptyWideString;
}
