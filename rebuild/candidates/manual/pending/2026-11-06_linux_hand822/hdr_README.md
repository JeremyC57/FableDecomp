# Chunk 822 header overlay

No shared header was changed in this chunk: hdr/ is intentionally empty apart from this file (it exists only so
FABLE_HDR_OVERLAY points at a real directory). All six rows grade against rebuild/include as it stood on 2026-11-07.
One probe (0x00DAF080 b10) used a temporary opt-in copy of engine/CIsThingConscious.h (`FABLE_RETAIL_CISTHINGCONSCIOUS_IMPLICIT_DTOR`
leaving the dtor implicit); it was worse (the implicit dtor drops retail's vtable store) and the copy was deleted.
