# Chunk 794 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Same-TU `__declspec(noinline)` bodies for header-inline callees that retail keeps out of line (CDefClassBase::GetSubDef<T>,
  CSpecialEffectsDef::GetEffectFromString, CDefinitionManager::GetDef<T>) change the CALLER: VC7.1 then knows the CDefPointer locals do
  not escape, so their release becomes retail's `dec [ecx+4] ; jne` (no `Object = 0` store) and the dead argument slots get reused for
  them (0x0079ACC0: 613 -> 156 diff lines; 0x007C4430: 492 -> 326).
- An inline setter taking `const CCharString&` (FableWin CTCEffectOnDie::SetSoundEffect) makes VC push a string temporary by `lea`
  (0x0079B36E); a direct `member = CCharString("...")` pushes the constructor's eax. When retail pushes `lea` after building a temporary,
  look for a FableWin inline setter.
- A one-line `(X*X+Z*Z+Y*Y)` loads Y, Z, X in 0x0079ACC0's Normalise, but `(X*X+Y*Y+Z*Z)` loads Z, Y, X (retail). The order VC7.1 emits for
  a commutative one-line sum depends on the TU: test both spellings (opt-in FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ).
- An inlined in-place matrix multiply (CMatrix3x4::PostMultiply) always evaluates the memory*memory product first, whatever the source term
  order (k1..k3, k3..k1, k2,k3,k1 all gave the same code in 0x007DE710); retail evaluates the register-operand products first there.
- Caching `GetTCGraphicAppearance()` in a local before two GetDummyObject calls is what retail does (`mov esi,[ecx+64h]` once, 0x007DE7B6):
  repeated `thing.GetTCGraphicAppearance()->...` reloads +0x64 before each call.
- RELOCATION_MATCH is not enough: check every relocation's symbol against the retail target. In 0x007039A0 a const PeekTC where retail
  used a non-const GetTC graded RELOCATION_MATCH but bound ?LowerBound@...QBE (0x004365B0) at a site retail calls 0x0040F020. A tiny
  objdump -dr + oracle pairing script (bin/relpair794.py / bin/lbsites794.py) finds such sites in seconds.
- VC7.1's per-site inline decision for a small STL call (vector<float>::push_back) follows the static frequency estimate: one extra `if`
  around the site outlines it (probe/hl_p3.cpp). When one of two identical calls is outlined in retail, the source has lower frequency
  there -- look for an extra condition or loop nesting, not for a budget (adding another inlined push_back earlier changed nothing).
- A same-TU out-of-line accessor (CTCHeroOpinionDeedLog::GetDeedFromHandle 0x006C58F0) is auto-inlined by /O2 into its TU's callers; the tell
  is a by-value argument copied into a local before its address is passed (0x006C6066 `mov [esp+40h],eax`).
- `return informed;` vs `return false;` with `informed` still false compiles the same (VC constant-propagates); retail's `mov al,bl` early
  exit comes from register allocation, not from the spelling.

Review at landing:
- 0x007039A0 needed 13 grades; its src changed after the first RELOCATION_MATCH (constness of the BARREL-branch village-member lookup) --
  regrade it with the relocation-identity check.
- 0x0079ACC0 keeps local models (CThingAICreatureInit, CSpecialAbilitiesSummonSpellDef, CTCSummonableCreature, CTCEnemy,
  CTCRegionFollower, CTCSpecialAbilities, CTCDParticleEmitter, CTCEffectOnDie, CTCSummonSpell) where no engine header exists; it defines
  three same-TU noinline bodies (GetSubDef<CSpecialEffectsDef>, GetEffectFromString -- copied from the landed 0x00667450 shape -- and
  GetDef<CBrainDef>).
- 0x007039A0 / 0x008304A0 / 0x0079ACC0 rely on the overlay headers listed in hdr/README.md.
- Catalog label 0x00568850 (Vector_PushElement / PushBack_dword) is a COMDAT-folded 4-byte vector push_back used as
  std::vector<float>::push_back by 0x007C4430.
