# 0x006C0970 naming evidence

The selected RTM object emits `?CreateSignPost@CTCBuyableHouse@@AAEPAVCThing@@ABVCCharString@@_N@Z`. Ego_r donor 0x006F8C10 and FableWin donor 0x024AE8F0 carries this exact decorated spelling.
The Ego_d module record identifies `CTCBuyableHouse::CreateSignPost` in `tc_buyable_house.obj`. CTCBuyableHouse::CreateSignPost (debug length 523).
Retail directly calls this member at 0x006C0F2A in landed 0x006C0F00 CTCBuyableHouse::CreateMainSignPost.
Retail directly calls this member at 0x006C0F7E in landed 0x006C0F70 CTCBuyableHouse::CreateDressingSignPost.
The imported draft used local models. The submitted port uses the engine headers. See the operand review for binding status.

Ego_d scoped locals include new_post (CThingObject*), player (CThingPlayerCreature*), post_pos and rhs. Native object creation, AddTC, the +0x78 physics virtual and the controlled-creature call corroborate these types. Ego_r names the leaf private with a CThing* return. The shared headers supply each member and the non-const player manager overload.
