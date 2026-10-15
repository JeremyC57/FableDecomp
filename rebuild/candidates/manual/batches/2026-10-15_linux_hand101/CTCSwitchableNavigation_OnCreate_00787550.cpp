// CTCSwitchableNavigation::OnCreate() -- look up the navigation map index at the parent thing's position
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787550, length 35 bytes; vtable 0x0123C02C (CTCSwitchableNavigation)
// slot 4, the CTCBase OnCreate slot (Ego_r ?OnCreate@CTCSwitchableNavigation@@UAEXXZ).
// Callees: the CTCBase getter @ 0x00686D30 (returns the game component's member at +0x28; the manifest's
// "CViewBase::GetDisplayEngine" is propagated, the object is named here by its use), CThing::GetPosition() const
// @ 0x004C73D0 (returns &[+0x60]->+0x0c; manifest "CVertexBufferWin32::DoSizeof", propagated), and that object's
// virtual slot 16 (+0x40) taking the position and returning the map index.
// Layout: Ego_r.pdb CTCSwitchableNavigation (0x10 bytes): MapIndex +0x0c.
// Shape: the map owner is a named local fetched first (its vtable is loaded into ebx before the position call).
struct C3DVector;

struct CNavMapSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetNavMapIndex(const C3DVector& position); // slot 16 (+0x40)
};

struct CThing {
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};

struct CTCSwitchableNavigation {
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    long    MapIndex;                                       // +0x0c
    CNavMapSource* GetNavMapSource();                       // 0x00686D30 (CTCBase)
    void OnCreate();
};

void CTCSwitchableNavigation::OnCreate()
{
    CNavMapSource* source = GetNavMapSource();
    MapIndex = source->GetNavMapIndex(ParentThing->GetPosition());
}
