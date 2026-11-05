// CTargetPoint::CTargetPoint() -- build the target-thing intelligent pointer and mark the point unindexed / never rejected
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Pragma: #pragma optimize("s", on) -- the `or dword ptr [..],-1` stores mark a size-optimised TU.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D7A76, length 26 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (PThing at +0; the inlined
// CIntelligentPointer<CThing> constructor then stores its vtable 0x01238C6C).
// Layout from Ego_r.pdb (CTargetPoint, 28 bytes, not polymorphic): PThing +0x00 (CIntelligentPointer<CThing>,
// 8 bytes), Index +0x08, TargetingPosition +0x0c (C3DVector, no constructor body), FrameRejected +0x18.
// Shape: the two `-1` initialisers are plain initialisers, so VC7.1 schedules their `or [..],-1` stores before
// the member's vtable store (wave 24i rule); the C3DVector is left uninitialised.
#pragma optimize("s", on)
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CThing;


#include "engine/C3DVector.h"

struct CTargetPoint {
    CIntelligentPointer<CThing> PThing;           // +0x00
    long                       Index;            // +0x08
    C3DVector                  TargetingPosition;// +0x0c
    long                       FrameRejected;    // +0x18
    CTargetPoint();
};

CTargetPoint::CTargetPoint()
    : PThing(), Index(-1), FrameRejected(-1)
{
}