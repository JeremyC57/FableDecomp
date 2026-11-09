#ifndef FABLE_ENGINE_CCRC_H
#define FABLE_ENGINE_CCRC_H
// CCRC (Ego_r.pdb ?Calc@CCRC@@SIKKPBXK@Z, ?CalcFromString@CCRC@@SIKABVCCharString@@@Z, ?CalcFromPathname@CCRC@@SIKABVCCharString@@@Z):
// the engine's table-driven CRC-32 (reflected, seed passed in, no inversion). Calc is out of line at 0x004014A0 (landed
// CCRC_Calc_004014a0.cpp, the body below); the table is 0x0129A168.
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CCRC_CALC_BODY
// Retail opt-in (chunk 892, row 0x00B21A50 CSplinePosParamData::DatabaseSerialise): retail is link-time code generated, so its
// callers see Calc's body and VC7.1's inliner decides per site -- 0x00B21A50 calls it out of line once (0x00B21AD0, the empty
// default's CRC) and expands it inside its load loop (0x00B21BEB..0x00B21C0E). The visible body reproduces both decisions.
extern unsigned long g_CRCTable[256];                                   // 0x0129A168
class CCRC {
public:
    static unsigned long FABLE_FASTCALL Calc(unsigned long seed, const void* data, unsigned long byteCount)
    {
        if (byteCount != 0) {
            const unsigned char* bytes = static_cast<const unsigned char*>(data);
            for (unsigned long index = 0; index < byteCount; ++index)
                seed = g_CRCTable[bytes[index] ^ (seed & 0xFF)] ^ (seed >> 8);
        }
        return seed;
    }
};
#else
class CCRC {
public:
    static unsigned long FABLE_FASTCALL Calc(unsigned long seed, const void* data, unsigned long byteCount);   // 0x004014A0
};
#endif
#endif
