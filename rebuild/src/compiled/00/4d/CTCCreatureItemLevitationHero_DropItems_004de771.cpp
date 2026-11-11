// CTCCreatureItemLevitationHero::DropItems() -- does nothing
// Leaf: ?DropItems@CTCCreatureItemLevitationHero@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE771, thiscall, `ret`.
// Identity: slot 29 of CTCCreatureItemLevitationHero's vtable 0x0124167C (defined here: the slot holds this body in 1 vtable(s), all of CTCCreatureItemLevitationHero or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts DropItems(void) at this slot; Ego_r/FableWin name set: ?DropItems@CTCCreatureItemLevitationHero@@MAEXXZ; `ret` matches the signature.
class CTCCreatureItemLevitationHero {
public:
protected:
    virtual void DropItems();
};
void CTCCreatureItemLevitationHero::DropItems()
{
}
