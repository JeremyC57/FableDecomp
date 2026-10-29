// CAnimComponentTransitionInTime::Initialise(CPersistContext& context, const CCharString& text) -- reset, then parse TransitionInTime (one long) out of the text; return true
// Leaf: ?Initialise@CAnimComponentTransitionInTime@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DC450, 125 bytes, thiscall, `ret 8`.
// Identity (proven): slot 3 of vtable 0x01255670 (RTTI .?AVCAnimComponentTransitionInTime@@); Ego_r.pdb ?Initialise@CAnimComponentTransitionInTime@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z;
//   Ego_r.pdb's only member of the class is long TransitionInTime +0x08 (12-byte class).
// Callees: this's virtual slot 8 (+0x20, the reset), CCharString::CCharString(const char*, long) 0x0099EBF0,
//   CStringParser::CStringParser 0x00404720, CCharString::~CCharString 0x0099EAE0, TransferIn(CPersistContext*,
//   CStringParser*, long*) 0x009E2590 (`__fastcall`), list<CStringParserCommentBoundaries>::~list 0x004046B0, CBaseClass::~CBaseClass 0x0099A2E0.
// Shape (family M157, representative 0x005DC450): the landed CAnimComponentSetCreatureMode::Initialise 0x005DC9B0 /
//   CAnimComponentFlags::Initialise 0x005DC3C0 -- `Reset(); CStringParser parser(text, 0, CCharString("", -1), 0);
//   TransitionInTime = ParseLong(context, parser); return true;` (the parsed long homed in the dead text slot). Speed TU.
class CCharString {
public:
    void* m_pData;
    CCharString(const char* text, long length);           // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
};
class CBaseClass {
public:
    virtual ~CBaseClass();                                             // 0x0099A2E0
};
struct CStringParserCommentList {                         // retail std::list<CStringParserCommentBoundaries>: sentinel only
    void* m_pHead;
    ~CStringParserCommentList();                          // 0x004046B0
};
class CStringParser : public CBaseClass {                      // retail 0x28 bytes
public:
    char* String;                                         // +0x04
    unsigned long BaseLineOffset;                         // +0x08
    CCharString Filename;                                 // +0x0c
    CCharString TempCharString;                           // +0x10
    CStringParserCommentList Comments;                    // +0x14
    void (*ErrorCallback)(CCharString*);                  // +0x18
    long CursorPos[3];                                    // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);                   // 0x00404720
};
class CPersistContext;
void __fastcall TransferIn(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590
inline long ParseLong(CPersistContext& context, CStringParser& parser)
{
    long value;
    TransferIn(&context, &parser, &value);
    return value;
}
class CAnimComponentBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Reset();                                 // slot 8 (+0x20)
    long _base04;                                         // +0x04
};
class CAnimComponentTransitionInTime : public CAnimComponentBase {
public:
    long TransitionInTime;                                           // +0x08
    virtual bool Initialise(CPersistContext& context, const CCharString& text);
};
bool CAnimComponentTransitionInTime::Initialise(CPersistContext& context, const CCharString& text)
{
    Reset();
    CStringParser parser(text, 0, CCharString("", -1), 0);
    TransitionInTime = ParseLong(context, parser);
    return true;
}