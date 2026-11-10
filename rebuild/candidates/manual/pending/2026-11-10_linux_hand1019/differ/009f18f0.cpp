// CGameEventDataPackage::GetData(long size, void* data) -- read size bytes from the 32-byte ring buffer (wrapping at the end) and consume them
// Grade: DIFFER(166v166) -- (agent1019 regrade, unchanged) allocation permutation only: retail this ebx / start edx / size ebp, ours size ebx / this edx / start esi. c2prio --blocks: start 61 < this 75 (retail needs start coloured before this: first eax, start edx, then this ebx); retail's (size - first) temporary also dies into the memcpy count copy (eax is stored to StartPos) where ours keeps it in ebx across rep movsd. 20 free probes inert or worse (end / rest / long first / unsigned start / Data+start / buffer and src pointer locals / start+size-32 forms; dropping `first` folds the 64-bit range check away, 139 bytes).
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
