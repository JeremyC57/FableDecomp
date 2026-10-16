// CEndGameBarrier::CEndGameBarrier(CPersonalScript_GlobalThingsScript* parent, CQ_SunnyvaleMasterData* masterData, const CScriptThing& me) -- entity script: entity-script base over the parent's interface and Me, the parent pointer, two empty script things (ForceField1, ForceField2), the master data last
// Leaf: ??0CEndGameBarrier
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00cdfeb0, length 102 bytes (ret 0Ch; vtable 0x012C3434). Catalog label
// "CTCLightningOrb::CTCLightningOrb" is propagated: the vtable store is an entity script's (base CEntityScriptBase 0x012C3224, no RTTI
// locator), and it sits right before CEndGameBarrier::OnPredicateFail @ 0x00CDFF50; its only caller is the 0x34-byte factory @ 0x00CDFE30.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, CBaseClass::CBaseClass @ 0x0099A2D0 (x2, inline
// CScriptThing() ctors storing vtable 0x01238C8C and zeroing PImp).
// Layout = Ego_r.pdb CEndGameBarrier (52 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18,
// ForceField1 +0x1C, ForceField2 +0x28.
// Shape: the landed CDarkwoodTrader constructor (0x00E01670): Game is a plain initialiser stored before the base vtable, the
// Me copy follows, the derived vtable precedes ParentClass, the two default CScriptThings, PMasterData a body assignment stored last.
struct CBaseClass {                           // vtable 0x01231710
    virtual void v00();
    CBaseClass();                             // 0x0099A2D0
};
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
struct CScriptThing : CBaseClass {            // vtable 0x01238C8C, 12 bytes (PImp_Data +4, PImp_Info +8)
    CCountedPointerWords PImp;                // +0x04
    virtual void v00();
    CScriptThing() { }
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
};
struct CScriptOwner {                         // the quest script that owns this entity script
    unsigned char _pad00[0x40];
    void*         m_pInterface;               // +0x40 CGameScriptInterfaceBase*
};
struct CEntityScriptBase {                    // vtable 0x012C3224, 0x14 bytes
    virtual void v00();
    void*        Game;                        // +0x04
    CScriptThing Me;                          // +0x08
    CEntityScriptBase(const CScriptThing& thing, void* game) : Game(game), Me(thing) { }
};

class CEndGameBarrier : public CEntityScriptBase {   // vtable 0x012C3434
public:
    CScriptOwner* ParentClass;                // +0x14 CPersonalScript_GlobalThingsScript*
    void*         PMasterData;                // +0x18 CQ_SunnyvaleMasterData*
    CScriptThing  ForceField1;                      // +0x1c
    CScriptThing  ForceField2;                      // +0x28
    virtual void v00();
    CEndGameBarrier(CScriptOwner* parent, void* masterData, const CScriptThing& me);
};

CEndGameBarrier::CEndGameBarrier(CScriptOwner* parent, void* masterData, const CScriptThing& me)
    : CEntityScriptBase(me, parent->m_pInterface), ParentClass(parent)
{
    PMasterData = masterData;
}
