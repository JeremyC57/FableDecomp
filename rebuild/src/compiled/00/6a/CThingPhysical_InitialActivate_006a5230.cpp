// CThingPhysical::InitialActivate() -- the base thing's initial activation, then the thing joins the thing manager's physical list
// Leaf: ?InitialActivate@CThingPhysical@@MAEXXZ
// Naming wave 28 (family d6, DECISIONS_W27 D6): identity from three sources -- slot 9 of vtable 0x01244E84 (RTTI COL .?AVCThingPhysical@@,
//   the vtable ~CThingPhysical 0x006A5D50 stores), the slot CThing's vtable 0x01239ABC fills with CThing::InitialActivate 0x004C9690; the
//   body (the base call, then AddToPhysicalList(*this)); Ego_r.pdb 0x00684320 / FableWin.pdb 0x01F20640 ?InitialActivate@CThingPhysical@@MAEXXZ.
//   Was a free __fastcall AddCombinationBlend (catalog C3DAnimationBlendState::AddCombinationBlend) calling @helper_6a5230@4 and
//   ?Add@BlendMgr@@ stand-ins.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A5230, 22 bytes, thiscall virtual, `ret`: push esi; mov esi,ecx; call 0x004C9690;
//   mov ecx,[0x013B8A1C]; push esi; call 0x0051DF60; pop esi; ret.
// Callees: CThing::InitialActivate 0x004C9690 (Ego_r / FableWin ?InitialActivate@CThing@@UAEXXZ, called qualified),
//   CThingManager::AddToPhysicalList(CThingPhysical&) 0x0051DF60 (an empty `ret 4` in the release build; chunk 695).
// Global: CThing::PCurrentThingManager 0x013B8A1C (private static, read through the protected inline accessor GetThingManager(), as the
//   landed ~CThingGameObject 0x00838A00).
class CThingPhysical;
class CThingManager {
public:
    void AddToPhysicalList(CThingPhysical& thing);               // 0x0051DF60
};
class CThing {                                                   // vtable 0x01239ABC (.?AVCThing@@)
public:
    virtual ~CThing();                                           // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void InitialActivate();                              // slot 9: 0x004C9690
protected:
    CThingManager& GetThingManager() { return *PCurrentThingManager; }   // Ego_r / FableWin ?GetThingManager@CThing@@IAEAAVCThingManager@@XZ (inline)
private:
    static CThingManager* PCurrentThingManager;                  // 0x013B8A1C ?PCurrentThingManager@CThing@@0PAVCThingManager@@A
};
class CThingPhysical : public CThing {                           // vtable 0x01244E84 (.?AVCThingPhysical@@)
protected:
    virtual void InitialActivate();                              // slot 9
};

void CThingPhysical::InitialActivate()
{
    CThing::InitialActivate();
    GetThingManager().AddToPhysicalList(*this);
}
