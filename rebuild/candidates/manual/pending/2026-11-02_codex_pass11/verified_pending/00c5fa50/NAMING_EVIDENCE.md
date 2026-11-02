# CImaAdpcmCodec::Convert identity and validation

## Identity and exact declaration

There is no CImaAdpcmCodec public symbol entry in the supplied donor PDB name sets. This is original Microsoft Xbox SDK vendor code, not a guessed engine function. Two independent sources settle its names and types:

1. The original Microsoft 2001 imaadpcm.h and imaadpcm.cpp preserve the complete class, private/public access, CODEC_MODE enum, converter arguments and source variable names. The function's distinctive diagnostics exactly identify its four-mode implementation: normal encoding, whole-file optimization, per-block optimization and decoding. References were retrieved read-only from ProjectDreamland/area51 commit dfaa140222e8918c47b844af15dd10150b05a9a9, Apps/SoundPackager/imaadpcm.h, and nillerusr/source-engine commit ed8209cc35c61fbd8ddff8480962a01c981eef2f, utils/xbox/MakeGameData/imaadpcm.cpp. Original copyright is preserved. These are vendor source references, not game-file links.
2. The native caller at 00c42688 supplies the codec object and three stack arguments; the target returns with ret 0x0c. Native mode dispatch, all five diagnostic strings, seven converter arguments, two-decoder table and 89-entry step search agree with the SDK source. Native offsets are format +0, CODEC_MODE +0x14, left/right step +0x18/+0x1c and converter +0x20. The actual class size is 0x24.

The full emitted Leaf is ?Convert@CImaAdpcmCodec@@QAEHPBXPAXI@Z. /Gr gives the static seven-argument converters their native fastcall ABI while the instance function remains thiscall. Private DecodeM16 / DecodeS16 must have CI, not the SI public-static access code in older landed rows. CalcDifference is private (AAE), returns unsigned 64-bit, and takes two BYTE pointers, two UINT counts and one DWORD block size. Its five stack words and EDX:EAX result agree with native call sites. All emitted spellings are checked in objdump. The old m_fEncoder BOOL model was wrong: the true SDK member is CODEC_MODE m_cmCodecMode.

## Header and SDK layout

hdr/engine/CImaAdpcmCodec.h preserves the complete actual SDK class, rather than a reduced stand-in. No corresponding input header exists. The Windows SDK's true tags are tWAVEFORMATEX and ima_adpcmwaveformat_tag, not newly named WAVEFORMATEX or IMAADPCMWAVEFORMAT structs. The native sizes are 18 and 20 bytes respectively. Their declaration boundary uses the original standard <pshpack1.h> / <poppack.h> SDK headers; no packing directive is authored in returned code. The workspace's stripped compiler distribution lacked these dependencies, so unchanged Microsoft SDK copies were installed in its system include directory. Reproduction needs those normal SDK headers. No compiler or SDK binary is part of the returned work. GNU64 fixture types preserve fixed-width DWORD; the host's aggregate padding is immaterial to behavior and native assertions prove the real sizes.

## Bytes, extent and relocations

Both grades reproduce all 1244 assigned bytes with relocated operands masked. The span is one complete function plus alignment and its own four-entry switch table at 00c5ff1c; it ends at 00c5ff2c. Table cases resolve to 00c5faa4, 00c5facc, 00c5fb54 and 00c5fce8. All 35 function relocations were reviewed: 24 external references, six string references and five internal labels. The two additional decoder-table relocations at 012b9314/012b9318 point to DecodeM16 00c5f6a0 and DecodeS16 00c5f7f0. The PE import table independently identifies IAT 01440004 as printf. Array allocation/deallocation use exactly ??_U at 00bfeb22 and ??_V at 00bfeb1c. All three direct CalcDifference calls bind 00c5f150. No scalar allocation substitution is present.

## Behavior validation and limits

The self-contained fixture pastes the selected body and the complete header. Twenty-four scenarios (12 each for mono/stereo) exercise direct decoding and its failure, normal encoding and each converter failure, whole-file search, a skipped candidate, all candidates failing, per-block constrained search, both range-clamping edges, and unknown modes. It checks output zeroing, candidate counts/order, decoder channel selection, fixed 64-sample decode calls, source/destination stride, selected output blocks, normalization count and diagnostic count. Codec dependencies are controlled fixture implementations; CalcDifference uses the SDK's normalized squared-error calculation. Therefore this tests Convert's orchestration and selection logic, not a second verification of the external compression kernels. Allocation exhaustion is not forced. The native code's unchecked final per-block allocation and ignored final decoder result are preserved, not repaired. GNU64 C++03 / no RTTI and VC7.1 RTM /GR- both pass with exactly 00c5fa50_TEST PASS.

## Naming corrections for landing

VENDOR_NAME_FIXES.tsv records the four converters' private-static access plus Initialize's original ima_adpcmwaveformat_tag pointer and CODEC_MODE enum. Older NAME_FIXES_QUEUE entries corrected argument counts but retained the wrong public access. The original SDK header resolves that remaining uncertainty. These reports do not edit existing landed files or the repository catalog.
