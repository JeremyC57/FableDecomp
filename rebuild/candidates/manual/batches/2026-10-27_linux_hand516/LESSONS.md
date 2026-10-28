# Chunk 516 lessons (GOTCHAS one-liners)

- RTM 13.10.3077 + the hand955 L083/L084 GetInterface model (CVectorMap Ego_r template LowerBound, bool/out-pointer `__forceinline` GetInterface) matched all 35 CGameScriptInterface component rows on the first grade; the 2026-10-14 hand44 "mov eax,[eax+4] ; mov ecx,eax register-choice gap" (31 parks) was the QFE lane plus non-out-pointer source forms -- regrade those parks with this model.
- CGameScriptInterface overloaded virtuals sit in the vtable in REVERSE Ego_r declaration order (EntitySetPersonalityOverride: slot 618 long, 619 CCharString; SetIsGossipForPlayer: 723 by-value, 724 const-ref); ghidra_out/cgsi_slots_aligned.tsv already has them right.
- An inlined static helper taking CCharString BY VALUE explains a "pointless" copy of a const-ref argument (built in the dead argument slot) -- SetIsGossipForPlayer is CTCAIScratchpad::SetIsGossipForPlayer(CCharString, bool) inlined.
- A TC interface kind is readable from each TC class's GetInterfaceType (RTTI vtable slot 21, `mov eax,imm ; ret`); 18 of the 24 kinds here came straight out of that scan.
- STLport vector construction needs the real `_Vector_base(const _Alloc&)` with the `_STLP_alloc_proxy` end-of-storage member: a plain third pointer member moved `push edi` before the three zero stores (0x008B0AB0, the old agent20 park).
- `if (p && !p->Dead) { ... }` with only two terms tests the bit field as a value (`mov al ; not al ; test al,1`); nested `if (p) { if (!p->Dead) ...` gives retail's `test byte ptr [..],1 ; jne` (0x008D03E0). With a third `&&` term the flat form is fine.
- A by-value C3DVector return copied member-wise from `this` needs C3DVector's inline user copy ctor; without it VC7.1 copies through `add edi,28h`.
- RockTroll Initialise (0x008C6160): a named creature local fixes the GetPItem-before-pushes order but swaps esi/edi (this vs &PCreature); no local keeps the registers but pushes first -- park after 2 grades next time.

## Review at landing
- 0x008B25C0 gets its first PDB-style binding (std::__copy<CCombatAction*, CCombatAction*, int>) from the same-TU helper in 008b0ab0; the landed source names it _global::Move_CountedPointer_008b25c0.
- Landed 0x00899060 still calls 0x00487DC0 CPlayer::GetPPlayerCharacter (settled: GetPControlledCreature).
