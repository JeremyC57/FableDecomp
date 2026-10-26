// CMapInfo::`scalar deleting destructor'(unsigned int flags) -- destroy a world map record (serialise script, level script name, level name, release the counted CMap) and free it when bit 0 of the flags is set
// Leaf: ??_GCMapInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00512050, length 98 bytes (+ int3 padding), `ret 4`, returns this; no
// references (an unreferenced COMDAT). The manifest label _Dest_val<allocator<CMapInfo>,CMapInfo> names the class.
// Layout: Ego_r.pdb CMapInfo (0x48) = C2DBoxI Area +0, CCountedPointer<CMap> PMap +0x10, CCharString LevelName +0x18,
// LevelScriptName +0x1c, ulong MapUID +0x20, bool Used/Placed/Activated/Loading/LoadedOnPlayerProximity +0x24..+0x28,
// CCharString SerialiseScript +0x2c, ulong64 LevStamp +0x30, TngStamp +0x38, bool IsSea +0x40 (the offsets agree with the
// bytes: strings at +0x2c/+0x1c/+0x18, the count block at +0x14).
// Callees: CCharString::~CCharString @ 0x0099EAE0 (x3), CCPPointerInfo::DeleteFunc through the block, operator delete
// @ 0x00BFE9BC (block, then the record).
// Shape: VC7.1 emits a NON-virtual `??_G` for a non-polymorphic class with a non-trivial destructor in a TU that deletes it
// (GOTCHAS family waves 5-6); members in reverse order, the two PMap zero stores scheduled after the flag test.
void __cdecl operator delete(void* p);                       // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                          // 0x0099EAE0
};

class CCPPointerInfo { public:                                    // Ego_r.pdb: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);               // called with Data in ecx, nothing on the stack
    void* Data;
};
template<class T> class CCountedPointer { public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    void ReleaseInfo()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};

class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CMap;

class CMapInfo { public:                                            // 0x48 bytes
    C2DBoxI               Area;                              // +0x00
    CCountedPointer<CMap> PMap;                              // +0x10
    CCharString           LevelName;                         // +0x18
    CCharString           LevelScriptName;                   // +0x1c
    unsigned long         MapUID;                            // +0x20
    bool                  Used;                              // +0x24
    bool                  Placed;                            // +0x25
    bool                  Activated;                         // +0x26
    bool                  Loading;                           // +0x27
    bool                  LoadedOnPlayerProximity;           // +0x28
    CCharString           SerialiseScript;                   // +0x2c
    unsigned __int64      LevStamp;                          // +0x30
    unsigned __int64      TngStamp;                          // +0x38
    bool                  IsSea;                             // +0x40
};

void DestroyMapInfo(CMapInfo* info)                          // the delete-expression that makes VC7.1 emit ??_GCMapInfo
{
    delete info;
}