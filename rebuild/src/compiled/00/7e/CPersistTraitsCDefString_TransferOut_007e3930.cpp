// CPersistTraits<CDefString>::TransferOut(CPersistContext*, CCharString*, const CDefString*) -- append the def string's text to the text save, quoted
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E3930, 149 bytes (+ int3 padding), `ret 0Ch` (__fastcall, the empty traits `this` is unused).
// Callees: CDefStringTable::GetString(long) @ 0x009D49B0 on g_DefStringTable 0x013CA828 (hidden CCharString result),
//   CCharString::CCharString(const char*, long) @ 0x0099EBF0, operator+(const CCharString&, const char*) @ 0x0099F600,
//   operator+(const CCharString&, const CCharString&) @ 0x0099F570 (both __fastcall, hidden result in ecx),
//   CCharString::operator+=(const CCharString&) @ 0x0099F0A0, ~CCharString @ 0x0099EAE0. Strings: "\"" 0x0122DF24,
//   "" 0x0122D70E.
// Layout: CDefString is the 4-byte def-string-table index.
// Shape: the text is looked up into a named local first (homed in the dead value-argument slot, destroyed last), then
//   `*out += CCharString("") + "\"" + text + "\"";` (right-to-left evaluation, four temporaries destroyed in reverse
//   order), as the CWideString twin 0x0040E840. Speed-optimised (default) TU.
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

struct CDefStringTable {
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828

struct CDefString {
    long Index;                                         // +0x00
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CDefString> {
    void TransferOut(CPersistContext* context, CCharString* out, const CDefString* value);
};

void CPersistTraits<CDefString>::TransferOut(CPersistContext* context, CCharString* out, const CDefString* value)
{
    CCharString text = g_DefStringTable.GetString(value->Index);
    *out += CCharString("") + "\"" + text + "\"";
}