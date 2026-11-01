# Hand chunk 693 (CVtable_012b8ac8 .. CBankFile): 9 of 12 byte-exact

Ten NA036 rows plus two REPAIR rows. 9 RELOCATION_MATCH (one of them retail_asm), 3 parked DIFFER. All tests PASS under
g++ (tall.sh) and VC7.1 (tall.py).

## What matched (9)
- **CVtable_012b8ac8::Process 0x00C6FE10** (retail_asm, /GS, 507 b) -- libpk MMX gain-ramp mixer, slot 2: under the effect lock
  it either cross-fades or mixes at constant gain. Shape: the static hand-MMX ramp kernel 0x00C6FC80 (pmulhw, which VC7.1 cannot
  emit) is repeated with its MixQuadsMMX caller. The three out-of-line MMX helpers MixGainRampUpMMX / MixGainRampDownMMX /
  MixGainMMX (0x00C6FD80 / 0x00C6FDB0 / 0x00C6FDE0) are written above Process with `CRITICAL_SECTION* section = &Lock.Section`.
- **CVtable_012bed7c::Process 0x00C6CC60** (648 b) -- libpk multi-tap delay, C path. Shape: the landed SSE2 Process 0x00C6D630
  body over the landed C tap mixers, which are defined above Process. VC7.1 itself inlines MixDelayTap at the first two sites only,
  as retail does. The block copies are the memcpy intrinsic.
- **deflate_fast 0x00C013C0** (909 b) -- zlib 1.2.1 verbatim. Shape: compile as C (`// Flags: /TC`). In C++ the d_code
  conditional is an lvalue and VC merges the arms (922v909). The `local` statics longest_match / longest_match_fast / flush_pending
  are repeated with their landed bodies (VC private conventions).
- **NScript::CQ_ArenaHoldingScriptScript::Main 0x00CF0790** (960 b) -- region wait, six bindings, then the Arena door is made
  unusable and persistent. Shape: the landed OakValeBanditRaid Main with a function-scope inline CTimer, over
  engine/CLevelScriptBase.h.
- **NScript::CQ_HerosOldHouseScript::Main 0x00D89E60** (976 b) -- the quest fails once if the fisherman's wife is attacked while
  the hero is helping, then it is deactivated. Shape: the SetQuestAsFailed reason is the IMPLICIT `L""` -> const CWideString&
  conversion; an explicit CWideString(L"") loads Game first (80 lines).
- **NScript::CQ_PrisonWardenGameScript::Main 0x00DDDE20** (1021 b) -- four bindings (flag 0), teleport the hero to
  WardensOfficeStart and face its GetAngleXY, wait for PrisonOffice, spawn WardenKeyMiniGame. Shape: the HerosOldHouse model with
  engine/CScriptThing.h and `inline ~CScriptThing() {}`, so the two GetThingWithScriptName temporaries expand their dtor inline.
  Matched on the first probe.
- **NScript::CPersonalScript_EmptyScript::CVillager::Main 0x00EF8D30** (1013 b) -- under a "Me, Crate and CrateHomeTarget alive"
  predicate it takes Me under scripted control. Whenever the crate has strayed more than 10 from home and nobody carries it,
  Me picks it up and, if Me carries it, walks it home, drops it and unsheathes. Shape: the landed CUndeadFellow::Main model
  WITHOUT its `#pragma inline_depth` regions. A plain inline ~CScriptThing from the header is expanded by VC7.1 at the first
  return only, as retail. The crate test is an early `if (!Over || Carried("")) { Game->NewScriptFrame(); continue; }`; the
  positive `&&` form inverts the materialised bool (85 lines).
- **REPAIR CEngineSubPrimitiveSpriteTrail ctor 0x00BC9C30** (34 b) -- see below.
- **REPAIR CBankFile::WriteRecoveryFile 0x009CDF50** (/GS, 246 b) -- see below.

### Repair rows: what was wrong in the landed sources
- 0x00BC9C30 (rebuild/src/compiled/00/bc/CEngineSubPrimitiveSpriteTrail_CEngineSubPrimitive2DClockSprite_00bc9c30.cpp):
  - There was no `// Leaf:` line, so vland graded the largest function, the out-of-line base ctor, not the row.
  - The parameter was spelled CEngineInternalPrimitiveBase* where Ego_r/FableWin give CEngineInternalPrimitiveSpriteTrail*.
  - The base class was a local CBaseClassNonCopyable model. It is now engine/CBaseClassNonCopyable.h.
  - The repaired source has the inline CEngineSubPrimitiveBase(parent) ctor and an empty derived body.
- 0x009CDF50 (rebuild/src/compiled/00/9c/CBankFile_WriteRecoveryFile_009cdf50.cpp):
  - There was no Leaf line.
  - The body was defined on an invented CBankFileRuntime, and its callees were spelled on CBankFileRuntime / CBankFile_Retail.
  - The "BREC" signature was memcpy'd from a global into a char[5], which put the buffer below the file object. That is the
    8-byte frame shift (locals at esp+0x1c/0x34 where retail has 0x14/0x2c).
  - Fix: the PDB CBankFile class, CDiskFileWin32 with an inline default ctor and an inline virtual dtor that calls Close() (over
    CAFile with an inline ctor), and `char id[5] = "BREC";` initialised from the literal. /GS then puts that buffer directly under
    the cookie.

## Parked for Fable 5.1 (3)
- 0x00C6E8F0 CVtable_012bedb8::Process DIFFER(673v673): LeaveCriticalSection's argument register (edx v eax) at both exits and the constant-arm gain-record pointer register (ecx v eax/esi, retail's dead `add eax,4`) | tried: section local / none / inside Enter, crossfade early return, lock guard, indexed / post-increment Mix bodies, member constant mixer, long** outputs, in-class Mix, `!= false`, /G7 (12 probes, best 19 normalised lines) | next: make the inlined Mix keep its ramp pointer live (gain read through the pointer inside the loop, or an SGainRamp member Mix)
- 0x00C6DBA0 CVtable_012bed90::Process DIFFER(689v689): same residue as 0x00C6E8F0, the Leave register and the record pointer register (retail keeps dead `add eax,4` / `add eax,8` / `add esi,0xc`) | tried: 14 probes (non-const ramps, lock guard, section forms, long**, /G7, `!= false`, indexed Mix, `m_pCurrent + k`, ramps local, const SGainRamp&, block-scope ramp pointer; best p4 crossfade early return) | next: as 0x00C6E8F0; out-of-line Mix copy 0x00C6DB60 is cdecl (source, count, ramp, accumulator), which confirms the argument order
- 0x0099D8C0 ErrorDialogProc DIFFER(685v689): centring arithmetic only. Retail reads both desktop extents first and computes `desk/2 + -(dlg/2)` (`sar; neg; add`); VC7.1 here gives `sar; sub` with the dialog extents first. Two separate tagRECT locals also swap the frame (dialog RECT at +0 v retail +0x10); `tagRECT rects[2]` fixes the frame | tried: 19 probes (declaration order, scope, names, width/height locals, one-expression MoveWindow, helper (not inlined), pointer local, array, `-(w/2) + d/2`) | next: a same-TU centring helper VC7.1 inlines, or int locals for the two halves; decide whether the RECT array is acceptable as retail's shape

## Skipped
None. All twelve rows were attempted.

## Label corrections
- 0x00C6CC60 `_gapscan::sub_00c6cc60` -> CVtable_012bed7c::Process (descriptive libpk; slot 2 of 0x012BED7C)
- 0x00C6DBA0 -> CVtable_012bed90::Process (slot 2 of 0x012BED90)
- 0x00C6E8F0 -> CVtable_012bedb8::Process (slot 2 of 0x012BEDB8)
- 0x00C6FE10 -> CVtable_012b8ac8::Process (slot 2 of 0x012B8AC8, the MMX table over CVtable_012bedb8)
- 0x0099D8C0 -> ?ErrorDialogProc@@YGHPAUHWND__@@IIJ@Z (Ego_r donor order + LocalizedDialogBox DLGPROC)
- 0x00C013C0 -> deflate_fast (zlib 1.2.1 configuration_table[1..3])
- 0x00CF0790 / 0x00D89E60 / 0x00DDDE20 -> ?Main@CQ_ArenaHoldingScriptScript@ / ?Main@CQ_HerosOldHouseScript@ /
  ?Main@CQ_PrisonWardenGameScript@NScript@@UAEXXZ (vtable slot 2 + Ego_r)
- 0x00EF8D30 -> ?Main@CVillager@CPersonalScript_EmptyScript@NScript@@EAEXXZ (entity vtable 0x012F6518 slot 1 + Ego_r)
- 0x00BC9C30: manifest label "CEngineSubPrimitive2DClockSprite" is wrong; the row is the CEngineSubPrimitiveSpriteTrail ctor
  (vtable 0x012A69D4 RTTI)
- Landed callee spellings proven wrong (NAME_FIXES.tsv): 0x009CBFE0 GetRecoveryFilePath and 0x009CDA70 InternalWriteEntryList are
  CBankFile members; 0x00994400 is ??0CFileDataOutputStream@@QAE@AAVCAFile@@@Z (by reference, not a pointer); 0x00994480 is
  the virtual ?Write@CFileDataOutputStream@@UAEXPBXJ@Z; 0x00E241B0 is ??1CIsThingAlive@NScript@@UAE@XZ, not the landed
  CScriptThingFunctor_00e241b0 (vland RELOC_NAME); 0x00DDE230's landed Leaf is the bare "WardenKeyMiniGame".

### Callee names
- libpk helpers, descriptive. The unlanded bed90 / bedb8 gain mixers are static members MixRampUp / MixRampDown / Mix
  (0x00C6DA40 / 0x00C6DAD0 / 0x00C6DB60 and 0x00C6E790 / 0x00C6E820 / 0x00C6E8B0); the Mix copies have no callers.
- MMX helpers 0x00C6FD80 / 0x00C6FDB0 / 0x00C6FDE0 are MixGainRampUpMMX / MixGainRampDownMMX / MixGainMMX.
- ErrorDialogProc globals, descriptive: g_bErrorDialogChecked 0x013BD6F4, g_hErrorDialogOwner 0x013BD0C0, g_bErrorDialogFatal
  0x013BCE3C, g_szErrorDialogTitle 0x013BCE40, g_szErrorDialogText 0x013BCA38, g_szErrorDialogURL 0x013BCEC0,
  g_szErrorDialogSystemInfo 0x013BD700.
- SConfigDetectionInfo fields: DriverName +0xC4, DriverVersion +0xC8, DeviceID +0xCC, SystemMemoryMB +0xDC, CPUSpeedMHz +0xE0,
  VideoMemory +0xE4.

### Header debt
- engine/CBankFile.h, CDiskFileWin32.h, CDataOutputStream.h and CFileDataOutputStream.h are flat pack(1) layouts with a
  __vftable data member and no virtuals or methods. 0x009CDF50 therefore models these classes locally; the headers need the
  polymorphic retail opt-in.
- engine/CEngineInternalPrimitiveBase.h includes <stddef.h>, which hung cl in this lane. 0x00BC9C30 declares the two
  internal-primitive classes as empty locals.
- The quest-script rows now use the shared headers. 0x00CF0790 / 0x00D89E60 use engine/CScriptThing.h (out-of-line dtor) and
  engine/CTimer.h (inline ctor/dtor defined after the include). 0x00DDDE20 / 0x00EF8D30 use engine/CScriptThing.h with an
  inline dtor, and 0x00EF8D30 also uses engine/CIsThingAlive.h / CPredicateBase.h / CEntityScriptBase.h. CPredicate_And and
  CEntityScriptBinding are still local templates (no shared header).
- g_pAudioMaster 0x0143BC2C has two spellings in landed libpk sources (`struct CAudioMaster` vs `class CAudioMaster`, U vs V
  in the mangled name). These rows use `struct`, the majority and the SSE2 sibling 0x00C6D630.
