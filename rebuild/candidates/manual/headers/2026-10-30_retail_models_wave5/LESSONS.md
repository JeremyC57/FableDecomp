# Wave 5 lessons (class-model migration, 2026-10-30)

1. **VC7.1 drops a class's own vptr store from an implicit dtor. A user-written `virtual ~X() { }` keeps it.**
   - For a class with no members to destroy, the implicit dtor collapses to the base dtor: a bare `jmp ~Base`, with no
     `mov [esi],<own vtable>` first.
   - Retail's dtors of CScriptGameResourceObjectBase keep the store (0x006E70A0, and the inline tails of 0x006E7B80 and
     0x007E74D0). So GROBase's dtor is user-written, and GRO went from 83 to 97 SAME when the header made it so.
   - STB's dtor 0x007E74D0 has no STB store before the PImp release, so STB's dtor is the implicit one. Declaring it
     dropped GRO to 84 SAME.
   - Read the dtor bodies for the store before choosing implicit or user-written.
2. **Mixed inlining is residue, not a model error.**
   - Retail inlines the implicit STB ctor and dtor at most call sites and calls the COMDAT copies 0x007E72A0 / 0x007E74D0
     at others.
   - A row whose local model declares them out of line is matching a call site; the header's implicit members inline
     there, and the row DIFFERs. These are the 33 `GRO` rows, the 23 `GRO-cp` rows, and the 50 out-of-line ctor rows in
     NEXT.md.
   - The fix is not in the header. Leave these rows local until VC7.1's per-site inlining decision is understood.
3. **CSpawnedFunc's ctor: `__forceinline` reproduces 238 of 240 rows, plain `inline` 224.**
   - RTM 3077 declines to expand the plain-inline ctor at 16 sites where retail expanded it.
   - g++ rejects `__forceinline`, hence the `FABLE_SPAWNEDFUNC_INLINE` macro.
   - Whether retail forced it, or a later compiler's heuristic expanded it, is unproven.
4. **The DirectShow strmbase CBaseObject (m_cObjects, CUnknown) is a different class.**
   - mig_bo.py excludes it by member and base names: `m_cObjects`, `WCHAR`, `CUnknown`, `INonDelegatingUnknown`.
   - One row slipped through the loose pass (00ca53f0) and was reverted, and the guard was widened.
5. **Order the helper scripts.**
   - mig_gro.py must remove the `X_HeaderLayout` alias includes before it computes which headers to add. Otherwise the
     STB include is left out, and 8 rows failed until they were redone.
   - The loose BO pass re-edited rows that an earlier pass had already reverted, because the skip list was built from the
     wrong file. Build skip lists from every `*_rev.tsv`.
6. **A late edit makes a regraded row stale.**
   - recheck.py embeds the source when it builds a batch. Run `scripts/payload_check.py` (stale = 0) before hand-back.
   - Wave 5 needed 5 batches. A 296-row batch took about 5 minutes and a 144-row batch about 3, with the chunk 584/585
     agents on the lane.
7. **Data-member blocks need offsets checked, not just names.**
   - `mig_cgsi_m.py` recomputes each local member's offset (vptr 4, natural alignment, pad arrays) and requires the
     header's offset. This caught 14 rows that read a `bool m_handLampLit` at +0x1c, where the header has
     `long CurrentPlayer`. Those rows stay local.
   - `X_Retail*` member types are respelled to the header's class. Where that respelling would create a new invented
     relocation spelling (0x00449970 `GetPlayer`), the row is reverted instead, and NAME_FIXES.tsv carries the real name.
8. **A row that declares its own leaf non-virtually still compiles the same body under the header's virtual.**
   - 134 CGSI rows did this. The leaf symbol becomes the PDB's `UAE` / `UBE` spelling, which is what most `// Leaf:` lines
     already said: 23 rows went from "matches no symbol in old.obj" to an exact match.
   - Where the row's signature differs from the PDB's, `sigfix.py` rewrites the definition head, and mcheck decides:
     - `long` vs `unsigned long`, `long` vs an enum, and `unsigned char` vs `bool` are SAME;
     - `long` vs `float` is DIFFER: the body stores the raw long. 8 rows were reverted.
9. **Typedef enum stand-ins clash with the forward header.**
   - `typedef long ECameraOp;` clashes with fable_nscript_forward.h's MS forward enum, and so do the others.
   - `tdfix.py` drops them and includes the forward header where the first one was, so the enum is declared before its
     first use. `NEntityEvents::EEventType` is one such stand-in.
10. **CGameScriptInterfaceBase rows are the cheapest family left.**
    - 146 of 202 were interface-only after expanding filler macros.
    - 144 of 146 were SAME on the first mcheck. The 2 failures were a std::list / std::vector arity clash.
