# Linking third-party library code (owner direction, 2026-11-08)

Owner, verbatim: "if the audio code is a library, then we can typically skip it in a byte matching decompilation. The core strategy is
to isolate, identify, and link original or rebuilt library object files rather than rewriting them by hand."

Region map: `LIBRARY_MAP.tsv` (this directory). Rule text for chunks: `EXTRA_RULES.md`, "Owner direction, 2026-11-08".

## Summary
- Third-party vendor code (link route): VC7.1 CRT (done, `toolchain_library`), zlib 1.2.1 (Z_PREFIX), LZO 1.08, libogg 1.1.1/1.1.2 +
  libvorbis 1.0.1 (except Lionhead's patched vorbisfile.c), DirectShow BaseClasses (DX9 SDK, version to pin), DXUT ImeUi.cpp (2004/05).
- Lionhead in-house code (reconstruction, NOT library): libpk + the "CODA" audio engine (0x00C086C0..0x00C89C10, 224 unlanded rows;
  `d:\dev\audio\ver_fable_1.5_b\libpk\` paths, same-directory `.\PKC*.cpp` / `.\CCodaFrameStreamer.cpp` asserts, compiled by the same
  cl 13.10.3077 as the game per the Rich header) and LHNetworkLib ("Lionhead Studios HTTP Library Version 1.0"). No legitimate source,
  SDK or object release exists for either, or for Sensaura's GameCODA. Anything circulating would be a leak: never fetched, never used.
- Probe (2026-11-08): the unmodified zlib 1.2.1 tarball (sha256 94ded520...) compiled `/TC /MD /GS /DZ_PREFIX` grades
  RELOCATION_MATCH for `z_inflate` 0x00C02470 under the lane grader (re-checked by the coordinator). Across the probe builds 304 retail
  functions match vendor bytes: all linked zlib and LZO C code, libogg 1.1.1 55/56 sections, libvorbis 1.0.1 176/196 (every miss in
  vorbisfile.c). Flags: zlib/LZO `/O2 /Oy /W3 /MD /GS`; Xiph `/O2 /Ob1 /Oy /W3 /MD /GS /TC /Gr`.

- **Done 2026-11-08 (vendor-library pass):** zlib 1.2.1, LZO 1.08, libogg 1.1.1 and libvorbis 1.0.1 are proven from
  their unmodified source with grade `toolchain_library` and a `// Vendor-library:` claim (config
  `tools/decomp_pipeline/libsig/vendor_libs.tsv`: URL, sha256, flags; trees under `FABLE_THIRDPARTY`, fetched by
  `fetch_vendor_libs.py`). `z_inflate` 0x00C02470 landed; the 248 landed hand-written rows the vendor build reproduces
  keep their bodies and carry a `// Vendor-proof:` line; 249/249 `lib_verify.py` MATCH. Details and the grade decision:
  `LIBRARY_SIGNATURES.md`, "Vendor libraries". Coverage by code origin (game / Lionhead library / third-party /
  toolchain) is on the dashboard (`rebuild/COVERAGE.md`).

## Availability
Verified 2026-11-08 from the Linux lane. "Verified" means the URL answered HTTP 200 to curl through the
session proxy in this session (hosts that the proxy would not resolve are marked). Nothing third-party
was copied into the repo; fetched sources live under `$S/w22/libresearch/dl/`.

Rich-header reading used throughout (product IDs per the community table at
<https://gist.github.com/skochinsky/07c8e95e33d9429d81a75622b5d24c8b>, which states most values come
from "old MSVC leak with private PDBs"): retail lists 1369 `Utc1310_CPP` (C++, cl 13.10.3077), 51
`Utc1310_C` (C, 3077), 12 `Masm710` (3077), 5 `Utc1310_C` (2179), 4+1+6 `Utc1310_C/CPP/Implib710`
(4035), 31 `Implib710`, 422 `Import0`, 1 `Utc13_C` (VC7.0 build 9178), 2 `Implib700` (9210), 1
`Linker710`, 1 `Cvtres710`. **There is no VC6, VC7.0 C++, Intel or LTCG object**, so every C++ library
in the image (strmbase, ImeUi, libpk, "CODA") was compiled from source by Lionhead's own cl 3077, and
only C libraries could have come in as prebuilt objects (none did: the 51 C objects are accounted for
by zlib 9 + LZO 5 + libogg 2 + libvorbis 21 + CRT 5 + a few C TUs of libpk's CPU-ID / audio format
code).

### 1. zlib 1.2.1 — third-party, source available, drop-in proven

* Official archive: <https://zlib.net/fossils/zlib-1.2.1.tar.gz> (verified 200; 345,833 bytes;
  sha256 `94ded52040ee9dd1c70cc3b9f01da283803c28c1194a5a40659e8cf7545990e3`; fetched to
  `$S/w22/libresearch/dl/zlib/`). Alternative: GitHub `madler/zlib` tag `v1.2.1` (<https://raw.githubusercontent.com/madler/zlib/v1.2.1/zlib.h> verified 200).
* Version match: retail strings "deflate 1.2.1 Copyright 1995-2003 Jean-loup Gailly" /
  "inflate 1.2.1 Copyright 1995-2003 Mark Adler" and `ZLIB_VERSION` "1.2.1"; **probe: 89 code
  sections byte-identical** (see PLAN.md and `probe/objscan_zlib_lzo_MD_GS.txt`).
* License: zlib License, <https://zlib.net/zlib_license.html> (verified 200): "Permission is granted
  to anyone to use this software for any purpose ... freely". Linking rebuilt objects is permitted;
  the only duty is to keep the notice and mark altered sources (we alter nothing).

### 2. LZO 1.08 — third-party, source available, drop-in proven (GPL caveat)

* Official archive: <https://www.oberhumer.com/opensource/lzo/download/LZO-v1/lzo-1.08.tar.gz>
  (verified 200; already present at `$S/thirdparty/dl/lzo-1.08.tar.gz`, `FABLE_THIRDPARTY`).
  Project page <https://www.oberhumer.com/opensource/lzo/> (verified 200).
* Version match: the ML-assembled `lzo1x_f1.asm` is already proven (`lib_verify.py` 0x00C069D0);
  **probe: all 26 linked C functions of lzo1x_9x.c / lzo1x_d1.c / lzo1x_d2.c / lzo_util.c are
  byte-identical** with the same VC7.1 flags as zlib.
* License: the page says "GNU General Public License (GPL v2+)" and "Commercial licenses are
  available through our LZO Professional license program". Lionhead shipped it under a commercial
  licence; for the reconstruction, linking the GPL'd rebuilt objects is legally the same situation
  the repo already accepted for the vendor `.asm` (owner, 2026-10-31). Flag it in the repo's
  licence notes: a redistributed rebuilt Fable.exe would carry GPL obligations for the LZO part.

### 3. libvorbis 1.0.1 — third-party (Xiph BSD), source available, drop-in proven for 20 of 21 TUs

* Tag `v1.0.1` on the vendor's own GitLab: <https://gitlab.xiph.org/xiph/vorbis/-/tags/v1.0.1>
  (verified 200) and GitHub mirror `xiph/vorbis` tag `v1.0.1`
  (<https://raw.githubusercontent.com/xiph/vorbis/v1.0.1/lib/info.c> verified 200; the whole `lib/`
  and `include/` tree was fetched file by file into `$S/w22/libresearch/dl/xiph/vorbis-v1.0.1/`).
  Official tarball <https://downloads.xiph.org/releases/vorbis/libvorbis-1.0.1.tar.gz> redirects to
  `ftp.osuosl.org`, which this session's proxy refuses (TLS) — fetch it from a normal machine.
* Version match: `lib/info.c` line 419 of v1.0.1 is `"Xiph.Org libVorbis I 20030909"`, the exact
  string at 0x012BF240. The landed notes that call it "post-1.0.1 CVS" are wrong: 1.0.1's release
  string *is* 20030909 (the release was tagged 2003-11-17 from that CVS state).
* **Probe: 176 of 196 code sections byte-identical** with `/O2 /Ob1 /Oy /W3 /MD /GS /TC /Gr`; the
  20 misses are all `vorbisfile.c`, which the landed rows already document as Lionhead's modified
  copy ("libpk fork of vorbisfile"). Four TUs (floor1.c, lsp.c, psy.c, sharedbook.c) do not compile
  under `/Gr` because their qsort comparators become `__fastcall`; retail's comparators are
  `__cdecl`, so Lionhead patched those four declarations (a three-token patch per file).
* License: BSD 3-clause (`COPYING`, "Copyright (c) 2002, Xiph.org Foundation", verified from the
  tag). Linking rebuilt objects is permitted with attribution.

### 4. libogg 1.1.1 / 1.1.2 — third-party (Xiph BSD), source available, drop-in proven

* Tags on GitLab <https://gitlab.xiph.org/xiph/ogg/-/tags> (verified 200; tags v1.1.1, v1.1.2,
  v1.1.3, ... — there is no `v1.1`/`v1.0` tag on either mirror) and GitHub `xiph/ogg`
  (<https://raw.githubusercontent.com/xiph/ogg/v1.1.1/src/framing.c> verified 200; v1.1.1, v1.1.2
  and v1.1.3 fetched into `$S/w22/libresearch/dl/xiph/ogg-v1.1.x/`).
* Version match: **1.1.1 and 1.1.2 give identical objects and match 55 of 56 retail sections**
  (only `oggpack_writecopy_helper` differs); 1.1.3 loses `ogg_stream_flush` and `ogg_stream_pagein`,
  so retail is 1.1.1 or 1.1.2, not 1.1.3 and not 1.1 (which lacks nothing we tested but is untagged).
* License: BSD 3-clause. Linking permitted.

### 5. DirectShow BaseClasses (strmbase) — Microsoft SDK sample source; several legitimate copies

* GitHub, MIT licence: <https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/Win7Samples/multimedia/directshow/baseclasses>
  (raw files verified 200, e.g. `.../baseclasses/amfilter.cpp`; `LICENSE` = "The MIT License (MIT)
  Copyright (c) Microsoft Corporation"; 68 files fetched into `$S/w22/libresearch/dl/ms/baseclasses/`).
  This is the Windows 7 SDK revision ("Copyright (c) 1992-2001 Microsoft Corporation" header).
* Microsoft download, still live: Windows SDK 7.1 ISO
  <https://www.microsoft.com/en-us/download/details.aspx?id=8442> (GRMSDK_EN_DVD.iso 567.3 MB;
  `Samples\multimedia\directshow\baseclasses`). Same revision as GitHub.
* Period revision (what Lionhead most likely compiled): the DirectX 9.0 SDK installers, mirrored on
  archive.org as Microsoft's own installers (items `dxsdk_sum2004`, `dxsdk_oct2004`, `dxsdk_dec2004`,
  `dxsdk_feb2005` + `dxsdk_feb2005_extras` (37 MB, the DirectShow extras), `dxsdk_apr2005`; metadata
  verified via <https://archive.org/metadata/dxsdk_apr2005> etc., uploader wobakj@web.de, files
  `dxsdk_*.exe`). Elecard's SDK page (verified 200) records: "The last version of the DirectX SDK
  containing DirectShow was the DirectX 9.0 SDK Update - (February 2005) Extras. After this version,
  DirectShow was moved to the Windows SDK." Retail's D3DX is `d3dx9_25.dll` (April 2005 SDK), so
  Lionhead's BaseClasses came from the Feb 2005 Extras or an earlier 2004 SDK. The archive.org
  `details` pages themselves are not reachable from this session (host not resolvable); the
  metadata API was.
* License: DirectX SDK EULA permits using and redistributing the sample code in your own products;
  the GitHub copy is MIT. Linking rebuilt objects is permitted.

### 6. DXUT `ImeUi.cpp` — Microsoft DXUT sample source

* GitHub, MIT: <https://github.com/microsoft/DXUT/blob/main/Optional/ImeUi.cpp> (raw verified 200,
  fetched to `$S/w22/libresearch/dl/ms/dxut/`). All 11 retail `ImeUi_*` exports exist in it, but the
  retail object also has `WA_GetCandidateList`, which this revision no longer has, so retail is an
  older (2004/2005) ImeUi.cpp.
* Period copies: the DX SDK installers above (`Samples\C++\Common\ImeUi.cpp`, from Oct/Dec 2004 on)
  and the still-downloadable June 2010 SDK
  <https://www.microsoft.com/en-us/download/details.aspx?id=6812> (DXSDK_Jun10.exe 571.7 MB,
  `Samples\C++\DXUT\Optional\ImeUi.cpp`, an intermediate revision).
* License: DX SDK EULA / MIT. Linking permitted.

### 7. OpenAL / EAX — dynamically loaded, no linked object

* Retail calls `LoadLibrary("OpenAL32.dll" / "wrap_oal.dll")` and `GetProcAddress` for every al*/alc*
  entry and for `EAXSet`/`EAXGet` (strings 0x012B877C-0x012B8F24), so **no OpenAL object is linked**;
  only the headers matter for reconstructing libpk's back end. Note the "GameCODA / OpenAL back end"
  wording in earlier notes is loose: the OpenAL back end is libpk's own C++ (`CAudioContext`,
  `CAudioSource`), not vendor code.
* Headers: Creative's "OpenAL 1.1 Core SDK (zip)" at <https://www.openal.org/downloads/>
  (page verified 200 via curl; link `OpenAL11CoreSDK.zip`); OpenAL Soft (LGPL) at
  <https://openal-soft.org/> (verified 200) ships `AL/al.h`, `AL/alc.h`. EAX 3.0 property-set GUIDs
  need Creative's `eax.h` (EAX SDK); it is no longer hosted by Creative — the GUID values are public
  constants and can be declared in a repo header without any SDK.

### 8. VC7.1 CRT — done

VS .NET 2003 `Vc7/lib` members from the archive.org item named in `docs/BUILDING.md`, fetched by
`tools/decomp_pipeline/libsig/fetch_vs2003_libs.sh`; 440 rows already `toolchain_library`.

### 9. STLport 4.6 (headers) and Dinkumware (MSVCP71 headers)

* STLport 4.6 is open source (SourceForge project `stlport`, <https://sourceforge.net/projects/stlport/files/>
  answered HTTP 200 in this session; the guessed sub-path `.../STLport/STLport-4.6/` returned 404, so browse the
  release list for the 4.6 archive). Not fetched in this session. Dinkumware headers are the VS 2003 `Vc7/include` set, already in the lane as the
  DDK/VS copies. Neither produces linkable objects for Fable (only 3 `msvcprt.lib` thunks are
  library bytes); their value is as *headers* for compiling Fable TUs (see PLAN.md).

### 10. GameCODA (Sensaura) — no legitimate SDK, source or object release exists

* Facts (Wikipedia "GameCODA", fetched 200 via curl): audio middleware by Sensaura, announced
  2002-03-12, "Release 2003", "Final release 2.1", platforms Windows/Xbox/Xbox 360/PS2/GameCube;
  Creative Technology bought Sensaura in December 2003 (Creative press release
  <https://sg.creative.com/corporate/pressroom?id=12009>); Sensaura stopped supplying audio
  technology in 2007-2008. The article cites "Sensaura and Lionhead - Working together to create
  'first-class' game audio" (Wayback, 2004-04-11) — Lionhead was a GameCODA customer. Wayback copies
  of the GameCODA "About", "Concepts Guide" and "Introductory FAQ" PDFs are listed there
  (web.archive.org/web/2006022108061x/http://www.gamecoda.com/documents/...); web.archive.org
  resets connections from this session, so they were not read.
* GameCODA was licensed to studios as an SDK under NDA; Creative never published it, and the product
  is dead. **Nothing legitimate is available.** In Fable.exe the "CODA" code (C_ERR_* table,
  `CCodaFrameStreamer.cpp`, `CInstance.cpp`, "CODA reports source IS playing") was compiled by
  Lionhead's own cl 3077 in the same directory as libpk's `.cpp` files, so even if a GameCODA SDK
  surfaced it would not be a drop-in object: this block has to be reconstructed like game code.
  If any copy of Sensaura's or Lionhead's audio source circulates it is a leak; it was not looked
  for and must not be used.

### 11. libpk, LHNetworkLib / "Lionhead Studios HTTP Library 1.0" — in-house, nothing legitimate

Lionhead-internal libraries compiled from `d:\dev\audio\ver_fable_1.5_b\libpk\` and the LHNetworkLib
namespace. No vendor, no SDK, no public release. They are reconstruction work, not link work; the
PDB-derived types (`fable_types.h`) remain the only legitimate source of structure. Any "source" of
these that could be found online would be leaked Lionhead material and is off limits.

### 12. Not identified as a library after all

* The MFC-style assert text "m_pchData[GetData()->nDataLength] == '\0'" (0x012B206C) belongs to the
  `CString` class at 0x00C0B000-0x00C0E2xx inside libpk: a copy of MFC 6's `strcore.cpp` CString
  adapted into the audio library. MFC 6 source shipped with Visual C++ 6.0 (not freely downloadable
  today); worth a probe only if someone has a licensed VC6 install. 49 rows, mostly landed.
* The CPU-ID table (0x012B87F8-0x012B89AC "Pentium 4" ... "GenuineIntel", used by `CCpuInfo`
  0x00C52xxx) follows the public Intel/AMD CPUID application notes but is Lionhead code.

## Plan
Scope: the "library block" 0x00BFE000-0x00CC0000 (5,648 manifest rows, 266 unlanded) plus the
CRT/LHNetworkLib pieces outside it. Everything below was measured in this session with the lane's
cl 13.10.3077 under Wine; scripts and logs are in `$S/w22/libresearch/probe/`
(`scripts/zlib_probe.py`, `scripts/build_libs2.py`, `scripts/build_xiph.py`, `scripts/objscan.py`,
`objscan_*.txt`, `probe_matches.tsv`).

### What the probe established

| Library | Source as fetched | VC7.1 flags that reproduce retail | Sections matched | Manifest rows matched (landed / unlanded) |
|---|---|---|---|---|
| zlib 1.2.1 | zlib.net fossils tarball, unmodified | `/O2 /Oy /W3 /MD /GS /DZ_PREFIX` | 89 / 89 linked (gzio/uncompr/infback are not linked) | 71 (70 / 1 = `z_inflate` 0x00C02470) + 18 statics/exports not in the manifest |
| LZO 1.08 | oberhumer tarball (already in `$FABLE_THIRDPARTY`) | `/O2 /Oy /W3 /MD /GS` (+ ML for lzo1x_f1.asm, already proven) | 26 / 26 linked C functions | 21 (21 / 0) |
| libogg 1.1.1 (= 1.1.2) | GitHub tag, unmodified | `/O2 /Ob1 /Oy /W3 /MD /GS /TC /Gr` | 55 / 56 (`oggpack_writecopy_helper` differs) | 37 (37 / 0) + 12 unmanifested |
| libvorbis 1.0.1 | GitLab/GitHub tag v1.0.1, unmodified | `/O2 /Ob1 /Oy /W3 /MD /GS /TC /Gr` | 176 / 196 (all 20 misses are vorbisfile.c, Lionhead-patched) | 166 (166 / 0) + 7 unmanifested; floor1/lsp/psy/sharedbook need `__cdecl` on their qsort comparators to compile under `/Gr` |
| union | | | 304 distinct retail functions | 266 landed rows + `z_inflate` + 37 not in the manifest |

Flag facts worth recording in GOTCHAS: zlib/LZO were built `/MD` (the `calloc`/`malloc`/`free` calls
go through `__imp_` slots: that is the only reason `zcalloc` differed under `/ML`) and `/GS`
(`gen_codes`, `inflate_table` and nothing else carry cookies); the Xiph C code was built `/Ob1`
(no automatic inlining: `todB`, `ilog`, `bitreverse` stay out of line), `/Gr` and `/GS`. No library
used `/Oa`, `/O1` or LTCG (the Rich header has no LTCG entries).

### Route per library

#### A. zlib 1.2.1 — DONE 2026-11-08 (z_inflate landed, 50 rows Vendor-proof; 13 unmanifested sections left for a boundary pass)
```
mkdir -p $S/thirdparty/dl $S/thirdparty/zlib
curl -L -o $S/thirdparty/dl/zlib-1.2.1.tar.gz https://zlib.net/fossils/zlib-1.2.1.tar.gz
echo 94ded52040ee9dd1c70cc3b9f01da283803c28c1194a5a40659e8cf7545990e3  $S/thirdparty/dl/zlib-1.2.1.tar.gz | sha256sum -c
tar xzf $S/thirdparty/dl/zlib-1.2.1.tar.gz -C $S/thirdparty/zlib
for c in adler32 compress crc32 deflate inffast inflate inftrees trees zutil; do
  wine $FABLE_VC71/bin/cl.exe /nologo /c /O2 /Oy /W3 /MD /GS /DZ_PREFIX /Fo$S/thirdparty/obj/zlib/$c.obj Z:$S/thirdparty/zlib/zlib-1.2.1/$c.c
done
```
Proof: extend `libsig_scan.py` with a `vendor` library class (`--libdir zlib121=$S/thirdparty/obj/zlib`
already parses, but step 3 of the scan drops patterns from libraries not in its "linked" set, so
the 420 thunks came back and the zlib sections did not: add the tag to the linked set, or add a
`--vendor` list). `lib_verify.py` then re-proves each row from the object exactly as it does for
`msvcrt.lib` members, with the relocations resolved against retail (`__imp__malloc` slots, same-TU
labels, `crc_table`/`length_code` data symbols in .rdata). Landing: body-less stubs with a
`// Vendor-library: zlib-1.2.1(inflate.obj) _z_inflate -- 5204 bytes verified against Fable.exe 0x00C02470`
line, graded by `crawl/purity.py` like `toolchain_library` (owner decision needed: reuse the grade
or add `vendor_library`; the dashboard should keep "third-party vendor bytes" apart from Fable
logic). Covered: the 50 landed hand reconstructions 0x00C00600-0x00C064D0 become stubs, `z_inflate`
0x00C02470 lands, and the 18 unmanifested sections (`z_deflateSetDictionary` 0x00C00470,
`z_deflatePrime`, `z_deflateBound`, `z_deflateCopy`, `z_deflateParams`, `fixedtables`,
`z_inflateSetDictionary`, `z_inflateSync`, `z_inflateSyncPoint`, `z_compress`, `_tr_tally`,
`bi_reverse`, `zError`, ...) become boundary rows the way the CRT ones did on 2026-10-30.
Estimated closure of the 266 unlanded: **1**.

#### B. LZO 1.08 — DONE 2026-11-08 (20 hand bodies kept with Vendor-proof; 5 unmanifested sections left)
Same as A with `lzo1x_9x.c lzo1x_d1.c lzo1x_d2.c lzo_util.c` (`/I include /I src`, no defines) plus
the proven `lzo1x_f1.asm`. `lzo1x_999_compress_level`, `lzo1x_999_compress_dict`, `code_run`,
`min_gain`, `lzo_adler32` (0x00C085A0) are matched but not in the manifest: boundary rows.
Licence note for the owner: GPL v2+ objects in the link (see AVAILABILITY.md §2).

#### C. libogg 1.1.1 + libvorbis 1.0.1 — DONE 2026-11-08 for the 18 TUs that build unmodified (178 rows Vendor-proof)

Status: the `__cdecl` comparator patch below is a source patch, so floor1.c / lsp.c / psy.c / sharedbook.c are NOT
claimed (owner: unmodified vendor source only); `vorbis_book_init_decode` 0x00C95580 stays unlanded and those TUs'
landed rows stay hand-written. Open question: the probe's cdecl build (no `/Gr`) matches exactly the nine statics
`icomp`, `comp`, `apsort`, `sort32a`, `min_curve`, `max_curve`, `vorbis_dBquant`, `post_Y`, `cheby` of these TUs, so
the patch may be wider than the comparator declarations; settle it before a patch file is recorded.
```
# from a normal machine (osuosl TLS is refused by this session's proxy):
curl -LO https://downloads.xiph.org/releases/ogg/libogg-1.1.1.tar.gz
curl -LO https://downloads.xiph.org/releases/vorbis/libvorbis-1.0.1.tar.gz
# or per-file from the tags (what this session did): raw.githubusercontent.com/xiph/{ogg,vorbis}/v1.1.1|v1.0.1/...
cl /nologo /c /O2 /Ob1 /Oy /W3 /MD /GS /TC /Gr /I libogg-1.1.1/include src/framing.c src/bitwise.c
cl /nologo /c /O2 /Ob1 /Oy /W3 /MD /GS /TC /Gr /I libogg-1.1.1/include /I libvorbis-1.0.1/include /I libvorbis-1.0.1/lib lib/*.c
```
Three deviations from "unmodified", each to be recorded as a vendor patch file (never as inline asm):
1. floor1.c / lsp.c / psy.c / sharedbook.c: declare the qsort comparators `__cdecl` (retail's
   `icomp`, `comp`, `apsort`, `sort32a` are `__cdecl`, proven by the cdecl build matching them).
   This also lands the last unlanded Xiph row `vorbis_book_init_decode` 0x00C95580 (sharedbook.c).
2. vorbisfile.c: stays reconstruction (Lionhead's fork; the 20 landed rows already carry the
   `libpk fork` notes). Do not try to stub it from the vendor object.
3. bitwise.c `oggpack_writecopy_helper` (1 section): investigate — probably a `/Ob1` vs inline
   difference or a 1.1.2 edit; the other 55 sections match either tag.
Proof and landing as in A. Estimated closure of the 266 unlanded: **1**; converts ~280 hand-written
C bodies into vendor stubs.

#### D. DirectShow BaseClasses — needs headers + a version probe (410 rows, up to 16 unlanded)
Blocked in this lane: there is no `dshow.h`/`strmif.h`/`uuids.h` under `$FABLE_VC71/include` (only
`strsafe.h`), so `streams.h` cannot be compiled yet. Route:
1. Get the period headers and sources together: `dxsdk_feb2005_extras.exe` (archive.org item of the
   Microsoft installer, 37 MB) or `dxsdk_sum2004.exe`/`dxsdk_oct2004.exe`; extract with 7z on the
   lane; also keep the Win7 SDK/GitHub revision (already fetched, 68 files in
   `$S/w22/libresearch/dl/ms/baseclasses/`) for diffing.
2. Build `amfilter.cpp renbase.cpp combase.cpp ctlutil.cpp mtype.cpp wxlist.cpp wxutil.cpp` with
   `/O2 /Oy /W3 /MD /GS /EHsc /DUNICODE /D_UNICODE /DWIN32 /DNDEBUG` (landed rows: UNICODE release,
   `this` at the interface subobject, `riid == IID_x` as memcmp) and run `objscan.py`; iterate the
   SDK revision until the match count stops rising. Expect >90 % of the 394 landed strmbase rows to
   match one revision; the 12 ret-only gapscan stubs (0x00CA6170.., 0x00CAA720..) are the
   `CBaseFilter`/`CBasePin` default handlers and come with the objects.
3. Prove with `lib_verify.py` (vtable relocations resolve to the retail `??_7CBaseFilter@@6B@`
   tables: the RTTI check already exists for `type_info`).
Estimated closure of the 266 unlanded: **up to 16** (CBasePin::QueryInterface 0x00CA53C0, the 12
stubs, 0x00CA7D60, 0x00CAA470, 0x00CAF430, ~CRendererPosPassThru 0x00CAFD60), pending the probe.

#### E. DXUT ImeUi.cpp — needs `msctf.h`/`imm.h` + a version probe (29 rows, 1 unlanded)
Blocked on the same header gap (ImeUi.cpp needs `imm.h`, `msctf.h`, `windows.h`). Source
candidates in order: DX SDK Oct 2004 / Dec 2004 / Feb 2005 / Apr 2005 `Samples\C++\Common\ImeUi.cpp`
(archive.org Microsoft installers), DXSDK_Jun10 `Samples\C++\DXUT\Optional\ImeUi.cpp`, GitHub
microsoft/DXUT (newest; already fetched; lacks `WA_GetCandidateList`, so it is not the retail
revision). Flags from the landed rows: `/Gr /GS` (plus `/MD`, `/EHsc`? — probe). Closure: **1**
(`ImeUI_FormatCompStringDisplay` 0x00CB2590).

#### F. STLport 4.6 headers — not a link route, but the biggest reconstruction lever
Fetch STLport-4.6 from the SourceForge `stlport` project into `$S/thirdparty/stlport-4.6` and add
`stlport/` ahead of the DDK include path for game TUs (`FABLE_HDR_OVERLAY` already exists for
this). Every `__median`, `_M_insert_overflow`, `_Rb_tree::insert_unique`, `vector::operator=` row
that GOTCHAS says had to be re-derived by hand (waves 17-25) is an instantiation of those headers;
compiling the real headers removes the hand-written STL fronts from the TUs and makes the remaining
STL-shaped unlanded rows (the `_Cons_val<...CParticleMorphs...>` family 0x00C11700/0x00C14700/
0x00C14710/0x00C2D9B0/0x00C47EA0/0x00C48740, `_Insert_n` 0x00C298D0, `_Fill_n` 0x00C6AD10) ordinary
compiles. Dinkumware instantiations inside libpk already use the lane's VC7.1 headers.

#### G. libpk + CODA audio engine — NOT skippable (4,028 rows, 224 unlanded)
This is 84 % of the unlanded library-block rows, and it is Lionhead code compiled from
`d:\dev\audio\ver_fable_1.5_b\libpk\` (Rich header: no foreign objects; `.\PKC*.cpp` and
`.\CCodaFrameStreamer.cpp` compiled in one directory). No vendor object can be linked and no
legitimate source exists (AVAILABILITY.md §10-11). It stays on the reconstruction path with the
PDB-derived types; the only library-style help is (i) the Intel `ivec.h`/`dvec.h` class names the
mixer rows already use (headers in the VS 2003 `Vc7/include`), and (ii) treating the OpenAL/EAX
entry points as plain function-pointer tables (no SDK needed). The 224 rows break down as ~95
`_gapscan`/`sub_` fragments (mostly tiny thunks and EH funclets), ~60 descriptive-named DSP/mixer
bodies (convolution kernels 0x00C79440-0x00C843F0 are large SSE loops), ~20 `PKC*`/`CHandleBasedMemoryPool`
container methods, and ~50 rows with propagated game-class labels that need renaming first.

#### H. LHNetworkLib (60 rows, 2 unlanded) and the CRT leftovers (2)
In-house; reconstruction only. `LHHttp2_HandleResponse` 0x008411D0 and `CheckForActivity` 0x008F8540
are ordinary C++ rows. The two CRT-adjacent rows are a gapscan fragment (0x00BFEEA9, inside the
already-proven `ehvecdtr` section: delete the manifest row) and `CFrameData::ReleaseTexture`
0x00BFEF20 (game stub between CRT objects: reconstruct).

### Expected effect on the 266 unlanded rows in 0x00BF0000-0x00CC0000

| Route | Closes now | Closes after header/version work | Converts landed hand bodies to vendor stubs |
|---|---|---|---|
| A zlib | 1 | - | 50 |
| B LZO | 0 | - | 20 |
| C ogg/vorbis | 1 (with the `__cdecl` patch) | - | ~280 |
| D strmbase | - | up to 16 | up to ~394 |
| E ImeUi | - | 1 | up to 28 |
| F STLport headers | - | ~8 STL-shaped rows become ordinary compiles | n/a |
| G libpk/CODA | 0 (reconstruction) | - | 0 |
| game code in the block (0x00BF0-0x00BFD, 0x00CBE-0x00CBF) | 0 (reconstruction, ~23 rows) | - | 0 |
| **total** | **2** | **~25** | **~770** |

So the library route closes at most ~27 of the 266 unlanded rows; its real value is proving ~770
landed rows as vendor bytes (no further maintenance, dashboard honesty) and pinning the exact
third-party versions. The remaining ~240 unlanded rows are Lionhead audio/engine code and stay on
the reconstruction pipeline.

### Tooling changes to request from the coordinator
Items 1-3 done 2026-11-08 (`libsig_scan.py --vendor`, `lib_verify.py` vendor claims, `toolchain_library` reused with a
`// Vendor-library:` line; `LIBRARY_SIGNATURES.md` "Vendor libraries"). Item 4: the trees are under `FABLE_THIRDPARTY`
and `vendor_lib.py --build` compiles them, but `build_candidates.ps1` does not yet compile/link the vendor objects for
the Windows build. Item 5 is still open (blocks D and E).
1. `libsig_scan.py`: a `--vendor TAG=DIR` library class whose patterns are kept (today only the VS
   2003/DDK "linked" set survives step 3), with the object file name recorded as `member`.
2. `lib_verify.py`: accept loose `.obj` directories as libraries (the COFF reader already does) and
   resolve `__imp_` and same-object data relocations as it does for `msvcrt.lib`.
3. `crawl/purity.py` / dashboard: a `vendor_library` grade (or widen `toolchain_library`'s proof
   line to `// Vendor-library:`), counted as genuine but reported separately from Fable logic.
4. Build: `FABLE_THIRDPARTY` already points at the LZO tree; add zlib-1.2.1, libogg-1.1.1,
   libvorbis-1.0.1 (+ the 4-file `__cdecl` patch) under it, built by `build_candidates.ps1` with the
   flags above, never committed.
5. Lane headers: a Windows SDK 7.1 / DX SDK Feb 2005 include set (`dshow.h`, `strmif.h`, `uuids.h`,
   `amvideo.h`, `imm.h`, `msctf.h`) in the Wine prefix for routes D and E.
