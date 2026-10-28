// CMapInfo::CMapInfo(const CMapInfo& other) -- copy constructor of a world-map entry: the area box, the counted map pointer (reference count bumped), the level / script / serialise-script names, the UID, the five state flags, the two 64-bit file stamps and the sea flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005110B0, length 157 bytes.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (x3).
// Layout (PDB CMapInfo, 0x48): Area (C2DBoxI) +0, PMap (CCountedPointer<CMap>) +0x10, LevelName +0x18, LevelScriptName +0x1c, MapUID +0x20,
//   Used/Placed/Activated/Loading +0x24..0x27, LoadedOnPlayerProximity +0x28, SerialiseScript +0x2c, LevStamp +0x30, TngStamp +0x38, IsSea +0x40.
// Shape: a member-wise copy constructor; C2DBoxI needs its own inline memberwise copy constructor for the direct [esi+N] stores
//   (a POD box copies through `mov eax,edi` and [ecx+N]); the counted pointer's copy constructor bumps the shared count when present.
void __cdecl operator delete(void* p);             // 0x00BFE9BC
class CCharString { public:
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
    CCharString(const CCharString& other);         // 0x0099EC30
    const CCharString& operator=(const char* text);      // 0x0099EFE0
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

CMapInfo::CMapInfo(const CMapInfo& o)
    : Area(o.Area), PMap(o.PMap), LevelName(o.LevelName), LevelScriptName(o.LevelScriptName), MapUID(o.MapUID), Used(o.Used),
      Placed(o.Placed), Activated(o.Activated), Loading(o.Loading), LoadedOnPlayerProximity(o.LoadedOnPlayerProximity),
      SerialiseScript(o.SerialiseScript), LevStamp(o.LevStamp), TngStamp(o.TngStamp), IsSea(o.IsSea)
{
}