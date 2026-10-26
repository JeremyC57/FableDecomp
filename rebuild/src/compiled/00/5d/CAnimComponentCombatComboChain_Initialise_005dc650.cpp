// CAnimComponentCombatComboChain::Initialise(CPersistContext&, const CCharString&) -- reset, parse "<id>" or "<stage>,<id>" from the text into ComboStage / ComboID
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DC650, length 210 bytes (virtual, `ret 8`), always returns true.
// Callees: this's virtual slot 8 (+0x20, the component reset), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0 ("" and ","), CStringParser::CStringParser @ 0x00404720 (landed), CCharString::~CCharString @ 0x0099EAE0,
// TransferIn(CPersistContext*, CStringParser*, long*) @ 0x009E2590 (__fastcall), CStringParser::SkipPastNextItemIfIs(const CCharString&)
// @ 0x009B9E00, std::list<CStringParserCommentBoundaries>::~list @ 0x004046B0 (landed), CBase::~CBase @ 0x0099A2E0.
// Shape: the landed sibling CAnimComponentFlags::Initialise (0x005DC3C0) parser and inline parse helper. The first value is
// homed in the dead context-argument slot (context itself stays in edi), the "," string and the second value in the
// dead text slot: the "," string is a NAMED local in its own block around the skip call (a bare temporary takes the
// context slot and pushes the first value into the text slot); the skip result is a bool declared before that block (bl). A single value is the
// ComboID; a second value that is not -1 makes the first the ComboStage.
// Layout: rebuild/include/engine/CAnimComponentCombatComboChain.h is data-only (ComboStage +0x08, ComboID +0x0c), so the
// polymorphic class is modelled locally.
// Leaf: ?Initialise@CAnimComponentCombatComboChain
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CBase {
    virtual void v00();
    ~CBase();                                   // 0x0099A2E0
};

struct CStringParserCommentList {               // retail std::list<CStringParserCommentBoundaries>: sentinel only
    void* m_pHead;
    ~CStringParserCommentList();                // 0x004046B0
};

class CStringParser : public CBase { public:                  // retail 0x28 bytes
    char* String;                               // +0x04
    unsigned long BaseLineOffset;               // +0x08
    CCharString Filename;                       // +0x0c
    CCharString TempCharString;                 // +0x10
    CStringParserCommentList Comments;          // +0x14
    void (*ErrorCallback)(CCharString*);        // +0x18
    long CursorPos[3];                          // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);         // 0x00404720
    bool SkipPastNextItemIfIs(const CCharString& item);   // 0x009B9E00
};

class CPersistContext;

void __fastcall TransferIn(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590

// The parse step as an inline helper: its local is homed in the caller's dead argument slot.
inline long ParseLong(CPersistContext& context, CStringParser& parser)
{
    long value;
    TransferIn(&context, &parser, &value);
    return value;
}

class CAnimComponentBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Reset();                       // slot 8 (+0x20)
    long _base04;                               // +0x04
};

class CAnimComponentCombatComboChain : public CAnimComponentBase { public:
    long ComboStage;                            // +0x08
    long ComboID;                               // +0x0c
    virtual bool Initialise(CPersistContext& context, const CCharString& text);
};

bool CAnimComponentCombatComboChain::Initialise(CPersistContext& context, const CCharString& text)
{
    Reset();
    CStringParser parser(text, 0, CCharString("", -1), 0);
    long first = ParseLong(context, parser);
    bool hasSecond;
    {
        CCharString comma(",", -1);
        hasSecond = parser.SkipPastNextItemIfIs(comma);
    }
    if (hasSecond) {
        long second = ParseLong(context, parser);
        if (second != -1) {
            ComboStage = first;
            ComboID = second;
            return true;
        }
    }
    ComboID = first;
    return true;
}