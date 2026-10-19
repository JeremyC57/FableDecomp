# Parked and fresh recovery batch 15

Three genuine C++ recoveries, 739 bytes, independently verified RELOCATION_MATCH + PASS before landing. Actual-source fixtures pass g++ and canonical VC7.1/Wine. Final evidence: landing_evidence.txt.

- 00a76580: CNavQuadTreeNode constructor,177 bytes. Explicit vector copy construction fixes the load order (authoring attempt5); coordinator attempt6 adds an unused default constructor to initialize a real vector safely in the test. Local polymorphic hierarchy restores constructors/vtable writes absent from flattened shared layout headers; all field offsets agree. Fixture checks vector/flags, four ordered child reads, byte count and raw flag.
- 00a8b4c0: C3DMesh2::RelocateData,304 bytes. Tests owned-block pointer rebasing, null members, ordered per-primitive virtual dispatch with forwarded arguments, bone-name rebasing/nulls, and qualified base fallback. No shared mesh header exists; offsets derive from landed initialization and size functions.
- 00979d80: CreatureTakeCover::IsRunnableLeaf,258 bytes. Coordinator attempt5 uses the compatible shared CAIBrain header. CreatureTakeCover's generated header exposes only opaque base bytes, so a documented method model describes inherited fields; the component map/dead flag use the retail layout. Fixtures use real component objects and verify every gate, including absent weapon interface, receivers, module arguments and complete short-circuit sequence. Host fixture adapts only the shared brain layout to widened host pointers; target fixture uses the real header.

The volatile-dependent 00a8b3a0 probe is rejected and not included; matching instruction order alone does not justify changing volatile semantics.
