# NF71 lessons (one line each)
- An opt-in `#define` placed after `#include "fable_string.h"` never applies to fable_containers.h (fable_string.h includes it first): NF60's 0x00E12580 draft carried two dead STLport defines, and the port only matched once they were dropped (the default inline ~vector, split by the budget).
- Bisect the inline budget on the ported text, not on the old draft: 0x00E12580 matched at T60 (T59 / T58 one exit short).
- A copy expanded at a by-value argument but called inside an expanded predicate ctor is the landed CIsThingAlive idiom (`#pragma inline_depth(0)` round the row-local ctor + CPREDICATEBASE_NOVTABLE + IMPLICIT_COPY); /d2inlT never split the two sites (0x00EE2490).
- A one-register residue after a port is often a missing PDB inline accessor: FableWin's GetMasterData moved PMasterData from ecx to eax (0x00EE2490).
- Header families document their release orders: `mov edi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edi` is CIVCountedPointer RELEASE_MEMBER + RELEASE_DELETES, and it needs FABLE_RETAIL_CIVCOUNTEDPOINTER too (without it the caller also deletes: 282 v 273).
- Read retail's frame before ordering locals: construction order and slots told which pair was declared first (0x004FE630), which Ego_d's /Od offsets alone did not.
- reloc_bindings.tsv keeps one example row per (symbol, target): it can show that a wrong spelling exists but not that only one row binds it; scan sources or use callers.py for that.
- Ego_d locals are the source names but not proof of types the retail bindings contradict (0x004FE630 `long` counters vs the landed ReadVar<int> 0x00516180): keep the landed binding.
- Name scratch scripts so they do not shadow stdlib modules (`dis.py` broke capstone's import of `dis`).
