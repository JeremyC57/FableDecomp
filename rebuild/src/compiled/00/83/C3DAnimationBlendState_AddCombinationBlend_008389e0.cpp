// Leaf: ?InitialActivate@CThingGameObject@@MAEXXZ
// agentNF15: respelled from the stand-in free function `AddCombinationBlend(C3DAnimationBlendState*)` (__fastcall) to the catalog /
//   Ego_r.pdb / FableWin.pdb protected virtual CThingGameObject::InitialActivate() (?InitialActivate@CThingGameObject@@MAEXXZ,
//   slot 9 of RTTI vtable 0x01244F7C .?AVCThingGameObject@@; ghidra_out labels). Callees: CThingPhysical::InitialActivate()
//   0x006A5230 (?InitialActivate@CThingPhysical@@MAEXXZ, landed own row; called non-virtually as the base step) and
//   CThingManager::AddToGameObjectList(CThingGameObject&) 0x0051DF70 (Ego_r.pdb ?AddToGameObjectList@CThingManager@@QAEXAAVCThingGameObject@@@Z;
//   labels: empty `ret 4` whose only caller is this function) on the private static CThing::PCurrentThingManager 0x013B8A1C
//   (?PCurrentThingManager@CThing@@0PAVCThingManager@@A, engine/CThing.h; was the stand-in global `g_theMgr`).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008389E0, 22 bytes: `push esi ; mov esi,ecx ; call 006A5230 ; mov ecx,[013B8A1Ch] ;
//   push esi ; call 0051DF70 ; pop esi ; ret`.
class CThingGameObject;
class CThingManager { public:
    void AddToGameObjectList(CThingGameObject& thing);   // 0x0051DF70
};
class CThing {
private:
    static CThingManager* PCurrentThingManager;      // 0x013B8A1C ?PCurrentThingManager@CThing@@0PAVCThingManager@@A (private static)
protected:
    virtual void InitialActivate();                  // slot 9 in retail (CThing::InitialActivate); partial view
public:
    static CThingManager* __fastcall GetPCurrentThingManager() { return PCurrentThingManager; }   // Ego_r / FableWin (inline)
};
class CThingPhysical : public CThing {
protected:
    virtual void InitialActivate();                  // 0x006A5230
};
class CThingGameObject : public CThingPhysical {
protected:
    virtual void InitialActivate();
};
void CThingGameObject::InitialActivate()
{
    CThingPhysical::InitialActivate();
    GetPCurrentThingManager()->AddToGameObjectList(*this);
}
