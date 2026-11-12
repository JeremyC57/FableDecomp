// CTCHealLife::SetAICast(long spellLevel) -- record the spell level an AI caster cast the heal at (SpellLevel +0x18)
// Leaf: ?SetAICast@CTCHealLife@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C3EA0: `mov eax,[esp+4] ; mov [ecx+18h],eax ; ret 4`.
// NF28 (2026-11-12, queue land1011ei): was landed as C_007c3ea0::M (catalog label CObservable::AddExclusiveObserver, propagated).
//   Identity: Ego_r.pdb 0x007C2CC0 / FableWin.pdb 0x02714670 ?SetAICast@CTCHealLife@@QAEXJ@Z; struct_layouts_egor CTCHealLife +0x18 is
//   long SpellLevel, the field the body stores; the landed caller CCreatureAction_CastWillHealLifeSpell::PerformAction 0x008B97E0 and
//   CCombatAction_WillHealInto::FinishAction 0x008C2AD0 pass the spell level to the AddTC("CTCHealLife") result.
class CTCHealLife {                                  // no shared header: the one member this row stores
public:
    void SetAICast(long spellLevel);
private:
    unsigned char _pad[0x18];                        // vptr, ParentThing, flags, PDef, TotalHealthAccum, LoopingSound (Ego_r)
    long          SpellLevel;                        // +0x18
};

void CTCHealLife::SetAICast(long spellLevel)
{
    SpellLevel = spellLevel;
}
