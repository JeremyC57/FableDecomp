# Chunk 766 notes

7 matched (all RELOCATION_MATCH, read-only vland: 7/7 RELOCATION_MATCH + behaviour PASS), 7 parked (6 with 10+ grades, 1 bounds
error), 12 not attempted (session time; triaged below so they can be reassigned).

## Results

| address | function | grade | grades | note |
|---|---|---|---|---|
| 004304e0 | NUISystem::CUIMiscThingsDef::CUIMiscThingsDef() | RELOCATION_MATCH | 3 | header overlays below; same TU as the landed dtor 0x004317D0 |
| 0049124e | CSubtitleRenderer::GetActionNameFromEmbeddedTag | RELOCATION_MATCH | 5 | size TU (y off, s on); inline StartsWith/EndsWith |
| 00c1a9c0 | PKCDefaultFileManager::AsyncReadThread | RELOCATION_MATCH | 8 | do/while over a quit flag |
| 00c30f60 | ReadBankData | RELOCATION_MATCH | 5 | temporaries for two record readers, named criteria reader |
| 00c42b80 | CUnprovenPtrList_00c42b50::RemoveAt | RELOCATION_MATCH | 1 | |
| 00c47f00 | CAIStateGroup_BirdFlyToFood_00c48300::Issue | RELOCATION_MATCH | 1 | landed spelling |
| 00c5a700 | CCodaVoice::UpdatePitchEnvelopeStages | RELOCATION_MATCH | 1 | switch with fallthrough on expiry |
| 004199c0 | (mid-function of 0x00419880) | PARKED | 0 | catalog bounds error: MIDSTART inside 0x00419880 (BOUNDS_FIX.tsv) |
| 00c74850 | CEffectFilter::SetShelves | PARKED | 10 | DIFFER(588v593) |
| 00c77800 | CEffectPart260::Stage_00c77800 | PARKED | 12 | DIFFER(725v725) |
| 00c5ca60 | CSoftwareVoice::ApplyParameters | PARKED | 10 | DIFFER(831v858) |
| 00627be0 | CTCQuickAccessMenu::GetExpressionsListUI | PARKED | 10 | DIFFER(1048v1056) |
| 00c1a510 | PKCDefaultFileManager::OpenFile | PARKED | 10 | DIFFER(850v850) |
| 00c16f00 | ReadDeferredBank | PARKED | 10 | DIFFER(848v857) |
| 00862aa0, 00c12940, 00c12e50, 00c271f0, 00c38d60, 00c3bf30, 00c3f9d0, 00c47f10, 00c4de00, 00c54190, 00c6f3e0, 00c89350 | | NOT_ATTEMPTED | 0 | see triage |

## Header overlays ($D/hdr/engine, all opt-in; hdrcheck2: default=same for all three, every existing macro =same)
- `CDefClassBase.h`: `FABLE_RETAIL_DEF_CLASS_BASE_CTORS` adds the private inline `CDefClassBase()` (PDefManager, Setup, Template,
  DefaultValsApplied zeroed; friend CParentDefClassBase). FableWin `??0CDefClassBase@@AAE@XZ` is inline in retail (0x004304E0 expands it).
- `CParentDefClassBase.h`: same macro makes `CParentDefClassBase()` an inline empty body (retail 0x00430370 is the inline copy).
- `CVectorMap.h`: `FABLE_RETAIL_CVECTORMAP_CTOR` adds `CVectorMap() : Dirty(false) { }` (FableWin `??0?$CVectorMap..QAE@XZ`, 82 instances).
- Siblings: the macros are new and no landed file defines them, so no landed includer changes (SIBLINGS.tsv).

## Header debt
- 0049124e reuses the landed FormatTextWithGameActionNames 0x004917B5 preamble; vland warns SHARED_HEADER_AVAILABLE for C2DBoxF,
  CRGBColour, CUserProfileManager and CPlayerGui (local models: the shared CPlayerGui.h has no static PeekGuiDef returning
  `const CPlayerGuiDef&`, CUserProfileManager.h no GetAssignedInputForMovementAction). Adding both declarations behind a macro
  would clear the strict-mode warning.
- libpk rows warn SHARED_HEADER_AVAILABLE CString: the libpk trace stream (one-word CString over 0x0139C900) is a different class from
  engine CString; every landed libpk row models it locally. A `libpk/CString.h` would retire about 40 local copies.

## Callee names used (bound spellings where rb.sh had one)
- PKCDefaultFileManager: Receive 0x00C1B030, CloseFile 0x00C1A370, PKCSemaphore::Release 0x00C2A150, PKCProtectionInstance
  0x00C0FFC0/0x00C0FFE0, PKCTRRedblackMap_00c1b720::Insert 0x00C1B720, trace stream 0x00C0DEC0 (text), 0x00C0DF00 (long),
  0x00C0DEF0 (unsigned int), 0x00C0DF10 (unsigned long), 0x00C0DF20 (const void*, "%p": a wide file name streams through it),
  g_pDebugBuffer 0x0139C900, g_pfnErrorReport 0x0143BB2C, CDebugBreak 0x00C16750.
- Segment reader (CSegmentFile): ctor 0x00C2DA20, dtor 0x00C2DBE0, OpenSegment 0x00C2D9C0, GetPosition 0x00C2DA10, Read 0x00C2DA00
  (the landed labels disagree: NAME_FIXES.tsv).
- CTCQuickAccessMenu::ConstructExpressionCategoryList 0x00627400 (FableWin), CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C /
  GetDef<CExpressionDef> 0x005F81BE, CDefClassBase::GetSubDef<CExpressionSubDef> 0x005DA240; CExpressionDef::ExpressionType is at
  retail +0x8C (Ego_r +0x90), CExpressionSubDef::ExpressionDef +0x28.
- CSubtitleRenderer: GameActionValues map at CPlayerGuiDef+0x940 (landed _M_find 0x00475861 model), GetAssignedInputForMovementAction
  0x00408D90 with GAME_ACTION 0x3C, GetActionName 0x00446720 (__fastcall), DELTA_NOTIONAL_ZERO 0x0129BA3C.

## Parked residues and next ideas
- 00c74850 SetShelves DIFFER(588v593): the `(float)sampleRate` CSE spills to a frame slot across the if/else join, retail keeps it
  on the x87 stack; a double conversion keeps it but uses the qword 2^32 fix. Next: convert through a named float declared inside each arm.
- 00c77800 Stage_00c77800 DIFFER(725v725): 55 lines are one uniform eax/ecx/edx rotation in every SetShelves argument setup (retail
  loads Context before the fmul). AirDecay (+0x22C) lies inside AllPass[1] (+0x30): modelled with a union. Int counters, a shared
  delay local, `offset += spread` and per-loop sampleRate locals do not move it. Next: find the extra/missing IR temp (see GOTCHAS).
- 00c5ca60 ApplyParameters DIFFER(831v858): retail keeps `changed` in its argument slot (buffer in ebx) and shrink-wraps edi/ebp around
  the level and pan blocks; ours keeps `changed` in ebx and pushes all four at entry. The MillibelsToAmplitude stand-in needs an
  observable store to keep retail's speaker-first call order. Uses libpk/FtoL.h at 0x00C5CC36..0x00C5CC3E (fistp truncation) and
  0x00C5CD79..0x00C5CD81: the attempts table is needed only if it lands. Next: split the function into blocks with their own locals.
- 00627be0 GetExpressionsListUI DIFFER(1048v1056): this/zero register swap (retail this in ebp, zero in ebx), and retail homes the
  result in a frame slot while CSEing the alignment/stealth `_M_start` into esi/edi for the empty tests and frees. Same-TU GetDef /
  GetSubDef stand-ins (noinline) and a noinline push_back reach the loop shape; a separate `result` variable fixed most of the tail.
- 00c1a510 OpenFile DIFFER(850v850): esi/edi swap of `this` against the file name/record (20 lines) and the record stores
  (count before handle). A named Insert result gives retail's `cmp eax,ebx`; a separate inserted `value` local keeps the record in a
  register across the EMEM trace.
- 00c16f00 ReadDeferredBank DIFFER(848v857): from the first Read the scratch registers rotate and the table lands in ebx; retail pushes
  ebp for the table, so its lookup-table failure tail stays separate (9 bytes). if/else and count-block variants break the tail merging.

## Triage of the rows not attempted
- 00862aa0 CModeStandBase::UpdateWalkingAnim (Fable): siblings UpdateIdleAnim 0x00862780 / SetUpMovementAnim 0x00861A80 landed (CModeStandBase
  model, CMovementSetup by reference); uses the thing's vslots +0x118, +0x140, +0x144, +0x148 and the TC 0xDF lookup.
- 00c54190 (3D source property set, a virtual): compare-and-set per EAX property with each setter on the +0x290 parameter block, then
  CVoiceEffect::Configure(+0x290, 0). Blocked on naming: the 15 setters are landed under four different class names (NAME_FIXES.tsv).
- 00c16f00's neighbours: 00c3bf30 (PKCFile.cpp read path, CRACBuffer), 00c38d60 (CInstance.cpp, CCodaFrameStreamer), 00c4de00 (OpenAL
  buffer upload through GetProcAddress "alGenBuffers"/"alBufferData": libpk wrapper, not third-party), 00c6f3e0 (mixer dispatch over
  CUnprovenMixKernel 0x00C6F1A0 / ScaleAccumulate16MMX 0x00C6F0F0), 00c89350 (reverb crossfade, `and esp,-16` frame), 00c12940 /
  00c12e50 (CSoundEventManager, many unproven callee names), 00c271f0, 00c3f9d0, 00c47f10.

## GOTCHAS one-liners
- A uniform eax/ecx/edx rotation over the rest of a function (0x00C77800, 0x00C16F00) means one IR temporary more or fewer earlier;
  scheduling, counter types and named locals do not move it.
- Size-TU (`optimize("s",on)`) inline helpers such as CCharString::StartsWith/EndsWith must be `__forceinline` or VC7.1 calls them.
- A trailing single `return false` (else-if instead of an early return) keeps all callee-saved pushes at entry and lets two identical
  tails cross-jump (0x0049124E); `if (offset >= 0) return ...; return false;` gives retail's `js false` order.
- An address-taken local that must stay in a register across a call needs a separate copy for the address (`void* value = file;
  Insert(key, &value)`, 0x00C1A510); naming a call result before `!= 0` gives `cmp eax,ebx` against the zero register.
- A template stand-in body under `#pragma auto_inline(off)` is still inlined at a later instantiation; mark it `__declspec(noinline)`.
- `GetLastError()` inside a trace chain runs before the chain (right-to-left argument order), as retail.
- libpk's wide file names stream through CString::operator<<(const void*) 0x00C0DF20 ("%p").
