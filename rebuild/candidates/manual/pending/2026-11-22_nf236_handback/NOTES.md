# NF236 notes (2026-11-22, round 11): nf235 follow-ups, the IsEventButton second spelling, the p27 held bindings

Base: the repo tree with nf235 and Codex pass 27 landed. Entries (ENTRIES.tsv ids): q12663 done, q12664 done, q12665 done (wider than
briefed), q12666 done, q12667 partly done (21 of 54 lines), q12659 not started.
Changed rows: 55 sources plus 6 test-only rows = 61 (rows.txt; 0x00435530 changes source and test). Headers: none (hdr/README.md), so no equivalence run is owed.
recheck_final.log: 61 of 61 MATCH / RELOCATION_MATCH + PASS (seven payloads under `timeout 900`; grp00 rerun after the 0x00435530 test fix; grp06 = the seven self rows added after the self-leaf census).
Locks: I wrote nothing in `$S/w22/nflocks` (brief: claims live in this folder; rows.txt is the claim list). Before each edit I checked the
lock dirs of every row I touched. None was held by AUTH24, C2T or another agent; 0x00494900 read `released`; the rest had no lock dir.
NF235's owner files on its nine landed rows are stale (0x00495620 0x00496070 0x00496CF0 0x00A0A110 0x00A0A150 0x00494370 0x00494350
0x00494360 0x00A09D80). I touched five of them, tests only. Decision 5 asks to release them.
Not touched: the AUTH24 rows, 0x00B4D370 / 0x00BD3070, 0x00B260B0 / 0x00B261D0, and `$S/w22/auth2*/` / `$S/w22/c2t*/`.
Tools: NF235's, re-pathed, plus these new ones:
- tools/ieb.py: the IsEventButton in-place respell.
- tools/ren.py: renames tokens on code lines only.
- tools/nsren.py: extern -> namespaced extern.
- tools/acc.py: access / const in-place respell.
- tools/bindat2.sh: multi-target bindat.
- tools/src.sh: handles both build_candidates formats.
Flags lines added: none. Descriptive names added: none (several descriptive names are replaced by PDB names).

## Entry 1 (q12663): 0x013B7D58 -> ?GOldGameComponentToDelete@@3PAVCGameComponent@@A -- done, 3 -> 1
absrefs census (work/absrefs_e1.tsv) found 9 sites in 4 rows: 0x00412F90, 0x004189C2, 0x0042EC7C, and 0x00496CF0 (already on the PDB
name). The extern line and its uses were respelled in 0x004189C2 (was g_pLastComponent), 0x00412F90 and 0x0042EC7C (were
g_pRetiredGameComponent). The 0x0042EC7C test's static model variable takes the same name.
Each row is RELOCATION_MATCH, and its symbol diff is the one UND data symbol (work/e1_sc.log). Table: two stand-in lines deleted.

## Entry 2 (q12664): 0x004065D0 -> CUserProfileManager::GetDefaultProfileName -- done
Proof for GetDefaultProfileName:
- Name: Ego_r 0x00405230 / FableWin 0x0187B2C0 ?GetDefaultProfileName@CUserProfileManager@@QBE?AVCWideString@@XZ (public const).
- Body: pushes L"Default" (0x0122DE80).
- Layout: Ego_r places it 0x20 before GetManualSaveBaseFileName (0x00405250), as retail 0x004065D0 sits 0x20 before the landed 0x004065F0.
- Binders: both (0x00496070, 0x004962C0) bind this spelling.

GetAutoSaveFileName's own address is retail 0x00406690:
- Ego_r 0x004052E0.
- The body pushes L"AutoSave" (0x0122DE9C).
- It is a landed row with a table line (witness 0x004A0AE0) and ten call sites.
So the catalog listed one name at two addresses.

The row:
- Ported onto fable_string.h and the existing CUserProfileManager.h DEFAULTPROFILENAME opt-in. 0x00406690 itself got the same port
  (leaf QAE -> QBE, SAVESLOTS opt-in, new header test): its callers all bind QBE. The local CWideString /
  CUserProfileManager models and the cast address are gone.
- Graded RELOCATION_MATCH. ??0CWideString@@QAE@PBG@Z still binds 0x0099B6B0 (this row is that line's witness).
- The literal binds the L"Default" COMDAT ??_C@_1BA@GHOECOCL@.., as 0x0040CEC0 already does, so 0x0122DE80 gains no spelling.
  0x004069E0's ?k_122de80 stand-in remains (NAME_FIXES).
- New header test.
- RENAMES section 1 has the file names. vland graded the row under the new catalog name: the payload name and oracle line were patched.

Link check (work/lnk: 0x004065D0 + 0x0040CEC0 0x004065F0 0x00406690 0x00496070 0x004962C0):
- One new LNK4006. 0x0040CEC0 (the CUserProfileManager ctor) defines GetDefaultProfileName inline in its row and emits a pick-any copy.
  At HEAD the owner row spelled another name, so there was no duplicate.
- The copy is EQ to the new owner (pk.py: 23 = 23 bytes, relocations bound, with the literal at 0x0122DE80).
- This is the nf235 ruling-1 pattern (decision 1).

## Entry 3 (q12665): 0x00687BD0 IsEventButtonReleased -> IBE -- done, 2 -> 1 (seven binders, not one)
The table keeps one witness per symbol, so I compiled all 53 landed callers of 0x00687A90 / 0x00687BD0 (callers.py + bindat2,
work/e3_bind.tsv).

Released (QAE binders):
- Seven rows bound QAE: 0x0068C760 0x0068C7F0 0x0068CAC0 0x0068F180 0x00690030 0x00690390 0x00690D30.
- Each row-local CInputProcessGameBase declaration was respelled in place to protected const (`protected:` ... `public:`, Ego_r / FableWin
  IBE). Every caller is a member of a derived CInputProcess* class.
- All seven are RELOCATION_MATCH; the only symbol diff is the UND callee.

Pressed (0x00687A90):
- 13 rows bound QAE. The other twelve are respelled the same way: 0x00690030 plus 0x00689C40 0x0068C6E0 0x0068D890 0x0068E2C0
  0x0068E8F0 0x0068EA60 0x0068ECA0 0x006909A0 0x00690C70 0x00691470 0x006A4040.
- That leaves 0x007E5230 CTCBetting::ProcessInput. It calls the protected member through a CInputProcessGameBase* from a non-derived class,
  which needs a friend. CTCTavernGame has one (CInputProcessTavernGame.h TAVERNGAME_FRIEND); none was found for CTCBetting.
- So 0x00687A90 stays at 2 spellings (13 -> 1 QAE binder). The table line is re-witnessed to 0x007E5230. Decision 2.

## Entry 4 (q12666): five header tests -- done (rows unchanged)
Each test copies the row's defines, includes and body verbatim, stubs only what the linker names, and seeds / reads the object at the
CTOR_DTOR offsets (no `#define private public`).
- 0x00A0A110: 3 cases. Base ctor once, self-linked sentinel, +4 dispatch; distinct sentinels; clear + base dtor on destruction.
- 0x00A0A150: 4 cases. Sole reference deleted; shared block 2 -> 1; empty entry; clear sees every entry null, then the base dtor runs once.
- 0x00495620: 4 cases. The member / base destructor order is SplashScreen, WorldToLoad, WorldNames x2, SaveToLoad, ProfileNames, then the
  base; it also checks the counted-pointer release / share.
- 0x00496070: 2 cases.
  - Default-profile load: the GetDefaultProfileName temporary goes to SetProfileName(.., true), mode 1, fields and key table checked.
  - Mode 12: ProfileNames refresh.
- 0x00496CF0: 4 cases. CreateGame (two frames, assignment sources, joystick, Init, GOldGameComponentToDelete); neither flag; release front
  end over CreateGame; Quit preset.
  - It keeps the row's in-place `class CSystemManager { ESystemReturn Update(); }` shape: the full header needs the Win32 types (nf235
    ruling 5).

## Entry 5 (q12667): the 54 held p27 lines -- 21 restorable, 33 listed
Census method:
- absrefs + callers over all 54 targets (work/e5_absrefs.tsv, e5_callers.tsv, e5_census.tsv).
- The spellings of the untouched binders come from objects (bindat2: work/e5_census_rest.tsv).
- Every changed row has an sc.sh HEAD-v-new diff (work/e5g_sc.log, e5a_sc.log, e5b_sc.log).
- Source grep for each retired name: no other landed source uses it (tests keep their own host-model names).
- Names: the global spellings are in all three PDBs (raw grep of the publics). Each name was checked against its p27 witness body (CEngineInit
  fields in 0x00435190) and Codex's NAME_FIXES cross-references (FableWin / Ego_r code sites), read for 0x013B86EC, 0x013B864A, 0x0137549C,
  0x013B8384, 0x01375459, 0x01375490, 0x01375741, 0x0143782C, 0x00445B90 / 0x00445BB0, 0x004BC800, 0x004C7E30, 0x00780400, 0x00B4B830 and
  0x006F2C00. For the access / const changes the second source is the other PDB: Ego_r and FableWin agree.

Self leaves: after the advisor's check I compiled the self rows of every respelled callee (work/selfleaf.tsv).
- Six still emitted the old spelling and got a leaf-only self respell in this hand-back (grp06; RENAMES section 1): 0x007267C0 ABE,
  0x005E9F30 / 0x005EC5C0 IAE, 0x0080F190 QBE (the definition takes const), 0x00665F90 IAE, 0x006A4D60 IAE.
- 0x00687A90 / 0x00687BD0 / 0x004BC800 / 0x009CE370 / 0x00C951B0 / 0x00C94FD0 already emitted the PDB spelling.
- 0x00637E70 / 0x00445B90 / 0x00445BB0 / 0x004C7E30 still do not (stand-in signatures or classes; NAME_FIXES). Those four are 3 -> 2, not 1.

On one spelling after this hand-back (14 addresses; the four self-leaf exceptions above are listed separately):

| Address | PDB name | Respelled from |
|---|---|---|
| 0x013B864A | GBuildRetailStaticMaps | g_bBuildRetailStaticMaps (0x004138D0) |
| 0x01375490 | GStaticMapQuality | g_CommandLineQ (0x004138D0) |
| 0x0137549C | GEngineMemoryInit | g_DefaultEngineMemoryInit (0x00413C50, 0x0042E204) |
| 0x01375459 | GAllowDataGeneration | g_WADBuildEnabled (0x00413C50 0x0042E204 0x00498490 0x00502170 0x00507C30) |
| 0x013B86EC | NGlobalConsole::GJossTestPhysics | g_flipMeshCollideNormals_013b86ec (0x00880730) |
| 0x01375741 | NGlobalConsole::GEnableRegionLockingSaveSystem | 4 -> 1 (below) |
| 0x00665F90 | PlayCreatureContinuousSounds IAE | QAE (0x008337B0) |
| 0x004BC800 | GetPositionMatrix QBE | QAE (0x006626E0) |
| 0x007267C0 | GetPossibleCollidedWithAreaWithImpulse ABE | QBE (0x00726BE0) |
| 0x005E9F30 / 0x005EC5C0 | ClearBetAndMoney / ClearBox IAE | QAE (0x005EC650) |
| 0x0080F190 | IsDecentAttackRange QBE | QAE (0x0080FAD0) |
| 0x00C951B0 / 0x00C94FD0 | @_book_unquantize@12 / @_make_words@12 | none: table only (below) |

- 0x01375741: g_bQuestFinishScreensEnabled (0x004A3740, 0x0061AC60), unsigned char g_questScreenFlag (0x004AF290) and char g_someFlag
  (0x00892F80, 0x00892FD0) all become bool; the bytes are the same.
- 0x00445B90 / 0x00445BB0 and 0x004C7E30: the old names are in neither PDB (made-up names).
  - For IsLocal the second source is thin: Codex NAME_FIXES line 45 plus the PDB name. The body reads byte +9 of the thing's type record.
    Decision 3.
- 0x00C951B0 / 0x00C94FD0: the old lines are stale. Their witness 0x00C954A0 already binds the C names (libvorbis 1.0.1 sharedbook.c,
  /Gr), so no row changes.

Count-reducing, line restored (7):

| Address | Retired | Still held by |
|---|---|---|
| 0x013B8384 | PRenderManager (0x0042DF9E 0x0042EC7C 0x00435530 0x00487570 0x00494900 0x0061AD60) | 0x0085CBD0 already binds GPRenderManager; four typed stand-ins remain (NAME_FIXES) |
| 0x009CE370 | CBankFile QAE (0x009CE8D0) | CBankFile_Retail stand-in (0x009CEA70) |
| 0x006A4D60 | CThingPhysical QAE (0x007032F0 + the self leaf) | ?ValidateInitialisation@CThingCreatureBase@@QAE_NXZ (0x006A9EF0, 0x00833A70) |
| 0x00637E70 | QBE (0x00637F40), 3 -> 2 | the self row's stand-in leaf |
| 0x00445B90 / 0x00445BB0 | IsSpellModeForced / IsExpressionShiftModeForced (0x006881D0), 3 -> 2 each | the self rows' stand-in leaves |
| 0x004C7E30 | IsMapwhoExempt (0x00637A10), 3 -> 2 | the self leaf ?GetTypeName@CThing@@QAEDXZ (catalog CThing::GetTypeName) |

Not done (33 lines), with the reason:

| Lines | Reason |
|---|---|
| 0x013D27D0, 0x013BB6E4, 0x013BB6DC, 0x013BB6EC, 0x01296F14 | The anonymous-namespace hash `?A0x...` is per TU: the older rows' TUs emit other hashes, so no respell can make them equal. Needs a table-normalisation ruling (decision 4). |
| 0x006F2C00 | The same hash issue, and the old binder 0x006F3530 names another functor (??RCRotateThing). |
| 0x005BFA44 | Old binders use the CTCInventoryBase_Retail / CItemVector stand-ins: needs the header port (vector<NInventory::CItem>). |
| 0x00A3AA40 | Stand-in CCompressorZlib_Retail (0x00A3AC40, 0x00A3ADD0); a header port (CCompressorZlib.h). |
| 0x009F32A0 | Stand-in CSurfaceNative (0x00B27EE0, 0x00F37390); a CSurface.h port. |
| 0x00700820 | Stand-in COpinionReactionManager_Retail / CThing_Retail (0x00890A50). |
| 0x008ED1F0 | Stand-in CTCContainer_Retail (0x00898AC0, 0x0089E780). |
| 0x00780400 | Stand-ins in 0x006A8A80 (?HasFreeActionPoint@CTCActionPoints) and 0x0089BD90 (CTCFlammable_Retail); a CTCFlammable.h port. |
| 0x00592BCE | _Rb_tree::_M_erase: the CRbTree stand-in in 0x00625ED0, plus 0x00593235; an STLport header port. |
| 0x00A97180 | _M_fill_insert: the CBoneWeightVector stand-in in 0x00AA6C70; an STLport header port. |
| 0x00660787 | RemoveThingFromTaskList: the CTaskList stand-in in 0x00659EA9; an STLport header port. |
| 0x005CBDFA | ??$?8: the stand-in ?IsThing@CTCInventoryWeapons in 0x005C2E82; a CIntelligentPointer header port. |
| 0x00685C00 / 0x008F00F0 | GetSubDef<T>: the old binders use non-template stand-ins (CDefPointer by reference / CThingDefHolder::GetDef); a CDefClassBase template port. |
| 0x00740040 | The placeholder class CClass_00740040 in 0x00742790; a header port. |
| 0x006B0C10 | The stand-in free function ?CTCPhysicsNavigator_Notify in 0x00726750. |
| 0x0098E770 | The old binders model CFileObject at global scope (PDB: nested, private); a CThreadedFileManager.h port. |
| 0x00B92450 | 0x00B936E0's local model (long parameter, public); its own self leaf is also a stand-in; a CEngineSceneGridCell.h port. |
| 0x0091C220 | A parameter type change (CThing* -> CThingCreatureBase*; the call passes PTarget.Get()); a CAIStateGroup_NymphAttack.h port. |
| 0x00843A80 | Four binders, pointer-v-reference parameter in two old spellings; a CCreatureAction_PlayAnimation.h port. |
| 0x00930D20 | AAV is in FableWin only (one PDB); 0x0092FAB0 binds ABV, and 0x0093A080 is a stand-in. Blocked on a second source. |
| 0x00B4B830 | The old binders call it as the virtual OnReleaseDefaultPoolResources (0x00B4B880 is that function itself). Respelling changes what the dtor / ChangeVideoOptionShadowQuality source calls, which is a body claim, not a respell. |
| 0x013D2838 | PActiveProcess, a protected static with 13 binders, non-members among them; needs a header accessor port. |
| 0x013B8774 | A protected static of CDisplayEngine: three of four binders are non-members (CTCGraphicAppearance, CThingCreatureBase), with no friend scaffold. Needs evidence of retail's access path. |
| 0x0126DEAC | A private static const of CTCDrunkenness, read by the non-member 0x007ED2C0. The same access issue. |
| 0x0143B8D8 | A private static, 7 binders; not attempted. |
| 0x01436E28 | NEngineInternal::GPDefinitionManager, const pointer to const, 6 binders; not attempted (time). |
| 0x006A1620 | 8 binders, 6 spellings (two say IsThingCarryingMeleeWeapon); not attempted. |
| 0x0143782C | The definer 0x01224830 (and 0x01224600) still use `g_CUnproven*` placeholder names. It is a TU-local static (S_LDATA32), so it needs those initializer rows re-authored (NAME_FIXES). |

## Entry 6 (q12659): 0x00418DCA -- not started
The brief allows it only with the full 469-includer equivalence proof, and context went to entry 5.

## Proof summary
- Recheck: recheck_final.log, 54 / 54 RELOCATION_MATCH + PASS.
  - WARNs as at HEAD: SHARED_HEADER_AVAILABLE on in-place local models, and 0x00496070's RELOC_NAME on 0x004065D0 (the catalog label,
    fixed by RENAMES section 1).
  - One pre-existing RELOC_CONFLICT in 0x0068D890 (?PushLookAround, NAME_FIXES finding; that binding is the same at HEAD).
- Symbol diffs: every changed row's defined symbols equal HEAD's, with these exceptions: 0x004065D0 / 0x00406690 rename their own leaf
  and add the literal COMDAT, and the six grp06 rows rename only their own leaf (work/e5s_sc.log, e6_sc.log).
  So no row emits a new pick-any function copy (pk.py has nothing new to compare). The two copies the link checks found (0x0040CEC0 GetDefaultProfileName, 0x00407E10 GetAutoSaveFileName) are EQ to their owners.
- Link: 0 LNK2005. +1 LNK4006 for 0x004065D0 (entry 2). 0x00406690 is net 0: the HEAD duplicate against 0x007D7C10 goes (same QAE leaf, a
  different body) and 0x00407E10's EQ inline QBE copy takes its place (work/lnk/link2.txt).
- Census: per entry above, self leaves included (work/selfleaf.tsv). No address grows; 0x013B7D58 3 -> 1, 0x004065D0 2 -> 1,
  0x00406690 2 -> 1, 0x00687BD0 2 -> 1, 0x00687A90 2 -> 2 (12 binders moved), plus entry 5's counts.
- Headers: none changed.

## Rule slips
- STATUS / APPLIED were written per batch of rows, not after every row.
- The first recheck of 0x00435530 hit LINK_FAIL: its HEAD test links against the row and defined ?PRenderManager. Fixed in
  rows/00435530_test.cpp and regraded. grp00 was regraded once more because a regenerated payload had lost the 0x004065D0 name patch.
- No `$S/w22/nflocks` claims were written (per the brief; listed above instead).
- One foreground `sleep 60` while waiting for the final recheck (the rule says to wait on the PID).
- Two census loops (bindat2) ran under outer `timeout 1800` / `3000` wrappers. Every grade inside them ran under `timeout 300`.
- The first entry-5 census counted callers only. Self leaves were added after the advisor check; that is what added the seven grp06 rows.

## Decisions for the coordinator
1. 0x004065D0: +1 LNK4006. 0x0040CEC0's inline GetDefaultProfileName copy (EQ) now meets its out-of-line owner. Accept, as nf235 ruling 1?
2. 0x00687A90: the last QAE binder is 0x007E5230 CTCBetting::ProcessInput, a non-derived caller of a protected member. Leave it until a
   friend / access path is proven, or allow a CInputProcessGameBase.h friend opt-in?
3. 0x004C7E30 CThing::IsLocal: the PDB name plus Codex's caller evidence replace the made-up IsMapwhoExempt. The catalog still says
   CThing::GetTypeName (self leaf ?GetTypeName@CThing@@QAEDXZ). Accept and queue the catalog / self rename, or hold that line?
4. The six `?A0x...` anonymous-namespace lines (and 0x006F2C00): per-TU hashes cannot be respelled equal. A table rule (normalise the hash,
   or allow one spelling per TU) is needed.
5. The stale NF235 lock owner files on its nine landed rows: please release them.
6. 0x007D7C10: its catalog name CUserProfileManager::GetAutoSaveFileName is a propagated label (a CTCTavernGameSpotTheAddition slot-42
   getter; NAME_FIXES). Queue a name lookup for it?
