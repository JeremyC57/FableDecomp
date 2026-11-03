/* pshpack1.h -- the Windows SDK's 1-byte packing boundary, as VS .NET 2003's PlatformSDK ships it (directive text only).
   Retail evidence (owner 2026-11-02, "if that is how retail did it then you should too"): CImaAdpcmCodec::Convert 0x00C5FA50 reads
   IMAADPCMWAVEFORMAT::wSamplesPerBlock at +0x12 and m_cmCodecMode at +0x14 (`movzx ebp,word [ebx+12h]`, `mov eax,[ebx+14h]`);
   natural alignment would put them at +0x14 / +0x18, so retail compiled mmreg.h's tWAVEFORMATEX / ima_adpcmwaveformat_tag inside
   <pshpack1.h> / <poppack.h>. Included only by SDK-boundary headers (engine/CImaAdpcmCodec.h). */
#if ! (defined(lint) || defined(RC_INVOKED))
#if ( _MSC_VER >= 800 && !defined(_M_I86)) || defined(_PUSHPOP_SUPPORTED)
#pragma warning(disable:4103)
#pragma pack(push,1)
#else
#pragma pack(1)
#endif
#endif
