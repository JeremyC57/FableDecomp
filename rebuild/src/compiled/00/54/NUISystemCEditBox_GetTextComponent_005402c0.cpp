// NUISystem::CEditBox::GetTextComponent() -- the first child component (8-byte slots at +0xb0) whose virtual type query (slot 65) answers 6 (text), else null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005402C0, length 91 bytes.
// Callees: virtual slot 65 (+0x104) of each component. Shape: the VC6 vector {first, last} at +0xb0 holds 8-byte entries whose first word is the component;
// the count is `(last - first) / 8` re-derived from the members every iteration (`sar eax,3`), the empty test is `test eax,0FFFFFFF8h` on the raw byte
// difference; the loop head is aligned with `lea ecx,[ecx]`; the hit returns the entry re-read through the index. No PDB layout for CEditBox; member
// names are descriptive.
namespace NUISystem {

class CComponent { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064();
    virtual long GetComponentType();               // slot 65 (+0x104): 6 = text
};

struct SChildEntry {
    CComponent* m_pComponent;                      // +0x00
    long        m_tag;                             // +0x04
};

class CEditBox { public:
    unsigned char _pad[0xb0];
    SChildEntry*  m_pChildrenFirst;                // +0xb0 VC6 vector<SChildEntry>::first
    SChildEntry*  m_pChildrenLast;                 // +0xb4
    unsigned long GetChildCount() const { return m_pChildrenLast - m_pChildrenFirst; }
    CComponent* GetTextComponent();
};

CComponent* CEditBox::GetTextComponent()
{
    for (unsigned long i = 0; i != GetChildCount(); ++i) {
        if (m_pChildrenFirst[i].m_pComponent->GetComponentType() == 6)
            return m_pChildrenFirst[i].m_pComponent;
    }
    return 0;
}

}