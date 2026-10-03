# `assets/` — portable retail-asset layer (C++23)

First building block of a **portable port** (x64 Windows, Linux, Android): read every asset straight
from the user's own retail install at runtime, with no Windows APIs, so no game data ever ships with
the port. This is a modern-lane module (see `../README.md`): it does not replace any
`rebuild/src/compiled` parity source and is not counted toward retail-match coverage.

## What it models

| Piece | Retail reference | Evidence |
|---|---|---|
| `GameInstall` | install layout under `Fable The Lost Chapters/` | required-file check; case-insensitive paths for non-Windows filesystems |
| `BigArchive` | BIGB v100 bank archives | `docs/formats/BIG.md` §2–3 (header, footer, stats header, TOC records) |
| `lzo1xDecompress` | `lzo1x_decompress` @ `0x00C06B90` / `_safe` @ `0x00C08170` | standard LZO1X grammar, bounds-checked |
| `decodeChunkedLzo` | Fable chunked-LZO frame | `docs/formats/BIG.md` §6 |
| `TextBank` | `text.big` TEXT_* bank; `NGameText::CDataBank` lookup by ID | `docs/formats/TEXT.md` §4 |
| `TextureInfo`, `decodeMip0`, `toRgba8` | 34-byte GBANK descriptor; DXT1 / DXT3 / A8R8G8B8 | `docs/formats/TEXTURE.md` §2–4 |

## Validation (2026-10-02, Steam `Fable.exe` 16,666,624 B install)

- `frontend.big`, `shaders.big`, `text.big`, `effects.big`, `fonts.big` parse with the entry counts in
  `BIG.md` §3 (394 / 465 / 28,913 / 1,165 / 26).
- All 394 `frontend.big` textures decode; every mip 0 is **byte-identical** to
  `tools/lionhead_lz.py` + raw tail, and consumes exactly `MipSize0` bytes.
- `FRONTEND_BACKDROP_01` (DXT1), `FRONTEND_TITLE_01_SPRITE` (ARGB) and `FRONTEND_BUTTON_L_SPRITE`
  (DXT3) export as coherent images.
- `text.big`: all 26,807 strings, 2,105 groups and 379 narrators decode with zero unexplained bytes.

Unit tests use synthetic data only. Point `FABLE_INSTALL_DIR` at an install to add the retail checks.

## Build and use

```sh
cmake -S rebuild/modern -B build/modern
cmake --build build/modern
ctest --test-dir build/modern
build/modern/assets/fable_assets install "<Fable The Lost Chapters>"
build/modern/assets/fable_assets list    "<...>/data/graphics/pc/frontend.big" --entries
build/modern/assets/fable_assets verify  "<...>/data/graphics/pc/frontend.big"
build/modern/assets/fable_assets text    "<...>/data/lang/English/text.big" TEXT_QST_028_ONSCREENHELP_FLOURISH_BASIC
build/modern/assets/fable_assets texture "<...>/frontend.big" FRONTEND_BACKDROP_01 out.tga
```

Exports are for local inspection only; never commit or redistribute them.

## Not yet covered

- Mips 1..n (stored raw after mip 0), multi-frame sprites, formats `0x18` / `0x23`
- Mesh (`MBANK_*`), animation (`3DAF`), lipsync, font, particle and shader payload decoders
- `.wad` / `.wld` / `.lev` / `.stb` level data, `.lug` / `.met` audio banks, `.lut` dialogue audio
- Memory-mapped I/O and Android asset-storage access (SAF) — the reader takes a filesystem path today
