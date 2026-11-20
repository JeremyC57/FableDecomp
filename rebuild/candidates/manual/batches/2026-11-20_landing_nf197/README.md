# Landing nf197 (Account A, 2026-11-20): returned engine / input / stream / frontend names (22 rows)

Agent NF197 (round 10) handed back early (context) with 50 entries. 32 of them go back to the queue unclaimed: the entries not reached,
the partial families q5052 / q6196 (rows left in APPLIED.tsv), and q4860, whose row NF194 held and has since landed. The other 18 are
done, done-already, rejected or blocked. Its files are in `naming/2026-11-20_agentNF197/`, with the HEAD probes (`work/probe_*.tsv`) and
the held 0x00890150 draft (`held/`).

- 22 rows land: 3 MATCH + 19 RELOCATION_MATCH, every test PASS.
- Self renames (RENAMES.tsv section 1), each on both PDBs unless noted:
  - 0x00687210 becomes `??1CMessageEvent@@UAE@XZ`, the implicit destructor from engine/CMessageEvent.h (nf86 scaffolding). The vtable
    0x0125BE8C slot 0 (0x006872B0) expands the same body. A user-written empty destructor through an opt-in differed (33 v 27 bytes:
    the vptr store), so it was dropped.
  - 0x00A55A30 `C2DBoxF::ToIntBox` -> `ToC2DBoxI` on engine/C2DBoxF.h + C2DBoxI.h; 0x009E41D0 -> `CJoystick::EmptyEventStore` (protected).
  - 0x00AD4A70 `CTwoLongSets` -> `QuadricOptimiserInternals::CUniqueVertex` destructor, on engine/QuadricOptimiserTypes.h and the
    STLport set; 0x00AD71B0 `Remove` -> `Kill`.
  - 0x00A93A60 `CCountedPointer<C3DClothPrimitive>::DeleteData` (FableWin.pdb exact symbol; it was a gapscan `DeleteIfSet`) and its
    caller 0x00A94DF0 `Reset`, both explicit instances from engine/CCountedPointer.h.
  - 0x00495830 -> static `CFrontendGameComponent::GetSaveNames` (the made-up `CFrontendProfileList::Refresh` is gone); 0x00495FE0
    `SetMode` becomes the private `(EGameMode)` signature; 0x00496EB0 -> the `GFWrapLimit<long,int,int>` template instance.
  - 0x006B82E0, catalogued as an STLport `_Cons_val`, is `CEnvironmentSpellEffectInterpolator::RadialBlurFadeOut`.
  - 0x005DB1E0 / 0x00AAD150 respell their self signatures onto engine/CAnimComponentFlags.h / CDataOutputStream.h (by reference).
- Callers respelled (RENAMES.tsv section 2): CDataOutputStream `WriteSLONG` / `WriteUWORD`, `~CBaseObject` as protected `MAE`, and six
  input rows (0x00AB4910 ... 0x00AB6D60) from the made-up `CSystemManager::HasInputFocus` to the PDB `IsApplicationInputFocus` 0x009A4FB0.
  q6296's claim of `HasInputFocus` is rejected (no PDB has it); a header item renames the declaration in CSystemManager.h / CJoystickDX.h
  so 0x00AB6E40 can follow.
- Headers: add-only, declaration-only `#ifdef` opt-ins. `orig` equals HEAD for all four, the default paths are token-identical, no landed
  includer defines them, and pin_asserts pinned 0 (`naming/.../hdr/README.md`).
  - engine/C3DClothPrimitive.h: VIRTUAL_DTOR (a `virtual ~C3DClothPrimitive();` in place of the `void* _vftable` member, same layout).
  - engine/CFrontendGameComponent.h: GETSAVENAMES.
  - engine/CEnvironmentSpellEffectInterpolator.h: RADIALBLURFADEOUT; NScreenEffect_CScreenEffectPriorityStack.h: DELETERADIALBLURENTRY.
- Held at landing: 0x00890150 `CGameScriptInterface::RadialBlurFadeOut`. It graded RELOCATION_MATCH, but it adds header-less overlay
  structs copied from 0x00890200 and casts `this` to them, which is a new local model (advisor rule). The draft is in `held/`; the port
  onto engine/CGameScriptInterface.h is queued.
- Blocked with decision items: q6197 (0x00AD3A10 copy constructor spelled as STLport `set::` or `_Rb_tree::`; 0x00513700 is the same
  shape for another tree), q6250 (the VC7.1 block number in `?sorted_combination_anims@?N??CalcInterpolatedMeshAnim@...`: Ego_r says
  23, FableWin 50).
- Pick-any (`logs/pickany_check.txt`):
  - `~vector<CWideString>` in 0x00496070 equals the owner 0x00412130 (49 = 49).
  - The DeleteData<C3DClothPrimitive> copy in 0x00A94DF0 equals the self row 0x00A93A60 of this landing (11 = 11, relocations equal).
  - MISSING: 0x00AD4A70's `~_Rb_tree<unsigned long>` against the table line at 0x00517850. That body erases through 0x004A9CB0, not
    `set<unsigned long>`'s 0x004CF810, so the line is not ownership; re-identifying 0x00517850 is queued.
  - Debt queued: landed 0x00A91640 uses the C3DCLOTHPRIMITIVE_CTOR opt-in (non-virtual destructor), so its DeleteData copy is 23 bytes
    against the owner's 11. Converging it needs a header decision.
- Link (`logs/link_landing.log`): 2 LNK2005 against 3 at HEAD (`logs/link_control_head.log`). The `?SetMode@CFrontendGameComponent@@QAEXJ@Z`
  clash is gone, and no new one appears.
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Includers and referencing rows 65 good, 0 bad.
- tablecheck (`logs/tablecheck.txt`): four addresses shrink (0x00993EB0 8 -> 6, 0x0099A430 5 -> 3, 0x00A93A60 2 -> 1, 0x004959E0
  2 -> 1). Two grow, both SAME (an unchanged binding the table lacked): 0x01238768 (the CFrontendGameComponent CBase vtable, from
  0x00496070) and 0x00AD5240 (`_M_erase` of the CContractionReference map, from 0x00AD71B0). No out-of-image oracle line.
