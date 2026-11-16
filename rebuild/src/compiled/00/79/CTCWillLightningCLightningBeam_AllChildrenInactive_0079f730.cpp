// CTCWillLightning::CLightningBeam::AllChildrenInactive() -- true when every child beam is inactive (state 0) or finished (state 6)
// Leaf: ?AllChildrenInactive@CLightningBeam@CTCWillLightning@@QAE_NXZ
// Name-fix NF38: Ego_r.pdb ?AllChildrenInactive@CLightningBeam@CTCWillLightning@@QAE_NXZ (non-const; engine/CTCWillLightning.h
// FABLE_RETAIL_CTCWILLLIGHTNING_BEAM_FRAMEUPDATE declares it); the landed caller 0x007A1B20 binds it. Was the local stand-in
// CLightningBeam::AllBeamsInactive() const (the twin component method name).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members Count->RefCount, Destroy->DeleteFunc, Object->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079F730, 104 bytes (+8 int3). __fastcall, returns bool in al.
//         Caller 0x007A22AF; twin body CTCWillLightning::AllBeamsInactive 0x0079F7A0 walks the component's m_lbBeams.
// Callees: operator delete 0x00BFE9BC (the counted pointer's info block) and the info block's deleter (+4, object in ecx).
// Shape: `for (it = begin(); it != end(); ++it)` over list<CCountedPointer<CLightningBeam>> m_lChildren with each element
//        copied into a local counted pointer (add-ref null-tested, release + delete on zero on both exits); end() is
//        re-read from the member every iteration.
// Layout: CLightningBeam (PDB, 0x84): m_lsState +0x00, m_lChildren +0x4C (retail STLport list = one sentinel pointer).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCWILLLIGHTNING_CLASS
#define FABLE_RETAIL_CTCWILLLIGHTNING_BEAM_FRAMEUPDATE
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCWillLightning.h"

bool CTCWillLightning::CLightningBeam::AllChildrenInactive()
{
    for (std::list<CCountedPointer<CLightningBeam> >::iterator it = m_lChildren.begin(); it != m_lChildren.end(); ++it) {
        CCountedPointer<CLightningBeam> beam = *it;
        if (beam->m_lsState != 0 && beam->m_lsState != 6)
            return false;
    }
    return true;
}
