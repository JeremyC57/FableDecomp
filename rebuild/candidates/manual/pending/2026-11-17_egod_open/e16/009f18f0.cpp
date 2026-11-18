// CGameEventDataPackage::GetData(long no_bytes, void* area) -- read no_bytes from the 32-byte ring buffer into area (wrapping at the end) and consume them
// Grade: DIFFER(166v166) -- allocation permutation only (as the 2026-11-10 draft): retail this ebx / read_pos edx / no_bytes ebp, ours no_bytes ebx / this edx / read_pos esi; inert under Ego_d declaration orders, byte_pointer as source or destination, dead-arm asserts, operand orders
// Leaf: ?GetData@CGameEventDataPackage@@QAEXJPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F18F0, 166 bytes, thiscall, `ret 8`.
// Identity: Ego_d.pdb lib_game_event.obj CGameEventDataPackage::GetData (params no_bytes / area; locals bytes_to_read, byte_pointer,
//   read_pos and the block-scope __int64 overflow_check) between CGameEvent::InitFromCompressedBuffer and
//   CGameEventPackageSet::CompressIntoBuffer; Ego_r.pdb ?GetData@CGameEventDataPackage@@QAEXJPAX@Z donor 0x00B454F0 between the same two
//   (0x00B45490 / 0x00B45570 = retail 0x009F1870 / 0x009F19A0); FableWin.pdb same symbol.
// Callees: memcpy (/Oi intrinsic: rep movsd / rep movsb).
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE
#include "engine/CGameEventDataPackage.h"
#include <string.h>

void CGameEventDataPackage::GetData(long no_bytes, void* area)
{
    unsigned long read_pos = StartPos;
    unsigned char* byte_pointer = (unsigned char*)area;
    long bytes_to_read;
    if (read_pos + no_bytes > sizeof(Data))
    {
        bytes_to_read = sizeof(Data) - read_pos;
        __int64 overflow_check = read_pos + bytes_to_read;
        if (overflow_check < 0 || overflow_check > 0x7fffffff)
            return;
        memcpy(byte_pointer, &Data[read_pos], bytes_to_read);
        memcpy(byte_pointer + bytes_to_read, Data, no_bytes - bytes_to_read);
        StartPos = no_bytes - bytes_to_read;
    }
    else
    {
        memcpy(byte_pointer, &Data[read_pos], no_bytes);
        StartPos = (read_pos + no_bytes) % sizeof(Data);
    }
    Length -= no_bytes;
}
