// NThingManager::CEntityRuntimePersistence::LoadFromGameStateBinary(CPersistContext& context) -- reload the runtime entity persistence from a binary game save: clear it, then for each saved level block open a memory sub-stream over the block, read its map UID (an old map number below the world map's map count is converted through GetMapNumberFromMapUID), the compressed and uncompressed sizes, the compressed save data and the script, and store them; then read the dead-entity records (UID + section CRC) and add each non-zero one as game-persistent
// Grade: DIFFER(1006v1006) -- register allocation only: retail gives the map UID ebx and spills it to its home after the conversion (compressed size in ebp, the sub-stream pointer in ebx), ours keeps the UID in ebp and the compressed size in ebx (so the sub-stream pointer spills and the compressed/uncompressed sizes share the UID slot), which shifts the frame slots of this / the world map / the loop counter / the dead-entity record; same 323 instructions, same calls and control flow
// Leaf: ?LoadFromGameStateBinary@CEntityRuntimePersistence@NThingManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00524EC0, 1006 bytes (+ 2 int3 padding), thiscall `ret 4`, frame 0x84.
// Identity: Ego_r.pdb names ?LoadFromGameStateBinary@CEntityRuntimePersistence@NThingManager@@QAEXAAVCPersistContext@@@Z
//   (name set only). The catalog label "CDrawQuestInfo_LoadDeadCreatures" is propagated; the callees are this class's
//   Clear 0x005236F0 / SetGamePersistentSaveDataCompressed 0x00523910 / SetGamePersistentScript 0x005237F0 (all landed).
// Callees: Clear 0x005236F0; the main game component 0x013B86A0 (+0x24 PWorld) virtual slot 13 GetWorldMap;
//   CPersistContext::CheckCRC 0x00404500 with "" 0x0122D70E; CPersistTraits<long>::TransferBinaryIn 0x0040FE60,
//   <unsigned long> 0x0040F8A0, <unsigned __int64> 0x0044B890; CDataOutputStream::WriteSLONG 0x00993EB0 and
//   WriteUHUGE 0x00993EC0; CDataInputStream() 0x00993B20 (vtable 0x0123675C), GetPosition slot 2,
//   SetPositionAssumeWithinSourceChunk 0x00993C10; CMemoryDataInputStream::OpenMemorySource 0x00994570,
//   ~CMemoryDataInputStream 0x00993B10; CPersistContext(CMemoryDataInputStream*, bool) 0x009BAE30;
//   CWorldMap::GetMapNumberFromMapUID 0x004FB920; CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0;
//   CPersistContext::Transfer(CCharString&) 0x00431143; the dead-entity map insert 0x0052A130 (hidden pair result).
// Layout: CPersistContext (0x2c, PDB): Mode +0x18, PLoadStream +0x24, PSaveStream +0x28; CMemoryDataInputStream
//   StreamSource +0x18; retail CWorldMap Maps (0x48-byte CMapInfo) +0x20; the persistence's dead-entity map +0x18 and
//   the CPersistedDeadEntity record {UID +0, GamePersistent +8, SectionNameCRC +0xc} (as the landed 0x00520330).
// Shape: the binary-only transfer inline (CheckCRC(""), mode 3 save / mode 2 traits load); the TransferBinaryIn
//   bodies are same-TU auto_inline(off) stand-ins that only write *value (retail's are header templates that never
//   touch `this`), which lets VC7.1 keep the counts in registers and reload only after the load call; the dead-entity
//   loop reads into a CPersistedDeadEntity local (UID / CRC at +0x2c / +0x38, flag word +0x34 untouched; the CRC save
//   re-reads memory); AddPersistedDeadEntity is the inline member body.
// Tried (free cmp.py, 30 variants): external TransferBinaryIn (all counts memory-resident), same-TU stand-ins (enregistered,
//   561 -> 184 diff lines), named/temporary/DefaultVal traits, the dead-entity record local (frame 0x88 -> 0x84 = retail),
//   a separate long map index, an escaping cast reference, declaration order/scope of the UID and sizes, the peek inside
//   the call argument, an inline map-number helper on CWorldMap, stand-ins for GetMapNumberFromMapUID / the setters
//   (no change) and for the map insert (worse).
//   The header-shaped bodies (`T v; stream->Read(&v, sizeof v); *value = v;` over the inline CDataInputStream::Read
//   compile byte-identical to retail 0x0040FE60) as the same-TU definitions: no change either.
// Next: lane gap (register colouring order between the map UID and the compressed size).
class CCharString {
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};

class CDataOutputStream {
public:
    void WriteSLONG(long value);                        // 0x00993EB0
    void WriteUHUGE(unsigned __int64 value);            // 0x00993EC0
};

class CDataInputStream {
public:
    unsigned long        StreamPos;                     // +0x04
    long                 StreamSize;                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long        SourceChunkStreamPos;          // +0x10
    long                 SourceChunkBytesRemaining;     // +0x14
    CDataInputStream();                                 // 0x00993B20
    virtual void v00();
    virtual void v01();
    virtual unsigned long GetPosition() const;          // slot 2
    void SetPositionAssumeWithinSourceChunk(unsigned long position);   // 0x00993C10
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
};

class CMemoryDataInputStream : public CDataInputStream {
public:     // vtable 0x0123675C
    const unsigned char* StreamSource;                  // +0x18
    CMemoryDataInputStream() : StreamSource(0) { }
    ~CMemoryDataInputStream();                          // 0x00993B10
    void OpenMemorySource(const void* data, unsigned long size);   // 0x00994570
    __forceinline const unsigned char* GetDataAndSkip(long size)
    {
        const unsigned char* data;
        if (size > 0) {
            unsigned long position = GetPosition();
            __int64 np = (__int64)(position + size);
            if (IsValidPosition(np)) {
                data = StreamSource + position;
                SetPositionAssumeWithinSourceChunk(position + size);
            }
            else
                data = 0;
        }
        else
            data = 0;
        return data;
    }
};

template <class T> struct CPersistTraits {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, T* value);
};

extern "C" int StandInSink(int, int);
#pragma auto_inline(off)
template <> void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream* stream, long* value)   // stand-in for 0x0040FE60
{
    *value = StandInSink((int)stream, 0);
}
template <> void CPersistTraits<unsigned long>::TransferBinaryIn(CMemoryDataInputStream* stream, unsigned long* value)   // stand-in for 0x0040F8A0
{
    *value = (unsigned long)StandInSink((int)stream, 0);
}
template <> void CPersistTraits<unsigned __int64>::TransferBinaryIn(CMemoryDataInputStream* stream, unsigned __int64* value)   // stand-in for 0x0044B890
{
    *value = (unsigned long)StandInSink((int)stream, 0);
}
#pragma auto_inline(on)
class CDefinitionManager;
class CPersistContext {
public:
    CDefinitionManager*     PDefinitionManager;         // +0x00
    void*                   PSymbolMap;                 // +0x04
    void*                   PParser;                    // +0x08
    void*                   ErrorCallback;              // +0x0c
    CCharString*            PStringToClean;             // +0x10
    bool                    UseDefaults;                // +0x14
    bool                    UseCRCCheck;                // +0x15
    bool                    SingleTransfer;             // +0x16
    bool                    Finished;                   // +0x17
    long                    Mode;                       // +0x18
    bool                    Binary;                     // +0x1c
    CCharString*            PStringToSaveTo;            // +0x20
    CMemoryDataInputStream* PLoadStream;                // +0x24
    CDataOutputStream*      PSaveStream;                // +0x28
    CPersistContext(CMemoryDataInputStream* loadStream, bool useCRCCheck);   // 0x009BAE30
    void CheckCRC(const char* name);                    // 0x00404500
    void Transfer(CCharString& value);                  // 0x00431143
    __forceinline void TransferBinary(long& value)
    {
        CheckCRC("");
        CPersistTraits<long> traits;
        switch (Mode) {
        case 3:
            PSaveStream->WriteSLONG(value);
            break;
        case 2:
            traits.TransferBinaryIn(PLoadStream, &value);
            break;
        }
    }
    __forceinline void TransferBinary(unsigned long& value)
    {
        CheckCRC("");
        CPersistTraits<unsigned long> traits;
        switch (Mode) {
        case 3:
            PSaveStream->WriteSLONG(value);
            break;
        case 2:
            traits.TransferBinaryIn(PLoadStream, &value);
            break;
        }
    }
    __forceinline void TransferBinary(unsigned __int64& value)
    {
        CheckCRC("");
        CPersistTraits<unsigned __int64> traits;
        switch (Mode) {
        case 3:
            PSaveStream->WriteUHUGE(value);
            break;
        case 2:
            traits.TransferBinaryIn(PLoadStream, &value);
            break;
        }
    }
};

struct CMapInfo { unsigned char _pad[0x48]; };
template <class T> struct CArray {
    T* Start;
    T* Finish;
    T* EndOfStorage;
    unsigned long size() const { return Finish - Start; }
};
class CWorldMap {
public:
    virtual void v00();
    unsigned char    _pad04[0x1c];
    CArray<CMapInfo> Maps;                              // +0x20
    long GetMapNumberFromMapUID(unsigned long uid) const;   // 0x004FB920
};
class CWorld {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012();
    virtual CWorldMap* GetWorldMap();                   // slot 13 (+0x34)
};
class CMainGameComponent {
public:
    unsigned char _pad[0x24];
    CWorld*       PWorld;                               // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;       // 0x013B86A0

struct CPersistedDeadEntity {
    unsigned __int64 UID;
    bool             GamePersistent;
    unsigned long    SectionNameCRC;
};
struct SDeadEntityPair {
    unsigned __int64     first;
    CPersistedDeadEntity second;
    SDeadEntityPair(unsigned __int64 key, const CPersistedDeadEntity& value) : first(key), second(value) { }
};
struct SDeadEntityIterator {
    void* m_pNode;
    SDeadEntityIterator() { }
};
struct SDeadEntityInsertResult {
    SDeadEntityIterator first;
    bool                second;
};
struct CDeadEntityMap {
    void*         m_pHead;
    unsigned long m_size;
    SDeadEntityInsertResult insert(const SDeadEntityPair& value);   // 0x0052A130
};

namespace NThingManager {
class CEntityRuntimePersistence {
public:
    unsigned char  _pad0[0x18];
    CDeadEntityMap PersistedDeadEntities;               // +0x18
    long           SaveFormatVersion;                   // +0x20
    void Clear();                                       // 0x005236F0
    void SetGamePersistentScript(long index, const CCharString& script);   // 0x005237F0
    void SetGamePersistentSaveDataCompressed(long index, const void* data, unsigned long size, unsigned long uncompressedSize);   // 0x00523910
    void AddPersistedDeadEntity(unsigned __int64 uid, bool gamePersistent, unsigned long sectionNameCRC)
    {
        if (uid != 0) {
            CPersistedDeadEntity entity;
            entity.UID = uid;
            entity.GamePersistent = gamePersistent;
            entity.SectionNameCRC = sectionNameCRC;
            PersistedDeadEntities.insert(SDeadEntityPair(uid, entity));
        }
    }
    void LoadFromGameStateBinary(CPersistContext& context);
};

void CEntityRuntimePersistence::LoadFromGameStateBinary(CPersistContext& context)
{
    Clear();
    CWorldMap* worldMap = g_pMainGameComponent->PWorld->GetWorldMap();
    long sectionCount = 0;
    context.TransferBinary(sectionCount);
    for (long i = 0; i < sectionCount; ++i) {
        long size = 0;
        context.TransferBinary(size);
        if (size > 0) {
            CMemoryDataInputStream stream;
            stream.OpenMemorySource(context.PLoadStream->GetDataAndSkip(size), size);
            CPersistContext sectionContext(&stream, true);
            unsigned long mapUID = 0;
            sectionContext.TransferBinary(mapUID);
            if (mapUID >= worldMap->Maps.size()) {
                long mapNumber = worldMap->GetMapNumberFromMapUID(mapUID);
                if (mapNumber > 0)
                    mapUID = mapNumber;
            }
            unsigned long compressedSize = 0;
            sectionContext.TransferBinary(compressedSize);
            unsigned long uncompressedSize = 0;
            sectionContext.TransferBinary(uncompressedSize);
            const unsigned char* data = sectionContext.PLoadStream->GetDataAndSkip(compressedSize);
            SetGamePersistentSaveDataCompressed(mapUID, data, compressedSize, uncompressedSize);
            CCharString script;
            sectionContext.Transfer(script);
            SetGamePersistentScript(mapUID, script);
        }
    }
    unsigned long deadCount = 0;
    context.TransferBinary(deadCount);
    for (unsigned long j = 0; j < deadCount; ++j) {
        CPersistedDeadEntity deadEntity;
        deadEntity.UID = 0;
        deadEntity.SectionNameCRC = 0;
        context.TransferBinary(deadEntity.UID);
        context.TransferBinary(deadEntity.SectionNameCRC);
        AddPersistedDeadEntity(deadEntity.UID, true, deadEntity.SectionNameCRC);
    }
}
}
