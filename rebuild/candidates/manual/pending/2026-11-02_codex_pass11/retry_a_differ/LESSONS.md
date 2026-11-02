# retry11a lessons

## Matching with persistence helpers

- **Out-of-line traits readers.** When retail calls small helpers such as `CPersistTraits<T>::TransferBinaryIn` out of
  line, give their real bodies in the same TU under `#pragma auto_inline(off)`.
  - Declaring them without a body moves the caller to a different register allocation, even though the call sequence
    is identical (1604v1524 and 1156v1129).
  - One generic out-of-line template body is also worse (1450, 1229).
  - Only explicit per-type bodies match.
- **CPersistContext::Transfer<T>.** Where retail inlines it for scalars and calls it for strings (0x00431143), write an
  explicit inline specialization per scalar type.
  - A generic body instantiates `CPersistTraits<CCharString>`, which fails with C2440.
  - Adding an explicit-specialization declaration on top of a generic body crashes VC7.1 with an internal compiler error
    (C1001).
- **Derived stream destructors.** A destructor that retail reaches only through a direct base-destructor call
  (`~CDataStream` 0x00993B10, with no derived vtable store) is compiler-generated. Delete the user declaration; a
  declared `virtual ~X();` adds a call.
- **`CArray(n)`.** `vector(n)` and `vector(n, T())` differ in register allocation in the caller, not only in the
  constructor.
- **STLport free templates under /Gr** (`__uninitialized_copy`) are fastcall (YI). An explicit specialization with the
  STLport body under auto_inline(off) reproduces retail's out-of-line copy at 0x005269B0.

## Header overlays

- **After a consolidation lands,** rebuild the overlay from the repo text. Do not keep old copies.
  - Express every edit as an assertion-checked substring replacement, or as an opt-in branch whose `#else` is the repo
    file verbatim.
  - Then run `hdrcheck.py`, and recheck every landed includer of an extended branch.
  - That recheck found a new includer, 00631c60, that landed after my first recheck.
- **Edits inside an existing retail branch are not free.** Recheck its landed includers with `recheck.py` and
  `vland.py ... --recheck-landed`. The flag goes after the payload and oracle.
- **New opaque models are debt.** A new header that keeps a member by name and offset but types it `unsigned long[]`,
  or ends in an opaque pad, has to be recorded as debt. Check whether an older model of the same class already exists
  elsewhere: `fable_game.h` already models CMainGameComponent and CGameComponent.

## Probing

- **Concurrent appends lose log lines.** Running three probes at a time that append to one log dropped lines twice.
  Recover them from the per-probe `.out` files, and count lines against the probe directory before you hand back.
- **Specify probes as substring replacements on a base file** (`mkp.py`). Anchor each pattern on a whole line; one
  pattern that was a prefix of another applied twice.
- **Inline budget is the dominant residue.** In 8 of the 20 parked rows, retail inlines an inline member at some call
  sites of a function and calls it at others. Tried without success: explicit and implicit argument forms,
  declaration-only bodies, and reordering. A declaration-only diagnostic (00ee8390 d03) is still useful, because it
  isolates which site is inlined in retail.
- **Choose the best probe by masked diff, not by length.** The Codex grades reproduced exactly on this lane
  (0048e0a2 1757v1772, 006c9490 924v1128).
  - The kept probes have smaller masked diffs even where the length is further from retail: 006c9490 p02 is 890 with
    -226/+158, against Codex's -249/+191.
  - 0048e0a2 p05 is 1760 with -436/+430, against Codex's -457/+451.

## Layout

- **An odd member offset is evidence of a packed member class.** In CTCOwnedEntity, POwner sits at +0x0D while the next
  ulong64 is at +0x18, so the member class is packed and the containing class is not. Retail confirms this
  (`lea edi,[eax+0xd]` at 0x005EBACD).
- **Packed classes go behind an opt-in branch.** A packed class used across many headers needs an opt-in branch with
  SDK `pshpack1.h`/`poppack.h` and a TPI check. It is not a per-row fix.
