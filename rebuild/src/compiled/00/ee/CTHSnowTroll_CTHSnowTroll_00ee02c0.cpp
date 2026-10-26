// CTH_SnowTroll::CTH_SnowTroll(CV_TravellingHeroesScript* parent, CQ_SunnyvaleMasterData* masterData, const CScriptThing& me) -- entity script: entity-script base over the parent's interface and Me, the parent pointer, two empty script things (Entrance, Exit), the master data last
// Leaf: ??0CTH_SnowTroll
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ee02c0, length 102 bytes (ret 0Ch; vtable 0x012F42B0). Catalog label
// "CTCLightningOrb::CTCLightningOrb" is propagated: the vtable store is an entity script's (base CEntityScriptBase 0x012C3224, no RTTI
// locator), and it sits among the CTH_ (Travelling Heroes) scripts; the only PDB entity scripts of 52 bytes with CScriptThings at +0x1C/+0x28 are CEndGameBarrier and CTH_SnowTroll (0x00CDFEB0 is the former); only caller the factory @ 0x00EE0240.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, CBaseClass::CBaseClass @ 0x0099A2D0 (x2, inline
// CScriptThing() ctors storing vtable 0x01238C8C and zeroing PImp).
// Layout = Ego_r.pdb CTH_SnowTroll (52 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18,
// Entrance +0x1C, Exit +0x28.
// Shape: the landed CDarkwoodTrader constructor (0x00E01670): Game is a plain initialiser stored before the base vtable, the
// Me copy follows, the derived vtable precedes ParentClass, the two default CScriptThings, PMasterData a body assignment stored last.
class CBaseClass { public:                           // vtable 0x01231710
    virtual void v00();
    CBaseClass();                             // 0x0099A2D0
};
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptThing : public CBaseClass { public:            // vtable 0x01238C8C, 12 bytes (PImp_Data +4, PImp_Info +8)
    CCountedPointerWords PImp;                // +0x04
    virtual void v00();
    CScriptThing() { }
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
};
struct CScriptOwner {                         // the quest script that owns this entity script
    unsigned char _pad00[0x40];
    void*         m_pInterface;               // +0x40 CGameScriptInterfaceBase*
};
class CEntityScriptBase { public:                    // vtable 0x012C3224, 0x14 bytes
    virtual void v00();
    void*        Game;                        // +0x04
    CScriptThing Me;                          // +0x08
    CEntityScriptBase(const CScriptThing& thing, void* game) : Game(game), Me(thing) { }
};

class CTH_SnowTroll : public CEntityScriptBase {   // vtable 0x012F42B0
public:
    CScriptOwner* ParentClass;                // +0x14 CV_TravellingHeroesScript*
    void*         PMasterData;                // +0x18 CQ_SunnyvaleMasterData*
    CScriptThing  Entrance;                      // +0x1c
    CScriptThing  Exit;                      // +0x28
    virtual void v00();
    CTH_SnowTroll(CScriptOwner* parent, void* masterData, const CScriptThing& me);
};

CTH_SnowTroll::CTH_SnowTroll(CScriptOwner* parent, void* masterData, const CScriptThing& me)
    : CEntityScriptBase(me, parent->m_pInterface), ParentClass(parent)
{
    PMasterData = masterData;
}