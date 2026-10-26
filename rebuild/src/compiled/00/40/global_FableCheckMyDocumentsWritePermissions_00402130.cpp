// _global::FableCheckMyDocumentsWritePermissions(const CWideString& myDocumentsPath) -- 0 when the My Documents folder is local and writable, else the CRC of the TEXT_GUI_WINLOGO error text key (redirected UNC folder, or restricted permissions)
// Leaf: ?FableCheckMyDocumentsWritePermissions@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00402130, 372 bytes (+12 int3), plain `ret`; the path by reference in ecx (/Gr
// link-time __fastcall).  Replaces the landed asm_bake bootstrap with compiler-emitted C++.
// Callees: CCharString() 0x0099E4B0, CWideString::GetSubString(0, 2) 0x0099BDB0, wcscmp 0x00BFE94C, char_traits<wchar_t>::compare
// 0x00403C60 (cdecl, landed MemCmp_Unsigned16), ~CWideString 0x0099B510, CCharString::operator=(const char*) 0x0099EFE0,
// operator+(const CWideString&, const wchar_t*) 0x0099BF30, CWideString::GetAsCharArray 0x0099AF10 (x3), CreateFileW /
// CloseHandle / DeleteFileW through the IAT (0x0143FE2C/30/34), _wfopen 0x00BFEA26 (L"w+"), fclose 0x00BFEA20,
// CCharString::operator!=(const char*) 0x0099E960, CCRC::Calc 0x004014A0, ~CCharString 0x0099EAE0.
// Data: L"\\\\" 0x0122D818, the probe-file name suffix (the sentence "This directory has restricted permissions, and Fable cannot
// run properly") 0x0122D750, error keys TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_REDIRECTED 0x0122D7E4 / _RESTRICTED 0x0122D710.
// Shape: `path.GetSubString(0, 2) == L"\\\\"` is CWideString::operator==(const wchar_t*) inline as an if/return pair (null data:
// wcscmp with L""; else STLport's `size() == traits::length(s) && traits::compare(...) == 0` with the index-loop length) --
// the if/return form keeps both arms' bool in bl (a ternary widens the && arm to `mov ebx,1`), and both inline levels need
// __forceinline.  The three failure arms (CreateFileW INVALID_HANDLE_VALUE, _wfopen null, DeleteFileW false) each assign the
// RESTRICTED key; VC7.1 tail-merges them.  The error key's CRC is the inline CCRC::CalcFromString.
#include <stddef.h>
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags
struct _iobuf;
extern "C" int __cdecl wcscmp(const wchar16* a, const wchar16* b);                    // 0x00BFE94C
extern "C" _iobuf* __cdecl _wfopen(const wchar16* name, const wchar16* mode);         // 0x00BFEA26
extern "C" int __cdecl fclose(_iobuf* stream);                                        // 0x00BFEA20
extern "C" __declspec(dllimport) void* __stdcall CreateFileW(const wchar16* name, unsigned long access, unsigned long share,
    void* security, unsigned long disposition, unsigned long attributes, void* templateFile);   // [0x0143FE2C]
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);             // [0x0143FE30]
extern "C" __declspec(dllimport) int __stdcall DeleteFileW(const wchar16* name);      // [0x0143FE34]

struct char_traits_w {
    static unsigned long length(const wchar16* s)
    {
        unsigned long i = 0;
        for (; s[i] != 0; ++i) { }
        return i;
    }
    static int __cdecl compare(const wchar16* a, const wchar16* b, unsigned long n);   // 0x00403C60
};
struct CBasicStringW {                                  // STLport basic_string<wchar_t>, 12 bytes
    wchar16* _M_start;                                  // +0x00
    wchar16* _M_finish;                                 // +0x04
    wchar16* _M_end_of_storage;                         // +0x08
    unsigned long size() const { return _M_finish - _M_start; }
    const wchar16* data() const { return _M_start; }
};
__forceinline bool operator==(const CBasicStringW& s, const wchar16* text)
{
    unsigned long n = char_traits_w::length(text);
    return s.size() == n && char_traits_w::compare(s.data(), text, n) == 0;
}
class CWideStringData { public:                                // 16 bytes
    CBasicStringW Data;                                 // +0x00
    long          NoRefs;                               // +0x0c
};
class CWideString {
public:
    CWideStringData* PStringData;                       // +0x00
    ~CWideString();                                     // 0x0099B510
    CWideString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099BDB0
    const wchar16* GetAsCharArray() const;              // 0x0099AF10
    __forceinline bool operator==(const wchar16* text) const
    {
        if (PStringData == 0)
            return wcscmp(text, (const wchar16*)L"") == 0;
        return PStringData->Data == text;
    }
};
CWideString operator+(const CWideString& lhs, const wchar16* rhs);   // 0x0099BF30

extern const char g_emptyLiteral_0129aaf4[];       // 0x0129AAF4: "" (the string library's shared empty literal; this TU's own "" is 0x0122D70E)
class CCharString {
public:
    struct CStringData {
        const char* Data;                               // +0x0
        long        Length;                             // +0x4
    };
    CStringData* PStringData;                           // +0x0
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);     // 0x0099EFE0
    bool operator!=(const char* text) const;            // 0x0099E960
    const char* GetString() const { return PStringData == 0 ? g_emptyLiteral_0129aaf4 : PStringData->Data; }   // the shared "" 0x0129AAF4
    unsigned long Length() const { return PStringData == 0 ? 0 : PStringData->Length; }
};
class CCRC {
public:
    static unsigned long Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
    static unsigned long CalcFromString(const CCharString& s) { return Calc(0, s.GetString(), s.Length()); }
};

unsigned long FableCheckMyDocumentsWritePermissions(const CWideString& myDocumentsPath)
{
    CCharString errorText;
    if (myDocumentsPath.GetSubString(0, 2) == (const wchar16*)L"\\\\")
        errorText = "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_REDIRECTED";
    else {
        CWideString testFile = myDocumentsPath + (const wchar16*)L"This directory has restricted permissions, and Fable cannot run properly";
        void* handle = CreateFileW(testFile.GetAsCharArray(), 0x10000000, 3, 0, 2, 0, 0);
        if (handle == (void*)-1)
            errorText = "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED";
        else {
            CloseHandle(handle);
            _iobuf* file = _wfopen(testFile.GetAsCharArray(), (const wchar16*)L"w+");
            if (file == 0)
                errorText = "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED";
            else {
                fclose(file);
                if (!DeleteFileW(testFile.GetAsCharArray()))
                    errorText = "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED";
            }
        }
    }
    if (errorText != "")
        return CCRC::CalcFromString(errorText);
    return 0;
}