// CPersistTraits<bool>::TransferIn(CPersistContext* context, CStringParser* parser, bool* value) -- text persist read of a bool: the next string token matched case-insensitively against TRUE/BTRUE and FALSE/BFALSE, otherwise an "Unknown boolean value entry" error
// Leaf: ?TransferIn@?$CPersistTraits@_N@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00403FF0, 521 bytes, `ret 0Ch` (traits `this` unused).
// Identity: called as CPersistTraits<bool>::TransferIn by the landed CPersistContext::TransferMap<CCharString,bool>
//   0x007AE3D0 (which carries a YES/NO stand-in of this body) and by the bool component reader 0x00403FC0.
// Callees: CStringParser::ReadAsString @ 0x009BA6F0 (by-value CCharString through a hidden pointer), CCharString(const char*,
//   long) @ 0x0099EBF0 ("TRUE" 0x0122D6D4, "BTRUE" 0x0122D704, "FALSE" 0x0122D6CC, "BFALSE" 0x0122D6FC), ~CCharString
//   @ 0x0099EAE0, _stricmp @ 0x00BFEBA8, operator+(const char*, const CCharString&) @ 0x0099F690 (`__fastcall`, hidden
//   result in ecx; "Unknown boolean value entry " 0x0122D6DC), CPersistContext::Error(const CStringParser&, const
//   CCharString&) @ 0x009BAA90. the conversion's empty string is the CCharString header's "" @ 0x0129AAF4.
// Shape: the spellings are compared as `token == "TRUE"` -- the implicit converting-ctor temporary bound to the
//   case-insensitive operator== reference, which VC7.1 re-reads from its frame slot (an explicit CCharString("TRUE")
//   functional cast instead dereferences the ctor's returned eax and keeps the flags only in ebx: 498v521). Each `||` builds its CCharString literal temporaries conditionally, so VC7.1 tracks them in a flags word (bits
//   1/2 for TRUE/BTRUE, 4/8 for FALSE/BFALSE, spilled to the frame) and destroys them after the test; the condition bool
//   of the first test is parked in the dead `parser` slot (parser lives in esi); each accepted branch stores the value,
//   destroys the token and returns on its own tail. No EH frame (the TU has no /EHsc). Speed-optimised (default) TU.
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
};
class CCharStringData { public:
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
extern "C" int __cdecl _stricmp(const char* a, const char* b);                 // 0x00BFEBA8
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    const char* GetString() const;
};
extern char EmptyString[];   // 0x0129AAF4, the CCharString header's shared ""
__forceinline const char* CCharString::GetString() const { return PStringData == 0 ? EmptyString : PStringData->Data.PData; }
// CCharString equality is case-insensitive (same inline as the landed CGenericVarRef::ReadFromParserAsBool 0x00A63DB0).
__forceinline bool operator==(const CCharString& a, const CCharString& b) { return _stricmp(a.GetString(), b.GetString()) == 0; }

CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690

class CStringParser {
public:
    CCharString ReadAsString();                        // 0x009BA6F0
};
class CPersistContext {
public:
    void Error(const CStringParser& parser, const CCharString& message);       // 0x009BAA90
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<bool> {
    void TransferIn(CPersistContext* context, CStringParser* parser, bool* value);
};

void CPersistTraits<bool>::TransferIn(CPersistContext* context, CStringParser* parser, bool* value)
{
    CCharString token = parser->ReadAsString();
    if (token == "TRUE" || token == "BTRUE")
        *value = true;
    else if (token == "FALSE" || token == "BFALSE")
        *value = false;
    else
        context->Error(*parser, "Unknown boolean value entry " + token);
}