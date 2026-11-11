# 2026-11-11 landing cs -- naming agent NF20: 68 landed rows respelled, 54 of them renamed (remaining name-fix queue + FAM06 / Q8 / Q9 finds)

Source: `naming/2026-11-11_agentNF20/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, QUEUE, hdr README). The queue had 52
entries: 10 are done and 42 partial. The partials are NF19's leftovers whose reasons still hold, plus new ones that each need a
header job first.

**Verification**
- vland --recheck-landed ran in two parts: all 68 grade MATCH or RELOCATION_MATCH.
- Running every test exe found its catalog PassPattern in 68 of 68. The 8 non-WINS rows in vland's summary are custom PassPatterns.
- Bindings were re-derived (-109 / +50 lines). bindings_multi_check is at its NullCoord baseline.
- NF20 copied its rows from a1cb23fab, and none of the 68 changed in the repo since. Its three headers were 3-way merged
  (base a1cb23fab) onto the tree after the Codex pass 21 merge: all clean.

**Changes**
- 34 slot-0 scalar deleting destructors renamed `??_G<Class>@@UAEPAXI@Z`. They bind `??1CTCActionUseBase@@UAE@XZ`, 0x004DA967.
- ~CTCBase (0x00686830) binders: 5 rows. Among the catalog fixes: 0x004DBF56 is CTCTargeted::~CTCTargeted and 0x004DDF6F is
  CTCUserActionPointBase::~CTCUserActionPointBase.
- CSurface:
  - GetDimensions is the self row.
  - 0x00415BD0 and 0x00415BF0 are GetWidth / GetHeight.
  - 0x009F4330 and 0x009F4360 are Swizzle / Unswizzle; the catalog had 0x009F4360 as Swizzle.
- 0x00AF0D80: FAM02's wrong rename is reverted to the CParticleSystem element type. Its caller 0x00AF10E0 is respelled.
- NFractal::CNoise::InitPerlinNoise and CNoise::CNoise.
- InitialiseLight's `const CShaderLight&` is respelled at 0x00B44F20.
- MakeOwnCopyOfData is respelled at 0x00A3C310 (it was bound as `Foo`).
- 10 CPSC*::Alloc calls change from SAPAV to SIPAV; both PDBs agree.
- STLport __push_heap / __adjust_heap / __destroy_aux binders.
- NInventory::CCategory::GetCategoryIdentifier at 5 binders.

**Headers (opt-ins only)**
- CSurface.h:
  - FABLE_RETAIL_CSURFACE_SIZE_OUTOFLINE
  - FABLE_RETAIL_CSURFACE_GETWIDTH_FORCEINLINE
  - FABLE_RETAIL_CSURFACE_GETHEIGHT_OUTOFLINE
  - FABLE_RETAIL_CSURFACE_SWIZZLE
- NFractal_CNoise.h: FABLE_RETAIL_NFRACTAL_CNOISE_CTOR
- CTCActionUseBase.h: FABLE_RETAIL_CTCACTIONUSEBASE_DTOR

Labels: ghidra_out/labels_landing_2026-11-11cs.tsv.
