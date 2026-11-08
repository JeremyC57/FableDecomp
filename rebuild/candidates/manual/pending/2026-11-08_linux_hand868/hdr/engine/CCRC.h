#ifndef FABLE_ENGINE_CCRC_H
#define FABLE_ENGINE_CCRC_H
// CCRC (chunk 868, row 0x00A2F740 C3DMeshStats::InitialiseFromFileData): the engine's CRC-32 helper. FableWin.pdb
// ?Calc@CCRC@@SIKKPBXK@Z (static __fastcall: seed in ecx, data in edx, byteCount on the stack), ?CalcFromString@CCRC@@SIKABVCCharString@@@Z
// 0x00AEAA10 and ?CalcFromPathname@CCRC@@SIKABVCCharString@@@Z 0x00AEAA70. Calc is out of line at 0x004014A0 (landed
// CCRC_Calc_004014a0.cpp: the crc0 table step over the 256-entry table at 0x0129A168, `byteCount != 0` guarded separately from the
// loop test). The same local model lives in engine/CSpecialEffectsDef.h (a declaration only); this header is the shared one.
#include "rebuild_abi.h"
extern unsigned long g_CRCTable[256];                                    // 0x0129A168 (the landed 0x004014A0 row's name; no PDB symbol)
class CCRC {
public:
#ifdef FABLE_RETAIL_CCRC_CALC_INLINE
    // Retail opt-in (chunk 868, row 0x00A2F740): the mesh-stats TU expands Calc in place inside CCharString::GetCRC three times
    // (0x00A2F88C..0x00A2F8BD, 0x00A2FB7E..0x00A2FBAD, 0x00A2FD12..0x00A2FD3D: `xor eax,eax` for the folded seed, `test ; je`, then
    // `xor ecx,ecx ; test ; jbe` and the table loop), the body of the landed 0x004014A0 row.
    static inline unsigned long FABLE_FASTCALL Calc(unsigned long seed, const void* data, unsigned long byteCount) {
        if (byteCount != 0) {
            const unsigned char* bytes = static_cast<const unsigned char*>(data);
            for (unsigned long index = 0; index < byteCount; ++index)
                seed = g_CRCTable[bytes[index] ^ (seed & 0xFF)] ^ (seed >> 8);
        }
        return seed;
    }
#else
    static unsigned long FABLE_FASTCALL Calc(unsigned long seed, const void* data, unsigned long byteCount);   // 0x004014A0
#endif
    static unsigned long FABLE_FASTCALL CalcFromString(const class CCharString& text);      // 0x00AEAA10
    static unsigned long FABLE_FASTCALL CalcFromPathname(const class CCharString& text);    // 0x00AEAA70
};
#endif // FABLE_ENGINE_CCRC_H
