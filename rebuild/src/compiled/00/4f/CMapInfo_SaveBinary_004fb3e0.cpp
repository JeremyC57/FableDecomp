// CMapInfo::SaveBinary(CDataOutputStream* stream) -- serialise a world-map entry: level and script names, the Used / LoadedOnPlayerProximity / IsSea flags, the area box (TLX, BRX, TLY, BRY), the Placed flag and the UID as a 64-bit value
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004FB3E0, length 143 bytes.
// Callees: CDataOutputStream::WritePresizedString @ 0x00995A80, WriteBool @ 0x00993E30, WriteLong @ 0x00993EB0, and the 8-byte writer
//   @ 0x00993EC0 (the manifest's "WriteCBYTE" label is wrong: it writes 8 bytes from its argument slot, so the UID goes out as `push 0 ; push uid`).
// Layout: see CMapInfo below (PDB, 0x48 bytes).
// Shape: the stream lives in edi; each bool argument is zero-extended with `xor reg,reg ; mov reg8,[esi+N]`.
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
#include "engine/CCountedPointer.h"
class CMap;
class CDataOutputStream { public:
    void WritePresizedString(const CCharString& s);   // 0x00995A80
public:
    void WriteEBOOL(bool);   // 0x00993E30
    void WriteUHUGE(unsigned __int64);   // 0x00993EC0
public:
    void WriteSLONG(long);   // 0x00993EB0
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

void CMapInfo::SaveBinary(CDataOutputStream* s)
{
    s->WritePresizedString(LevelName);
    s->WritePresizedString(LevelScriptName);
    s->WriteEBOOL(Used);
    s->WriteEBOOL(LoadedOnPlayerProximity);
    s->WriteEBOOL(IsSea);
    s->WriteSLONG(Area.TLX);
    s->WriteSLONG(Area.BRX);
    s->WriteSLONG(Area.TLY);
    s->WriteSLONG(Area.BRY);
    s->WriteEBOOL(Placed);
    s->WriteUHUGE(MapUID);
}