# Wave 4 lessons (class-model migration, 2026-10-30)

1. **The lane regrades far faster than the wave 3 estimate.**
   - vland `--recheck-landed` took about 6 minutes per 292-306 row batch, about 50 rows a minute, with the chunk 583 agent
     on the same lane.
   - mcheck -j 3 took about 3 minutes for 650 rows.
   - So an 800-row budget is about 20 minutes of regrading, not 2.5 hours. Wave 4 edited 1,228 rows and regraded all of them.
2. **recheck.py embeds the source in payload.json when it builds the batch.**
   - An edit made after a batch is built is not regraded by that batch.
   - The 52 rows that took the CGSI edit on top of their BC edit (step 1b below) were regraded a second time in a later
     batch.
   - Before hand-back, check that the regraded source of every row equals the final worktree file
     (`scripts/payload_check.py`).
3. **Interface-only CGSI rows: the header's signatures decide compilation, not the bytes.**
   - `scripts/cgsi_sigcheck.py` compares every slot a row calls with the header's spelling.
   - 235 differences were only const-ness, and 27 were the other way round. GetHero is `CScriptThing&()` non-const in the
     header, and rows spell it `const CScriptThing&() const`. All of these rows stay SAME: a virtual call carries no
     relocation, and const changes nothing in the call.
   - 82 rows fail to compile, for these reasons:
     - 33 pass a row-local `vector<T>` or `CScriptThingVector` where the header wants `std::vector<T, std::allocator<T> >&`;
     - 26 pass an int literal to a header enum (EScriptAIPriority, EMusicSetType, ECutsceneBehaviour, ...);
     - 11 use row-local CScriptThing stand-in types (CScriptThingRuntime / Local / Ref);
     - 9 have a local CCountedPointer that clashes with fable_nscript_forward.h;
     - 2 have a local enum typedef;
     - 1 other.
   - Each of these needs a per-row edit: a STLport vector model in namespace std, a cast to the real enumerator, or a
     CScriptThing model. Wave 5.
4. **CBaseClass slot-0 stand-ins (`v00`, `v000`, `v0`, `b0`, `b00`, `VTableAnchor`) are the dtor slot.**
   - `scripts/mig_bc_v.py` folds them into the header and drops derived mirrors of the stand-in at brace depth 1 only.
   - The nested-class bug: a first version also dropped `virtual void v000()` inside a nested class (CGraphicInfoFrame::
     CFrameData, which is not a CBaseClass). That moved its slot-1 call from +4 to +0, and 4 rows came back DIFFER. The
     depth-1 rule fixed all 4.
   - 226 rows had a mirror dropped. All of them are SAME.
5. **mig_bc_copyinit.py lookahead bug (fixed).**
   - The pattern `:\s*(?!CBaseClass\()` lets `\s*` backtrack, so a copy ctor that already had `: CBaseClass(other)` got a
     second base initializer (C2437 on 232 rows).
   - The fix is `:(?!\s*CBaseClass\()\s*`. Wave 3 never ran copyinit on rows where mig_bc_copy had already rewritten the
     initializer, so it never hit this.
6. **Behaviour harnesses are written against the row's old local model.**
   - On the header, 00a10dc0 (CRenderTarget ctor) is mcheck SAME, but its behaviour link fails (LINK_FAIL).
   - The old model's never-defined anchor virtual put the anchor in CRenderTarget's vtable, and the harness defines the
     anchor. Now the vtable holds ??_G, which needs ~CRenderTarget, or ~CBaseClass if the dtor stays implicit, and the
     harness defines neither.
   - An out-of-line `virtual ~CRenderTarget();` does not help: MSVC emits the vtable in every TU that runs the ctor and has
     no key-function rule.
   - Reverted. The fix is to add dtor stubs to the harness. This was the only such row in 1,229 regrades.
7. **Def dtor rows on the shared chain need `__declspec(novtable)` on the derived def.**
   - Retail def dtors have no vtable store of their own. The landed 0x0047747C model (novtable derived def) reproduces
     that on the shared chain: 00458b9d, 00459e46, 004575fc, 0045784b.
   - The leaf and the base-dtor call become the virtual spellings (`??1X@@UAE@XZ`, `??1CParentDefClassBase@@UAE@XZ`).
     Both are bound.
8. **The shared def chain is 0x39 bytes packed, so the first derived member must be 4-aligned to land at +0x3C.**
   - A row-local opaque member typed `unsigned char _body[0xc]` (alignment 1) moves to +0x39.
   - Model STLport containers as pointer words (0045a0ee, 004773f1).
9. **Rows that cannot take the shared def chain (residue, by design):**
   - the 5 /O2 expanders and their family;
   - CControlsDef's /O2 dtor 0x004304A0, which inlines the implicit ~CParentDefClassBase;
   - the CParentDefClassBase ctor 0x00430370 itself, which inlines CDefClassBase(). The shared chain has no CDefClassBase
     default ctor;
   - CBaseClass.cpp's own TU (0x0099A310-0x0099A4D0), where retail auto-inlines the CBaseClass members (wave 2 LESSONS
     item 2);
   - 9 typed-PDCB rows whose retail bytes inline the chain ctor (they call CResource() 0x009FBEC0).
10. **Respelling a base call that has no member behind it.**
    - 0046ef87 called `CParentDefClassBase::Copy`, which does not exist on the chain (slot 19 is pure). The call is really
      PDCB::operator= 0x00431ED0 with the source pointer.
    - Writing `CParentDefClassBase::operator=(*source)` gives the same bytes and binds the proven spelling
      `??4CParentDefClassBase@@QAEAAV0@ABV0@@Z`.
    - The stale `?Copy@CParentDefClassBase@@UAEXPBVCDefClassBase@@@Z` binding is still used by other rows (coordinator).
11. **An inline local `CBaseClass() { }` used outside a copy ctor is often dead in the leaf.**
    - 30 rows define the inline ctor and use it elsewhere, for example in a derived default ctor that the graded function
      never runs.
    - With `mig_bc_v.py --force-other`, mcheck finds 21 of them SAME: the use never reaches the leaf.
    - The other 9 DIFFER. Retail constructs the base inline in the leaf (REVERTED.tsv `BC-other`), so the header's
      out-of-line ctor changes the bytes.
