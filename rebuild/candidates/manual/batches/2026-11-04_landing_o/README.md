# Landing 2026-11-04o: chunk 753 (small NUISystem slots, adjustor thunks, libpk and vcall thunks), 19 landed

## Result
- **Landed 19 / 19** (`logs/land.log`). Before landing, all 19 were graded read-only on the merged tree with no overlay
  (`logs/new2.log`): 7 MATCH and 12 RELOCATION_MATCH, all PASS.
  - No row has asm, a helper, a pragma or `volatile`.
  - The agent's notes are in `agent_NOTES.md`.
- **13 of the 19 catalog labels were wrong.** The real names, from vtables, the FableWin inline order and callers:
  - NUISystem::CSprite::GetWidth / GetHeight (slots 147 / 148; offsets are the Ego_r offsets minus 0x30).
  - NUISystem::CComponent::SetName 0x0052F0F0 (slot 83 of CComponent's own vtable; the address appears in 43 vtables).
  - NUISystem::CEditBox::SetAllowedCharacters / SetForbiddenCharacters (slots 154 / 155).
  - NUISystem::CFrontEndList::ChangeState (slot 48; its cases are written in retail's order).
  - The adjustor{4}/{24} destructor thunks of CSprite, CEditBox and CFrontEndList. CFrontEndList's destructor is inline, so its
    thunks come from the constructor TU.
  - The "CIsThingAlive / CThingFilter_IsValid" functors were not functors:
    - three libpk free-list helpers (CSoundEventSet::IsBusy / GetFreeSlot, CSoundGroupPool_00c21f20::Allocate) and
      CRegionMap::GetCount. libpk has no PDB, so these are descriptive names that reuse the landed spellings.
    - three VC7.1 stdcall vcall thunks `??_9@$B<off>@AG` for IMediaSeeking slots 10 / 19 / 11, from DirectShow
      CPosPassThru::GetSeekingLongLong.
  - The SHARED_HEADER_AVAILABLE CBaseObject warning on the thunk rows is a false positive: that header is Lionhead's class.

## Headers
- Three new headers, none existing changed: `engine/NUISystem_CSprite.h` (0x184), `NUISystem_CEditBox.h` (0x18C),
  `NUISystem_CFrontEndList.h` (0x1FC, on CList with FABLE_RETAIL_CLIST).
- hdrcheck2: NEW for all three (`logs/hdrcheck2_pre_merge.txt`). No landed source includes them, so there are no siblings.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04o.tsv` (31 labels). `apply_labels.py` made 31 catalog and 31 manifest renames:
  the 19 rows plus 12 landed gapscan or placeholder rows that are real CSprite / CComponent / CEditBox slots, for example
  0x0052F100 CComponent::GetName and 0x00540CF0 CEditBox::GetText (both were "CBankFile::GetBankHandle").
- **Queued PRIORITY** (tag `land0411o`): the chunk's NAME_FIXES.
  - CRegionMap::GetCount replaces CAliveHelper_00c36cf0::Check.
  - Stale reloc_bindings spellings: CPanelItemSet::IsBusy, and CCharString / CWideString operator=.
  - CSprite::SetGraphicIndex is landed as a global.
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.
