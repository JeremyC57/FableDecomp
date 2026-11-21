# AUTH4 (Account A authoring agent, 2026-11-21): 0x00A4EFC0 -- not matched, mechanism proven

Still DIFFER(3491v3491), residue 16 (`00a4efc0/a2.cpp`, /Gr, overlay `00a4efc0/hdr.diff`; V1 AllocateType under the new name
FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_IFBODY). `00a4efc0/ESCALATE.md` and `LOG.md` hold the measurements; `tools/il/`, `tools/ord.sh`,
`c2x/` are the IL-key kit (no retail data).

- Proven: the residue is the evaluation order of the position block's `compressed_size < uncompressed_size` operands in c2's tree
  optimiser, decided by a 16-bit hash of symbol numbers when register need and node count tie. Forcing the right operand's key gives a
  RELOCATION_MATCH object. Not the register allocator (forced colouring leaves the order).
- Advisor ruling (2026-11-21, owner delegated): because any text change anywhere re-rolls that hash, a variant picked because the readout
  says it flips is fitted by construction: no open search. Next step, bounded: apply as one set every correction that has its own evidence
  (Ego_d locals / symbols / line attribution, e.g. separate CArray declarations; a diff of the named locals against Ego_d's), read the order
  once; if it does not flip, test whether fields #2493 / #2484 move under an unrelated declaration-only opt-in (then the tie depends on the
  TU's header set and the row goes to the fitted-dead-temporary ruling or to the owner).
- VC10 note: the GetBoneName / C3DVectorWord::operator== gaps and the 3297-v-3872 body size were measured with the retail-configuration
  headers; until the VC10 STL mode exists they are unresolved, not proof of a later revision.
