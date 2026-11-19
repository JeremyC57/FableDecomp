# Landing hand4a b10 (2026-11-19): CCpuInfo::GetAMDModelName (0x00C52BB0, 140 bytes) -- Inferred dead test

vland --land, strict headers: RELOCATION_MATCH + PASS (162-case family x model grid; the only surviving mutant `> 8` is equivalent). 72 grades
(plus 254 from earlier lanes). Vendor code with no source anywhere on disk (strings only in retail, after the OpenAL entry names); name descriptive
as the landed siblings; local CCpuInfo model (no shared header).
`// Inferred:` K5 as `Model <= 3` plus a behaviour-neutral `if (Model > 9) return "<unknown>";` at the end of the family-5 arm (falls through to the same
return; VC7.1 deletes the compare, no bytes). Its extra Model reference breaks hand864's D1 allocator tie so the load takes eax (0x00C52BFC). This is the
2026-11-12 rule's "test whose arm was compiled out" form, a fitted tie-breaker: accepted as Inferred on the 0x006E89B0 / 0x00F3ABF0 precedent (coordinator,
under the advisor decision of 2026-11-19). Other measured placements in the source note.
