# Sweep of the nine class-model migration waves (2026-11-02)

Why: naming wave 31's recheck found 0x00692230 / 0x006922C0 DIFFER on the current tree although class-model migration wave 4 (84edb11e4) recorded them
as passing; its independent check covered 114 of 1,228 rows. This sweep regrades every landed row any of the nine waves touched
(`headers/2026-10-29_retail_models_wave1` .. `2026-10-30_retail_models_wave9`, ROWS.tsv): 5,152 distinct rows (`ROWS.txt`), payloads built from the
tree at 5abdb61c4's parent (after chunk 704), `vland.py --recheck-landed` in three parts.

Result (`sweep_grades.txt`): 5,106 MATCH / RELOCATION_MATCH + PASS (186 MATCH, 4,920 RELOCATION_MATCH); 46 DIFFER: 0x00692230 / 0x006922C0 (repaired
in chunk 705 while the sweep ran) and 44 more (`DIFFER_NO_LEAF.txt`). None of the 44 lost its bytes: each source has no `// Leaf:` line, and since its
class gained a virtual dtor through a shared header (engine/CBaseClass.h / CParentDefClassBase.h / CBaseObject.h) the TU also emits an implicit
??_G + ??1, which is larger (30 or 28 bytes) than the ctor (18-26), so the verifier graded the wrong function. Fix: a `// Leaf:` line naming the ctor
symbol taken from each object (`objdump -t`): 44 / 44 RELOCATION_MATCH + PASS (`leaf_recheck.log`). Comment-only change; no bytes change.

Lesson (GOTCHAS 2026-11-02): every landed source needs a `// Leaf:` line; a shared-header change can change which function a Leaf-less row is
graded on. The landed tree still has ~23,000 sources without a Leaf line (sync_catalog_names.py: "no Leaf line"); a full-tree sweep would find
any other such rows.

CORRECTION (same day): the 44 DIFFER (and the two wave 31 "failures" 0x00692230 / 0x006922C0) were produced by the payload builder used here
(`naming/2026-11-01_wave27/scripts/coordinator_mk_rc_list.py`), which passes an empty leaf when a source has no `// Leaf:` line, so vland graded the
largest function. The lane's own `lane_tools/recheck.py` falls back to the mangled prefix of the catalog name (`??0Class@` for a ctor) and grades
the same rows RELOCATION_MATCH (proved at 5abdb61c4 on 0x00455572 / 0x004C20D0). So none of these rows was broken and migration wave 4's recorded pass
was right; the added Leaf lines only make grading independent of the payload builder. Use `lane_tools/recheck.py` for recheck payloads (it also
sets EH-funclet offsets).

