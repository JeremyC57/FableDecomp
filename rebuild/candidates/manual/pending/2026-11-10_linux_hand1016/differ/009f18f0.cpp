// CGameEventDataPackage::GetData(long size, void* data) -- read size bytes from the 32-byte ring buffer (wrapping at the end) and consume them
// Grade: DIFFER(166v166) -- (w22 agent1016) allocation permutation only: retail this=ebx, start=edx, size=ebp; ours size=ebx, this=edx, start=esi (c2prio: this prio 75 > start 61). No-start-local variant 174v166; c/d/e variants insensitive.
// Leaf: ?GetData@CGameEventDataPackage@@QAEXJPAX@Z
// Retail: Fable.exe @ 0x009F18F0, 164 bytes, thiscall, `ret 8`.
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE
#include "engine/CGameEventDataPackage.h"
#include <string.h>

void CGameEventDataPackage::GetData(long size, void* data)
{
    long start = StartPos;
    if ((unsigned long)(start + size) > sizeof(Data)) {
        unsigned long first = sizeof(Data) - start;
        __int64 position = (__int64)(first + start);
        if (position < 0 || position > 0x7fffffff)
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
