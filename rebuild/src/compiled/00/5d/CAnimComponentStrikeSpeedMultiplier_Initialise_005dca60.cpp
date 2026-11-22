// CAnimComponentStrikeSpeedMultiplier::Initialise(CPersistContext& context, const CCharString& text) -- text initialisation of the component: reset to the default, parse the one float (the animation speed multiplier) out of the text, return true
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DCA60, 123 bytes, __fastcall, ret 8; vtable 0x01255800 slot 3.
// Leaf: ?Initialise@CAnimComponentStrikeSpeedMultiplier@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z
// NF112 (2026-11-20, q5205): CPersistTraits<float>::TransferIn 0x0040F7E0 respelled in place to the Ego_r.pdb / FableWin.pdb
//   ?TransferIn@?$CPersistTraits@M@@QAEXAAVCPersistContext@@AAVCStringParser@@AAM@Z (non-const, references; was ...AAVCStringParser@@PAM@Z, a float pointer).
// Callees: SetDefaultValue (vtable slot 8, 0x005DB410), CCharString::CCharString(const char*, long)
// 0x0099EBF0, CStringParser::CStringParser(const CCharString&, void (__fastcall*)(const CCharString&), const CCharString&,
// long) 0x00404720, CCharString::~CCharString 0x0099EAE0, CPersistTraits<float>::TransferIn 0x0040F7E0,
// std::_List_base<CStringParserCommentBoundaries>::~_List_base 0x004046B0, CBaseClass::~CBaseClass 0x0099A2E0.
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

class CCharStringData;
class CCharString {
public:
    CCharStringData* m_pData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CStringParserCommentBoundaries;
// NF245 (2026-11-22, the 0x004046B0 family; nf243 ruling 2 (a)): the row-local std::allocator / std::list stand-ins replaced by the
//   shared STLport list (declaration-only _List_base ctor / dtor): the comment list's destructor call
//   binds ??1?$_List_base@VCStringParserCommentBoundaries@@..@std@@QAE@XZ (0x004046B0; list's own ~list is the 5-byte jmp 0x00404710).
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#include "stlport/fable_containers.h"

class CStringParser : public CBaseClass {
public:
    char* String;                                  // +0x04
    unsigned long BaseLineOffset;                  // +0x08
    CCharString Filename;                          // +0x0C
    CCharString TempCharString;                    // +0x10
    std::list<CStringParserCommentBoundaries> Comments;             // +0x14 (STLport list: the sentinel pointer, 4 bytes)
    void (*ErrorCallback)(CCharString*);           // +0x18
    long CursorPos[3];                             // +0x1C (retail CStringParser is 0x28 bytes; NF245: was an 8-byte list stand-in + CursorPos[2])
    CStringParser(const CCharString& text, void (__fastcall* errorCallback)(const CCharString&), const CCharString& filename, long mode);  // 0x00404720
};

template <class T>
class CPersistTraits {
public:
    CPersistTraits() {}
    void TransferIn(CPersistContext& context, CStringParser& parser, T& value);   // float: 0x0040F7E0
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
    CPersistTraits<float>().TransferIn(context, parser, AnimationSpeedMultiplier);
    return true;
}