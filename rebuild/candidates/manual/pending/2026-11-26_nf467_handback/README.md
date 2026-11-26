# NF467 hand-back (2026-11-26): short-vftable ports, pilot plus CAIStateGroup ctor wave 1 (r459a)

`README_agent.md` and `PILOT.md` are the agent's own report and method. Batches stay in the agent folder; none is landed.

## Result
- **42 ctor rows were ported.** The pilot is 0x008FDE1B, the CAIStateGroup_StandStill ctor; the other 41 are direct CAIStateGroupBase subclasses.
  - Ctor bytes are unchanged, and every `??_7CAIStateGroup_<X>@@6B@` comes out at retail's 56 bytes with retail's slot targets. Before, each was 4 bytes.
  - The added deleting-destructor and base-destructor copies are byte- and relocation-equal to retail.
  - Full caller census: 42 / 42 OK.
- **All 42 are HOLD.**
  - Every row now emits `??1CAIStateGroupBase@@UAE@XZ`, which collides with the landed 0x008FD288 row's no-duplicates definition (r467a).
  - 22 rows were built on the wrong view and need redoing on the headers' existing retail views (r467f).
- **The 61 NAME_FIXES addresses are all open:** 60 rtti_identified, 1 split_fixable. None is counted proven by mistake.
  - So these ports close names once they land with their deleting-destructor rows as thin rows (the nf424 pattern).

## Rulings (advisor decision, owner delegated)
- **r467a: open; NF469 gathers evidence first.**
  - "Accept one duplicate" is declined: a no-duplicates definition beside pick-any copies is a link error.
  - Before choosing between reshaping 0x008FD288 into a pick-any or thin row and declaring the base destructor in the view, establish:
    1. whether retail's `~CAIStateGroupBase` is compiler-generated;
    2. how rows on the existing NF330 retail views coexist with 0x008FD288 at HEAD.
  - Whichever it is, one base class gets one form. A reshape of 0x008FD288 lands first, as its own batch.
- **r467b: split.**
  - Slots that point at row-less targets are covered by r464c / r466c.
  - Slots that point at misspelt landed rows land with, or after, that row's respell.
- **r467c: granted per row, on two conditions.** Retail shows size mode at the row (unaligned start, no padding), and the row's own leaf is byte-identical with and without the pragma. It is recorded as TU mode (2026-11-25 size-pragma rule; NF419 removed pragmas that only framed generated copies).
- **r467d: granted; do it next.** The CInputProcess header-only fix covers 60 names from one header batch. It must be an opt-in, since other rows include those headers.
- **r467e: held.** CACompressor waits on the libcp decision (NF468).
- **r467f: granted.** Rework onto the existing views under the r451a conditions.

## Correction to NF461
NF461's `retail_size` overshoots for at least 38 names (`evidence/vtaudit.tsv`). It counted a following data dword that looks like a code address. For example, CThreadedFileCallback is 12 bytes plus the string "XMV\0", not 16 bytes. NF461's 2,079 "no copy at retail size" is therefore an upper bound. Use counted function slots, not `retail_size`.
