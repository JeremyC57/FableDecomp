// CTCWillLightning::AllBeamsInactive() const -- true when every beam of the spell is inactive (state 0) or finished (state 6)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079F7A0, 104 bytes (+8 int3). __fastcall, returns bool in al. Caller 0x007A2944.
//         Twin body: CTCWillLightning::CLightningBeam::AllBeamsInactive 0x0079F730 (walks a beam's m_lChildren at +0x4C).
// Callees: operator delete 0x00BFE9BC (the counted pointer's info block) and the info block's deleter (+4, object in ecx).
// Shape: `for (it = begin(); it != end(); ++it)` over list<CCountedPointer<CLightningBeam>> m_lbBeams, each element copied
//        into a local counted pointer (add-ref null-tested; release + delete on zero on both exits); end() re-read each pass.
// Layout: retail CTCWillLightning (vtable 0x0126C724) keeps m_lbBeams at +0x68 (PDB +0x6C: a member below +0x60 is
//         4 bytes smaller in retail, see m_fTime +0x60 in Deactivate 0x007A0310; the STLport list is one sentinel pointer, PDB 12 bytes); CLightningBeam m_lsState +0x00.
class CCPPointerInfo { public:                          // counted-pointer info block
    long Count;                                  // +0
    void (__fastcall* Destroy)(void*);           // +4 per-type deleter, object in ecx
    void* Object;                                // +8
};
void operator delete(void* p);                   // 0x00BFE9BC
template <class T> class CCountedPointer { public:
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->Count; }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->Count == 0) {
                Info->Destroy(Info->Object);
                delete Info;
            }
        }
    }
    T* operator->() const { return Data; }
};
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