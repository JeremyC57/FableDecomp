# Chunk 279 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Word-local HasTC: `unsigned long word = m_presentBits[t >> 5]; return ((word >> (t & 31)) & 1) != 0;` bound as `const bool& present = HasTC(t)` in the GetInterface helper gives retail's `shr ; and cl,1 ; mov [esp+x],cl` byte-homed test with no byte-load narrowing. It also fixed a this/&member register swap (0x009378F0, 0x00929820, 0x009236C0). The masked `word & (1 << n)` form is the `test dword` / `test byte` shape.
- GFFloatRandom 0x004978A0 is `__fastcall (float range, unsigned long* seed)`: the seed is in ecx and the range on the stack, so GetPWorldSeed() is called BEFORE the range push. Declaring it `(seed, range)` swaps that order (0x00985440).
- Copying both map-id pairs into locals before comparing (`CMapCoords a = threat->MapID; CMapCoords b = me->MapID; if (a.X == b.X && a.Y == b.Y)`) gives retail's four-register load-then-compare. A member IsOnSameMap compares against memory (0x00984F60).
- A sub-scope `{ CCharString text(...); failed = Speak(text, 1) != 2; } if (failed) return false;` gives the `lea` + ctor before the layer push and the `setne ; dtor ; test` order. A temporary `Speak(CCharString(...), 1)` pushes the layer first and uses the ctor's eax (0x00985440).
- A brain local (`CAIBrain* brain = PBrain;`) keeps PBrain in a callee-saved register across an inlined LowerBound call. GetCreature() reloads it (0x00985440).
- Two scoped `{}` blocks around consecutive searches (hero search, then mode lookup) make the out-pointer and lookup-key slots share. This fixed a frame 8 bytes too big (0x00985440).
- `CThingFilter_In2DRange` is `{ C3DVector Pos; float Range; }` (16 bytes) with a `__forceinline` ctor. Its radius argument is evaluated first (right to left), so the sight-radius call precedes GetPosition (0x00985440, landed 0x006A76B0).
- A rough-area box built by a ctor from (position, radius) gives retail's `ftol X ; ftol Y` order with the filter temp stored before GetPosition. It matched the whole search prologue of 0x0097BB30. Named x/y locals before the call put GetPosition ahead of the filter store.
- Definition-name lookups as a scoped NAMED string (`{ CCharString n("..."); idx = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(n); }`) give `lea edx,[str] ; push` (0x0097BB30). A temporary `CCharString(...)` argument pushes the ctor's eax instead.
- The engine header's CAIStateGroup_ScorpionKing offsets after TrapThings are PDB (16-byte vector). Retail has a 12-byte STLport vector, so ChargeCount sits at +0x2c, not +0x30.
- A derived class starting after a `CThing` whose last member is a byte at +0x91 begins at +0x94 (sizeof rounds up), not +0x92. Pad derived members from 0x94 (0x009456A0 building pointer at +0x168).

Review at landing:
- Rows that needed more than 4 grades in an earlier segment: 0x00958840 (parked), 0x0090C2D0, 0x00930A70 and 0x009236C0 (matched only after more than 4 grades; re-check that the sources contain no probe leftovers).
- Local base models next to existing engine headers: every matched source models CAIStateGroupBase / CThing locally. Headers exist for CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose, _MeleeCombatNearbyBase, _MinionGuardGreetPlayer, _MinionKidCalledToMother, _PerformAction, _ReactToDeadLeader and others: the generated headers flatten the vptr / base, so the local models carry a `// Layout:` comment instead.
- Local CThing models use the word-local HasTC. Check that the shared headers' GetInterface keeps the masked form wherever retail has `test dword`.
- The packed CAIStateGroup_OpinionReactionRoot (pack 1, ThingReactingTo at +0x6b) is real, per the engine header and the landed 0x009830C0. Do not "fix" it.
- Near-fixes to apply first in 5.1: 0x0090FDB0 (a local fps copy inside the Random helper), 0x0093A080 (GetPThingWorld result local before the fps load), 0x00911300 (the flags dword in the zero-mask test), 0x00985440 (two scheduling hunks).
- 0x00984F60's model declares GFFloatRandom with the wrong argument order (see the lesson above); fix it before regrading.
- Already-landed name mismatch: rebuild/src/compiled/00/74/CTCPerceiveThing_GetSightRadius_007448f0.cpp v the manifest's GetExtendedSightRadius for 0x007448F0 (the landed 0x00933B30 comment uses GetExtendedSightRadius).
- Label corrections listed in NOTES.md need a labels_*.tsv entry (several CAIStateGroupBase getters: 0x008FCFA0 GetPThingWorld, 0x008FCFB0 GetPDefinitionManager, 0x008FCFC0 GetPWorld, 0x008FCF40 Terminate).
