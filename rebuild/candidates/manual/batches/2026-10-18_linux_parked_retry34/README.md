# Steal/recovery AI event validation

00965520 and009643d0 (279 bytes each, including embedded switch tables): actual state/brain/entity-event headers and PDB scratchpad flag offsets105/104. Canonical verify_land_jumptable.py reads raw COFF to preserve both switch tables. Main review retains cases1/6/13 and conditional aggressive27, all-event traversal and cleanup state.

Both fixtures execute exact source on host and VC7.1:62 type/aggression combinations, disabled/empty paths, full traversal after interruption, sentinel lookup and existing cleanup persistence. Source grades3/2, including recorded initial compile failures.

All independently RELOCATION_MATCH + PASS, with verbatim-source host and canonical VC7.1 fixtures.
