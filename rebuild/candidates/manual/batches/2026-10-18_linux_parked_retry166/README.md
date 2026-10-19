# Fixed-size allocator allocation and deallocation

00A66450 and00A662A0 independently pass RELOCATION_MATCH250/242 with the entire production source and expanded actual allocator header in C++98/O2 host, no-elision and canonical VC7.1 fixtures. Seven immutable grades retained (3 and4); frozen fixture/source hashes match worker evidence.

Root audits all six masked call relocations and native linked-list helpers, genuine NewChunk pointer return and allocation/deallocation signatures. Chunk24 and area4 layouts are asserted; the native fixed-size area is distinct from similarly named variable-size areas. Actual header POD extensions are field/method views initialized by fixtures, not claims to construct full polymorphic engine objects. No fabricated virtual padding or constructors are added.

Tests cover current/first/new chunk fallback, exhaustion, reversed free indices, allocated/free list insertion/removal, head/interior/tail unlink, full-chunk movement, empty-chunk threshold, unchanged allocator IDs and returned data addresses. Native helper definitions are retained where their known ECX preservation affects caller code. Dependency allocation/deletion is stubbed; these are behavioral unit fixtures, not full-game heap integration.

Terrain0081E810 initially passed bytes/tests but a pointer versus actual const-reference dependency declaration was found. Old evidence preserved under height_signature_hold; excluded from this payload/count pending corrected source review.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
