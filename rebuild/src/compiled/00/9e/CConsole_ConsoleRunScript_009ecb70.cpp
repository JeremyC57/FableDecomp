// CConsole::ConsoleRunScript(CConsole& console, const std::vector<CGenericVar>& args) -- console command handler: take args[0] as a config file name, append ".ini" unless it already ends in it (case-insensitively), and execute the file
// Leaf: ?ConsoleRunScript@CConsole@@SIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009ECB70, 255 bytes, public static `__fastcall` (console in ecx, args in edx), `ret`.
// Identity (proven): CConsole::Initialise 0x009ED190 registers this handler (`mov [edi+18h],9ECB70h`) beside the landed
//   ConsoleAddKeyBinding 0x009ECC70 (same `SIXAAV1@ABV?$vector@VCGenericVar@@...` static-handler signature); of the PDB's
//   static handlers only ConsoleRunScript fits a body that hands a file name to CConsole::ExecuteConfigFile.
// Callees: CCharString copy ctor 0x0099EC30 (through CGenericVar::GetString), CCharString(const char*, long) 0x0099EBF0,
//   CCharString::ToLower() const 0x0099ED10, CCharString::operator+=(const char*) 0x0099F100, CCharString::ToWideString()
//   const 0x0099E480, CConsole::ExecuteConfigFile(const CWideString&) 0x009EC890, ~CWideString 0x0099B510, ~CCharString
//   0x0099EAE0. Data: ".ini" 0x0129C5F4 (both uses), the empty string 0x0129AAF4.
// Shape: the inline CCharString EndsWith of the landed 0x008EA500 / 0x005955AB (offset = Length() - suffix.Length(); a
//   negative offset is false; else intrinsic strcmp == 0) applied to the ToLower() temporary. The ".ini" suffix is a named
//   local in its own block (retail reads its data straight from the frame slot; a CCharString(".ini") temporary is
//   addressed through the constructor's eax, DIFFER 257v255); it is built before ToLower runs and dies after it.
#include <string.h>
extern char g_emptyString[];                            // 0x0129AAF4
struct CStringData { char* Text; long Length; };
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};
class CCharString {
public:
    CStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString ToLower() const;                        // 0x0099ED10
    const CCharString& operator+=(const char* text);          // 0x0099F100
    CWideString ToWideString() const;                   // 0x0099E480
    long Length() const { return PStringData == 0 ? 0 : PStringData->Length; }
    const char* c_str() const { return PStringData == 0 ? g_emptyString : PStringData->Text; }
    __forceinline bool EndsWith(const CCharString& suffix) const
    {
        long offset = Length() - suffix.Length();
        if (offset >= 0)
            return strcmp(c_str() + offset, suffix.c_str()) == 0;
        return false;
    }
};

class CGenericVar {                                     // Ego_r.pdb, 0x1C bytes, polymorphic
public:
    virtual ~CGenericVar();
    CCharString GetString() const { return ValString; }
    long          Type;                                 // +0x04 EGenericVarType
    CCharString   ValString;                            // +0x08
    long          ValSLONG;                             // +0x0C
    unsigned long ValULONG;                             // +0x10
    bool          ValBool;                              // +0x14
    float         ValFloat;                             // +0x18
};

namespace std {
    template <class _Tp> class allocator { public: };
    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
    template <class _Tp, class _Alloc> class _Vector_base {
    public:
        _Tp* _M_start;
        _Tp* _M_finish;
        _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    };
    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector : protected _Vector_base<_Tp, _Alloc> {
    public:
        typedef const _Tp& const_reference;
        typedef unsigned int size_type;
        const _Tp* begin() const { return this->_M_start; }
        const_reference operator[](size_type __n) const { return *(begin() + __n); }
    };
}

class CConsole {
public:
    virtual ~CConsole();
    static void __fastcall ConsoleRunScript(CConsole& console, const std::vector<CGenericVar>& args);
    void ExecuteConfigFile(const CWideString& fileName); // 0x009EC890
};

void __fastcall CConsole::ConsoleRunScript(CConsole& console, const std::vector<CGenericVar>& args)
{
    CCharString fileName = args[0].GetString();
    bool hasExtension;
    {
        CCharString extension(".ini");
        hasExtension = fileName.ToLower().EndsWith(extension);
    }
    if (!hasExtension)
        fileName += ".ini";
    console.ExecuteConfigFile(fileName.ToWideString());
}