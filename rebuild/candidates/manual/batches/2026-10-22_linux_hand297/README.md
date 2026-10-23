# Hand chunk 297 (CRadialBlurProcess .. CScrumpyCrate): 32 of 40 byte-exact

All 32 src rows grade RELOCATION_MATCH (gall.sh), and their tests print `<addr>_TEST PASS` under both g++ (tall.sh) and VC7.1 (tall.py).

## What matched (32)

- **CRadialBlurProcess** 00880ae0 Process: blends the stack's current radial-blur params and position towards an entry (`cur*(1-t) + e*t`). SHAPE: `1 - t` is computed once. The by-value operator*/operator+ temporaries are evaluated right operand first, the sum temp is copied with integer moves, and the scalars are blended in place.
- **CRandomAttacker** 00de21d0 Main (catalog said CRansomRevenge): the ransom-victim attacker waits until the hero is within a random 8..19 units, then turns on the victim or the hero. SHAPE: the identical "turn on the hero" arms are cross-jumped by VC7.1. The `&&`/`||` conditional string temps carry ebx flag bits 1/2/4/8, and their results share one byte slot.
- **CRandomPopulationCreature** 00ec1ef0 Main: attaches Me to its evil/good population village, then walks. SHAPE: the identical village arms are cross-jumped. The village `~CScriptThing` exits sit under `#pragma inline_depth(0)`. The FreedomFighter walk loop is reused.
- **CRangeCompressor** 00f3a2e0 Compress: writes each column's script-entry header and bit-packs its values. SHAPE: bind `const CScriptEntry& entry = entries[i]` once per column (indexing at each use re-reads begin after every char store). Read values at `source + j*stride`.
- **CRangeCompressor** 00f39ed0 Decompress: does a raw memcpy, or unpacks each column per its header, and times the call with the double clock. SHAPE: write the values at `column + j*stride` (a named out pointer moves the accumulator into ebx). VC hoists the flags-bit tests into byte locals. The `__int64` range check of blockSize*8 returns without closing the timer.
- **CRansomRevenge** 00de3830 Main: the avenger turns on the victim or the hero, then reacts to hits. SHAPE: the sibling of 00de21d0, with a break-out `if (a || (b && !c))` hit chain carrying flag bits 1/2/4 and cross-jumped conversation tails.
- **CReactionFrequencyTraits_ControlledCount** 006fff00 TransferBinaryIn: reads six persisted members. SHAPE: a non-template `__forceinline ReadValue(T&)` per member over the inlined CMemoryDataInputStream::Read (64-bit range check, memcpy fast path, overflow reader). The bool is normalised with `test al,al; setne`.
- **CReactionMatch_SecondaryAxisMRShifted** 006ff040, **_SecondaryAxisCentredMRShifted** 006ff2c0 and **_AsymmetricPyramid** 006ff540 TransferBinaryIn: each calls the base read, then reads the axis/opinion types and six floats. SHAPE: the same ReadValue recipe. The inlined locals share the dead stream-arg slot, and the last fast path is tail-duplicated.
- **CRenderManager2D** 009d9a30 ctor: in retail mode opens `<data>pc\shaders.big`, then the SHADERS_BBBLIB vertex-shader bank and its 2D shader. SHAPE: a named `CWideString platform(L"pc\\")` inside the block, a named CCharString bankName (it lands in the dead arg slot), and the counted pointer's inline operator->. The literal converts implicitly to `const CCharString&`. VC7.1 has no native wchar_t, so declare `const unsigned short*`.
- **CRenderManager2D** 009db810 AddTri, 009dbae0 AddLine, 009dbd10 AddAlphaTri, 009dc030 Add2DTri: each queues overlay/alpha/2D primitives with viewport-to-clip conversion. SHAPE: the landed Add2DLine model. The X origin is a named float (its int copy goes in the dead v0 slot), and the Y origin is a box reference. A same-TU read-only `_M_insert_overflow` stand-in keeps the vertex local from escaping. AddAlphaTri adds ZAverage `*(1/3)`, so its frame is 0x60.
- **CRenderManager2D** 009dc870 Draw2DTexture: a textured box as two triangles with a texel nudge. SHAPE: per vertex, Diffuse/X/Y are stored first and U/V/Z/RHW after the offset is computed. The blend mode is chosen by flags 2/1.
- **CRenderManager2D** 009dca80 Draw2DGraphic and 009dd210 Draw2DScaledGraphic: a bank graphic frame at natural size or stretched. SHAPE:
  - the counted frame release of RenderSubPrimitive 0x00BCB800;
  - chained `a = b = f()` stores b first;
  - the UVs are copied from a const-ref-bound temporary;
  - the texture goes through an inline GetTexture();
  - the 0.01-texel divides sit inside the loop, so the quotients are hoisted onto x87;
  - GFAXToVX/GFAYToVY are `__fastcall`.
- **CRenderManager2D** 009dda50 Draw2DBoxColours: a four-corner-coloured box as two triangles. SHAPE: the four ARGB words are packed first and the edges converted in TLX, TLY, BRX, BRY order. Then the source order is U, V, Diffuse, Z, RHW for every vertex, with the X/Y of all four last. This reproduces retail's store schedule.
- **CRockTrollTrigger** 00e0ad40 Main: plays the Darkwood trader-troll cut-scene once, when the hero is within the def distance. SHAPE: the PrisonRaceWarden handle/map/movie views plus a scoped alive-filter.
- **CRuntimeScriptedEntityPersist** 00cb8440 LoadFromGameStateScript: parses `START_NEW_ENTITY_SCRIPT` blocks into the persist map, then sorts it. SHAPE: the inline Reset (erase plus Clear), the parser's implicit inline dtor, and a same-TU read-only `_M_insert_overflow` stand-in for push_back.
- **CSafeTrader** 00e4a280 Main: walks to SafeTraderEscape, warns the hero once, and fades out on arrival. SHAPE: a scripted-thing handle with an inline ctor and an out-of-line dtor at every exit, and the out-of-line ~CIsThingAlive.
- **CSP_SecretPassageGuard** 00ded810, **CSP_InvestigatingGuard** 00dedb80, **CSP_DeskGuard** 00dedf30 (catalog said CSP_SecretPassageGuard) Main: the secret-passage guards mutter or investigate until the hero is seen. SHAPE: the CHTE_Audience handle (inline ctor, out-of-line dtor) plus the CRockTrollTrigger scoped filter.
- **CSP_ScreamerTrigger** 00def3b0 Main: spawns the N undead sword soldiers given by Me's data string near random spawn points. SHAPE: the AmbushPointBackup model. The loop counter starts at 0 and doubles as the zero source of the two default constructions.
- **CSP_UndeadTrigger** 00def760 Main: spawns SwitchDoorsOnAndOff, then turns spawn markers into undead. SHAPE: `__forceinline` CScriptThing/CCountedPointer operator=. The second use binds the element to a reference and assigns under `#pragma inline_depth(0)`.
- **CSUMMONED_CREATURE** 00f1b220 Main (parent CQ_ArenaScript): the arena creature scores the crowd on flourish or decapitation hits. SHAPE: the timer goes through inline CTimer accessors reading the handle via `const long&` (advisor lesson). That reference keeps the conditional-temp flags word in its frame home.
- **CScrollableList** 00555440 HideComponent and 005556f0 UnhideComponent: set the position/alpha of all eight UI states, then slide/fade. SHAPE: every state key is a fresh `const unsigned long&` temporary, and the key 6 used five times is hoisted into ebp.
- **CScrollingViewport** 0053eda0 ScrollChildren: scrolls the children one step and toggles the scroll bars. SHAPE: the virtual-slot calls over the PDB layout, with the -1.0f pool constant.
- **CScrumpyCrate** 00ef2ba0 Main: hides the crate and opens the gate while the hero carries it. SHAPE: the carry waits are `while (!(!x))` (`test al,al; setne`), and the first carry test is a plain `if (x)`.

## Parked for Fable 5.1 (8)

- 00555a90 CScrollableList::UpdateVisibleList DIFFER(595v595): visible-window loop by-value C2DVector X/Y in edx/ecx vs ecx/edx, first/last loops edx vs eax (register colouring only) | tried: 21 source variants (probe/555a90b..v: loop, vector and temp placements); the best leaves only the register colouring | next: an Opus regalloc sweep; try splitting the C2DVector ctor out to a helper
- 00853d30 CScriptModuleOpenGate::OnInitLeaf DIFFER(627v625): the dot products load toCamera before forward/side (`fld tc.Y; fmul fwd.Y` vs retail `fld fwd.Y; fmul tc.Y`); frame 0x30 vs 0x24 (retail reuses toCamera's slots for side*1.5/offset) | tried: Dot this/v swap, reversed formula, ctor-built toCamera, named floats, toCamera reused, forward declared first | next: a Dot taking `const C3DVector&` on the other side via a free function; scope the offset temps in a block
- 00881a40 CScreenEffectPriorityStack::ScreenFilterSetFlags DIFFER(591v581): ebx/ebp swapped between the entry pointer and the loop counter, cascading into an extra `mov ecx,eax`, an immediate-0 store and a 10-byte loop pad | tried: named vs block-scoped find iterator, `end()==find()` order, no filter local, separate counters, hoisted reset ref, inline loop-1 helper, named element ref | next: reorder the two loops' locals and declare the entry pointer first in an outer block
- 009ddc60 CRenderManager2D::Draw2DBoxWithZ DIFFER(899v899): only the top/right converted-edge frame slots are swapped ([esp+14h] vs [esp+18h]) | tried: SetRHWForZ void helper, rest-first store order, declaration orders, `mode=1` before the ARGB pack | next: convert the edges into a C2DBoxF local, or reuse the Draw2DBoxColours edge order with Z set per vertex
- 00a87700 CSBCAgents::Process DIFFER(603v603): retail keeps `this` in edi and the agent iterator in esi, ours swaps them, plus temp naming and the Input copy interleave (register colouring only) | tried: 16 source variants (probe/a87700b..s); the best leaves only the colouring | next: an Opus regalloc sweep
- 00aa3c50 CRightHandedSet::Interpolate DIFFER(562v671): lane gap; the table slerp rounds indices with a bare dword `fistp` after a float store/reload (the original's hand-written FastFloor), which compiled C++ cannot emit (a cast calls `__ftol2`; /QIfist gives a qword fistp) | tried: a compiled rounding helper (the cast calls `__ftol2`; /QIfist emits a qword fistp without the reload) | next: the baked-oracle lane (as for the chunk 145/150/236 fistp family)
- 00f3a890 CRangeCompressor::CalcBestCompressionForBlock DIFFER(844v857): retail keeps entries[0].Flags in ebp across CArray::operator= and drops entries[1]'s ctor zero-store, with ebx=0 for the whole body and `this` spilled; ours reloads Flags and keeps `this` in ebp | tried: flags local, arrays first, copy before arrays (832v857), bestIndex/best order | next: model CScriptEntry copy as a forceinline member-wise assign; check whether the second CArray is copy-constructed rather than default-built plus assigned
- 00f3abf0 CRangeCompressor::CalcCompressionScript DIFFER(605v605): `this` in ebp and the remaining-width counter in ebx, swapped (register swap only) | tried: one graded source, then parked under the register-swap rule | next: an Opus regalloc sweep

## Skipped

None. No row holds two functions.

## Label corrections

- 00de21d0: the catalog's CRansomRevenge::Main is really **CRandomAttacker::Main** (CQ_RansomVictimScript; factory 0x00DE2100 registered as "RandomAttacker").
- 00dedf30: the catalog's CSP_SecretPassageGuard::Main is really **CSP_DeskGuard::Main**.
- 0x008AC970 is the vector<CScriptThing> dtor. The manifest's ~vector<CConversation> label was propagated across byte-identical bodies.
- 0x00A14440 is Normalise (not GetScaled).
- 0x0042D5B1 is map<unsigned long, CUIState>::operator[].
- 0x00CBD370 is sort, 0x00CBC130 is _M_insert_overflow and 0x00CBBDA0 is vector erase (CRuntimeScriptedEntityPersist map).
- 0x0099E960 is CCharString::operator!=.
- 0x009FC810 is CGraphicFrameData::GetTextureQuadUVs (the catalog says GetObjectTransform).
- 0x00DEFB10 is CQ_SecretPassageScript::SwitchDoorsOnAndOff.
- 0x009A80B0 is CBankFileManager::BankExists (forge-autoname says GetLanguageData).
- 0x0099BE70 and 0x0099BF30 are CWideString operator+ (const CWideString& / const unsigned short*), `__fastcall` with a hidden result. Forge says CCharString::AssignFromData/AssignFromValue.
- 0x00A5D7C0 is CShaderResource::OpenShaderBank (forge says CResourceIndex_InitializeFromBank).
- 0x009E1BC0 is a high-resolution clock returning a double.
- The parent of CSUMMONED_CREATURE is CQ_ArenaScript.
