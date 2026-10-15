// NCreatureMode::CModeCrouch::CModeCrouch(CThing*) -- mode ctor over CModeBase: crouch animation name at +0x1c, two-group CAnimFilterOf2Groups at +0x20
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073FDB0, 144 bytes (padding included).
// Shape: CModeBase ctor (0x0073EAC0, 0x1c bytes), vtable 0x01267B34 (rtti NCreatureMode::CModeCrouch),
// the CCharString member at +0x1c built straight from the literal @ 0x01258F14 (0x0099EBF0), then the
// CAnimFilterOf2Groups member at +0x20 (vtable 0x01267578, inline ctor as landed at 0x0073E9D0: both
// strings default-constructed (0x0099E4B0) then SetGroups(a, b) @ 0x0073E040). Its two CCharString
// arguments are implicit conversion temporaries built right to left: the second (literal 0x0122D70E)
// in the frame word reserved by `push ecx`, the first (literal 0x01267B28) in the dead `thing`
// argument slot; destroyed first-argument first (0x0099EAE0).
struct CThing;

struct CCharString {
    void* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
};

struct CModeBase {
    virtual void v00();
    unsigned char _body[0x18];             // ParentThing, WeaponBasedFilter, FramesBeforeRemovingSelf, Active
    CModeBase(CThing* thing);              // 0x0073EAC0
};

struct CAnimFilterBase {
    virtual void v00();
    CAnimFilterBase() { }
};

struct CAnimFilterOf2Groups : CAnimFilterBase {    // vtable 0x01267578
    CCharString m_groupA;                  // +0x4
    CCharString m_groupB;                  // +0x8
    CAnimFilterOf2Groups(const CCharString& groupA, const CCharString& groupB)
        : m_groupA()
        , m_groupB()
    {
        SetGroups(groupA, groupB);
    }
    void SetGroups(const CCharString& groupA, const CCharString& groupB);   // 0x0073E040
};

namespace NCreatureMode {

struct CModeCrouch : CModeBase {
    CCharString m_animName;                // +0x1c
    CAnimFilterOf2Groups m_filter;         // +0x20
    virtual void v00();
    CModeCrouch(CThing* thing);
};

CModeCrouch::CModeCrouch(CThing* thing)
    : CModeBase(thing)
    , m_animName("CROUCH")                 // literal @ 0x01258F14 (masked)
    , m_filter("CROUCH_GROUP", "")         // literals @ 0x01267B28 / 0x0122D70E (masked)
{
}

}