# NF183 header overlay

## engine/CDynamicObjectHeap.h
- Base: the repo working-tree copy at the start of this round (orig/engine/CDynamicObjectHeap.h).
- Opt-in added: `FABLE_RETAIL_CDYNAMICOBJECTHEAP_FREE_INLINE` -- `CDynamicObjectHeap<N,M>::Free(void*)` with its inline body (FableWin.pdb
  ?Free@?$CDynamicObjectHeap@...@@QAEXPAX@Z; retail inlines it: --AllocCount, push on FreeList, RefCountedRelease 0x00F36CA0 at zero when
  release is enabled). Without the macro the `#else` branch is the original `void Free(void*);` line, so the default path is token-identical.
- Used by rows 0x00B49AA0, 0x00B49A40, 0x00B49FE0, 0x00B47980 (q4502 / q4505).
- pin_asserts.py: pinned 0 (the header has no FABLE_STATIC_ASSERT).
- Landed includers of the header (grep rebuild/src/compiled): 0x00B47FD0 (through engine/CEngineLightingManager.h, which also includes
  it under FABLE_RETAIL_CENGINELIGHTINGMANAGER_SHADOWED_UPDATE_PASS23). None defines the new macro, so the opt-in changes none of them.
- The ?Free@?$CDynamicObjectHeap@... COMDAT it emits was already emitted by the rows' local models at HEAD; reloc_bindings.tsv maps no
  address to it (no landed owner row).

## Not in the overlay
- engine/NUISystem_CText.h opt-in FABLE_RETAIL_CTEXT_GETDATAFROMDEF was drafted for row 0x0054ED90 and withdrawn with that row (pick-any
  hold, see NOTES.md); the draft is in work/blocked/hdr/.
