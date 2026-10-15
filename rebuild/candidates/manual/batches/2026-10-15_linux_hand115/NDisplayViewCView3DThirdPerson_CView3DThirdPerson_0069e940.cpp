// NDisplayView::CView3DThirdPerson::CView3DThirdPerson() -- inlined CView3DBase constructor (its vtable, cleared target/flags, the display engine's camera manager at +4), then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069E940, length 64 bytes.
// Callees: CViewBase::GetDisplayEngine @ 0x00640030 (a `__thiscall` member that reads the global engine
// record 0x013B86A0 -> +0x28; ecx is still `this` at the call so no reload is emitted).
// Vtables: 0x0125C7E0 = NDisplayView::CView3DBase, 0x0125C7F8 = NDisplayView::CView3DThirdPerson
// (rtti-vtables.tsv). Shape: the base constructor is inline -- its vtable store, the zero register into
// the word at +0x18 and the bytes at +0x1c, +0x44, +0x45, +0x7c, +0x7d (init list, declaration order),
// then the body `m_pCameraManager = GetDisplayEngine()->m_2c` -- and the derived class adds only its vtable.
// No engine header for the NDisplayView classes; the layout is modelled from the stores.
namespace NDisplayView {

struct CDisplayEngine {
    unsigned char _pad00[0x2c];
    void*         m_pCameraManager;           // +0x2c
};

struct CView3DBase {                          // vtable 0x0125C7E0
    virtual void v00();
    void*         m_pCameraManager;           // +0x04
    unsigned char _pad08[0x10];               // +0x08..+0x17
    void*         m_pTarget;                  // +0x18
    unsigned char m_targetLocked;             // +0x1c
    unsigned char _pad1d[0x27];               // +0x1d..+0x43
    unsigned char m_flag44;                   // +0x44
    unsigned char m_flag45;                   // +0x45
    unsigned char _pad46[0x36];               // +0x46..+0x7b
    unsigned char m_flag7c;                   // +0x7c
    unsigned char m_flag7d;                   // +0x7d
    CDisplayEngine* GetDisplayEngine();       // 0x00640030
    CView3DBase()
        : m_pTarget(0), m_targetLocked(0), m_flag44(0), m_flag45(0), m_flag7c(0), m_flag7d(0)
    {
        m_pCameraManager = GetDisplayEngine()->m_pCameraManager;
    }
};

struct CView3DThirdPerson : CView3DBase {     // vtable 0x0125C7F8
    virtual void v00();
    CView3DThirdPerson();
};

CView3DThirdPerson::CView3DThirdPerson()
    : CView3DBase()
{
}

}
