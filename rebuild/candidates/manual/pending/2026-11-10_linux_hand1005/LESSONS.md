# Chunk 1005 lessons (GOTCHAS-ready)
- VC7.1 frame slots: a `CText* t = static_cast<CText*>(cp.Get()); t->SetValue("...")` named local makes the object pointer load (and its spill around the CCharString temp) come before the argument temp; reusing ONE local for the three subtitles and separate locals for the descriptions reproduced retail's [ebp+68h] / [ebp+6Ch] spill slots (0x00574885, 0x005759BF).
- VC7.1 evaluates the later-declared callee of `f() < g()` first: declare GetAbilityMaximumLevel before GetAbilityLevel (opt-in MAXLEVEL_FIRST) to get retail's call order (0x005726A0, 0x00570332).
- A class's NEW virtuals take slots in header declaration order: CTCInventoryExperience 86..90 = OpenInventoryInternal, OpenInventoryInternalPC, OpenInventoryOld, CloseInventoryInternal, CloseInventoryInternalPC; a wrong order shows as `call [eax+160h]` v `[eax+164h]` (0x0056F30B).
- A PC twin of a matched console row: copy the source, swap UI_* -> PC_* names and callees, add the PC_UI_FRAME FindChild/AddPositionChild calls in retail order -- first-try RELOCATION_MATCH (0x005759BF from 0x00574885).
- 0x0041D165 / 0x0041D122 are NUISystem::CManager::OnCloseMenus / OnOpenMenus (static fastcall, PDB) -- not free functions (0x0056F30B).
- FABLE_RETAIL_CDELETION_BODIES + an explicit `next = it; ++next; erase(it); it = next` loop reproduce the STLport erase(iterator) call pattern (0x005726A0).
- A PAREN around a float sub-expression `(W * 0.5f) - s.X * 104.0f` fixed the fsubp operand order (0x00576C4F / 0x00576ED1).
