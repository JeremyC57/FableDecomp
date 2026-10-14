// NParticleEngine::CPSCSpline::CPSCSpline() -- CPSCSplineBase constructor, then five members cleared in the body (+0x3c, +0x34, +0x39, +0x38, +0x3a) before the vtable store
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1B350, length 48 bytes.
// Callees: NParticleEngine::CPSCSplineBase::CPSCSplineBase @ 0x00B21530. Vtable 0x012A0934 (NParticleEngine::CPSCSpline, rtti-vtables.tsv).
// Manifest label "CGameJoystickManager::CGameJoystickManager" is propagated; the vtable names the class.
// Shape: the stores are not in offset order (+0x3c before +0x34), so they are body assignments in source order rather than an
// initialiser list (which VC emits in declaration order); all of them precede the vtable store.
// Leaf: ??0CPSCSpline
namespace NParticleEngine {
struct CPSCSplineBase {
    virtual ~CPSCSplineBase();
    unsigned char _body[0x30];                      // +0x04 .. +0x34
    CPSCSplineBase();                               // 0x00B21530
};

struct CPSCSpline : CPSCSplineBase {
    long m_pointCount;                              // +0x34
    bool m_looped;                                  // +0x38
    bool m_dirty;                                   // +0x39
    bool m_closed;                                  // +0x3a
    unsigned char _pad3b;                           // +0x3b
    long m_pCurrentPoint;                           // +0x3c
    CPSCSpline();
};

CPSCSpline::CPSCSpline()
{
    m_pCurrentPoint = 0;
    m_pointCount = 0;
    m_dirty = false;
    m_looped = false;
    m_closed = false;
}
}
