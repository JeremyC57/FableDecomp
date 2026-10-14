# 2026-10-14_linux_hand44 — hand-authored chunk 44 of the class-labelled 17–96-byte pool

Coordinator-authored (no sub-agent) on the Linux VC7.1 lane. Chunk 44 (`mkchunk.py 44`, 40 rows, all
`CGameScriptInterface` entry points over the thing's component map): **9 of 40** landed as `RELOCATION_MATCH`/`MATCH`
with behaviour tests; 31 are parked on ONE systematic register-choice gap (below).

## The component-lookup model (`$S/w22/agent44/gen.py`, BASE block)
Every row: the thing (script thing slot 11, after slot 75 `IsValid` for the mangled-name entries; the hero through
`PlayerManager->GetMainPlayer()` @ 0x00449970 then `GetHeroCreature` @ 0x00487DC0 / `GetControlledCreature` @ 0x00487DD0
for the hero entries), the component-kind mask at +0x20 (`HasTC(kind)` = `m_tcMask[kind >> 5] & (1 << (kind & 31))`,
folded by the compiler into `test byte/dword` or `test ch,imm` / `jns`), then the `CVectorMap<ETCInterfaceType, CTCBase*>`
at +0x44: `LowerBound(const unsigned long&)` (@ 0x0040F020, or @ 0x004365B0 for the const overload) with the kind in a
stack temporary, the inlined `find` tail (`cmp end ; cmp [it],kind ; jle ; mov eax,end`) and the value word at +4.
`HasTC` must return the masked word (a bool return materialises `shr ; and 1`); the key must be CONVERTED for the
reference parameter (`(unsigned long)kind`) so the compare after the call uses the constant and not the temporary.

## The gap (31 rows parked, sources in the lane's `differ/`)
Retail loads the component pointer with `mov eax,[eax+4]` and then copies it (`mov ecx,eax` before a member call
with arguments; `mov al,[eax+N]` for the getters); the lane loads it straight into the register it needs
(`mov ecx,[eax+4]` / `mov edx,[eax+4]`). Fifteen source shapes (member / free / `__forceinline` / non-inline keyword
backend-inlined / reference-returning / iterator-returning / entry-pointer / const forms) all produce the lane's form.
The nine rows that match are the ones where retail ALSO loads straight into ecx: the component used as `this` of a
member call with no argument pushed in between (SetFireToThing, ExtinguishFiresOnThing, IsThingOnFire,
IsReportedOrUnreportedCrimeKnown, EntityGetHearingRadius, EntityGetBossPhase, SetLightColour, EntityResetForceToLookAt)
or kept in esi (EntityGiveGold). Expect the same split on every remaining CGameScriptInterface component row (chunks
43 and 45 of the pool; 2,793 copies of the find tail exist in the binary). `IsHeroHandLampLit` additionally reads `this`
back from the `push ecx` frame slot in retail (the lane keeps it in esi).

## Label notes
- 0x0089C470 is the opinion-table variant of EntitySetPersonalityOverride (0x00681E60).
- 0x00744440 "CMap::DrawGetSeaLevel", 0x007AED10 "CWorld::DrawGetEnvironment", 0x00780410 "CMusicManager::ResetMusicEntriesOffsets"
  and 0x00449970 "Catch@0040d959" are propagated labels (hearing radius getter, boss phase getter, CTCFlammable::SetOnFire,
  CPlayerManager::GetMainPlayer).
