# 0x00E0CA50 CUndeadRisingFirefly::Main -- ESCALATE (AUTH30, 2026-11-27)
- Bytes: c0 (Codex) RELOCATION_MATCH 1223/1223. The leaf references one NEQ copy: `??4CScriptThing@@QAEAAV0@ABV0@@Z`, 51 v 74 B at
  0x008AB980 (landed self). It calls ReleaseData out of line because the row text sets `#pragma optimize("s", on)` before the row-local
  `inline CScriptThing::operator=`.
- No HEAD witness: no landed object emits that 51-byte copy (LOG.md: 157 inline definers, none with the RELEASEDATA opt-in).
- g1 removes every row-text optimize toggle and marks the row-local operator= noinline. The copy becomes EQ (74 B), but the leaf becomes
  DIFFER(1090v1223).
- The leaf needs a plain inline (size-built) operator=; the copy needs the speed body. In retail the CScriptThing.cpp TU's copy won the link.
- c0 also carries row-text optimize toggles ("gt" / "s") that the rulings allow only as shared-header opt-ins.
- Question: is this compile-order / link-order class (wait for object mode), or may a size-built row-local inline whose copy is NEQ land as
  debt when no landed object emits it? The 2026-11-24 rule needs an identical landed copy, and there is none.
Grades: c0 + g1.
