// NUISystem::CList::AreThereAnyDeletions() -- true when any child of the pointer vector at +0x164 reports a pending deletion (the state returned by its virtual slot 63 (+0xfc) has a non-zero first word); false otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005362F2, length 76 bytes.
// Leaf: AreThereAnyDeletions
// Shape: the size-optimised module (shared tail), the index compared with `!=` against `size()` (so the entry test is `test eax,0FFFFFFFCh`), `ret`.
#pragma optimize("s", on)
struct SDeletionState_005362f2 {
    long m_pending;                                // +0x00
};
struct CComponent_005362f2 {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual SDeletionState_005362f2* GetDeletionState();   // slot 63 (+0xfc)
};
struct CRetailVector_005362f2 {
    CComponent_005362f2** m_first;
    CComponent_005362f2** m_last;
    CComponent_005362f2** m_end;
    unsigned long size() const { return m_last - m_first; }
};

namespace NUISystem {
struct CList {
    unsigned char        _pad00[0x164];
    CRetailVector_005362f2 m_children;                // +0x164
    bool AreThereAnyDeletions();
};
}

bool NUISystem::CList::AreThereAnyDeletions()
{
    for (unsigned long i = 0; i != m_children.size(); ++i) {
        if (m_children.m_first[i]->GetDeletionState()->m_pending != 0)
            return true;
    }
    return false;
}
