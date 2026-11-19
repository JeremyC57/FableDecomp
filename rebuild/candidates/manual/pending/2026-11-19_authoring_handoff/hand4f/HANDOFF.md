# hand4f handoff (2026-11-19, stopped on owner wrap-up order)

## Done (8 of 10 rows; all RELOCATION_MATCH/MATCH + behaviour test PASS in the strict read-only vland)
| batch | rows | state |
|---|---|---|
| b1 | 0x00B6DF90, 0x00B6DFD0 (CEngineWaterRenderer::GetIceForeground{Shadowed,ColbuffShadowed}VertexShader; renamed) | landed |
| b2 | 0x005DC340 CAnimationSet::GetAnim (`__assume(0)` unreachable END, accepted in EXTRA_RULES 2026-11-19) | landed |
| b3 | 0x0053848F / 0x005384DD NUISystem::CList::AddActionOnInactive / AddActionOnSelected (second renamed) | landed |
| b4 | 0x00554030, 0x005698A0, 0x004C5B90 std::map operator[] family (two renamed) | READY, sent to main |
Per-row detail: NOTES.md; names: NAME_FIXES.tsv / LABELS.tsv (incl. 0x00535140 = _List_base ctor, not list::list); grades: ATTEMPTS.log;
summary: STATUS.tsv; snapshots: batches/b1..b4 (src, test, payload, oracle, hdr copy, README).
Open decision in b4: 0x004C5B90 names the protected CTCGraphicAppearance::CMeshEffectPriorityEntry through a derived-class typedef
(VC7.1 access check on explicit instantiation; no code) and defines the entry's default ctor body `{}`.

## Remaining rows (untouched, 0 grades by hand4f)
- 0x006CA8C0 (138 bytes). Catalog CEngineTextureMorphSet::AddEngineMorph; an older draft says CTextureMorphs::AddMorph -- IDENTIFY first.
  Prior best DIFFER(128v138): ref/006ca8c0/prior_006ca8c0.cpp (local model). Residue: in the in-place push_back arm retail stores MorphID
  from edi (zero) and has no temporary-blends free, with its own `++finish; return finish-1` tail; the draft merges both arms.
  Next: port onto shared headers in FableWin /Od order with Ego_d locals (ref/006ca8c0/egod_locals.txt), shared stlport vector opt-ins.
- 0x00A0CA90 CWParsedItem::InitAsFloat (187 bytes). Prior best DIFFER(176v187): ref/00a0ca90/prior_00a0ca90.cpp (engine/CWParsedItem.h).
  Residue: x87 allocation (retail homes the divisor in the push-ecx frame word, fld/fmul/fstp per digit, fild before the multiply).
  Next: copy the landed narrow twin 0x009B9770 CParsedItem::InitAsFloat (hand4e: FableWin order + Ego_d locals + retail literal
  types, whole part `*= 10.0f`, fraction `*= 10.0`) and widen it to unsigned short text.
- The map family (0x00554030 / 0x005698A0 / 0x004C5B90) is DONE (b4), not remaining.

## Overlays
hdr/ is empty: no un-landed header opt-ins.

## Tools (workspace root; copied from hand4e, paths switched to hand4f)
g.sh `<addr> <src> [leaf] [--flags ..]` grade + diff + ATTEMPTS.log line (IDEA env = note); qq.sh grade only; qd.sh grade + compact diff;
mt.sh `<addr> <test>` compile+run a behaviour test like vland; tc.sh `<test>` show test compile errors; vl.sh strict read-only vland of
payload.json/oracle.tsv; mkpayload.py builds payload.json/oracle.tsv from matched.tsv; ddis.py `fw|ed|er <va> <len>` disassemble
FableWin / Ego_d / Ego_r; fwn.sh resolve FableWin call thunks to names; ld.py masked straight diff of a q.py output.
