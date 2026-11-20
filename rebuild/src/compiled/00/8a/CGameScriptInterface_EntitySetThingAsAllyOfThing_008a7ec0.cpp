class CScriptThing;

class CGameScriptInterface
{
private:
    void ImpSetEntityAsAllyOfThing(const CScriptThing& entity, const CScriptThing& otherThing, bool isAlly) const;   // private const: Ego_r 0x008DA6D0 / FableWin 0x02A8ED70 ?ImpSetEntityAsAllyOfThing@CGameScriptInterface@@ABEXABVCScriptThing@@0_N@Z (Naming NF194, q11668)
public:
    void EntitySetThingAsAllyOfThing(const CScriptThing& entity, const CScriptThing& otherThing) const;
};

void CGameScriptInterface::EntitySetThingAsAllyOfThing(const CScriptThing& entity, const CScriptThing& otherThing) const
{
    ImpSetEntityAsAllyOfThing(entity, otherThing, true);
}