// CCodaListener::Commit() -- GameCODA listener: publish the listener's changed-parameter bits to the global commit mask; when the orientation changed, renormalise the front and top vectors and rebuild the frame: a top vector more than 0.05 off perpendicular to the front is forced perpendicular (top = right-orthogonal of right and front), else the right vector is rebuilt from front and top; then clear the update bits
// Leaf: ?Commit@CCodaListener@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40590, 192 bytes, thiscall, plain `ret`. Constants 0.05f (0x0125A15C), -0.05f (0x012B7B78).
// Identity: byte-identical to GameCODA 2.10 CODAApi.lib member .\Release\CodaListener.obj, ?Commit@CCodaListener@@QAEXXZ (192 bytes; relocations
//   ?m_ffCommit@CCodaListener@@0IA x2, ?Normalize@CCodaVector@@QAEMXZ x2, __real@3d4ccccd, __real@bd4ccccd, ?OrthogonalRight@CCodaVector@@QAEXABV1@0@Z).
//   Authored as C++ per EXTRA_RULES 2026-11-19 ("author first"); it matches as ordinary C++, so no library grade is claimed.
// Shape: the CODAApiDbg.lib object's Commit (.\CodaListener.cpp): m_ffCommit |= m_ffUpdate & 0x38000; on bit 0x20000 m_vFront.Normalize(),
//   m_vTop.Normalize(), CODA_ASSERT(m_vFront != m_vTop) (Dbg line 0x67, compiled out), dot = m_vFront.Dot(m_vTop); outside +-0.05 the Dbg
//   build prints "WARNING: Orientation vectors not ..." / "Front : %s" / "Top : %s" / "Top vector forced to perpendic..." (CodaDbgPrintf,
//   compiled out) and calls m_vTop.OrthogonalRight(m_vRight, m_vFront), else m_vRight.OrthogonalRight(m_vFront, m_vTop); m_ffUpdate = 0.
//   Release VC7.1 expands the first OrthogonalRight (header inline) and calls the COMDAT copy 0x00C40370 at the second at the default
//   inline budget, as retail does. The two release-only lines compiled out of the Dbg build (assert, printf) are not written.
// Callees: CCodaVector::Normalize 0x00C402E0 (result dropped, `fstp st(0)`), CCodaVector::OrthogonalRight 0x00C40370.
// Globals: CCodaListener::m_ffCommit 0x0143BC24.
// Header opt-in: FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT (engine/CCodaVector.h: Normalize declaration, inline Dot and OrthogonalRight with the
//   SDK Toolkit CCtkVector bodies, CtkVector.h, the same vendor's twin class; the Dbg OrthogonalRight 104 bytes loads in that order).
#define FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT
#include "engine/CCodaListener.h"

void CCodaListener::Commit()
{
    m_ffCommit |= (m_ffUpdate & 0x38000);
    if (m_ffUpdate & 0x20000) {
        m_vFront.Normalize();
        m_vTop.Normalize();
        float dot = m_vFront.Dot(m_vTop);
        if (dot > 0.05f || dot < -0.05f)
            m_vTop.OrthogonalRight(m_vRight, m_vFront);
        else
            m_vRight.OrthogonalRight(m_vFront, m_vTop);
    }
    m_ffUpdate = 0;
}