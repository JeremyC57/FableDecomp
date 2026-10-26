// CMapInfo::Clear() -- reset a world-map entry: drop the counted map pointer (shared block freed through its delete function on the last reference), clear the state flags, empty the level and script names, zero the UID, clear the sea flag and mark the map as loaded on player proximity
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004FDDE0, length 97 bytes.
// Callees: the shared block's DeleteFunc, operator delete @ 0x00BFE9BC, CCharString::operator=(const char*) @ 0x0099EFE0 (x2, the empty literal 0x0122D70E).
// Layout: see CMapInfo below (PDB, 0x48 bytes).
// Shape: ebx is the zero register; the counted-pointer Reset re-reads Info after the decrement and clears Info before Data
//   (landed CCountedPointer model of 0x00AF69D0); the flag stores are scheduled around the first literal push.
void __cdecl operator delete(void* p);             // 0x00BFE9BC
class CCharString { public:
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
    CCharString(const CCharString& other);         // 0x0099EC30
    CCharString& operator=(const char* text);      // 0x0099EFE0
};
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(const C2DBoxI& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) { }
};
class CCPPointerInfo { public:                            // the shared block of a CCountedPointer: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);     // called with Data in ecx
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
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
    void Reset() { ReleaseInfo(); Info = 0; Data = 0; }
};
class CMap;
class CDataOutputStream { public:
    void WritePresizedString(const CCharString& s);   // 0x00995A80
    void WriteBool(bool value);                       // 0x00993E30
    void WriteLong(long value);                       // 0x00993EB0
    void WriteUInt64(unsigned __int64 value);         // 0x00993EC0
};
class CMapInfo { public:                                  // PDB CMapInfo, 0x48, no vtable
    C2DBoxI                Area;                   // +0x00
    CCountedPointer<CMap>  PMap;                   // +0x10 (Data +0x10, Info +0x14)
    CCharString            LevelName;              // +0x18
    CCharString            LevelScriptName;        // +0x1c
    unsigned long          MapUID;                 // +0x20
    bool                   Used;                   // +0x24
    bool                   Placed;
    bool                   Activated;
    bool                   Loading;
    bool                   LoadedOnPlayerProximity;   // +0x28
    CCharString            SerialiseScript;        // +0x2c
    unsigned __int64       LevStamp;               // +0x30
    unsigned __int64       TngStamp;               // +0x38
    bool                   IsSea;                  // +0x40
    CMapInfo(const CMapInfo& o);
    void Clear();
    void SaveBinary(CDataOutputStream* s);
};

void CMapInfo::Clear()
{
    PMap.Reset();
    Used = false;
    Placed = false;
    Activated = false;
    Loading = false;
    LevelName = "";
    LevelScriptName = "";
    MapUID = 0;
    IsSea = false;
    LoadedOnPlayerProximity = true;
}