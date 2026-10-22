// CAnimComponentStrikeSpeedMultiplier::Initialise(CPersistContext& context, const CCharString& text) -- text initialisation of the component: reset to the default, parse the one float (the animation speed multiplier) out of the text, return true
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DCA60, 123 bytes, __fastcall, ret 8; vtable 0x01255800 slot 3.
// Leaf: ?Initialise@CAnimComponentStrikeSpeedMultiplier@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z
// Callees: SetDefaultValue (vtable slot 8, 0x005DB410), CCharString::CCharString(const char*, long)
// 0x0099EBF0, CStringParser::CStringParser(const CCharString&, void (*)(CCharString*), const CCharString&,
// long) 0x00404720, CCharString::~CCharString 0x0099EAE0, CPersistTraits<float>::TransferIn 0x0040F7E0,
// list<CStringParserCommentBoundaries>::~list 0x004046B0, CBase::~CBase 0x0099A2E0.
// Shape: the default reset is a real virtual call (`call [eax+20h]`). The parser is a 0x28-byte stack
// object built from the text with no error callback, an explicit `CCharString("", -1)` temporary as
// its file name (the constructor's returned `this` is pushed straight from eax; the literal is the
// pooled empty string at 0x0122D70E) and mode 0 -- VC pushes the last argument (0) before building
// the temporary, and the temporary dies right after the full expression. CPersistTraits<float> is an
// empty class whose temporary is homed in the dead `text` argument slot (`lea ecx,[esp+44h]`); it
// needs a user default constructor or VC zero-fills that byte. The parser's destructor is compiler-
// generated (no vtable store): the comment list, the two strings, then the out-of-line CBase base
// destructor. Retail CStringParser is 0x28 bytes (the PDB's 0x30 has the debug-sized list).
class CPersistContext;

struct CCharStringData;
class CCharString {
public:
    CCharStringData* m_pData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};

class CBase {
public:
    virtual ~CBase();                              // 0x0099A2E0
};

class CStringParserCommentList {                   // list<CStringParserCommentBoundaries>, retail 8 bytes
public:
    void* m_pHead;
    unsigned long m_size;
    ~CStringParserCommentList();                   // 0x004046B0
};

class CStringParser : public CBase {
public:
    char* String;                                  // +0x04
    unsigned long BaseLineOffset;                  // +0x08
    CCharString Filename;                          // +0x0C
    CCharString TempCharString;                    // +0x10
    CStringParserCommentList Comments;             // +0x14
    void (*ErrorCallback)(CCharString*);           // +0x1C
    long CursorPos[2];                             // +0x20
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename, long mode);  // 0x00404720
};

template <class T>
class CPersistTraits {
public:
    CPersistTraits() {}
    void TransferIn(CPersistContext& context, CStringParser& parser, T* value);   // float: 0x0040F7E0
};

class CAnimComponentStrikeSpeedMultiplier {
public:
    virtual ~CAnimComponentStrikeSpeedMultiplier();                                   // slot 0
    virtual void v01();
    virtual void v02();
    virtual bool Initialise(CPersistContext& context, const CCharString& text);       // slot 3
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void SetDefaultValue();                                                    // slot 8 (0x005DB410)
    long IVRefCount;                                     // +0x04 (base component data)
    float AnimationSpeedMultiplier;                // +0x08
};

bool CAnimComponentStrikeSpeedMultiplier::Initialise(CPersistContext& context, const CCharString& text)
{
    SetDefaultValue();
    CStringParser parser(text, 0, CCharString("", -1), 0);
    CPersistTraits<float>().TransferIn(context, parser, &AnimationSpeedMultiplier);
    return true;
}