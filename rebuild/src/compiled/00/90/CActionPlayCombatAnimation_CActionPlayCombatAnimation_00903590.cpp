// NTCScriptedControl::CActionPlayCombatAnimation::CActionPlayCombatAnimation(const CCharString& animation, bool a, bool b, bool c, bool d, bool e) -- constructor: CBase(), the common action members zeroed, flag a at +0xd, own vtable, CCharString copy at +0x10, four flags at +0x14..+0x17
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903590, length 96 bytes.
// Callees: CBase::CBase @ 0x0099A2F0, CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Vtable 0x0128D728 = NTCScriptedControl::CActionPlayCombatAnimation (rtti-vtables.tsv). Shape:
// base constructor; the zero stores (+4 dword, +8/+9/+0xc bytes) share eax; the second argument
// (a byte) is stored at +0xd and the vtable store sinks to just before the string copy constructor
// (ecx = &m_animation, the source reference pushed); the last four byte arguments are loaded and
// stored in declaration order (ret 0x18 = six arguments).
struct CCharString {                         // the string copy runs out of line (0x0099EC30), so the copy constructor is declared only
    void* m_pData;
    CCharString(const CCharString& other);  // 0x0099EC30
};

struct CThing;

struct CBase {
    virtual void v00();
    CBase();                                // 0x0099A2F0
};

struct __declspec(novtable) CAction : CBase {   // NTCScriptedControl::CAction: abstract, its own vtable store is dead
    CThing* m_pThing;                       // +0x04
    bool    m_active;                       // +0x08
    bool    m_finished;                     // +0x09
    CAction() : CBase(), m_pThing(0), m_active(false), m_finished(false) { }
};

struct CActionPlayCombatAnimation : CAction { // vtable 0x0128D728
    bool        m_playing;                  // +0x0c
    bool        m_loop;                     // +0x0d
    CCharString m_animation;                // +0x10
    bool        m_interruptible;            // +0x14
    bool        m_blendIn;                  // +0x15
    bool        m_blendOut;                 // +0x16
    bool        m_holdLastFrame;            // +0x17
    virtual void v00();
    CActionPlayCombatAnimation(const CCharString& animation, bool loop, bool interruptible, bool blendIn, bool blendOut, bool holdLastFrame);
};

CActionPlayCombatAnimation::CActionPlayCombatAnimation(const CCharString& animation, bool loop, bool interruptible, bool blendIn, bool blendOut, bool holdLastFrame)
    : CAction()
    , m_playing(false)
    , m_loop(loop)
    , m_animation(animation)
    , m_interruptible(interruptible)
    , m_blendIn(blendIn)
    , m_blendOut(blendOut)
    , m_holdLastFrame(holdLastFrame)
{
}