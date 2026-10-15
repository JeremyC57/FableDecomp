// CCreatureAction_Read::CCreatureAction_Read(CThingCreatureBase*, const CWideString&) -- base CCreatureActionBase(owner, 0, 0, the owner's "STANDARD_CAST" animation, zeroed CSoundPair, 0, 0), then a copy of the text tag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E7FD0, length 117 bytes (+ int3 padding). Vtable 0x01270AC4.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the creature's animation lookup by name @ 0x006924B0 (`__fastcall`: ecx owner,
// edx the name), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*, const CSoundPair&, long, long)
// @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0, CWideString::CWideString(const CWideString&) @ 0x0099B720.
// Shape: the animation name is an IMPLICIT conversion temporary (`GetAnimationEntry(owner, "STANDARD_CAST")` through the defaulted
// length): VC builds it first (before the owner is loaded), re-addresses it with `lea` and destroys it right after the base constructor,
// while the sound pair's immediate zero stores stay next to the lookup call (an inline helper returning the string by value hoists
// them above the string constructor). The two float zeros push as `push 0`; TextTag (PDB +0xb4 = retail +0xa8) is copy-constructed
// after the vtable store.
struct CThingCreatureBase;
struct CAnimationEntry;
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);  // 0x0099B720
};
struct CSoundPair {                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
CAnimationEntry* __fastcall GetAnimationEntry(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};

struct CCreatureAction_Read : CCreatureActionBase {   // vtable 0x01270AC4
    CWideString TextTag;                    // +0xa8 (PDB +0xb4)
    virtual void v00();
    CCreatureAction_Read(CThingCreatureBase* owner, const CWideString& textTag);
};

CCreatureAction_Read::CCreatureAction_Read(CThingCreatureBase* owner, const CWideString& textTag)
    : CCreatureActionBase(owner, 0.0f, 0.0f, GetAnimationEntry(owner, "STANDARD_CAST"), CSoundPair(), 0, 0)
    , TextTag(textTag)
{
}