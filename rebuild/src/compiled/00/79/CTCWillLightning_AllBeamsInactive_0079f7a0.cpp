// CTCWillLightning::AllBeamsInactive() const -- true when every beam of the spell is inactive (state 0) or finished (state 6)
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members Count->RefCount, Destroy->DeleteFunc, Object->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079F7A0, 104 bytes (+8 int3). __fastcall, returns bool in al. Caller 0x007A2944.
//         Twin body: CTCWillLightning::CLightningBeam::AllBeamsInactive 0x0079F730 (walks a beam's m_lChildren at +0x4C).
// Callees: operator delete 0x00BFE9BC (the counted pointer's info block) and the info block's deleter (+4, object in ecx).
// Shape: `for (it = begin(); it != end(); ++it)` over list<CCountedPointer<CLightningBeam>> m_lbBeams, each element copied
//        into a local counted pointer (add-ref null-tested; release + delete on zero on both exits); end() re-read each pass.
// Layout: retail CTCWillLightning (vtable 0x0126C724) keeps m_lbBeams at +0x68 (PDB +0x6C: a member below +0x60 is
//         4 bytes smaller in retail, see m_fTime +0x60 in Deactivate 0x007A0310; the STLport list is one sentinel pointer, PDB 12 bytes); CLightningBeam m_lsState +0x00.
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
};
class CTCWillLightning { public:
    void* __vftable;                             // +0x00
    unsigned char _pad_0x04[0x68 - 0x04];
    CRetailList<CCountedPointer<CLightningBeam> > m_lbBeams;     // +0x68
    bool AllBeamsInactive() const;
};
bool CTCWillLightning::AllBeamsInactive() const
{
    for (CRetailList<CCountedPointer<CLightningBeam> >::Node* it = m_lbBeams.begin(); it != m_lbBeams.end(); it = it->Next) {
        CCountedPointer<CLightningBeam> beam = it->Value;
        if (beam->m_lsState != 0 && beam->m_lsState != 6)
            return false;
    }
    return true;
}