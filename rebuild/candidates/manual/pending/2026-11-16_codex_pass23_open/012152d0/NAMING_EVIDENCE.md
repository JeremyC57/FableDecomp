# Identity and provenance
The reserved012152d0 range is8 bytes of destructor cleanup and10 bytes of handler, generated from00c425e0(217bytes); FuncInfo0136f4fc/unwind map0136f4f8 and handler012152d8 point back to this parent.

Parent DecodeXboxAdpcm is descriptive, NOT a recovered original libpk name. Searched all three original PDB name sets; no DecodeXbox/DecodeIma hit. Current canonical caller binding at00c34630 is ?DecodeXboxAdpcm@@YIJPBXKKPAXK@Z. Native whole-block channels*36 input, channels*128 output,44100Hz,64samples and codec calls independently establish behavior and ABI.

Codec uses existing engine/CImaAdpcmCodec.h. Original Microsoft2001 imaadpcm.h/.cpp retained in the workspace were read for constructor/destructor, Initialize and CreateImaAdpcmFormat, confirming class/member/enum/parameter names. Native C5EFA0 mov eax,ecx;ret and C5EFB0 ret independently prove empty bodies. Format native offsets0,2,4,8,12,14,16,18 match the existing SDK packing boundary; mode20 and indices24/28 and converter32 match Initialize. Existing SDK packing is reused, none introduced. Original upstream helpers default cdecl become fastcall in this native library, proven by ECX/EDX and stack cleanup, already modeled in shared header.

The current old binding for Initialize uses int rather than original CODEC_MODE and names IMAADPCMWAVEFORMAT instead of its actual ima_adpcmwaveformat_tag. This experiment uses original SDK types. It is not landed; final full emitted-symbol and relocation acceptance remains pending and any found wrong bindings must be in NAME_FIXES before acceptance.

## Supplementary retry provenance
The original SDK1532-line .cpp and153-line .h were read completely. This adds no newly recovered original libpk parent name. DecodeXboxAdpcm remains explicitly descriptive. Actual body visibility trials retain genuine SDK names/types and do not establish a matched helper. The old hand438 file was unavailable to this worker; prior main reading claims are historical rather than newly verified. No new naming or header correction is proposed.


Current final disposition after Sol continuation42–66: canonical descriptive DecodeXboxAdpcm and the existing real Microsoft SDK shared declarations remain selected unchanged. Parent217/native vs221/emitted is NOT matched. The earlier historical-source gap is resolved by locating/reading the actual hand438 file (receipt RETRY_SOL_NEARMISSES/HISTORY_RECOVERY.json); its forbidden local models are not reused. Full type/operand acceptance remains pending an exact parent. No new header/ABI or helper-match claim. FROZEN.json records exact source/test hashes and final same-tree702native/GCC plus3GCCthrow checks.
