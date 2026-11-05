// CMapInfo::~CMapInfo() -- destroy the members in reverse order (strings / containers, then the counted pointer's inline release)
// Leaf: ??1CMapInfo@@QAE@XZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0050F910, 73 bytes, thiscall, plain `ret`.
// Identity (probable): its caller is the vector<CMapInfo> resize/erase code 0x0051A6D0 (a direct call: the out-of-line destructor); it destroys SerialiseScript +0x2C, LevelScriptName +0x1C, LevelName +0x18 and releases PMap +0x10 -- Ego_r.pdb CMapInfo, which has ??1CMapInfo@@QAE@XZ; its byte twin 0x00513F60 is unreferenced, so which of the two COMDATs is the destructor is inferred.
// Callees: the members' out-of-line destructors, the count block's deleter and operator delete 0x00BFE9BC.
// Shape (family M103, representative 0x0050F910): an empty user destructor -- the implicit member destruction in reverse
//   declaration order; the counted pointer's `if (Info) { if (--RefCount == 0) { DeleteFunc(Data); delete Info; } } Data =
//   Info = 0;` inline (edi = 0 for the stores).
void __cdecl operator delete(void* block);               // 0x00BFE9BC
#include "engine/CCountedPointer.h"
class CCharString {                                       // 4 bytes (PStringData)
public:
    ~CCharString();                                       // 0x0099EAE0
    void* PStringData;
};
class CMap;
class C2DBoxI {                                           // 16 bytes
public:
    long X0, Y0, X1, Y1;
};
class CMapInfo {                                          // Ego_r.pdb, 0x48 bytes
public:
    ~CMapInfo();
    C2DBoxI Area;                                         // +0x0
    CCountedPointer<CMap> PMap;                           // +0x10
    CCharString LevelName;                                // +0x18
    CCharString LevelScriptName;                          // +0x1c
    unsigned long MapUID;                                 // +0x20
    bool Used;                                            // +0x24
    bool Placed;                                          // +0x25
    bool Activated;                                       // +0x26
    bool Loading;                                         // +0x27
    bool LoadedOnPlayerProximity;                         // +0x28
    CCharString SerialiseScript;                          // +0x2c
    unsigned __int64 LevStamp;                            // +0x30
    unsigned __int64 TngStamp;                            // +0x38
    bool IsSea;                                           // +0x40
};
CMapInfo::~CMapInfo()
{
}