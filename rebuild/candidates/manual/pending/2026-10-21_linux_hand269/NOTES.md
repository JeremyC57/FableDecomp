# Hand chunk 269 (CSubtitleRenderer .. CTCDivineWrath): 13 of 40 byte-exact

## What matched (13)
- **CSystemManager** 0x009A5800 `~CSystemManager`: compiler-generated member destruction in reverse order; the counted pointer's inline dtor re-reads the block after the decrement.
- **CTCCreatureGenerator** 0x0074B900 `Persist` (catalog `vfunc_1`): scalar defaults are temporaries bound to const refs and homed in the dead context slot; the script-name default is a block-scoped copy of an inline EmptyName() (no NRVO).
- **CTCActionUseBuyHouse** 0x0072E940 `OnActionUseMain`: sibling OnActionUseRent shape; symbol/text temporaries in dead argument slots; the MARITAL/plain picks are if/else pairs of whole `text += GetTextBySymbol(...)` statements that VC7.1 cross-jumps.
- **CTCBuyableHouse** 0x006C17B0 `SetOwnedByPlayer`: unchecked stats lookup (miss path reads the stale out local sharing the dead `owned` slot); world is a named local read before the position.
- **CTCBattleCharge** 0x007A71B0 ctor and 0x007A9E40 (really `CTCAssassinRush::CTCAssassinRush`): the landed CTCWillEnflame/CTCForcePushPower ctor family; default intelligent pointers store only the vtable.
- **CTCBoastingArea** 0x007828E0 `FrameUpdate`: inline InRange(offset, radius) with a by-value radius and `X*X + (Y*Y + Z*Z)`; every state-switch arm returns.
- **CTCCreatureOpinionOfHero** 0x00684050 `DecriminaliseAllKnownCrimes`: unchecked deed-log lookup, inline per-list helper, the village lookup repeated with its result unused.
- **CTCBulletTime** 0x0079D110 `~CTCBulletTime`: local class model with real member types (CDefPointer release dtor at +0x20, CEnginePrimitiveHandle inline Clear + counted-pointer dtor); the end sound is a named CCharString local.
- **CTCCreatureTintEffect** 0x007CEE40 `FrameUpdate` (catalog `vfunc_9`): a `__forceinline SetTint(parent, amount)`; the TintList loop as `it = begin(); end = end(); for (; it != end; ++it)`.
- **CTCCreatureExpression** 0x007294A0 `FrameUpdate`: the id is a named `unsigned __int64` read before the frame call; Dot through statement accumulation (`d = Z*Z; d += Y*Y; d += X*X`) for retail's Z,Y,X order; SetExpression is out of line at four sites and inline at the last, reproduced with a `__forceinline` copy (SetExpressionInline).
- **CSurface** 0x009F37E0 `CopyFromSurface` (replaces the landed asm_bake): the inline GetPixelFormat()/GetDimensions() getters return temporaries so VC7.1 packs the frame (source format in the dead `source` slot); the temp-format argument is the inline `CPixelFormat(D3DFORMAT){Initialise(f);}` ctor, the kept A8R8G8B8 format is the `-1` default ctor + Initialise; the locked rect in its own `{}` block; RECT copies member by member top, bottom, left, right.
- **CSurface** 0x009F4190 `ClearArea`: inline GetWidth()/GetHeight() GetDesc reads called twice per clamp; the box is a local C2DBoxI filled member by member (the copy ctor gives the wrong load registers).

## Parked for Fable 5.1 (9)
- 0072eb40 CTCActionUseBuyHouse::ReceivedConfirmation DIFFER(467v463): presence-bit test form of the bool HasTC (shr vs test of the masked word) | tried: bool/unsigned long HasTC, ternary GetTC, named bools | next: the HasTC body from a landed sibling with the exact return type
- 00736760 CTCCombat::UpdateFlourishParticles DIFFER(427v427): ebx push not shrink-wrapped into the particle branch; loop-1 index/element registers swapped | tried: index vs pointer loop, named element, scoped blocks | next: lane gap: register allocation
- 0074e280 CTCContainerRewardHero::GetDynamicPotionDefIndex DIFFER(469v469): frame-slot assignment only (inventory out/key/name temporaries in other words) | tried: block scopes, declaration order, const-ref locals (f..h) | next: lane gap: frame packing
- 006c0970 CTCBuyableHouse::CreateSignPost DIFFER(421v419): register picks (movsx eax, hoisted lea ecx) + `fld pos.x; fadd st(2)` | tried: variants b..k (named floats, inline position, bool HasTC) | next: lane gap: x87 operand order with three live lanes
- 0079cc80 CTCBulletTime::PlayEffect DIFFER(496v512): retail keeps 0 in ebx (xor ebx,ebx) for stores/compares/pushes; ours uses immediates | tried: same-TU stand-ins (fixed the def pointer), named zero, scoped blocks | next: lane gap: zero register
- 007fd650 CTCDPhysicalObstruction::FrameUpdate (catalog vfunc_9) DIFFER(434v448): retail gives the three line/vector/colour temporaries separate frame words (sub esp,0xd8) | tried: named primitive locals (0xd0, engine moves into a register) | next: lane gap: temporaries' frame sharing
- 0078f8f0 CTCDivineWrath::CancelForCutscene DIFFER(497v505): ebx holds the constant 1 in retail, caster in ours | tried: advisor a15 (function-scope key, closest), v1/v2 extra constant uses | next: lane gap: constant-in-register tie
- 00490a22 CSubtitleRenderer::IsInInteractiveState DIFFER(442v446): shared `mov al,1` exit after the cast check, not after the epilogue; three key words (0xc frame) vs retail's key sharing the out-pointer word (8) | tried: flat returns, do/while+break, goto, nested-if helper, ternary a/b (shares the word but moves a into a register) | next: lane gap: block placement + frame packing
- 009f4390 CSurface::CopyFromMemory DIFFER(500v507): dstBox/srcBox in edi/ebx (retail ebx/edi) and the widths not spilled to the dead box argument slots | tried: plain longs, POD C2DExtentsI, C2DBoxI::GetSize() | next: lane gap: register allocation / dead-argument-slot homing

## Not reconstructed (18)
Brief look only, or not reached in the session's time; see STATUS.tsv for each reason: 0048f230, 0048f50c, 00783850, 008139c0,
00782390, 007877e0, 0075c280, 00705240, 007502d0, 007363a0, 0074ee90, 00670820, 007dd270, 007ddda0, 00749f00, 0077a180,
0080ca20, 006e0be0.

## Skipped
None. No row holds two functions.

## Label corrections
- 0x007A9E40 catalog `CTCBattleCharge::CTCBattleCharge` is `CTCAssassinRush::CTCAssassinRush`.
- 0x0074B900 catalog `CTCCreatureGenerator::vfunc_1` is `Persist(CPersistContext*, bool)`.
- 0x007CEE40 catalog `vfunc_9` is `CTCCreatureTintEffect::FrameUpdate`.
- 0x007FD650 catalog `vfunc_9` is the CTCDPhysicalObstruction FrameUpdate slot.
- 0x006C59A0 is the deed log's decriminalise call. 0x006C0120 GetPrice takes no arguments; AddMoney is (long, bool).
- 0x0079CE80 PlaySoundW receives a CCharString address (landed declaration says unsigned long).
- 0x0049D870, manifest name "GFGetPreMainMemoryUsed", is GFGetCurrentFrame.
