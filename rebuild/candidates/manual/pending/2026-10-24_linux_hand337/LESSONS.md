# Chunk 337 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- CTCEntityEvents::GetEvent / HasEvent: model the newest-first walk with STLport's real `reverse_iterator` (`*it` copies
  the base `_List_iterator` and decrements the copy; `++` decrements the base; `!=` compares `base()`), not the
  hand-folded `current->_M_prev`. It fixes the "node in edi / event in ebx / `this` in ebp, filter re-read from
  [esp+18h]" register choice of 8 rows (0x005ED5A0, 0x008AC7A0, 0x00953000, 0x00953090, 0x0095C680, 0x0095C720,
  0x0094F890, 0x008D49B0) and keeps 13 of the 14 rows the folded form matched; only 0x007FCE10 still wants the folded form. Agent 336's
  parked 0x0095D920 / 0x0096BC20 / 0x0094E530 ("lane gap: register tie") grade RELOCATION_MATCH with only this iterator
  swapped into their differ/ sources (probe/o336_<addr>.cpp here) -- land them with chunk 336.
- GetEvent filters take `const CEventHitBase*` (Ego_r.pdb `unary_function<const CEventHitBase*, bool>`); a GetEvent
  instance's event class is the slot-1 GetType constant (0x1B CEventHitBySpecialAbility, 0x1D CEventHitThing, 0x27
  CEventShotStruck); 0x00810F70 is the shot test, 0x00810F80 the melee test.
- Retail CHitParameters (STLport map/vector 12 bytes): KnockDown +0x1a, Decapitate +0x1b, Flourish +0x1d, Blocked +0x26,
  PThingHitting +0x50, PWeapon +0x58 (Ego_r.pdb +0x58 / +0x60); CEventHitBase vtable slot 5 is IsFlourish.
- CThingSearchTools::GetPBestThing / GetPBestLeafThing: `CFilter_Bind2/Bind4::operator()` and an inline
  `std::binary_compose::operator()` need `__forceinline` or VC7.1 calls them out of line (004ac2c0, 009650c0).
- Size-optimised GetPBestThing (0x0065F6DA, IsIncludedInThingScans called): `CThing* thing = *it;` for the scan call
  and the candidate, but `IsThingOfType<T>(*it)` on a fresh `*it`; handing `thing` to the type test makes VC7.1 treat it
  as dereferenced and drop retail's `test ebx,ebx`.
- An `unsigned short` 1-bit field read through an inline `bool IsDead()` gives `mov al ; shr al,5 ; test al,1`
  (0x0079DC20; the direct field read folds to `test byte ptr [eax+40h],20h`).
- A by-value CIntelligentPointer argument of a thiscall functor (`ret 8`) is destroyed by the callee through
  `lea ecx,[esp+8] ; call ~CBaseIntelligentPointer`; the bool result is taken into bl (`sete bl`) before it (0x0065F152).

Review at landing:
- 0x00738A80 (parked) is the only GetEvent row neither iterator form matches (esi/edi swap) -- try the next idea in NOTES.
- 0x00A98760 / 0x00A987B0 (parked) are `??_G` of CVertexShaderInputStatic / Animated: the landed non-deleting dtor
  0x00A98640 uses a non-virtual CBaseClass dtor, which cannot give a `??_G`; with a virtual base dtor VC7.1 leaves
  `??1CVertexShaderInput` out of line.
- 0x0065FBE1 (parked): `lea` vs `add` for the building pointer -- one small modelling step left.
- 0x0048CBD0's thing type is modelled `const CThingVillage` (unproven; the twin of 0x0048CB50 for the second
  CPlayer::GetPNearestTCVillage copy 0x004886F0).
- Local models next to existing engine headers: none (CTCEntityEvents.h / CThingSearchTools.h are included; the event,
  filter, CThing and intelligent-pointer models are local because no header exists or the retail layout differs).
- Already-landed sources: the landed CTCEntityEvents GetEvent siblings (0x008D47A0 etc.) use the folded iterator and take
  the event by reference; both still match, but the STLport form + pointer filters is the more general model.
