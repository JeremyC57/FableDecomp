// CGameScriptInterface::EntityGetOpinionAttitudeToPlayer(const CScriptThing& entity) const -- script call: the opinion reaction manager's attitude of the entity's thing to the player, 0 without a thing
// Leaf: ?EntityGetOpinionAttitudeToPlayer@CGameScriptInterface@@UBE?AW4EOpinionAttitudeType@@ABVCScriptThing@@@Z
// Retail: Fable.exe @ 0x00890990, 38 bytes, thiscall `ret 4` (the found path tail-jumps with the thing in the argument slot).
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 608 holds this body; slot 608 is
//   ?EntityGetOpinionAttitudeToPlayer@CGameScriptInterface@@UBE?AW4EOpinionAttitudeType@@ABVCScriptThing@@@Z.
// Body: t = entity.GetThing() (CScriptThing slot 11, +0x2c); if t: World (+0x04)->GetOpinionReactionManager() (inline,
//   the pointer at CWorld +0x74; FableWin.pdb ?GetOpinionReactionManager@CWorld@@QAEAAVCOpinionReactionManager@@XZ)
//   .GetThingAttitudeToPlayer(*t) (0x00701840, FableWin.pdb
//   ?GetThingAttitudeToPlayer@COpinionReactionManager@@QBE?AW4EOpinionAttitudeType@@ABVCThing@@@Z; the manifest names
//   the same method).
enum EOpinionAttitudeType { };
class CThing;
class COpinionReactionManager {
public:
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing& thing) const;   // 0x00701840
};
class CWorld {
public:
    unsigned char _pad00[0x74];
    COpinionReactionManager* POpinionReactionManager;    // +0x74 (member name descriptive)
    COpinionReactionManager& GetOpinionReactionManager() { return *POpinionReactionManager; }
};
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                    // slot 11 (+0x2c)
};
class CGameScriptInterface {
public:
    virtual EOpinionAttitudeType EntityGetOpinionAttitudeToPlayer(const CScriptThing& entity) const;
    CWorld* World;                                       // +0x04
};
EOpinionAttitudeType CGameScriptInterface::EntityGetOpinionAttitudeToPlayer(const CScriptThing& entity) const
{
    CThing* t = entity.GetThing();
    if (t)
        return World->GetOpinionReactionManager().GetThingAttitudeToPlayer(*t);
    return (EOpinionAttitudeType)0;
}