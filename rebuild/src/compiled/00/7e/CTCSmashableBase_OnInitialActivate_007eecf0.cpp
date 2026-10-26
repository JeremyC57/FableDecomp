// CTCSmashableBase::OnInitialActivate() -- persist the parent thing's game state when its smashable def says it is smashable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EECF0, length 48 bytes.
// Callees: CThing::SetAsGameStatePersisted(bool) @ 0x004C9C10; the def's virtual Destroy (slot 1) on the last release.
// Layout: Ego_r.pdb CTCSmashableBase (0x18 bytes): LastWorldFrameHit +0x0c, Smashed +0x10, Definition +0x14
// (CDefPointer<CSmashableDef const>); CSmashableDef: refcount +0x04, Smashable +0x25.
// Shape: `GetDef()->Smashable` copies the def pointer into a by-value temporary (null-tested add-ref) and releases it
// after the flag read. The release must be a member of the DEF (`if (--m_refCount == 0) Destroy();`) called from the
// pointer's destructor: written inline in the destructor the decrement is scheduled above the flag load.
class CThing { public:
    void SetAsGameStatePersisted_004c9c10(bool persisted);        // 0x004C9C10
};

class CSmashableDef { public:
    virtual void v00();
    virtual void Destroy();                              // slot 1
    long          m_refCount;                            // +0x04
    void Release() { if (--m_refCount == 0) Destroy(); }
    unsigned char _pad[0x25 - 8];
    bool          Smashable;                             // +0x25
};

struct CDefPointer {
    CSmashableDef* m_pDef;
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) ++m_pDef->m_refCount; }
    ~CDefPointer() { m_pDef->Release(); }
    CSmashableDef* operator->() const { return m_pDef; }
};

class CTCBase { public:
    void*   __vftable;                                   // +0x00
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
};

class CTCSmashableBase : public CTCBase { public:
    long        LastWorldFrameHit;                       // +0x0c
    bool        Smashed;                                 // +0x10
    CDefPointer Definition;                              // +0x14
    CDefPointer GetDef() const { return Definition; }
    void OnInitialActivate();
};

void CTCSmashableBase::OnInitialActivate()
{
    bool smashable = GetDef()->Smashable;
    if (smashable)
        ParentThing->SetAsGameStatePersisted_004c9c10(true);
}