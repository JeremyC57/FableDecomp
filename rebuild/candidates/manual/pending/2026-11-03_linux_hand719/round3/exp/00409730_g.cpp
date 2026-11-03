// CUserProfileManager::VerifySignature(const CWideString& filename) -- check a profile ("FablePro") or save ("FableSav")
// file: header tag, save version 0x2165, the CRC of the signed block, the padded total size and the pad bytes
// Leaf: ?VerifySignature@CUserProfileManager@@SI_NABVCWideString@@@Z
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_READTYPE
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CARRAY
#include "fable_string.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CFileDataInputStream.h"
#include "engine/CArray.h"
#include "fable_user_profile.h"
#include <string.h>
#pragma intrinsic(memcmp)

extern unsigned long g_CRCTable[256];                   // 0x0129A168 (landed spelling; no PDB name)
class CCRC { public:
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
};
// Same-TU body of the header COMDAT 0x004014A0 (Ego_r.pdb has it at 0x00401920, beside the other header COMDATs): VC7.1 then
// knows it writes no memory and keeps the buffer's _M_start in esi across the call (landed body CCRC_Calc_004014a0.cpp).
unsigned long __fastcall CCRC::Calc(unsigned long seed, const void* data, unsigned long length)
{
    if (length != 0) {
        const unsigned char* bytes = static_cast<const unsigned char*>(data);
        for (unsigned long index = 0; index < length; ++index)
            seed = g_CRCTable[bytes[index] ^ (seed & 0xFF)] ^ (seed >> 8);
    }
    return seed;
}

bool CUserProfileManager::VerifySignature(const CWideString& filename)
{
    if (!CAFile::PathExists(filename) || !CAFile::IsPathFile(filename))
        return false;
    CDiskFileWin32 file(filename, FILE_ACCESS_READ_WRITE, 2);
    if (file.GetLength() < 16)
        return false;
    char header[8];
    file.Read(header, 8, 0);
    bool isProfile = memcmp(header, "FablePro", 8) == 0;
    bool isSave = memcmp(header, "FableSav", 8) == 0;
    if (!isProfile && !isSave)
        return false;
    CFileDataInputStream stream(file, 0x4000);
    unsigned long version = stream.ReadULONG();
    if (isSave && version != 0x2165)
        return false;
    long dataSize = stream.ReadULONG();
    if ((unsigned long)(dataSize + 4) > (unsigned long)file.GetLength())
        return false;
    CArray<char> buffer(dataSize);
    stream.SetPosition(0);
    stream.Read(buffer.GetAsCArray(), dataSize);
    unsigned long crc = stream.ReadULONG();
    if (CCRC::Calc(0, buffer.GetAsCArray(), dataSize) != crc)
        return false;
    unsigned long total = isProfile ? 0x4000 : 0x4B000;
    if (total < stream.GetPosition())
        total = stream.GetPosition();
    if ((unsigned long)file.GetLength() != total)
        return false;
    char padByte = isProfile ? ' ' : 0;
    long remaining = total - stream.GetPosition();
    buffer.resize(0x4000);
    while (remaining > 0) {
        long chunk = remaining > 0x4000 ? 0x4000 : remaining;
        char* data = buffer.GetAsCArray();
        stream.Read(data, chunk);
        long i;
        for (i = 0; i < chunk; i++)
            if (data[i] != padByte)
                break;
        if (i < chunk)
            return false;
        remaining -= chunk;
    }
    return true;
}
