# NF70 notes: name fixes, round 7, slice land1117fu (q10724..q10818)

All counts below come from STATUS.tsv and APPLIED.tsv.

- **Rows:** 183 in rows.txt (174 from the slice plus 9 claimed extras).
  - 88 ported
  - 55 not-a-binder
  - 40 blocked
  - 0 deferred
- **Entries:** 95.
  - 61 done
  - 10 done-already
  - 24 blocked
  - 0 rejected

Every ported row:
- is in rows/;
- graded RELOCATION_MATCH or MATCH in the private pS;
- passed vland (`recheck_final.log` is one run over all 88).

Blocked drafts are in work/blocked/.

## Descriptive names (no PDB / FSE / Ego_d / EgoCore name; one name per address)

- **0x004E8FCD** `CPersistContext::TransferHelper(CWeaponTrailGraphicSet*)`.
  - The descriptive spelling of its NF62-landed sibling, 0x004E9550 `TransferHelper(CObjectAugmentationParticleSet*)`.
  - No PDB names a one-argument transfer of this kind.
  - Decision: keep one helper name for the family rather than invent a per-type name.
  - The overlay opt-in `FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERHELPER_WEAPONTRAILGRAPHICSET` declares it.
- **0x00662450** `CThingCreatureBase::SetPItemToUnsheatheAfterCutscene(CThing*)`.
  - The repo header's spelling (NF66), which the landed caller 0x00694430 already binds.
  - Descriptive: no PDB has it.
  - The member it sets, PItemToUnsheatheAfterCutscene +0x150, is Ego_r's.
- **libpk, descriptive by library (libpk has no PDB or RTTI):**
  - 0x00C10140 `CItemDispatcher::FindItem(long)`: spelling from libpk/CItem.h (NF65).
  - 0x00C26580 `CPooledResource::Deactivate(CSoundEventManager*)`.
  - 0x00C2DDE0 `PKCTRList<CSegment>::~PKCTRList`.
- **Descriptive by library (STLport source names, not PDB entries):**
  - 0x004F9763 `_M_create_node`
  - 0x004F96D3 `_M_copy`
  - 0x0053C8F8 / 0x0053C91C `std::find`
  - 0x006C9B70 `std::__copy`
  - 0x006C31B0 `_M_find<_KT>`
  - 0x00467831 / 0x004676EF vector members

## One-definition entries

- **q10735 (0x0081F910 GetStrongestEngineThemeAt): done.**
  - The body is in the engine/CMap.h overlay under `FABLE_RETAIL_CMAP_GETSTRONGESTENGINETHEMEAT_BODY`.
  - With BODY alone it is an `inline __declspec(noinline)` COMDAT. The caller sees the body but still calls it, which gives VC7.1 its mod/ref knowledge.
  - The self row adds `_DEFINE` and gets an ordinary (strong) definition. The rows 0x0081FA20 and 0x0081FA90 lost their own copies.
  - Caveat for the final link: the self row's object holds the strong definition, and every BODY-only TU holds a pick-any COMDAT of the same name. Under MSVC's linker the strong definition wins and the COMDATs are discarded, with no LNK2005. If the build links with /OPT:NOICF and checks one-definition, treat it like the existing IsActive pattern.
- **q10763 (CBasicString<char>::operator< 0x00429950): blocked/header.**
  - The fix needs a body opt-in in `fable_string.h`. This round did not overlay that header: the attempt was refused by the session's permission check and nothing was written.
  - Exact change (also in `work/q10763_change.txt`). Insert it inside the `FABLE_RETAIL_CCHARSTRING` block, right after the closing `};` of `template<class T> class CBasicString`, before `class CCharStringData`:

```cpp
#if defined(FABLE_RETAIL_CCHARSTRING_LESS_INLINE) && defined(FABLE_RETAIL_CBASICSTRING_LESS_BODY)
template<>
#ifndef FABLE_RETAIL_CBASICSTRING_LESS_DEFINE
inline __declspec(noinline)
#endif
bool CBasicString<char>::operator<(const CBasicString<char>& other) const
{
    const char* b = other.PData;   // the other string's pointer first (retail's induction pointer)
    const char* a = PData;
    for (;;) {
        if (*a == 0 && *b == 0) return false;
        if (*a < *b) return true;
        if (*a > *b) return false;
        ++a; ++b;
    }
}
#endif
```

  - Then change two rows:
    - **0x00429950** becomes `#define FABLE_RETAIL_CCHARSTRING`, `_CCHARSTRING_LESS_INLINE`, `_CBASICSTRING_LESS_BODY`, `_CBASICSTRING_LESS_DEFINE` and `#include "fable_string.h"`. Regrade it: retail tail-duplicates `Compare(...) < 0` (setl), so the long-result form `long r; ... return r < 0;` may be what gives the 88 bytes.
    - **0x009EFB80** drops its `auto_inline(off)` copy and defines `FABLE_RETAIL_CBASICSTRING_LESS_BODY`.
  - The same body is copied, under local models, by the `_M_find` / `insert_unique` family:
    - 0x006C31B0
    - 0x00756350
    - 0x007ACC90
    - 0x00756250
    - and others

    They should take the opt-in as they move onto fable_string.h.
  - All 12 q10763 binders model CBasicString / CCharString (and their tree) locally under made-up spellings, so they are blocked/port.

## Coordinator decisions needed

1. **libpk CItem identity.**
   - libpk/CBankProcessor.h has `struct CEventTriggerRef { CNamedSet* m_pRegistry; CItem* m_pTrigger; }`.
   - The landed rows 0x00C1EE60, 0x00C1F320, 0x00C1F380 and 0x00C1F3C0 model the same 8-byte object as `CItemAction { CItemDispatcher*; CItem*; }`.
   - libpk/CItem.h says the registry is a CItemDispatcher.
   - A same-class rename draft grades RM + PASS (work/blocked/, work/vland_p.log). It was withdrawn because it renames a stand-in to a class that has a shared header.
   - The port needs:
     - the registry type settled (CNamedSet or CItemDispatcher);
     - a SetTypeBits declaration on CEventTriggerRef;
     - the CItemTable slot-0 type getter.
2. **CPlaybackSlot vs CCodaPlayback for vtable 0x012B4034 (q10731, 0x00C276B0).**
   - CPlaybackSlot.h lacks the loop fields this body reads.
   - The sibling landed rows use CCodaPlayback.
3. **Same-TU earlier-compiled callee bodies (0x00C265B0 / 0x00C265E0, as 0x00597006 / 0x00596CCE).**
   - These rows need VC7.1 to know that IsActive 0x00C265A0 preserves ecx.
   - Only a body compiled before the caller in the same object gives that. An `inline __declspec(noinline)` header body is compiled after the caller (DIFFER 28v17).
4. **Anonymous namespace GetPActionAnimSet (q10818).**
   - The self row 0x00693250 now defines `?GetPActionAnimSet@?A0x52b71103@@...`.
   - Its caller 0x00694430 references `?A0x86e294ff`.
   - In retail they are one TU (file-local helper). As separate rows they cannot link to each other, whatever the spelling. The landing has to put them in one TU or accept the per-TU hash.
5. **CTCMeshAutomaticEntityCreator::CreateBuilding 0x0072DF50.**
   - It is not in the PDBs. Its siblings CreateObject and CreateParticle are private (AAE) in Ego_r.
   - The landed self row and the binder 0x0072E290 keep QAE (the binder now uses `PAV`, the class key Ego_r gives NGraphicAppearance::CTCGraphicAppearance).
   - It is probably AAE, but only one source (sibling inference) says so.

## Rows respelled while keeping a pre-existing local model

These rows are respelled only. Each keeps its own local model of a class that has a shared header, and the model is not new. Moving them onto the header is follow-up work.

| Row | Local model kept |
|---|---|
| 0x00506F30 | CWorldMap |
| 0x006994F0 | CGameCameraManager |
| 0x0069AC30 | CGameCameraManager, CCountedPointer |
| 0x0069A8B0 | CGameCameraManager |
| 0x00687B30 | CInputProcessGameBase, CInputEvent (CInputEvent.h has no GetEventType opt-in) |
| 0x00687BD0 | CInputProcessGameBase, CInputEvent |
| 0x00688A80 | CInputProcessGameBase |
| 0x00688EB0 | CInputProcessGameBase |
| 0x0072DD60 / 0x0072DDB0 / 0x0072E290 | CTCMeshAutomaticEntityCreator, CTCGraphicAppearance |
| 0x006AD9D0 | CThingCreatureBase (WorldUpdate made virtual; the class was already polymorphic, so the layout is unchanged) |
| 0x006C9B70 / 0x006C31B0 | std models given STLport's template arguments |
| 0x00693250 | a vector stand-in |
| 0x0080CA20 / 0x00838BC0 / 0x00798770 | CHitParameters |

## Other notes

- **0x00C10140** uses `reinterpret_cast<const unsigned long&>(key)` to hand slot 17 the key's own stack slot (retail `lea edx,[esp+4]`). This is a reference reinterpretation, not an address cast. MATCH.
- **0x006C9EE0.** Its baseline grade (the landed source) took the wrong function (a 9-byte ctor), so its RENAMES section-2 lines have `-` as the old symbol. The landed source spelled it `CPersistContext::Transfer<COpinionDeedRecord>` on local models.
- **0x006C9B70** keeps its in-TU `noinline` `_M_insert_overflow` (0x006C80E0) copy, as landed. Now it is protected with the allocator argument, which is the spelling the landed 0x006C8610 / 0x006C8730 bind.
- **0x00C1EE60** has an EH funclet, 0x01212900, which includes it by path. That row is unchanged (blocked), so the funclet is unaffected. Under this tooling the funclet grades DIFFER(128v11) both before and after, because the grader takes the parent.
- **Funclets.** No funclet or other row includes any of the 88 ported row files (grep over rebuild/src/compiled).
- **Extra rows claimed** (in work/extra_rows.tsv and rows.txt): 0x00C1F320 is the callee of 0x00C1F380 / 0x00C1F3C0. It was claimed, then left unchanged and blocked with the CEventTriggerRef group.
- **NAME_FIXES.tsv** lists the callers outside this slice that bind wrong spellings of addresses this slice fixed (12 lines).

## Rule slips and judgment calls (for the coordinator)

### Slips

1. **Stray write into the repo.** The vland run for 0x00506F30 was given a relative log path (`work/vland_l.log`). vl.sh `cd`s
   into the repo first, so it wrote `/home/user/FableDecomp/work/vland_l.log` (456 bytes, 12:38). That directory is git-ignored
   `work/`. It is left untouched, as no further repo writes are allowed. Please delete it. Later runs used absolute paths.
2. **Write outside the agent directory.** One write went to `/tmp/x1` (a `sort` output, 12:37). It has been removed.
3. **Refused write attempt.** One Bash call tried to create the `fable_string.h` overlay and the 0x00429950 row. The session's
   permission check refused it and nothing was written; q10763 is blocked/header instead.
4. **Non-zero exit.** Before the context compaction, one Bash call exited with code 2: a `grep -m1 '^#' ... 2>/dev/null` on a
   missing file, in a pdb.sh / leaf check.

### Judgment calls to accept or bounce

1. **Local STLport models respelled in place.** 0x006C9B70 gives its local std model STLport's real template arguments: a new
   local `std::_Nonconst_traits<V>`, `_Rb_tree_iterator<V, Traits>`, `vector<T, A = allocator<T> >` and a protected
   `_M_insert_overflow`. 0x004676EF does the same for its vector (`class allocator`, protected `_M_insert_overflow`), and
   0x006C31B0 makes its local `_M_find` a member template. These respell local models of std classes that have shared stlport
   headers. They follow the landed precedent 0x00756350 (local model, member-template `_M_find`). They are not ports onto
   stlport/. If that counts as renaming a stand-in to the real class, they become blocked/port like the CEventTriggerRef rows.
2. **Accessibility and constness changed on local models.** Several rows (table above) keep a pre-existing local model of a
   real-named class and only change a member's access or const-ness to the PDB spelling. No model was renamed, and no new
   local model of an engine class was added.
