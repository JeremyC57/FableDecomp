# Wave 3 class-model migration: shared headers (2026-10-30)

No header was added. Two existing headers changed in comments only, so no includer's bytes can change. Paths are under
`rebuild/include/engine/` in the worktree `$S/mig3/repo`.

## CParentDefClassBase.h (retail branch, FABLE_RETAIL_DEF_CLASS_BASE)

- The "Unproven: whether the copy ctor / operator= are user-declared or the implicit ones" paragraph is replaced by the
  wave 3 result (LESSONS.md item 1). The copy ctor 0x0046EE98 and operator= 0x00431ED0 stay declared out of line.
- Implicit members reproduce the /Os callers and the standalone 0x00431ED0 body. At /O2 they inline 0x00431ED0 into
  0x00434370 / 0x00625350, which retail makes call it, and they do not expand it in 0x00432E60, where retail does.
- No declaration gives both (implicit, explicit inline, out of line in TU order, inline_depth 1-3, optimize s/t/g, QFE 4035).
- The 5 /O2 expanders (0x00432C70, 0x00432E60, 0x00434450, 0x004345A0, 0x0067C980) keep row-local `__forceinline` models.
  This residue is now named in the header.

## CDefClassBase.h (retail branch)

- The operator= 0x00431F10 line now says why it is out of line. An implicit CDefClassBase::operator= inlines the
  0x00431F60 CDefPointeeBase/CResource member copy (102 bytes against retail's 77), and retail calls it.

## The experiment's temporary guard (removed)

- During the experiment, `#ifndef FABLE_DEF_IMPLICIT_COPY` wrapped the two copy declarations in both headers. It was taken
  out again when the rule came out "keep the declarations".
- The experiment sources in `logs/copy_experiment/` (`chain.h`, `chainH.h`) therefore compile as written only when that
  guard is put back. `chainH.h` is the declared variant and needs no guard.

## CBaseClass.h (unchanged; now used by 838 more rows)

- `CBaseClass()` 0x0099A2D0, `virtual ~CBaseClass()` slot 0 0x0099A2E0 (vtable 0x01231710), implicit copy.
- Evidence is in wave 1/2 HEADERS.md. This wave adds 838 includers, all byte-identical to HEAD (mcheck) and regraded PASS
  (ROWS.tsv).
