// CConversation::operator=(const CConversation& other) -- memberwise assignment: the def base
// assigned out of line, Lines, then the four CCharString vectors assigned out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E54C50, length 73 bytes (+ int3 padding).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the def base, as in the landed CCardDef::Copy),
// vector<CCharString>::operator= @ 0x00432EE9 (x4; the manifest's "CCombatWheel::operator="
// label is propagated). No vtable store (assignment). Returns `this`.
// PDB layout (retail vectors are the 12-byte {first, last, end}): long Lines +0x28,
// vector<CCharString> Speaker +0x2c, Dialogue +0x38, Animation +0x44, AnimLoop +0x50.
// engine/CDefClassBase.h (PDB, 0x25 bytes padded to 0x28) is the base layout, modelled locally
// with its operator=. The manifest label "Copy" is rejected: the body is the assignment
// operator (the copy constructor twin is @ 0x00E54CA0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

struct CRetailVector_CCharString {             // VC6-era Dinkumware vector<CCharString>: first / last / end
    void* first;
    void* last;
    void* end;
    CRetailVector_CCharString& operator=(const CRetailVector_CCharString& other);   // 0x00432EE9
};

// def vtable 0x012E5FF0: slot 0 0x00E54D60 (implicit inline dtor); real overrides from the def vtable scan
class CConversation : public CDefClassBase { public:  // vtable 0x012E5FF0
    long                      Lines;           // +0x28
    CRetailVector_CCharString Speaker;         // +0x2c
    CRetailVector_CCharString Dialogue;        // +0x38
    CRetailVector_CCharString Animation;       // +0x44
    CRetailVector_CCharString AnimLoop;        // +0x50
    virtual void v00();
    CConversation& operator=(const CConversation& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x00E579E0
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x00E54D10
    virtual unsigned long GetSizeofClass();                  // slot 20 0x00E54D00
};

CConversation& CConversation::operator=(const CConversation& other)
{
    CDefClassBase::operator=(other);
    Lines = other.Lines;
    Speaker = other.Speaker;
    Dialogue = other.Dialogue;
    Animation = other.Animation;
    AnimLoop = other.AnimLoop;
    return *this;
}