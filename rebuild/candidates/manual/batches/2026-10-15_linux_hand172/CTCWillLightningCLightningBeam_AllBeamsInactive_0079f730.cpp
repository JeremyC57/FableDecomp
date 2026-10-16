// CTCWillLightning::CLightningBeam::AllBeamsInactive() const -- true when every child beam is inactive (state 0) or finished (state 6)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079F730, 104 bytes (+8 int3). __thiscall, returns bool in al.
//         Caller 0x007A22AF; twin body CTCWillLightning::AllBeamsInactive 0x0079F7A0 walks the component's m_lbBeams.
// Callees: operator delete 0x00BFE9BC (the counted pointer's info block) and the info block's deleter (+4, object in ecx).
// Shape: `for (it = begin(); it != end(); ++it)` over list<CCountedPointer<CLightningBeam>> m_lChildren with each element
//        copied into a local counted pointer (add-ref null-tested, release + delete on zero on both exits); end() is
//        re-read from the member every iteration.
// Layout: CLightningBeam (PDB, 0x84): m_lsState +0x00, m_lChildren +0x4C (retail STLport list = one sentinel pointer).
struct CCPPointerInfo {                          // counted-pointer info block
    long Count;                                  // +0
    void (__fastcall* Destroy)(void*);           // +4 per-type deleter, object in ecx
    void* Object;                                // +8
};
void operator delete(void* p);                   // 0x00BFE9BC
template <class T> struct CCountedPointer {
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
