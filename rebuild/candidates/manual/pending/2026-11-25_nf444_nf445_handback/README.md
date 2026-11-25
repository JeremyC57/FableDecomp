# NF444 + NF445 hand-back (2026-11-25): GameCODA CSalRender header port and the CreateRenderer closure

`NF444_NOTES.md` covers the pilot and the closure measurement. `NOTES.md` is NF445's notes: the port, the R-NF445-1 checks and the landing simulation.

## Landed: 28 rows (`batches/2026-11-24_landing_nf445_nf444_hdr_c`)
- **Option S unit 0x00C6E630** (retail SalRender.obj):
  - `~CSalRender` 0x00C6E630, the owner;
  - thin rows `CSalRender::Initialize` 0x00C6E660, `ClearFilterHistory` 0x00C6E690, `CSalRender()` 0x00C6E6A0 and `??_G` 0x00C6E6D0.
- **12 derived ctor / dtor / `??_G` binders:** 0x00C6CA20, 0x00C6CAC0, 0x00C6D9D0, 0x00C6DA30, 0x00C6E760, 0x00C6E780, 0x00C70120, 0x00C701D0, 0x00C70A30, 0x00C70B00, 0x00C70BB0, 0x00C71400.
- **The voice rows:**
  - `CSalSw3DVoice::Create` 0x00C556E0 (vftable 0x012B8AE0 slot 1);
  - `Initialize` 0x00C55680;
  - `CreateRenderer` 0x00C55270, with its 8 EH funclets 0x01216180..0x012161CD.
- **Headers (add-only opt-ins):**
  - `CSalRender.h`: `FABLE_RETAIL_CSALRENDER_MEMBERS`;
  - `CSalSw3DVoice.h`: `FABLE_RETAIL_CSALSW3D_CREATE`, under PASS23;
  - `CCoda3DSource.h`: `FABLE_RETAIL_CCODA3DSOURCE_HRTFMODE`, the SDK `GetHrtfMode` at +0x1CC.
- **Names:** each one has two sources, the GameCODA SDK symbol plus the retail body, call or slot tie.
  - ClearFilterHistory is tied by three sibling slot-1 bodies (23 / 58 / 44 bytes, matching the SDK).
  - The SHrtfHeader placeholder is replaced by the SDK members.

## Rulings (advisor decision, owner delegated)
- **R-NF444-1 (`/Ob1`): granted.** It applies to every row of the render TUs (17 rows) and rests on retail's bytes, under the owner's 2026-11-08 inline-control rule.
  - /Ob0 fails three rows; /Ob2 fails the `??_G` bodies.
  - SDK 2.10 expands `??_GCSalRender`; that is noted as a dissent, because that build also inlines `~CSalMutex` differently.
  - Scope check: 20 of 20 landed rows in 0x00C5–0x00C7 still match under /Ob1.
  - The SalSw3DVoice rows keep their HEAD flags.
- **R-NF444-2: declined.** 0x00C6E660 is folded into the unit.
- **R-NF445-1: granted with booked debt.** 0x00C55270 and the derived binders now emit SDK-signature slot overrides. The landed slot rows still carry stand-in leafs (`PBQAJ` Process, SetVolume).
  - `evidence/sigcheck_19.tsv`: all 19 slot rows pop the same retail argument bytes as the SDK signatures, so the stand-ins are the stale side.
  - Debt: `DEBT_SLOTS.tsv` lists 20 addresses. /FORCE unresolved goes 27 → 35 (22 added slot names), duplicates 3 → 2.
  - The nf445 T3 family line queues the fix.

## Coordinator checks at landing
- **Lander traps (GOTCHAS):**
  - the header base copy sits in the batch's `orig/`;
  - a CompilerFlags old value of `-` must be written `(none)`;
  - the head payload carried new text, so it was rebuilt from `orig/` (`nf444_hdr_c`);
  - the catalog PassPattern is `PASS_<addr>` for the two gap-scan tests, which NF445 fixed.
- **Recheck:** 28 / 28 landed, and the 4 option S members pass.
- **Unchanged file names:** the source files keep their stand-in names; the thin rows include the owner by path. apply_renames now accepts that.
- **Tracker:** 16 addresses improve and none gets worse. Two become proven (standard names), and 14 placeholder or split addresses move to one agreed spelling. The SDK names are not PDB names, so most of them count as agree_unverified, not proven.
- **equiv_check** (copy against copy, three headers): 57,160 rows checked, 23 differ, all of them rows of this batch that opt into the new members; 0 rows outside the batch (`evidence/EQUIV_CHECK.tsv`).
- **Baselines:** bindings_multi 1 (NullCoord), funclets 1,695 checked with 0 broken, s units 155 checked with 0 failed, descriptive_check 0 FAIL.

## Queued
- **T3 nf445 family line: the 19 slot rows**, onto the derived render headers, together with the NF441 NAME_FIXES. Do the SIMD ctors 0x00C55070 / 0x00C550D0 first: they emit `??_7C3DAnimationUtil@@6B@` for both 0x012B8A64 and 0x012B8AA0.
- **T3 nf445 header line: CSalVoice.h PASS22 slot order.** Retail slot 2 is Release-shaped, and slot 3 is SetBuffer, which four retail `+0xC` calls on `m_pVoice` confirm. That is two sources against the header's order.
- **NAME_FIXES.tsv (not applied):** the CSalMutex copies 0x00C5D550 / 0x00C5D570 / 0x00C5D5B0, `CSalSwBaseVoice::Create` 0x00C5A1D0, `CSalSwBuffer::Create` 0x00C5CE00 and `_l_frameBuffer` 0x0143BCB0.
- **Closed:** the nf441 NAME_FIXES line for 0x00C55270.
