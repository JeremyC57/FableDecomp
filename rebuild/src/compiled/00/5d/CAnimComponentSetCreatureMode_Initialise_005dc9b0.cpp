// CAnimComponentSetCreatureMode::Initialise(CPersistContext&, const CCharString&) -- reset, parse the creature mode from the text; a zero (no) mode fails, otherwise it is stored in Mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DC9B0, length 170 bytes (virtual, `ret 8`).
// Callees: this's virtual slot 8 (+0x20, the component reset), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0, CStringParser::CStringParser @ 0x00404720 (landed), CCharString::~CCharString @ 0x0099EAE0,
// TransferIn(CPersistContext*, CStringParser*, long*) @ 0x009E2590 (__fastcall), std::list<CStringParserCommentBoundaries>::~list
// @ 0x004046B0 (landed), CBaseClass::~CBaseClass @ 0x0099A2E0.
// Shape: the landed sibling CAnimComponentFlags::Initialise (0x005DC3C0): the parser local, the inline parse helper whose
// value is homed in the dead text-argument slot; the zero test picks one of two inlined parser-destructor tails.
// Layout: rebuild/include/engine/CAnimComponentSetCreatureMode.h is data-only (Mode +0x08, EMode), so the polymorphic class
// is modelled locally.
// Leaf: ?Initialise@CAnimComponentSetCreatureMode
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

struct CStringParserCommentList {               // retail std::list<CStringParserCommentBoundaries>: sentinel only
    void* m_pHead;
    ~CStringParserCommentList();                // 0x004046B0
};

class CStringParser : public CBaseClass { public:                  // retail 0x28 bytes
    char* String;                               // +0x04
    unsigned long BaseLineOffset;               // +0x08
    CCharString Filename;                       // +0x0c
    CCharString TempCharString;                 // +0x10
    CStringParserCommentList Comments;          // +0x14
    void (*ErrorCallback)(CCharString*);        // +0x18
    long CursorPos[3];                          // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);         // 0x00404720
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

class CAnimComponentSetCreatureMode : public CAnimComponentBase { public:
    long Mode;                                  // +0x08 EMode
    virtual bool Initialise(CPersistContext& context, const CCharString& text);
};

bool CAnimComponentSetCreatureMode::Initialise(CPersistContext& context, const CCharString& text)
{
    Reset();
    CStringParser parser(text, 0, CCharString("", -1), 0);
    long mode = ParseLong(context, parser);
    if (mode) {
        Mode = mode;
        return true;
    }
    return false;
}