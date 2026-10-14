// NUISystem::CList::OnActivate() -- fetch the list's owning component as a counted def pointer (virtual slot 108 (+0x1b0), returned through a hidden pointer into the frame); when that component's byte +0x15c is set, set this list's byte +0x1e8; the def pointer's destructor releases the component (reference count at +4, virtual slot 1 at zero)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005366FF, length 60 bytes.
// Leaf: OnActivate
// Shape: the size-optimised frame-pointer module, a 4-byte frame slot for the returned pointer, `this` in esi, `leave ; ret`.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CComponentOwner_005366ff {                  // reference-counted: count at +4, Delete in slot 1
    virtual void v000();
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +0x04
    unsigned char _pad08[0x154];
    bool          m_active;                        // +0x15c
};
struct CDefPointer_005366ff {                      // 4 bytes: the counted object
    CComponentOwner_005366ff* m_pObject;
    CDefPointer_005366ff() { }
    CDefPointer_005366ff(const CDefPointer_005366ff& other);
    ~CDefPointer_005366ff()
    {
        CComponentOwner_005366ff* object = m_pObject;
        unsigned long* count = &m_pObject->IVRefCount;
        --*count;
        if (*(volatile unsigned long*)count == 0)
            object->Delete();
    }
};

namespace NUISystem {
struct CList {
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
    virtual CDefPointer_005366ff GetOwner();       // slot 108 (+0x1b0)
    unsigned char _pad04[0x1e4];
    bool          m_ownerActive;                   // +0x1e8
    void OnActivate();
};
}

void NUISystem::CList::OnActivate()
{
    CDefPointer_005366ff owner = GetOwner();
    if (owner.m_pObject->m_active)
        m_ownerActive = true;
}
