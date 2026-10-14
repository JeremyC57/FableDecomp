// NUISystem::CList::AllScrollingChildrenDeleting() -- true when every scrolling child (the 8-byte {component, index} pairs of the vector at +0xb0) answers true to its virtual slot 49 (+0xc4); false at the first that does not
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052CBF0, length 96 bytes.
// Leaf: AllScrollingChildrenDeleting
// Shape: `this` in esi, the index in edi compared unsigned against `size()` (the pointer difference shifted by 3) each iteration, `ret`.
struct CComponent_0052cbf0 {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048();
    virtual bool IsDeleting();                     // slot 49 (+0xc4)
};
struct SChildEntry_0052cbf0 {                      // 8 bytes
    CComponent_0052cbf0* m_pComponent;
    long                 m_index;
};
struct CRetailVector_0052cbf0 {
    SChildEntry_0052cbf0* m_first;
    SChildEntry_0052cbf0* m_last;
    SChildEntry_0052cbf0* m_end;
    unsigned long size() const { return m_last - m_first; }
};

namespace NUISystem {
struct CList {
    unsigned char          _pad00[0xb0];
    CRetailVector_0052cbf0 m_scrollingChildren;    // +0xb0
    bool AllScrollingChildrenDeleting();
};
}

bool NUISystem::CList::AllScrollingChildrenDeleting()
{
    for (unsigned long i = 0; i < m_scrollingChildren.size(); ++i) {
        if (!m_scrollingChildren.m_first[i].m_pComponent->IsDeleting())
            return false;
    }
    return true;
}