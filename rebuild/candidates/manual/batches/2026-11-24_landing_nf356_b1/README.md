# nf356 b1: owner 0x004291DE ReleaseData<NUISystem::CComponent> ported onto its NF352 size-body opt-in (1 row of 6)

Agent NF356 (Account A naming queue; hand-back `pending/2026-11-24_nf356_handback/`). The owner was an explicit instantiation of the
class template's ReleaseData under a row-head `optimize("s",on)` on a local CComponent model. It now defines
FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RELEASEDATA_SIZE_BODY and explicitly instantiates the shared specialization (no class model; the
pragma follows the include, nf189). The opt-in block's condition (b) (owner sets `optimize("s", on)`, no CompilerFlags) was proved on the
pre-port text; the header comment now says so (comment-only edit).

Coordinator verification: vland `--recheck-landed` RELOCATION_MATCH PASS on HEAD and from the repo after landing; objcmp of the two
objects: every section equal except `.debug$S`, DEF and UND sets equal. T3 nf352 0x004291DE closed.

The other five owner ports are HELD (advisor decision, owner delegated, 2026-11-24): an owner port onto a shared header is a move, and
the move gate (NF320 Q2) needs the COMDAT set unchanged. 0x0042AF3C / 0x0042B040 / 0x0042B312 / 0x005F81BE need the nf135 `?A0x` address
helper (VC7.1 C2945 rejects explicit instantiation of the specialization: a new strong symbol, and the sibling predates NF320, so NF348 D1)
and swap inline COMDATs. 0x00429C6F would stack the libcp exception on a section change (`??0CCPPointerInfo` rebuilt in speed mode).
Their T3 lines carry the measurements. Queued PRIORITY instead: 0x0042AF3C's made-up self name respelled in place, and 0x00428B10
(`@DeleteIfSet@4` v the referenced DeleteData<CComponent>). The two nf340 lines on 0x0042B312 / 0x0042B040 are closed (EQ since nf349).
