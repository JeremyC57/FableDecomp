// CUserProfileManager::VerifySignature(const CWideString& filename) -- check a profile ("FablePro") or save ("FableSav")
// file: header tag, save version 0x2165, the CRC of the signed block, the padded total size and the pad bytes
// Leaf: ?VerifySignature@CUserProfileManager@@SI_NABVCWideString@@@Z
// Grade: DIFFER(1154v1078) -- every instruction up to 0x00409B4E matches; the one residue is the pad-byte mismatch exit
//   inside the inner for loop: VC7.1 expands ~CDiskFileWin32 (vtable store, Close, ~CWideString, ~CBaseClassNonCopyable) at a
//   cleanup site nested two loops deep, so that exit keeps its own 76-byte epilogue instead of cross-jumping into the shared
//   false block 0x00409B34 that calls the COMDAT 0x00401630. A return in the outer loop is not expanded (probe), and the break,
//   flag and goto-free variants (g, h, i, k) move the frame or the layout. 14 attempts; last three moved nothing.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00409730, 1078 bytes, static __fastcall (filename in ecx), /GS (cookie 0x0139C8A8),
//   no EH frame (the TU is built without /EHsc).
// Identity: Ego_r.pdb 0x00407790 + FableWin.pdb ?VerifySignature@CUserProfileManager@@SI_NABVCWideString@@@Z.
// Callees: CAFile::PathExists 0x00999230 / IsPathFile 0x00999A30 (static __fastcall), CDiskFileWin32(const CWideString&,
//   FileAccessType, unsigned long) 0x0099AD80, CDiskFileWin32::Read 0x0099A960 / Close 0x0099A920 (direct: a local of
//   known type), ~CDiskFileWin32 COMDAT 0x00401630 (expanded once, at the first exit), ~CWideString 0x0099B510,
//   ~CBaseClassNonCopyable 0x0099A300, CFileDataInputStream(CAFile&, unsigned long) 0x00994700 / ~ 0x00994780,
//   CDataInputStream::SetPosition 0x00993BC0 (direct) / ReadWithSrcChunkOverflow 0x00993CA0, vector<char>(n) 0x00411910,
//   vector<char>::resize(n) 0x00412880, CCRC::Calc 0x004014A0, free 0x00BFEA14.
// Literals: "FablePro" 0x0122E258, "FableSav" 0x0122E24C (memcmp intrinsic: `repe cmpsd`, ecx 2).
// Levers: FABLE_RETAIL_CDATAINPUTSTREAM_READTYPE (Read / ReadType / ReadULONG __forceinline: plain inline expands only in a
//   loop or a tiny caller, retail expands all five); a same-TU CCRC::Calc body (VC7.1 then keeps _M_start in esi across the
//   call, as retail); the CRC and size checks as nested ifs with one trailing `return false` (retail's one false block).
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
    if (CCRC::Calc(0, buffer.GetAsCArray(), dataSize) == crc) {
        unsigned long total = isProfile ? 0x4000 : 0x4B000;
        if (total < stream.GetPosition())
            total = stream.GetPosition();
        if ((unsigned long)file.GetLength() == total) {
            char padByte = isProfile ? ' ' : 0;
            long remaining = total - stream.GetPosition();
            buffer.resize(0x4000);
            while (remaining > 0) {
                long chunk = remaining > 0x4000 ? 0x4000 : remaining;
                char* data = buffer.GetAsCArray();
                stream.Read(data, chunk);
                for (long i = 0; i < chunk; i++)
                    if (data[i] != padByte)
                        return false;
                remaining -= chunk;
            }
            return true;
        }
    }
    return false;
}
