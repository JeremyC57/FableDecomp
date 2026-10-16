// CTCInventory::OnCreate() -- registers the inventory screen with the GUI manager, resolves its inventory def from the thing's CInventory sub-def, runs the base OnCreate and leaves quick-access assignment off
// Leaf: OnCreate
// Grade: DIFFER(137v137) -- only the zeroed sub-def CDefPointer's `and [ebp-4],0` lands before the def-chain load (retail after `mov edi,[eax+70h]`): the CDefPointer zero-store scheduling lane gap. Tried a named thingDef copy in a block, a by-value GetDef() temporary, a __forceinline GetInventorySubDef() helper (moves the zero but re-allocates registers)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00582944, 137 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), no EH frame.
// Callees: the player GUI global @ 0x013B8790's manager getter @ 0x00437CD0 (+0x32C) and its RegisterScreen
//          @ 0x0066EB11 (argument = this as the screen base at +0x130, null-safe `neg ; sbb ; and`),
//          CDefClassBase::GetSubDef<CInventorySubDef>(CDefPointer*) @ 0x00485815, NInventory::CTCInventoryBase::SetPDef
//          (long) @ 0x005BDB4D, NInventory::CTCInventoryBase::OnCreate() @ 0x005C0121 (qualified base call),
//          SetQuickAccessAssign(bool) @ 0x00581A14.
// Shape: the screen argument is evaluated before the manager getter (arguments first); the thing's def pointer is a
// by-value GetDef() temporary (a named copy in a block compiles the same);
// by-value CDefPointer copy (add-ref and release both null-tested) living in edi just for the GetSubDef call; the
// zeroed sub-def pointer local is released by hand at the end without a null test. In this size TU the CDefPointer
// copy ctor and dtor only inline as `__forceinline`.
// Layout: CTCInventory screen base subobject +0x130, quick-access-assign-pending flag +0x1CE; retail CThing def
// pointer object +0x70; sub-def +0x78 the inventory def's global index.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CRefCounted {
    virtual void v00();
    virtual void Destroy();                          // slot 1 (+0x04)
    long RefCount;                                   // +0x04
};

struct CInventorySubDef : CRefCounted {
    unsigned char _pad_0x08[0x70];
    long          InventoryDefIndex;                 // +0x78
};

template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};

template <class T> struct CRawDefPointer {
    T* Object;
    CRawDefPointer() : Object(0) {}
};

struct CThingDef : CRefCounted {
    bool GetSubDef(CRawDefPointer<CInventorySubDef>* out) const;   // 0x00485815
};

struct CThing {
    unsigned char           _pad_0x00[0x70];
    CDefPointer<CThingDef>  PDef;                    // +0x70
    __forceinline CDefPointer<CThingDef> GetDef() const { return PDef; }
};

struct CScreenBase {
    virtual void v00();
};

struct CScreenManager {
    void RegisterScreen(CScreenBase* screen);        // 0x0066EB11
};

struct CPlayerGui {
    CScreenManager* GetScreenManager();              // 0x00437CD0 (+0x32C)
};

extern CPlayerGui* g_pPlayerGui;                     // 0x013B8790

struct CTCInventoryBase {
    virtual void v00();                              // +0x00 vtable
    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x128];
    void SetPDef(long globalDefIndex);               // 0x005BDB4D
    void OnCreate();                                 // 0x005C0121
};

struct CTCInventory : CTCInventoryBase, CScreenBase {    // CScreenBase at +0x130
    unsigned char _pad_0x134[0x9A];
    bool          QuickAccessAssignPending;          // +0x1CE

    void SetQuickAccessAssign(bool assign);          // 0x00581A14
    void OnCreate();
};

void CTCInventory::OnCreate()
{
    g_pPlayerGui->GetScreenManager()->RegisterScreen(this);
    CRawDefPointer<CInventorySubDef> pdef;
    ParentThing->GetDef()->GetSubDef(&pdef);
    CInventorySubDef* def = pdef.Object;
    SetPDef(def->InventoryDefIndex);
    CTCInventoryBase::OnCreate();
    SetQuickAccessAssign(false);
    QuickAccessAssignPending = false;
    if (--def->RefCount == 0)
        def->Destroy();
}
