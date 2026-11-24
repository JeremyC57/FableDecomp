# NF322 lessons (2026-11-23/24, flat-header class-model ports)
- Read the class's retail vftable straight from RTTI (type descriptor `.?AV<X>@@` -> complete object locator -> vftable[-1]); tools/vtdump.py
  does it in a second for any class and labels each slot from labels_rtti_port(_dtors).tsv. That one table decides the model: which slots
  are X's own (declare them), which are inherited (leave them to the base), and whether X adds new slots.
- Generate the model, do not hand-write it (tools/genmodel.py): copy the flat member lines verbatim from sizeof(base) on, pin sizeof and
  every offset inside the opt-in, carry existing opt-in lines of the flat class, and let pin_asserts.py fix the moved flat asserts.
- A flat `_pad_*` member can straddle the base's end (Defs: +0x24[4] over a 0x25-byte packed base): keep its tail as an unnamed pad or every
  later offset shifts; the asserts catch it at once.
- Prove slot order with an emitted vftable, not by reading: a scratch TU with `new X` (or `new X(x)` when there is no default ctor) makes
  VC7.1 emit ??_7X; compare it with retail (tools/vprobe.py + vtcmp.py). This found two landed base-model defects (EnemyCreatureBase lacks
  4 overrides; CEngineInternalPrimitiveBase has SetChildPrimitive 7/8 reversed and ParticleGroup lacks slot 28) that no grade would show.
- `grep -v " SAME "` also hides vtcmp's summary line ("SAME 25 DIFF 0"): grep for "retail slots" separately.
- Constant-return / single-store bodies move with no byte change; most other "flat" rows fail on local C2DVector / C3DVector / CCharString /
  STL models the real base's header now pulls in. Triage by compiling the mechanical draft (clang) before any lock: it sorts the batch.
- Base headers often need a forward declaration they use undeclared (EnemyCreatureBase uses CThingCreatureBase) or a sibling switch for
  their forward enums (AIACTIONS): find the smallest landed definer of the base switch and copy only what the error names.
- `.drectve` gains libcp only when the base header pulls stlport (CNavigatorExternalInterface): expect it on those rows only.
- Run links and equiv_check in the background like grades; a 3-object link is still a link (slip).
