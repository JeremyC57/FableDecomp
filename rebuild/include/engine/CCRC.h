#ifndef FABLE_ENGINE_CCRC_H
#define FABLE_ENGINE_CCRC_H
// CCRC (Ego_r.pdb ?Calc@CCRC@@SIKKPBXK@Z, ?CalcFromString@CCRC@@SIKABVCCharString@@@Z, ?CalcFromPathname@CCRC@@SIKABVCCharString@@@Z):
// the engine's table-driven CRC-32 (reflected, seed passed in, no inversion). Calc is out of line at 0x004014A0 (landed
// CCRC_Calc_004014a0.cpp, the body below); the table is 0x0129A168.
#include "rebuild_abi.h"
#if defined(FABLE_RETAIL_CCRC_ORIGINAL_TYPED_LOOP_PASS27)
// Original FableWin 0186D2D0 and Ego_d local records agree on each typed intermediate.
#ifdef FABLE_RETAIL_CRC32TAB_PASS29
// Ego_r and FableWin ?CRC32TAB@@3QBKB; all 256 values agree with retail 0129A168.
extern const unsigned long CRC32TAB[256];
#else
extern unsigned long g_CRCTable[256];
#endif
class CCRC {
public:
    static unsigned long FABLE_FASTCALL Calc(unsigned long lOldCRC32, const void* pData, unsigned long lNumOfBytes) {
        const unsigned char* pRun = static_cast<const unsigned char*>(pData);
        unsigned long lNewCRC32 = lOldCRC32;
        unsigned long lI;
        unsigned long lX;
        if (lNumOfBytes == 0) return lNewCRC32;
        for (lI = 0; lI < lNumOfBytes; ++lI) {
            lX = pRun[lI] ^ (lNewCRC32 & 0xff);
            lNewCRC32 >>= 8;
#ifdef FABLE_RETAIL_CRC32TAB_PASS29
            lNewCRC32 ^= CRC32TAB[lX];
#else
            lNewCRC32 ^= g_CRCTable[lX];
#endif
        }
        return lNewCRC32;
    }
};
#elif defined(FABLE_RETAIL_CCRC_PASS22_BODY)
// Pass22: equivalent seeded CRC body; old/default arms retained below.
extern unsigned long g_CRCTable[256];
class CCRC {public: static unsigned long FABLE_FASTCALL Calc(unsigned long seed,const void* data,unsigned long byteCount){
if(byteCount==0)return seed;
 const unsigned char* bytes=static_cast<const unsigned char*>(data);
 for(unsigned long index=0;index<byteCount;++index)
  seed=g_CRCTable[bytes[index]^(seed&0xff)]^(seed>>8);
 return seed;
}};
#elif defined(FABLE_RETAIL_CCRC_BANK_STATEMENTS_PASS27)
// FableWin 0x0186D30D..0x0186D339 materialises the index, shifts the running
// CRC, then XORs the table value. The retail BankFile::Update loop expands these statements.
extern unsigned long g_CRCTable[256];                                   // 0x0129A168
class CCRC {
public:
    static unsigned long FABLE_FASTCALL Calc(unsigned long seed, const void* data, unsigned long byteCount)
    {
        const unsigned char* bytes = static_cast<const unsigned char*>(data);
        if (byteCount == 0)
            return seed;
        for (unsigned long index = 0; index < byteCount; ++index) {
            unsigned long table_index = bytes[index] ^ (seed & 0xFF);
            seed >>= 8;
            seed ^= g_CRCTable[table_index];
        }
        return seed;
    }
};
#elif defined(FABLE_RETAIL_CCRC_CALC_BODY)
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
