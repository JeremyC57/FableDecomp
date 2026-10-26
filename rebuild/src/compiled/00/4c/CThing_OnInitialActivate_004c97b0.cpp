// CThing::OnInitialActivate() -- tell the quest manager, advance the initialisation stage, activate every component, and join the render list if drawable
// Leaf: ?OnInitialActivate@CThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C97B0, 137 bytes (+7 int3). __fastcall virtual (CThing vtable 0x01239ABC slot 10), `ret`.
// Callees: CQuestManager::OnActivateEntity(CThing*) 0x004AFA60 on the global 0x013B89FC; the component vector's copy ctor
//          0x004CC290 (catalog "Vector_CopyFrom"); each component's CTCBase::OnInitialActivate (slot 6, +0x18); CThing slot 27
//          (+0x6C) DrawIsDrawable(); CThingManager::AddToRenderList(CThing*) 0x0051DF80 on the global 0x013B8A1C; free
//          0x00BFEA14 (the copy's storage).
// Shape: InitialisationStage (uchar:3 at +0x94) ++ (`inc cl ; xor ; and 7 ; xor`); the Components vector is COPIED into a local
//        (components may be added/removed during activation) and walked with a plain begin()/end() loop; a component flagged
//        ToBeRemoved (CTCBase +0x08) is skipped; the local's inline dtor frees the storage. The copy ctor is a template
//        instantiation in retail's TU, so VC7.1 sees that the local does not escape: begin stays in ebp across the virtual
//        calls (reused for the free) and end in ebx. The same-TU stand-in below sets the members itself to reproduce that.
// Layout: retail CThing = Ego_r.pdb - 0x10: Components +0x44 (12-byte STLport vector of {type, CTCBase*}), flag byte +0x94.
extern "C" void __cdecl free(void* p);           // 0x00BFEA14
class CThing;
class CTCBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void OnInitialActivate();            // slot 6 (+0x18)
    CThing* ParentThing;                         // +0x04
    bool ToBeRemoved;                            // +0x08
    bool IsInGlobalUpdateTCs;                    // +0x09
};
class CQuestManager { public:
    void OnActivateEntity(CThing* thing);        // 0x004AFA60
};
extern CQuestManager* g_pQuestManager;           // 0x013B89FC
class CThingManager { public:
    void AddToRenderList(CThing* thing);         // 0x0051DF80
};
extern CThingManager* g_pThingManager;           // 0x013B8A1C
struct CInterfaceEntry { long type; CTCBase* component; };
struct CInterfaceVector {                        // CVectorMap: STLport vector<pair<ETCInterfaceType, CTCBase*> > + comparator
    CInterfaceEntry* _M_start;
    CInterfaceEntry* _M_finish;
    CInterfaceEntry* _M_end_of_storage;
    unsigned long Compare;                       // +0x0c CKeyPairCompareLess (padded)
    CInterfaceVector(const CInterfaceVector& other);   // 0x004CC290
    ~CInterfaceVector() { if (_M_start) free(_M_start); }
    CInterfaceEntry* begin() const { return _M_start; }
    CInterfaceEntry* end() const { return _M_finish; }
};
CInterfaceEntry* CopyEntries_004cc290(const CInterfaceEntry* first, const CInterfaceEntry* last);   // allocate + copy
// Stand-in for the vector copy ctor @ 0x004CC290 (a template instantiation in retail's TU): it sets the members itself,
// so the local copy's address does not escape.
#pragma auto_inline(off)
__declspec(noinline) CInterfaceVector::CInterfaceVector(const CInterfaceVector& other)
{
    _M_start = CopyEntries_004cc290(other._M_start, other._M_finish);
    _M_finish = _M_start + (other._M_finish - other._M_start);
    _M_end_of_storage = _M_finish;
    Compare = other.Compare;
}
#pragma auto_inline(on)
class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void OnInitialActivate();            // slot 10
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual bool DrawIsDrawable() const;         // slot 27 (+0x6c)
    unsigned char _pad04[0x44 - 0x04];
    CInterfaceVector Components;                 // +0x44
    unsigned char _pad54[0x94 - 0x54];
    unsigned char InitialisationStage : 3;       // +0x94
};
void CThing::OnInitialActivate()
{
    g_pQuestManager->OnActivateEntity(this);
    InitialisationStage++;
    CInterfaceVector components(Components);
    for (CInterfaceEntry* it = components.begin(); it != components.end(); ++it) {
        CTCBase* component = it->component;
        if (!component->ToBeRemoved)
            component->OnInitialActivate();
    }
    if (DrawIsDrawable())
        g_pThingManager->AddToRenderList(this);
}