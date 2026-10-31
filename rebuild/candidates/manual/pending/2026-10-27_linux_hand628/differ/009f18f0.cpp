// CGameEventDataPackage::GetData(long size, void* data) -- read size bytes from the 32-byte ring buffer (wrapping at the end) and consume them
// Grade: DIFFER(166v166) -- same instructions and layout, a register permutation only: retail keeps this in ebx, the
//   size in ebp and StartPos in edx (dest staged through edx); ours puts the size in ebx, this in edx and StartPos in esi.
// Retail: Fable.exe @ 0x009F18F0, 166 bytes, `ret 8`. PDB: ?GetData@CGameEventDataPackage@@QAEXJPAX@Z (catalog label
//   "CircularBuffer_Read" is descriptive; the landed GetUHUGE 0x00628E80 calls it with (8, &value)).
// Layout: Ego_r.pdb CGameEventDataPackage (0x2C): Data[32] +0, Length +0x20, StartPos +0x24, EndPos +0x28.
// Shape: the wrap branch is guarded by the Lionhead memory-stream range test `IsValidPosition((__int64)(first + start))`
//   (the jl/jg/jb + jg/jl/ja 0..0x7fffffff sequence of the landed CWoundMorphs::TransferBinaryIn 0x00757990); a failed
//   test returns before Length is touched. The else branch reuses the compare's start + size (a StartPos local).
// Tried: plain body (139v166), + IsValidPosition (174v166), + `end` local (166v166), + `start` local (this file, 166v166).
// Next: vary how this/Data are reached (a `unsigned char* buffer = Data` local, or the size compare written
//   `size > sizeof(Data) - start`) to move this into ebx.
// Leaf: ?GetData@CGameEventDataPackage@@QAEXJPAX@Z
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int size);
#pragma intrinsic(memcpy)

class CGameEventDataPackage
{
public:
    unsigned char Data[32];                          // +0x00
    long          Length;                            // +0x20
    long          StartPos;                          // +0x24
    long          EndPos;                            // +0x28
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    void GetData(long size, void* data);
};

void CGameEventDataPackage::GetData(long size, void* data)
{
    long start = StartPos;
    if (start + size > sizeof(Data)) {
        unsigned long first = sizeof(Data) - start;
        if (!IsValidPosition((__int64)(first + start)))
            return;
        memcpy(data, &Data[start], first);
        memcpy((unsigned char*)data + first, Data, size - first);
        StartPos = size - first;
    } else {
        memcpy(data, &Data[start], size);
        StartPos = (start + size) % sizeof(Data);
    }
    Length -= size;
}
