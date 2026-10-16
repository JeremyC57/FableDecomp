// CTCWillLightning::Deactivate() -- start the deactivation phase and deactivate every beam toward the casting player
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A0310, 146 bytes (+14 int3). __fastcall, `ret`. Callers 0x007A03F3, 0x007A272C,
//         0x007A273A, 0x007A2B3F.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10; GetPItem 0x00A01B50; SetPItem 0x00A01B90;
//          CTCWillLightning::CLightningBeam::Deactivate(CIntelligentPointer<CThingCreatureBase>) 0x0079FE90 (by value, callee
//          destroys); operator delete 0x00BFE9BC. CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8.
// Shape: in states 3/2/1 the time base takes the current time (integer move), the state becomes 5 (deactivating), and each
//        beam (a local CCountedPointer copy, Data kept in ebp) gets Deactivate(m_cbpPlayer) -- the by-value intelligent
//        pointer is copy-built in the argument area (base ctor, vptr, SetPItem(other.GetPItem())).
// Layout: retail CTCWillLightning (vtable 0x0126C724): m_fTime +0x60, m_fTimeBase +0x64, m_lbBeams +0x68 (STLport list,
//         4 bytes), m_lsState +0x6C, m_cbpPlayer +0x70 (PDB +0x64/+0x68/+0x6C/+0x78/+0x7C).
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
struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                               // +4
    CBaseIntelligentPointer();                   // 0x00A01B10
    CThing* GetPItem() const;                    // 0x00A01B50
    void SetPItem(CThing* item);                 // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CLightningBeam {
    long m_lsState;                              // +0x00 ELightningState
    void Deactivate(CIntelligentPointer player); // 0x0079FE90
};
struct CTCWillLightning {
    void* __vftable;                             // +0x00
    unsigned char _pad_0x04[0x60 - 0x04];
    float m_fTime;                               // +0x60
    float m_fTimeBase;                           // +0x64
    CRetailList<CCountedPointer<CLightningBeam> > m_lbBeams;     // +0x68
    long m_lsState;                              // +0x6c ELightningState
    CIntelligentPointer m_cbpPlayer;             // +0x70
    void Deactivate();
};
void CTCWillLightning::Deactivate()
{
    if (m_lsState == 3 || m_lsState == 2 || m_lsState == 1) {
        m_fTimeBase = m_fTime;
        m_lsState = 5;
        for (CRetailList<CCountedPointer<CLightningBeam> >::Node* it = m_lbBeams.begin(); it != m_lbBeams.end(); it = it->Next) {
            CCountedPointer<CLightningBeam> beam = it->Value;
            beam->Deactivate(m_cbpPlayer);
        }
    }
}