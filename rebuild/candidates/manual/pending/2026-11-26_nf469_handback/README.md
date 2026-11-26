# NF469 hand-back (2026-11-26): r467a evidence and the CAIStateGroup fold batch

`README_agent.md` and `R467A.md` are the agent's own report and evidence. Batches stay in the agent folder; none is landed.

## r467a: retail's `~CAIStateGroupBase` is implicit
- **PDBs:** Ego_r and FableWin both flag it `public virtual compiler-generated`; the controls are not flagged.
- **Bytes:**
  - The implicit form is 19 bytes, with no vptr store, pick-any.
  - Any declared form is 25 bytes and starts with a vptr store.
  - Retail 0x008FD288 is the 19-byte form, in the base-ctor TU's COMDAT run.
- **HEAD today:**
  - 113 landed objects already emit pick-any copies beside 0x008FD288's strong definition. 102 of those copies equal retail; 11 do not.
  - With real HEAD objects this gives LNK4006 under /FORCE, and LNK2005 / LNK1169 without it (link harness).
- **So the one form for the base class is:** 0x008FD288 becomes a pick-any thin row, and every port emits the implicit copy.

## Batches (land order)
1. **bR0, 11 rows:** declaration-only destructor in their local models, removing the 11 non-retail copies.
2. **bR1, 1 row:** 0x008FD288 becomes a thin row of 0x008FE862, the ApprenticeHero ctor in the same retail TU.
3. **b0_hdr:** 3 headers.
4. **b4, 40 rows:** 20 CAIStateGroup ctor ports (4 → 56-byte vftables equal to retail), each with its `??_G` home as a thin row.
   - Sandbox tracker: +20 rtti_identified → agree_pdb.
   - 9 of the 20 also need slot-row respells (r467b / r469c).

## Rulings (coordinator, on precedent)
- **r469a: granted on full gates.** The declaration-only form is the r451b form, and 640 landed local models already use it. bR0 still needs a vland pair, a link and the sandbox runs.
- **r469b: granted.** 0x008FE862 owns the thin row. Recheck it when 0x008FE862 gets its wave-2 port.
- **r469c: yes.** Respell the misspelt slot rows next, as units.
- **r469d: T3 line 651 (nf303 0x008FD288) holds bR1.**
  - The line's subject is 0x008FD288's six stand-in table binders (FREE 1, STANDIN 5).
  - bR1 changes 0x008FD288's own source, which is exactly what that line protects.
  - Releasing it without fixing those binders would break the rule that a release needs its subject resolved.
  - **Route:** port the six binders in the same unit as bR1, then land with `REL=nf303`.
