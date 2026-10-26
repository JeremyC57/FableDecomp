// CMapInfo::LoadBinary(CDataInputStream& stream) -- read the level and level-script names, the Used / LoadedOnPlayerProximity / IsSea flags, the map area (left, right, top, bottom), the Placed flag and the 64-bit map UID (low dword kept)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FB4F0, length 912 bytes (+ int3 padding), `ret 4`.
// Callees: CDataInputStream::ReadPresizedString 0x00996390 (CCharString by value), CCharString::operator= 0x0099EFB0,
//   CCharString::~CCharString 0x0099EAE0, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inlined
//   CMemoryDataInputStream::Read slow path).
// Layout (PDB CMapInfo, 72 bytes): Area C2DBoxI +0 (TopLeft X/Y, BottomRight X/Y), PMap +0x10, LevelName +0x18,
//   LevelScriptName +0x1c, MapUID +0x20, Used +0x24, Placed +0x25, LoadedOnPlayerProximity +0x28, IsSea +0x40.
// Shape: each scalar goes through an inlined read helper whose local is homed in the dead stream-argument slot; the
//   64-bit UID is the 8-byte frame local, and VC7.1 duplicates the epilogue into each exit of that last read.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
};

class CDataInputStream {
public:
    void*                m_pVtable;         // +0
    unsigned long        m_position;        // +4
    long                 StreamSize;        // +8
    const unsigned char* m_pData;           // +0xC
    long                 SourceChunkStreamPos;
    long                 m_remaining;       // +0x14
    CCharString ReadPresizedString();                                  // 0x00996390
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline bool ReadBool() { unsigned char v; Read(&v, sizeof(v)); return v != 0; }
    __forceinline long ReadLong() { long v; Read(&v, sizeof(v)); return v; }
    __forceinline unsigned __int64 ReadULong64() { unsigned __int64 v; Read(&v, sizeof(v)); return v; }
};

class C2DCoordI { public: long X, Y; };
class C2DBoxI { public: C2DCoordI TopLeft, BottomRight; };
struct CCountedPointerCMap { void* Data; void* Info; };

class CMapInfo {
public:
    C2DBoxI             Area;                    // +0x00
    CCountedPointerCMap PMap;                    // +0x10
    CCharString         LevelName;               // +0x18
    CCharString         LevelScriptName;         // +0x1c
    unsigned long       MapUID;                  // +0x20
    bool                Used;                    // +0x24
    bool                Placed;                  // +0x25
    bool                Activated;               // +0x26
    bool                Loading;                 // +0x27
    bool                LoadedOnPlayerProximity; // +0x28
    CCharString         SerialiseScript;         // +0x2c
    unsigned __int64    LevStamp;                // +0x30
    unsigned __int64    TngStamp;                // +0x38
    bool                IsSea;                   // +0x40
    void LoadBinary(CDataInputStream& stream);
};

void CMapInfo::LoadBinary(CDataInputStream& stream)
{
    LevelName = stream.ReadPresizedString();
    LevelScriptName = stream.ReadPresizedString();
    Used = stream.ReadBool();
    LoadedOnPlayerProximity = stream.ReadBool();
    IsSea = stream.ReadBool();
    Area.TopLeft.X = stream.ReadLong();
    Area.BottomRight.X = stream.ReadLong();
    Area.TopLeft.Y = stream.ReadLong();
    Area.BottomRight.Y = stream.ReadLong();
    Placed = stream.ReadBool();
    MapUID = (unsigned long)stream.ReadULong64();
}