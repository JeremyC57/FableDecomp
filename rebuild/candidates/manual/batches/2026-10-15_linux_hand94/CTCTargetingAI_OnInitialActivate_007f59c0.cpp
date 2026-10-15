// CTCTargetingAI::OnInitialActivate() -- resolve the owner def's sub-def into the component's def pointer
// Retail 0x007F59C0, 44 bytes (+4 int3). CTCTargetingAI vtable 0x012411FC slot 6 (16 of the CTC* slot-6
// entries are OnInitialActivate; the manifest label "CTCInventoryItem::OnCreate" is propagated).
// Shape: the owner thing's def pointer (CThing +0x70) is copied into a local CDefPointer (add-ref under
// a null test), CDefClassBase::GetSubDef<T> 0x007EA960 is called on it with the address of the
// component's def pointer (+0x30, `add ecx,30h ; push ecx`), and the copy is released with its own
// null test (`if (p && --p->refCount == 0) p->Destroy()`, the virtual destroy tail-jumped).
// engine headers: CTCTargetingAI's PDB layout only names members from +0x38 (the base is opaque), so
// the base members are modelled locally with descriptive names.

// Leaf: OnInitialActivate

struct CSubDef;

struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long refCount;                                 // +0x04
    void AddRef() { ++refCount; }
    void Release() { if (--refCount == 0) Destroy(); }
    bool GetSubDef(CSubDef** out);                 // 0x007EA960 (GetSubDef<CAICreatureWillPowerIndicatorDef>)
};

struct CDefClassPointer {                          // CDefPointer<const CThingDef>
    CDefClassBase* Object;
    CDefClassPointer(const CDefClassPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefClassPointer() { if (Object) Object->Release(); }
    CDefClassBase* operator->() const { return Object; }
};

struct CThing {
    unsigned char _pad_00[0x70];
    CDefClassPointer PDef;                         // +0x70
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
};

struct CTCTargetingAI : CTCBase {
    unsigned char _pad_0c[0x24];
    CSubDef* PSubDef;                              // +0x30
    void OnInitialActivate();
};

void CTCTargetingAI::OnInitialActivate()
{
    CDefClassPointer def(ParentThing->PDef);
    def->GetSubDef(&PSubDef);
}
