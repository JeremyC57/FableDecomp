// CPersistTraits<CWideString>::TransferOut(CPersistContext*, CCharString*, const CWideString*) -- append the wide string to the text save as a quoted narrow string
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040E840, 137 bytes, `ret 0Ch` (__fastcall, the empty traits `this` is unused).
// Callees: CWideString::ConvertToCharString() const @ 0x0099B2C0, CCharString::CCharString(const char*, long)
//   @ 0x0099EBF0, operator+(const CCharString&, const char*) @ 0x0099F600, operator+(const CCharString&,
//   const CCharString&) @ 0x0099F570 (both __fastcall, hidden result in ecx), CCharString::operator+=(const CCharString&)
//   @ 0x0099F0A0, ~CCharString @ 0x0099EAE0. Strings: "\"" 0x0122DF24, "" 0x0122D70E.
// Shape: one expression `*out += CCharString("") + "\"" + value->ConvertToCharString() + "\"";` -- VC evaluates right
//   to left (the closing quote is pushed first, then the conversion, then the opening quote and the empty string), the
//   five temporaries are destroyed in reverse order of construction after the append (as the landed
//   CPersistContext::TransferVectorSaveText 0x009BAC80). Speed-optimised (default) TU.
struct CCharStringData;
struct CPersistContext;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    CCharString ConvertToCharString() const;            // 0x0099B2C0
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CWideString> {
    void TransferOut(CPersistContext* context, CCharString* out, const CWideString* value);
};

void CPersistTraits<CWideString>::TransferOut(CPersistContext* context, CCharString* out, const CWideString* value)
{
    *out += CCharString("") + "\"" + value->ConvertToCharString() + "\"";
}