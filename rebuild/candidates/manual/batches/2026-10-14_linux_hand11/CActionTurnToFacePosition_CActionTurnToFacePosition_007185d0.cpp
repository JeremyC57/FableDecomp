// NTCScriptedControl::CActionTurnToFacePosition::CActionTurnToFacePosition(const C3DVector& position, long flags) -- constructor: CBase(), the common action members zeroed, own vtable, the position copied to +0xc, the flags at +0x18
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007185D0, length 64 bytes.
// Callees: CBase::CBase @ 0x0099A2F0.
// Vtable 0x01265534 = NTCScriptedControl::CActionTurnToFacePosition (rtti-vtables.tsv). Shape: base
// constructor, zero stores (+4, +8, +9) sharing eax, the vtable store, then the three dwords of the
// by-reference C3DVector copied through ecx/edx/eax interleaved with the flags load (a plain POD
// member copy) and the flags store at +0x18 (ret 8).
struct C3DVector {                          // copied memberwise (one register per dword, see GOTCHAS wave 19); modelled as dwords because retail moves them with mov, not through the FPU
    long x;
    long y;
    long z;
    C3DVector(const C3DVector& other) : x(other.x), y(other.y), z(other.z) { }
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

struct CActionTurnToFacePosition : CAction {  // vtable 0x01265534
    C3DVector m_position;                   // +0x0c
    long      m_flags;                      // +0x18
    virtual void v00();
    CActionTurnToFacePosition(const C3DVector& position, long flags);
};

CActionTurnToFacePosition::CActionTurnToFacePosition(const C3DVector& position, long flags)
    : CAction()
    , m_position(position)
    , m_flags(flags)
{
}
