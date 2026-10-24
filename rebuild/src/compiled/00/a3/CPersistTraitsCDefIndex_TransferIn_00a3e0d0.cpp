// CPersistTraits<CDefIndex>::TransferIn(CPersistContext*, CStringParser*, CDefIndex*) -- parse a definition name and store its global index; "" and "NULL" leave the index alone, an unknown name is reported
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3E0D0, 168 bytes (+ int3 padding), `ret 0Ch` (__fastcall, the empty traits `this` is unused).
// Callees: CCharString::CCharString() @ 0x0099E4B0 (x2), CPersistTraits<CCharString>::TransferIn @ 0x0040E510,
//   CCharString::operator!=(const char*) @ 0x0099E960, CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&)
//   @ 0x009AD410 on context->PDefinitionManager (+0), operator+(const char*, const CCharString&) @ 0x0099F690 (__fastcall),
//   CPersistContext::Error(const CStringParser&, const CCharString&) @ 0x009BAA90, ~CCharString @ 0x0099EAE0.
//   Strings: "" 0x0122D70E, "NULL" 0x012393E4, "Unknown definition symbol " 0x0129D240.
// Layout: PDB CDefIndex { long Val; }; CPersistContext +0 PDefinitionManager (as the landed 0x00463334 model); the
//   CCharString traits holds its DefaultVal string (4 bytes, constructed first, destroyed last).
// Shape: the manager in a named local is loaded into ecx before the argument (as retail); the index is stored before
//   its zero test; the message temporary is homed in the dead parser-argument slot.
//   Speed-optimised (default) TU.
struct CCharStringData;
struct CStringParser;
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString& operator=(const char* text);           // 0x0099EFE0
    bool operator!=(const char* text) const;            // 0x0099E960
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690

struct CDefString {
    long TablePos;                                      // +0x00 (PDB)
};
struct CDefIndex {
    long Val;                                           // +0x00 (PDB)
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
    CDefString GetDefNameFromGlobalIndex(long index);           // 0x009ACCC0
};
struct CPersistContext {
    CDefinitionManager* PDefinitionManager;             // +0x00
    void Error(const CStringParser& parser, const CCharString& message);   // 0x009BAA90
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CCharString> {
    CCharString DefaultVal;
    void TransferIn(CPersistContext* context, CStringParser* parser, CCharString* value);   // 0x0040E510
    void TransferOut(CPersistContext* context, CCharString* out, CCharString* value);       // 0x0040E630
};
template<> struct CPersistTraits<CDefIndex> {
    void TransferIn(CPersistContext* context, CStringParser* parser, CDefIndex* value);
};

void CPersistTraits<CDefIndex>::TransferIn(CPersistContext* context, CStringParser* parser, CDefIndex* value)
{
    CPersistTraits<CCharString> traits;
    CCharString name;
    traits.TransferIn(context, parser, &name);
    if (name != "" && name != "NULL") {
        CDefinitionManager* manager = context->PDefinitionManager;
        long index = manager->GetDefGlobalIndexFromName(name);
        value->Val = index;
        if (index == 0)
            context->Error(*parser, "Unknown definition symbol " + name);
    }
}