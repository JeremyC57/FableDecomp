// CTCWillLightning::CLightningBeam::AllBeamsInactive() const -- true when every child beam is inactive (state 0) or finished (state 6)
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members Count->RefCount, Destroy->DeleteFunc, Object->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079F730, 104 bytes (+8 int3). __fastcall, returns bool in al.
//         Caller 0x007A22AF; twin body CTCWillLightning::AllBeamsInactive 0x0079F7A0 walks the component's m_lbBeams.
// Callees: operator delete 0x00BFE9BC (the counted pointer's info block) and the info block's deleter (+4, object in ecx).
// Shape: `for (it = begin(); it != end(); ++it)` over list<CCountedPointer<CLightningBeam>> m_lChildren with each element
//        copied into a local counted pointer (add-ref null-tested, release + delete on zero on both exits); end() is
//        re-read from the member every iteration.
// Layout: CLightningBeam (PDB, 0x84): m_lsState +0x00, m_lChildren +0x4C (retail STLport list = one sentinel pointer).
#include "engine/CCountedPointer.h"
void operator delete(void* p);                   // 0x00BFE9BC
template <class T> struct CRetailList {          // STLport list: the sentinel node pointer only (4 bytes; PDB 12)
    struct Node { Node* Next; Node* Prev; T Value; };
    Node* Head;
    Node* begin() const { return Head->Next; }
    Node* end() const { return Head; }
};
struct CLightningBeam {
    long m_lsState;                              // +0x00 ELightningState
    unsigned char _pad_0x04[0x4c - 0x04];
    CRetailList<CCountedPointer<CLightningBeam> > m_lChildren;   // +0x4c
    bool AllBeamsInactive() const;
};
bool CLightningBeam::AllBeamsInactive() const
{
    for (CRetailList<CCountedPointer<CLightningBeam> >::Node* it = m_lChildren.begin(); it != m_lChildren.end(); it = it->Next) {
        CCountedPointer<CLightningBeam> beam = it->Value;
        if (beam->m_lsState != 0 && beam->m_lsState != 6)
            return false;
    }
    return true;
}