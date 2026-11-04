// CLifetimeRef_00c26060::~CLifetimeRef_00c26060() -- drops the referenced object's 16-bit lifetime counter (@ 0x00C1EA40) and nulls the reference
// Vendor naming V2 (plan phase 1, one address, one name): 0x00C1EA40 ?Release@CSoundSample@@QAEXXZ (was ?DecrementLifetimer@CLifetimedObject@@QAEXXZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C26060, length 18 bytes.
// Callees: CSoundSample::DecrementLifetimer @ 0x00C1EA40 (`dec word ptr [this+0x1e]`).
// The manifest label (C3DAnimationUtil::C3DAnimationUtil) and the callee's label
// (CParticle::IncrementLifetimer) are propagated names: the callee decrements a word at +0x1e
// of the object held at +0, and this body then clears the holder -- a destructor / release of a
// one-pointer reference holder, named by what it does.
struct CSoundSample {
    unsigned char _pad[0x1e];
    unsigned short m_lifetime;             // +0x1e
    void Release();             // 0x00C1EA40
};

struct CLifetimeRef_00c26060 {
    CSoundSample* m_pObject;           // +0
    ~CLifetimeRef_00c26060();
};

CLifetimeRef_00c26060::~CLifetimeRef_00c26060()
{
    m_pObject->Release();
    m_pObject = 0;
}