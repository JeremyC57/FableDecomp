// CConsoleCommandParameters::GetParameterTypeAsString(long index) const -- the console type name ("slong", "ulong", "float", "bool", "string", else "null") of parameter `index`, returned by value
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EA740, 136 bytes including its 5-entry jump table (144 with padding).
// Callees: CCharString::CCharString() 0x0099E4B0, CCharString::operator=(const char*) 0x0099EFE0,
//          CCharString::CCharString(const CCharString&) 0x0099EC30, CCharString::~CCharString 0x0099EAE0.
// Literals: "bool" 0x0129C434, "slong" 0x0129C424, "ulong" 0x0129C458, "float" 0x0129C42C,
//           "string" 0x0129C41C, "null" 0x0129C450.
//
// Shape: the parameter types are a long array reached through the first word of the object (the
// vector's first pointer); `dec eax ; cmp eax,4 ; ja` + `jmp [table]` is a dense switch over 1..5. The
// case bodies are laid out bool, slong, ulong, float, string, default -- the source order (the
// table maps 1 slong, 2 ulong, 3 float, 4 bool, 5 string). The result is a named local assigned in
// each case and copied out through the hidden return pointer (VC7.1 has no NRVO), then destroyed.
// CCharString is modelled locally: fable_string.h inlines the copy constructor that retail calls
// out of line (GOTCHAS wave 24a).
class CCharString {
public:
    CCharString();                                  // 0x0099E4B0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const char* text);       // 0x0099EFE0
private:
    void* m_pStringData;
};

enum EConsoleParameterType {
    CONSOLE_PARAM_SLONG = 1,
    CONSOLE_PARAM_ULONG = 2,
    CONSOLE_PARAM_FLOAT = 3,
    CONSOLE_PARAM_BOOL = 4,
    CONSOLE_PARAM_STRING = 5
};

struct CConsoleCommandParameters {
    long* m_pTypes;                                 // +0x00 parameter types (vector first)
    CCharString GetParameterTypeAsString(long index) const;
};

CCharString CConsoleCommandParameters::GetParameterTypeAsString(long index) const
{
    CCharString result;
    switch (m_pTypes[index]) {
    case CONSOLE_PARAM_BOOL:
        result = "bool";
        break;
    case CONSOLE_PARAM_SLONG:
        result = "slong";
        break;
    case CONSOLE_PARAM_ULONG:
        result = "ulong";
        break;
    case CONSOLE_PARAM_FLOAT:
        result = "float";
        break;
    case CONSOLE_PARAM_STRING:
        result = "string";
        break;
    default:
        result = "null";
        break;
    }
    return result;
}
