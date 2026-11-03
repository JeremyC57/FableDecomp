#ifndef FABLE_ENGINE_CCRC_H
#define FABLE_ENGINE_CCRC_H
// Retail model (chunk 733): the engine CRC-32 helper class. Ego_r.pdb / FableWin.pdb: ?Calc@CCRC@@SIKKPBXK@Z (seeded),
// ?Calc@CCRC@@SIKPBXK@Z, ?CalcFromString@CCRC@@SIKABVCCharString@@@Z, ?CalcFromPathname@.. (static __fastcall); the table
// ?Table@CCRC@@2QBKB 0x0129A168 (bound spelling). Reflected CRC-32 step with no pre/post inversion.
// The seeded overload is the header inline: Ego_r.pdb / FableWin.pdb list its COMDAT among the first TU's functions (Ego_r
// 0x00401920, FableWin 0x0186D2D0; retail 0x004014A0, landed), and CUserProfileManager::SaveGameState 0x00409FD0 expands it
// twice (seed 0 over the header stream: its `length == 0` path returns the seed, `xor eax,eax`). The unseeded overload and
// CalcFromString / CalcFromPathname sit with the engine library (FableWin 0x02E60C80 / 0x02F6DFD0; retail unseeded copy
// 0x00A00950): declarations only.
#include "rebuild_abi.h"
class CCharString;
class CCRC {
public:
    static const unsigned long Table[256];                           // 0x0129A168
    static unsigned long FABLE_FASTCALL Calc(unsigned long crc, const void* data, unsigned long length)
    {
        if (length == 0)
            return crc;
        const unsigned char* bytes = (const unsigned char*)data;
        for (unsigned long i = 0; i < length; ++i)
            crc = Table[bytes[i] ^ (crc & 0xFF)] ^ (crc >> 8);
        return crc;
    }
    static unsigned long FABLE_FASTCALL Calc(const void* data, unsigned long length);                   // 0x00A00950
    static unsigned long FABLE_FASTCALL CalcFromString(const CCharString& text);
    static unsigned long FABLE_FASTCALL CalcFromPathname(const CCharString& path);
};
#endif
