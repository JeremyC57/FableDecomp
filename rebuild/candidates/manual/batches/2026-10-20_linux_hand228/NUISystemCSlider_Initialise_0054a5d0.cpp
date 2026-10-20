// NUISystem::CSlider::Initialise() -- the CSprite initialisation, then hand the right and left arrow components to the slider's parent: add each as a child (through a temporary counted pointer), re-parent it and initialise it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054A5D0, length 274 bytes (vtable 0x01248D04 slot 43).
// Callees: CSprite::Initialise @ 0x0041A8D0 (base call), CCountedPointer<CComponent>::CCountedPointer(CComponent*) @ 0x00429C15,
// operator delete (MSVCR71 import thunk 0x00BFE9BC); component virtual slots 43 Initialise (+0xAC), 51 (+0xCC, re-parent,
// descriptive SetParent), 52 (+0xD0, descriptive GetParent) and 59 AddChild(const CCountedPointer<CComponent>&) (+0xEC,
// as in the landed CTCInventoryStats::CreateTitle 0x005CCF0E).
// Layout: retail NUISystem::CSlider = Ego_r.pdb minus 0x30 (landed ctor 0x0054C050): PLeftArrow +0x1d8, PRightArrow +0x1dc.
// Shape: two identical blocks (right arrow first); the counted pointer lives in the 8-byte frame and its destructor is the
// inline {count, __fastcall destroy, object} release of 0x004291DE without the dead clears; the block pointer is re-read from
// its address-taken frame word after every write through it. `m->SetParent(GetParent())` fetches m's vtable before the
// inner call (plain nested call).
void __fastcall Destroy_00428b10(void* object);   // the per-type deleter stored in the block (never called directly here)

struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};

template<class T> struct CCountedPointer {
    T*           m_pObject;                // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer(T* object);            // 0x00429C15 (T = NUISystem::CComponent)
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
    }
};

namespace NUISystem {
struct CComponent {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void Initialise();                                          // slot 43 (+0xAC)
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void SetParent(CComponent* parent);                         // slot 51 (+0xCC) descriptive
    virtual CComponent* GetParent();                                    // slot 52 (+0xD0) descriptive
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void AddChild(const CCountedPointer<CComponent>& child);    // slot 59 (+0xEC)
};
}

struct CSprite : NUISystem::CComponent {
    unsigned char _pad004[0x1d4];
    void Initialise();                     // 0x0041A8D0
};

namespace NUISystem {
struct CSlider : CSprite {
    CComponent* PLeftArrow;                // +0x1d8
    CComponent* PRightArrow;               // +0x1dc
    virtual void Initialise();
};

void CSlider::Initialise()
{
    CSprite::Initialise();
    if (PRightArrow) {
        CCountedPointer<CComponent> child(PRightArrow);
        GetParent()->AddChild(child);
        PRightArrow->SetParent(GetParent());
        PRightArrow->Initialise();
    }
    if (PLeftArrow) {
        CCountedPointer<CComponent> child(PLeftArrow);
        GetParent()->AddChild(child);
        PLeftArrow->SetParent(GetParent());
        PLeftArrow->Initialise();
    }
}
}
