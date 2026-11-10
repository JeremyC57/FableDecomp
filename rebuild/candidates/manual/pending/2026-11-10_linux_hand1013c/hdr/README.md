# Chunk 1013c header overlay

Every grade ran with `FABLE_HDR_OVERLAY=$A/hdr`, but this chunk changed no header: the overlay is empty and every row builds
on the committed rebuild/include (HEAD). The probe-only copy of engine/CEngineWorldMap.h with GetMap variants (`$A/ph`, selected
by a `PV` macro) was used for free c2prio compiles only and is not part of the hand-back.
