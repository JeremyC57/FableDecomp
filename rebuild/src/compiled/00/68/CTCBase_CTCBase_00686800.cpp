// CTCBase::CTCBase(void* owner) -- CBase, the owner at +4, two cleared flag bytes at +8 / +9, this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00686800, length 33 bytes.
// Callees: CBase::CBase @ 0x0099A2F0. Vtable 0x0125BE0C = CTCBase (rtti-vtables.tsv). Shape: the owner store and the two byte stores (`xor al,al`) precede the vtable
// store; `this` is returned. The chunk label "CEngineSubPrimitive2DClockSprite" is propagated.
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};

struct CTCBase : CBase {                           // vtable 0x0125BE0C
    void* m_pOwner;                                // +0x04
    bool  m_active;                                // +0x08
    bool  m_dirty;                                 // +0x09
    virtual void v00();
    CTCBase(void* owner);
};

CTCBase::CTCBase(void* owner)
    : CBase()
    , m_pOwner(owner)
    , m_active(false), m_dirty(false)
{
}