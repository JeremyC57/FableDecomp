// CLightningBranch::CLightningBranch(void* target) -- two intelligent pointers (vtable 0x01238C6C): the first (+0) bound to the target through SetPItem (0x00A01B90), the second (+8) empty, and the dword at +0x10 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F3AD0, length 64 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, SetPItem @ 0x00A01B90. Shape: `this` in esi, the second pointer in edi, `ret 4`.
struct CBaseIntelligentPointer {                   // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                                 // +0x04
    CBaseIntelligentPointer();                     // 0x00A01B10
    void SetPItem(void* item);                     // 0x00A01B90
};

struct CIntelligentPointer_01238c6c : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer_01238c6c() : CBaseIntelligentPointer() { }
    CIntelligentPointer_01238c6c(void* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct CLightningBranch {
    CIntelligentPointer_01238c6c m_target;         // +0x00
    CIntelligentPointer_01238c6c m_other;          // +0x08
    long                         Timer;             // +0x10
    CLightningBranch(void* target);
};

CLightningBranch::CLightningBranch(void* target)
    : m_target(target), m_other(), Timer(0)
{
}