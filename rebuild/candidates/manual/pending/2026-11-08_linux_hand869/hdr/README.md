# Header changes -- none new

Chunk 869 adds no header overlay (hdr/ holds only this file). Shared headers used as landed:

- `libvorbis/vorbis_ftoi.h` (landed 2026-11-05 asm migration): the ov_read / ov_read_float TU (`src/00c8bb20.cpp`, `src/00c8bee0.cpp`)
  includes it and gives os.h's `vorbis_ftoi` a same-TU body through `VORBIS_FTOI(i, f)` -- the retail path is the header's isolated
  `fld f / fistp i` block, the FABLE_PORTABLE path is `fable_portable::vorbis_ftoi` (X87FistpNear); the row sources themselves carry
  no asm. The body is there so VC7.1 sees the helper's register use (eax only), which LTCG gave retail; see NOTES.md. The landed
  lsp.c TU (`libvorbis_vorbislsptocurve_00ca4570.cpp`) does the same with a `/QIfist` C++ body; for ov_read that form grades 1000v950,
  the header form matches.
- `engine/Vorbis.h` (unchanged, C-only because of its `class` member): the bitrate.c TU (`src/00c9cc60.cpp`, `src/00c9cff0.cpp`,
  `src/00c9da80.cpp`) includes it under `/TC /Gr`. It has no vorbis_info_psy / vorbis_look_psy / OggVorbis_File / codebook
  declarations, so the psy.c, codebook.c, sharedbook.c and vorbisfile.c rows model those structures the way the landed
  libvorbis neighbours in rebuild/src/compiled/00/c8 and 00/c9 do (same member names and offsets).

Header follow-ups for the coordinator (not done here, outside the chunk):
- `rebuild/include/engine/Vorbis.h` could grow psy.h's `vorbis_info_psy` / `vorbis_info_psy_global` / `vorbis_look_psy`, codebook.h's
  `static_codebook` / `codebook` / `encode_aux_threshmatch` and the libpk `OggVorbis_File` (0x2C0 bytes, no ov_callbacks) now that
  five TUs spell them identically; the rows would then `#include` instead of modelling.
