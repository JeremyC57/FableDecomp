# Batch `2026-10-14_linux_hand26` — delegated hand-authoring chunk 26 (CCreatureAction_SheatheItemToInventory_ForUnholyPower .. CCreatureAction_WhisperSommersaultBackward): 40 of 40 landed

Chunk 26 of the class-labelled 17–96-byte pool, authored by the session's single sub-agent (`$S/w22/BRIEF.md`) and
landed by the coordinator: every row MATCH/RELOCATION_MATCH with behaviour PASS under VC7.1 + Wine, no flags or pragmas
needed anywhere. Creature-action constructors over CCombatActionBase / CCreatureActionBase (zeroed CSoundPair, `GetSoundPair(owner, 0)`
with the null name converting through `CCharString(const char*, long = -1)`), Initialise / PerformAction / FinishAction bodies
(scoped CCharString names, GetPItem-fed virtual calls, world messages, the inlined HasTC bitset probe), Clone bodies, two
UnsheatheWeapons-family constructors, IsToHaveEmptyHands (`unsigned char` result).

Label corrections (vtable stores in rtti-vtables.tsv): 008c9620 "SheatheWeapons" -> CCreatureAction_BanditKingUnstuck ctor;
0097cf90 "SummonCreatures" -> CCreatureAction_VillagerAttackKnockdown (owner, target, bool) ctor; 008c5b30 "UnsheatheWeapons" ->
CCreatureAction_WaspQueenDie (owner, long) ctor (code 250); 00883f10 "TeleportOut::Clone" is a Dinkumware list node factory
(`CRetailList_00883f10::BuyNode`, CDisplacementMonochromeEffectColourEntry value); 004b99e0 "TentacleStrikeLoop::Clone" is an
STLport-style `list::insert(iterator, const T&)` (`CRetailList_004b99e0::Insert`, iterator returned through the hidden pointer);
00c33f20 "TrollWhackGroundBase::Initialise" is a free 3-argument `__stdcall` (`_global::SetRangeOnAcceptingObject`). Callee notes:
0x004C9840 is CThing::RemoveTC(type) (the "OnPropertyChanged" label is propagated), 0x007EA270 a `mov [this+0Ch],arg` setter.

Lessons (also in GOTCHAS, wave 24b): a byte stored before the derived vtable in a 0-code constructor is an inlined
`__declspec(novtable)` intermediate base; a C3DVector copied one register per dword needs a memberwise user copy constructor;
CIntelligentPointer is 8 bytes (vtable + item); a GetPItem() result evaluated before the argument pushes, or `&=`-ed directly, is a
named local; a conditional flag toggle fed to a setter reproduces only as the inline ternary argument; the inlined HasTC probe
is the raw bitset test in the `if` (`test ch,1`), not a bool accessor; two byte stores sharing one register = chained assignment;
STLport list insert reads `prev = pos->prev` into a named local before `node->next = pos`; `fild ; fmul 0.25 ; __ftol2` =
`(long)(m * 0.25f)`. Agent notes: `$S/w22/agent26/NOTES.md`.
