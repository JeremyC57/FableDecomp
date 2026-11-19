# Landing hand4d b5 (2026-11-19): NCreatureMode::CModeThunderLightningStorm::UpdateDamage (0x008DD7E0)

**Result:** strict vland --land gives RELOCATION_MATCH + PASS; no flags.

**New header:** engine/CThunderBattleDef.h (CThunderBattleDef : CParentDefClassBase, Ego_d / Ego_r members, LightningDamage at retail +0x44). Only a generated
copy existed before, under engine/_quarantine/. It has no other users.

**Source:** FableWin statement order with Ego_d locals.
- The filter is Bind2<IsNot, In2DRange>, not the draft's sphere.
- VC6 for-scope idiom (FableWin `xor edx,edx ; je` at 0x0285A2AC).
- Dead Ego_d locals are kept.
- GETSUBDEF_NOINLINE as `// Inline-budget:`.

**Fitted choices** (`// Inferred:` lines, the 0x006E89B0 precedent):
- (a) HasTC(TCI_ELECTROCUTED_EFFECT) rather than FableWin's GetTC: retail tests only the presence bit.
- (b) The hit direction is built per component from two named position references (descriptive local names caster_pos / creature_pos). Every shared operator- /
  C3DVector spelling of FableWin's expression stayed at ndiff 19.

**NAME_FIXES:**
- 0x008E3C50 is GetAllThingsInRoughArea<CThingCreatureBase, vector, CFilter_Bind2<IsNot, In2DRange>> (Ego_r 0x00825D60, FableWin 0x0286D1E0), replacing a
  propagated catalog label.
- 0x006C4060's local-model namespace label is flagged only, since its std::__find spelling has a single source.
