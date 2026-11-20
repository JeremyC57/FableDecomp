# NF176 header overlay

Every overlay header is a copy of the repo working-tree header (base copy in ../orig/) with `#ifdef` opt-ins only: with none of
the new macros defined, the text the compiler sees is token-identical to the repo header.

## engine/CMeshDataBank.h
- Adds `FABLE_RETAIL_CMESHDATABANK_CLOSE` inside the `FABLE_RETAIL_CMESHDATABANK_BASES` + `_CTOR` class variant (after the
  PROCESSLOADINGQUEUE block): `virtual void Close();` (0x00A26C50, Ego_r / FableWin ?Close@CMeshDataBank@@UAEXXZ) and, under
  `private:`, `void CloseMeshes();` (0x00A26900, ?CloseMeshes@CMeshDataBank@@AAEXXZ), then `public:` again. Declarations only,
  no layout change. Used by rows/00a26c50.cpp and its test.
- pin_asserts.py: pinned 0 (the header's asserts are FABLE_STATIC_ASSERT_AT with fixed line numbers).
- Landed includers: 74 rows include engine/CMeshDataBank.h; none defines the new macro (it exists nowhere in the repo), so none changes.
- Neither NF174's nor NF175's overlay has this header.
