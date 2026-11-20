# NF160 lessons

- coll.py's "OK" for a callee can rest on a split that already exists. RenderProgress 0x00499880 was bound as both QAE and AAE at HEAD.
  Before saying "no split", check reloc_bindings.tsv for every spelling at the address and say in NOTES which one the row moves to.
- A self row with only a prefix Leaf line (`?RenderProgress@CProgressDisplay@`) does not prove its full spelling. Find its access
  (struct means public QAE) in the source.
- To port an STLport vector stand-in, start from the landed recipe row (0x0064E1E4). Most SRC_FAILs were local leftovers:
  - a private std::allocator block;
  - a forward declaration where a full header include was needed;
  - stand-in accessor names such as GetCount and _M_start.
  Grep for them before the first grade.
- Moving a member to std::vector in a class that has a row-local base model makes the constructor emit the base's implicit destructor
  COMDAT. Expect it in the objdump defs diff, and flag it if the base class has a header.
- An argument-splitting transformer (tinit.py) breaks on nested calls such as `C2DExtentsI(256, 256)`. Check every rewritten call site
  with `cpp`, or parse with a bracket counter.
- When a row asserts a token is gone after a respell, keep that old token out of new comments too. 0x0049124E failed its own check on
  a comment.
- In the StopProgress port, CRenderManager.h had to come before CDisplayManager.h, and FABLE_RETAIL_CPIXELFORMAT was needed for
  D3DCAPS9. Read a header's opt-in prerequisites before the first precheck.
- std::list ports need FABLE_RETAIL_STLP_LIST. Without it, `erase` is missing and the error message does not point to the macro.
- A world-map family (OnGameMapUnloaded, SetMapVisibility, SetMapVisibilityAllMaps and their callee 0x00B41DB0) can only move
  together. Claim the callee's lock early and grade all four in one batch.
