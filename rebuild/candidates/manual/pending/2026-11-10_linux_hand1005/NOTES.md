# Chunk 1005 -- CTCInventoryExperience (11 rows)

**Grading:** every grade and probe ran with `FABLE_HDR_OVERLAY=$A/hdr` exported (tools/pq.sh, gall.sh). The overlay is
documented in `hdr/README.md` (HEAD copies + opt-in additions; rechecked against HEAD after Codex pass 16).

## Result: 7 RELOCATION_MATCH, 4 parked DIFFER
| Row | Grade | Identity / lever |
|---|---|---|
| 0x005726A0 RecheckAvailabilities | RELOCATION_MATCH | PDB AAE; `optimize("y",off)+("s",on)`; CDELETION_BODIES, MAXLEVEL_FIRST (declaration order), explicit next-iterator erase |
| 0x00576C4F UndoBuyAbility | RELOCATION_MATCH | PDB QAE; UNDO / CYESNO_CALLBACKS / SETLEVEL opt-ins; centre position written `(W*0.5f) - scale.X*104.0f` |
| 0x00576ED1 UndoBuyStat | RELOCATION_MATCH | twin; MODIFYLEVEL opt-in |
| 0x00577216 OpenInventoryInternal | RELOCATION_MATCH | vtable 0x0124F11C slot 86 (catalog `vfunc_86`); same-TU GetOriginalExperience body |
| 0x00574885 ConstructCategoryStrength | RELOCATION_MATCH | PDB AAE; frame lever: one `CText* subtitleText` reused by the 3 branches, one `CText*` per description |
| 0x005759BF ConstructCategoryStrengthPC | RELOCATION_MATCH | console source + PC_* names, the *PC callees, `list->FindChild("PC_UI_FRAME")` once and `frame->AddPositionChild(x.Get())` (slot 124) for every title/ring/subtitle/description; first try |
| 0x0056F30B CloseInventory | RELOCATION_MATCH | slot 31; rewritten on shared headers (CIEngine Enable3DSceneRendering / CutscenePreload, CWorld::SetAsStarting3DRendering / GetMessageEventManager, CUserProfileManager::GetSoundVolume, CASoundManager SetGroupVolume / slot-9 owner, CManager PlaySoundW(GetMiscThingsDef()->SoundExit, 1), CloseInventoryInternal(true), CManager::OnCloseMenus) |
| 0x0056EF2D UpdateMenuScrollButtonsItem | DIFFER(66v66) | slot 29 identity proven; register tie only (retail loads statIndicatorMax into edi before the up-arrow setg); ~19 spellings |
| 0x00570332 AddAbilityToList | DIFFER(3918v3918) | frame packing only: frame 0xDC v 0xD8, textBank in its own slot (retail shares it with showMorality at [ebp+5Ch]); MAXLEVEL_FIRST fixed the level compare; 10 grades used |
| 0x00571280 AddAbilityToListPC | DIFFER(3962v3962) | same frame packing as the console twin |
| 0x00577AC0 OpenInventoryInternalPC | DIFFER(2157v2157) | integer sum PointsGeneral..Will operand order (retail +268,+26C,+264,+260; ours +264,+260,+268,+26C) invariant under ~16 spellings; the console twin 0x00577216 got its own retail order naturally |

Relocation identities of every matched row checked with tools/relchk.py against rebuild/oracles/reloc_bindings.tsv (no
conflicts except the spellings listed in NAME_FIXES.tsv); every string literal checked against the retail exe (tools/str.py).

## Identities proven
- vtable 0x0124F11C (read from the exe): 29 0x0056EF2D UpdateMenuScrollButtonsItem, 30 0x0056FAD7 OpenInventory, 31 0x0056F30B
  CloseInventory, 86 0x00577216 OpenInventoryInternal, 87 0x00577AC0 OpenInventoryInternalPC, 88 0x00574576 OpenInventoryOld,
  89 0x0057770A CloseInventoryInternal, 90 0x005778CA CloseInventoryInternalPC (Ego_r.pdb declaration order of the five new
  virtuals). The landed `vfunc_89` / `vfunc_90` rows are CloseInventoryInternal(PC); the landed OpenInventory 0x0056FAD7 is
  virtual (UAEXXZ, not QAEXXZ) and its slot-87 `SetScreenActive` is OpenInventoryInternalPC.
- 0x0041D165 = ?OnCloseMenus@CManager@NUISystem@@SIXXZ, 0x0041D122 = ?OnOpenMenus@CManager@NUISystem@@SIXXZ (FableWin + Ego_r,
  body sizes equal); the landed `CloseMenus` / `OnCloseMenus@@YI` / `ResetMenus` (descriptive) spellings are wrong.
- 0x0066E7BD GetCurrentScreen returns `const CBase*` (QAEPBV); 0x0066E799 is SelectScreenOfType(EScreenType).
- 0x005EE01A = CTCInventoryAbilities::SetAbilityLevelTo (not CTCHeroAbilities::SetAbilityLevel).
- 0x0057914C / 0x00579114 are STLport `_Rb_tree::_M_find` instances, 0x00579EA3 / 0x00579ECC `_Rb_tree::clear` instances (the
  landed CAbilityUndoMap / CUndoMap / CStatUndoMap stand-in spellings are wrong). All in NAME_FIXES.tsv.

## Callee names used (shared headers)
CTCBase::GetWorld 0x00686A70; CWorld::GetFrame 0x0049D870 (inline SetAsStarting3DRendering); CMainGameComponent::Get /
GetConstantFPS / GetDisplayEngine / GetTextBank; CDisplayEngine::Get3DEngine; CIEngine slots 56 / 76; CUserProfileManager::Get
0x0040D2A0; NUISystem::CManager::GetInstance 0x0041E5F2, CreateComponent 0x0041DB1D, CreateMenuEntryComponent 0x00428494,
PlaySoundW (40), GetMiscThingsDef (42); CComponent::FindChild (3), AddPositionChild (124); CList AddNonScrollingChild /
InitialiseOffsets / AddScrollingChildAndSetOffsets; CText::SetValue(CCharString, bool, bool) (145); CViewport::AddFakeChild (142);
AddStatToList(PC) 0x005729D8 / 0x00573792, AddAbilityToList(PC) 0x00570332 / 0x00571280, AddMagicCategoryToList(PC)
0x0056F69F / 0x0056F8A4, AddSoundAction 0x0056F615, GetXString 0x0056EA4E.

## Header debt
- CASoundManager.h names are descriptive (`SetGroupVolume`, `Virtual09_00a3e3a0`; the header says it is not landable as named until
  the owner accepts descriptive slot names). CloseInventory 0x0056F30B uses them, plus the local `CASoundSampleOwner_Unproven`
  (slot 9 StopSample) exactly as the landed 0x0041E6D3, and a local `CMessageEventManager` (AddMessage 0x00687540, Ego_r mangling)
  as the landed 0x0096DBC0 -- no shared header exists for either. EMessageEventType has no enumerator names (0x4A cast).
- The VIRTUALS opt-in should eventually become the default declaration order of CTCInventoryExperience's new virtuals.

## What each unmatched row needs next
- 0x0056EF2D: a spelling that makes VC7.1 load `statIndicatorMax` before the up-arrow `setg` (register tie edi/edx).
- 0x00570332 / 0x00571280: the frame packer must give the textBank reference no slot of its own (share with showMorality); try
  moving/renaming locals around the cost/morality block or a scoped block for the description text.
- 0x00577AC0: the commutative int sum order; probably needs the exact original expression shape of the points snapshot.
