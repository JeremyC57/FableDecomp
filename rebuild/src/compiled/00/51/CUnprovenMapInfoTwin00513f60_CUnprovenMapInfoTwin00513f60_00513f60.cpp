// CUnprovenMapInfoTwin_00513f60::~CUnprovenMapInfoTwin_00513f60() -- destroy the members in reverse order (strings / containers, then the counted pointer's inline release)
// Leaf: ??1CUnprovenMapInfoTwin_00513f60@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00513F60, 73 bytes, thiscall, plain `ret`.
// Identity (descriptive): unreferenced byte twin of CMapInfo's destructor 0x0050F910 (the CMapInfo TU of the landed CMapInfo::operator= 0x00513BF0); Ego_r.pdb has no second class of this member layout, so the symbol is not decidable (another inline-destroy COMDAT of CMapInfo, e.g. std::_Destroy<CMapInfo>, is as likely).
// Callees: the members' out-of-line destructors, the count block's deleter and operator delete 0x00BFE9BC.
// Shape (family M103, representative 0x0050F910): an empty user destructor -- the implicit member destruction in reverse
//   declaration order; the counted pointer's `if (Info) { if (--RefCount == 0) { DeleteFunc(Data); delete Info; } } Data =
//   Info = 0;` inline (edi = 0 for the stores).
void __cdecl operator delete(void* block);               // 0x00BFE9BC
class CCPPointerInfo { public:                          // the shared count block
    long RefCount;                                      // +0x0
    void (__fastcall *DeleteFunc)(void*);               // +0x4
    void* Data;                                         // +0x8
};
template <class T> class CCountedPointer {             // 8 bytes: object, shared count block
public:
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
    T* Data;                                            // +0x0
    CCPPointerInfo* Info;                               // +0x4
};
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
class CUnprovenMapInfoTwin_00513f60 {                                          // Ego_r.pdb, 0x48 bytes
public:
    ~CUnprovenMapInfoTwin_00513f60();
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
CUnprovenMapInfoTwin_00513f60::~CUnprovenMapInfoTwin_00513f60()
{
}