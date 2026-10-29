// NTCScriptedControl::CActionDropGenericBox::CActionDropGenericBox() -- default constructor.
// Leaf: ??0CActionDropGenericBox@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009036A0, length 39 bytes.
// Manifest label "CAIStateGroup_GazeAtHome" is propagated; the vtable stored, 0x0128D7BC, is
// NTCScriptedControl::CActionDropGenericBox (rtti-vtables.tsv). Callee: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0.
//
// Shape: base ctor, then the initialiser list in declaration order: a null pointer at +4, three false
// flags at +8/+9/+0x10 and a -1 index at +0xc; the retail scheduler shares one zero register for the
// four zero stores and sinks the vtable store below them (the -1 store follows it). Member names are
// descriptive (no PDB header for this scripted-control action).
class CThing;

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                    // 0x0099A2F0
};

struct CActionDropGenericBox : CBaseClassNonCopyable {
    CThing* m_pBox;                             // +0x04
    bool m_dropped;                             // +0x08
    bool m_started;                             // +0x09
    long m_frame;                               // +0x0c = -1
    bool m_finished;                            // +0x10
    CActionDropGenericBox();
};

CActionDropGenericBox::CActionDropGenericBox()
    : CBaseClassNonCopyable()
    , m_pBox(0)
    , m_dropped(false)
    , m_started(false)
    , m_frame(-1)
    , m_finished(false)
{
}