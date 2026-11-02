# Codex pass 11 -- 0x00C5FA50 CImaAdpcmCodec::Convert (2026-11-02)

RELOCATION_MATCH + PASS (`verify.log`). The Microsoft 2001 Xbox IMA ADPCM SDK codec (imaadpcm.h / imaadpcm.cpp): the class and the mmreg.h
format structs in new `engine/CImaAdpcmCodec.h`, which brackets `tWAVEFORMATEX` / `ima_adpcmwaveformat_tag` with the SDK's `<pshpack1.h>` /
`<poppack.h>` as mmreg.h does. Owner decision 2026-11-02: "if that is how retail did it then you should too". Retail evidence for the 1-byte
packing: Convert reads wSamplesPerBlock at +0x12 (`movzx ebp,word ptr [ebx+12h]`), m_cmCodecMode at +0x14 (`mov eax,[ebx+14h]`) and the step
indices at +0x18 / +0x1C; natural alignment would put them at +0x14 / +0x18 / +0x1C / +0x20. The two SDK boundary headers now live in
rebuild/include (directive text only, with that evidence), so `#include <pshpack1.h>` resolves on the lane as it did in the retail build.
The first regrade's TIMEOUT was a Wine hang (the same source compiles in 0.2 s by hand). Second spelling queued PRIORITY: 0x00C5F150 is
CalcDifference (private), landed as CalculateError.
