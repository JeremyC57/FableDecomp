// NUISystem::CMouseCursor::TurnToClosedHand() -- show the cursor's closed-hand graphic (graphic index taken from the cursor's def)
// Leaf: TurnToClosedHand
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055C860, 71 bytes (plain `ret`).
// Callees: own virtual slots 108 (+0x1b0, GetDef(), a CDefPointer returned through the hidden pointer into the `push ecx`
// slot) and 145 (+0x244, SetGraphicIndex(unsigned long)); the def's virtual slot 1 (+0x04, destroy at the last reference).
// Label: CMouseCursor vtable 0x0124C22C slot 150 (rtti-vtables.tsv); Ego_r.pdb has ?TurnToClosedHand@CMouseCursor@NUISystem@@UAEXXZ,
// ?TurnToHand@..@ and ?TurnToPointer@..@. Slot 151 (0x0055C5B0, the default graphic) is TurnToPointer: the scroll
// bar's OnUnhovered (0x0055A740) calls it when no scroll bar stays hovered, and its OnHovered (0x0055A5B0) calls slot 149,
// so slot 149 is TurnToHand and slot 150 (0x0055C860) TurnToClosedHand.
// Layout: the cursor def's closed-hand graphic index at def +0x12c (the hand one, slot 149, reads +0x4c) (descriptive name; the def class is not in the PDB dump).
// Shape: the def pointer temporary dies at the end of the statement; its inline destructor is the landed
// InitializeRumbleActions (0x0053DA30) def release through a CRefCounted base at +4 (DecRef, then GetRefCount), which gives
// retail's `add eax,4 ; dec esi ; mov edx,esi ; test edx,edx`.
struct CRefCounted {                             // reference count base at +4 of a def
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CMouseCursorDef {
    virtual void v00();
    virtual void Destroy();                      // slot 1
    CRefCounted   Ref;                           // +0x04
    unsigned char _pad08[0x4c - 8];
    unsigned long HandGraphicIndex;              // +0x4c
    unsigned char _pad50[0x12c - 0x50];
    unsigned long ClosedHandGraphicIndex;        // +0x12c
    void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Data;
    ~CDefPointer()
    {
        if (Data)
            Data->ReleaseTemporary();
    }
    T* operator->() const { return Data; }
};

namespace NUISystem {
class CMouseCursor {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual CDefPointer<CMouseCursorDef> GetDef();          // slot 108 (+0x1b0)
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116();
    virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140();
    virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void SetGraphicIndex(unsigned long index);     // slot 145 (+0x244)
    void TurnToClosedHand();
};

void CMouseCursor::TurnToClosedHand()
{
    SetGraphicIndex(GetDef()->ClosedHandGraphicIndex);
}
}