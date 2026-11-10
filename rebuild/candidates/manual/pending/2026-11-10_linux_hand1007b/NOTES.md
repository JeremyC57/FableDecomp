# Chunk 1007b notes (retry of the 11 CTCInventory rows chunk 1007 parked)

All grades and c2prio probes ran with `FABLE_HDR_OVERLAY=$A/hdr`, one compile at a time, each wrapped in `timeout 240` (grades) /
`timeout 400` (c2prio). Grades: ATTEMPTS.log (19 this pass); probes: PROBES.log.

## Matched
| row | grade | grades | lever |
|---|---|---|---|
| 0x00589A30 OpenPCMapMenu | RELOCATION_MATCH | 2 | One named local per child component instead of one reused `scroll` / `clickable`. c2prio: split pieces are created in candidate-id order and the reloads after a clobber follow piece order; the new candidate ids put `this`'s split before `i`'s. Shared headers only. |
| 0x0058F649 OpenPCInventory | RELOCATION_MATCH | 4 | Separate skills / photojournal entry and arrow locals (frame packing puts the manager in [ebp-4]) + `FABLE_RETAIL_CDELETION_BODIES` (visible noinline CDeletion ctors fix the schedule around SetDeletion). |

## Unmatched (differ/)
- **0x00584B86 AssignThingPC** DIFFER(854v854), 34 lines. Rewritten from a local model onto shared headers (new CAbilityDef.h,
  ASSIGNPC opt-in). Levers that moved it: FindChild per arm; same-TU `auto_inline(off)` GetDef<T>/GetSubDef<T> bodies +
  CDEFPOINTER_RELEASE_POINTEE (short `dec [ecx+4]; jne` releases, right frame); `const CWorld& world = PeekWorld();` as its own
  statement (GetDef pushes after the call). Residue: the swap-action ability arm is laid after the slot push_back's init jmp in retail
  (deferred cold block, hand572 model: at a chain break the earliest pending block in source order is emitted; cross-jump survivor
  choice decides what is pending). Next: find a spelling whose cross-jump keeps B's init and A's null path but leaves A pending
  (ability-arm-first sh4 gets the survivors but lays B first).
- **0x005850FD GetStyleCardsListUIComponent** DIFFER(1660v1678), 154 lines. Residue: hair-card arm deferred after the def releases
  (case 4) / after the epilogue (default). `continue;` at the end of case 4's hair arm reproduces case 4's layout but moves `i` from
  [ebp-0xc] to [ebp-0x1c] (frame-masked 98 lines) and does not move the default case. Next: combine the continue with a declaration
  order that restores the slots, and find the default-case equivalent (its arm is placed after the epilogue).
- **0x00589402** (PC twin) DIFFER(1581v1582), same residue, 401 lines; baseline only.
- **0x0058F016 ConstructItemCategoryList** DIFFER(1584v1587), 7 lines. `long count = item->second;` before the found test gives
  retail's colouring (c2prio: item 36->45 > start temp 40->43, so the start temp coalesces into edi) but hoists the load before the
  `jne`. Inert in c2prio (start temp 40 eax > item 36 edi): per-arm count (h4-h7), const-reference count (h8, h9), reference item,
  const item, key/found/item declared outside the loop, while loop, inverted test, header temp, bool test. Next: a candidate that
  stays live in both arms without a single hoisted def (a per-arm copy is eliminated as single-use).
- 0x00590235, 0x00590F0B, 0x00591B7F: priors kept, not regraded (see differ/ Grade lines and NOTES_chunk1007.md).
- 0x0058807D, 0x0058F9BC: not attempted (size / unnamed callees).

## Names
- LABELS.tsv: 0x00584B86 AssignThingPC, 0x00584157 SetThingAsQuickAccessItem (both PDBs).
- No NAME_FIXES. Note for the coordinator: landed 0x00584F15 / 0x00584EDC use local models with descriptive virtual-slot names
  (`SetActionParameter_slot62` = NUISystem::CComponent::SetDeletion, `SetContents_slot60` = AddChildDynamically,
  `SActionParamPair` = NUISystem::CDeletion). They are virtual slots, not addresses, so they are recorded here, not in NAME_FIXES.
- EActionType values used by 0x00584B86: 252 UNASSIGN_EXPRESSION_ITEM_PC, 246 SET_SPELL_TO_ASSIGN_PC, 253
  SET_EXPRESSION_ITEM_TO_ASSIGN_PC, 327 SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC, 329 SET_SPELL_TO_ASSIGN_IN_ITEMS_SWAPPING_PC.
