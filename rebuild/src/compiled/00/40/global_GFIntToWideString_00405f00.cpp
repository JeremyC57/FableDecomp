// IntToWideString(long value) -- format `value` with L"%d" into a CWideString temporary and return a copy of it (__fastcall: the hidden result in ecx, the value in edx)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00405F00, length 64 bytes.
// Callees: CWideString::CWideString() @ 0x0099AED0 (the folded default constructor shared with CCharString), the wide formatter @ 0x0099BA70 (`__cdecl` CWideString::Sprintf(const wchar_t*, ...), `this` pushed first,
// `add esp,0Ch`; manifest "CCharString::SetFromFormatV"), CWideString::CWideString(const CWideString&) @ 0x0099B720 into the hidden result, ~CWideString @ 0x0099B510.
// The literal @ 0x0122DE44 is L"%d". Shape: a one-word frame for the temporary, the value kept in edi, the hidden pointer in esi and returned. The chunk label
// "CGuiControlNumber::SetSLongValue" is propagated; the body is a free `__fastcall` formatter (0x0040xxxx module built with /Gr).
typedef unsigned short wchar_t;                    // VC7.1 without /Zc:wchar_t

class CWideString { public:
    void* m_pStringData;
    CWideString();                                 // 0x0099AED0
    CWideString(const CWideString& other);         // 0x0099B720
    ~CWideString();                                // 0x0099B510
    void __cdecl Sprintf(const wchar_t* format, ...);   // 0x0099BA70
};

// Leaf: GFIntToWideString
CWideString __fastcall GFIntToWideString(long value)
{
    CWideString text;
    text.Sprintf(L"%d", value);
    return text;
}