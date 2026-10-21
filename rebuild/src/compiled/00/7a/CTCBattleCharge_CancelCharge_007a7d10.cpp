// CTCBattleCharge::CancelCharge() -- ends the battle charge for the owning creature without a hit (FinishMove(owner, false))
// Retail: Fable.exe @ 0x007A7D10, 12 bytes. Catalog label CWorldMap::PrepareForLoad is wrong: the body calls
// CTCBattleCharge::FinishMove(CThingCreatureBase*, bool) 0x007A7BF0 (landed) on its own `this` with the component's
// owning thing (CTCBase +0x04) and false; it sits right after FinishMove in the CTCBattleCharge TU and its only caller
// is CTCSpecialAbilities::CancelCurrentMutexAbility 0x0072B693 (the name CancelCharge is descriptive).
class CThing;
class CThingCreatureBase;
class CTCBattleCharge {
public:
    void*   __vftable;                                   // +0x00
    CThing* Thing;                                       // +0x04 CTCBase owner
    void FinishMove(CThingCreatureBase* creature, bool hitSomething);   // 0x007A7BF0
    void CancelCharge();
};

void CTCBattleCharge::CancelCharge()
{
    FinishMove((CThingCreatureBase*)Thing, false);
}