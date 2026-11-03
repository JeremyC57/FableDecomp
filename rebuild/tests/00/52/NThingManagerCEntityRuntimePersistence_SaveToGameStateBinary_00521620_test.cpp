// Test for 0x00521620 NThingManager::CEntityRuntimePersistence::SaveToGameStateBinary(CPersistContext&): the script count,
// then per script slot a sub-stream (empty for a missing/invalid slot; else map UID of that slot, save-data size, the
// uncompressed size, the raw save data and the script text) written as size + bytes; then the game-persistent dead
// entities only (UID, section-name CRC), preceded by their count.
// Self-contained model (C++98; VC7.1 /Od and g++): 4-byte little-endian words, 8-byte UIDs; the function body below is the
// reconstruction's text.
#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>
#ifdef _MSC_VER
typedef unsigned __int64 FableU64;
#else
typedef unsigned long long FableU64;
#endif

class CCharString {
public:
    char Text[32];
    CCharString() { Text[0] = 0; }
    CCharString(const char* s) { strncpy(Text, s, 31); Text[31] = 0; }
};

class CDataOutputStream {
public:
    std::vector<unsigned char> Bytes;
    virtual ~CDataOutputStream() { }
    virtual void Write(const void* p, long n) { const unsigned char* b = (const unsigned char*)p; Bytes.insert(Bytes.end(), b, b + n); }
    void WriteSLONG(long v) { unsigned long u = (unsigned long)v; unsigned char b[4] = { (unsigned char)u, (unsigned char)(u >> 8), (unsigned char)(u >> 16), (unsigned char)(u >> 24) }; Write(b, 4); }
    void WriteUHUGE(FableU64 v) { WriteSLONG((long)(unsigned long)(v & 0xFFFFFFFFu)); WriteSLONG((long)(unsigned long)(v >> 32)); }
};
class CMemoryDataOutputStream : public CDataOutputStream {
public:
    unsigned long GetLength() const { return (unsigned long)Bytes.size(); }
    const void* PeekData() const { return Bytes.empty() ? 0 : &Bytes[0]; }
};

class CPersistContext {
public:
    CDataOutputStream* PSaveStream;
    CPersistContext(CDataOutputStream& os, bool) : PSaveStream(&os) { }
    CDataOutputStream* GetPSaveStream() { return PSaveStream; }
    void Transfer(long& v) { PSaveStream->WriteSLONG(v); }
    void Transfer(unsigned long& v) { PSaveStream->WriteSLONG((long)v); }
    void Transfer(FableU64& v) { PSaveStream->WriteUHUGE(v); }
    void Transfer(CCharString& v) { PSaveStream->Write(v.Text, (long)strlen(v.Text) + 1); }
};

template<class T> class CArray : public std::vector<T> {
public:
    T* GetAsCArray() { return this->empty() ? 0 : &(*this)[0]; }
};

class CWorldMap { public: unsigned long GetMapUID(long map) const { return 100 + (unsigned long)map; } };
class CWorld { public: CWorldMap Map; const CWorldMap& PeekWorldMap() const { return Map; } };
class CMainGameComponent {
public:
    CWorld World;
    static CMainGameComponent* PMainGameComponent;
    static CMainGameComponent* Get() { return PMainGameComponent; }
    const CWorld& PeekWorld() const { return World; }
};
CMainGameComponent* CMainGameComponent::PMainGameComponent = 0;

namespace NThingManager {
class CEntityRuntimePersistence {
public:
    class CEntityScriptInfo {
    public:
        CCharString Script; CArray<unsigned char> SaveData; unsigned long UncompressedSize; bool Valid;
    };
    class CPersistedDeadEntity {
    public:
        FableU64 UID; bool GamePersistent; unsigned long SectionNameCRC;
    };
    CArray<CEntityScriptInfo> GamePersistentEntityScripts;
    std::map<FableU64, CPersistedDeadEntity> PersistedDeadEntities;
    const CCharString* GetGamePersistentScript(long level) const {
        if ((unsigned long)level < GamePersistentEntityScripts.size() && GamePersistentEntityScripts[level].Valid)
            return &GamePersistentEntityScripts[level].Script;
        return 0;
    }
    void SaveToGameStateBinary(CPersistContext& context);
};
}

void NThingManager::CEntityRuntimePersistence::SaveToGameStateBinary(CPersistContext& context)
{
    const CWorldMap& world_map=CMainGameComponent::Get()->PeekWorld().PeekWorldMap();
    long no_scripts=GamePersistentEntityScripts.size();
    context.Transfer(no_scripts);
    for(long c1=0;c1<no_scripts;++c1) {
        const CCharString* script=GetGamePersistentScript(c1);
        CMemoryDataOutputStream script_os;
        CPersistContext script_context(script_os,true);
        if(script) {
            unsigned long map_uid=world_map.GetMapUID(c1);
            script_context.Transfer(map_uid);
            CEntityScriptInfo& info=GamePersistentEntityScripts[c1];
            unsigned long data_size=info.SaveData.size();
            script_context.Transfer(data_size);
            script_context.Transfer(info.UncompressedSize);
            script_context.GetPSaveStream()->Write(info.SaveData.GetAsCArray(),data_size);
            script_context.Transfer(info.Script);
        }
        long script_size=script_os.GetLength();
        context.Transfer(script_size);
        context.GetPSaveStream()->Write(script_os.PeekData(),script_size);
    }
    CArray<CPersistedDeadEntity> dead_entities;
    for(std::map<FableU64,CPersistedDeadEntity>::iterator it=PersistedDeadEntities.begin();it!=PersistedDeadEntities.end();++it)
        if(it->second.GamePersistent) dead_entities.push_back(it->second);
    unsigned long no_dead_things=dead_entities.size();
    context.Transfer(no_dead_things);
    for(unsigned long c2=0;c2<no_dead_things;++c2) {
        context.Transfer(dead_entities[c2].UID);
        context.Transfer(dead_entities[c2].SectionNameCRC);
    }
}

static const unsigned char* g_p;
static unsigned long get32() { unsigned long v = (unsigned long)g_p[0] | ((unsigned long)g_p[1] << 8) | ((unsigned long)g_p[2] << 16) | ((unsigned long)g_p[3] << 24); g_p += 4; return v; }

int main()
{
    CMainGameComponent component; CMainGameComponent::PMainGameComponent = &component;
    NThingManager::CEntityRuntimePersistence p;
    NThingManager::CEntityRuntimePersistence::CEntityScriptInfo info;
    info.Valid = false; info.UncompressedSize = 0; p.GamePersistentEntityScripts.push_back(info);     // slot 0: invalid
    info.Valid = true; info.Script = CCharString("Scr"); info.UncompressedSize = 9;
    info.SaveData.push_back(0xAB); info.SaveData.push_back(0xCD); p.GamePersistentEntityScripts.push_back(info);  // slot 1
    NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity d;
    d.UID = ((FableU64)5 << 32) | 1; d.GamePersistent = true; d.SectionNameCRC = 0x1111; p.PersistedDeadEntities[d.UID] = d;
    d.UID = 2; d.GamePersistent = false; d.SectionNameCRC = 0x2222; p.PersistedDeadEntities[d.UID] = d;
    d.UID = 3; d.GamePersistent = true; d.SectionNameCRC = 0x3333; p.PersistedDeadEntities[d.UID] = d;
    CMemoryDataOutputStream os; CPersistContext context(os, true);
    p.SaveToGameStateBinary(context);
    g_p = &os.Bytes[0];
    bool ok = get32() == 2 && get32() == 0;                       // two slots; slot 0 writes an empty sub-stream
    ok = ok && get32() == 4 + 4 + 4 + 2 + 4;                      // slot 1 sub-stream size
    ok = ok && get32() == 101 && get32() == 2 && get32() == 9 && g_p[0] == 0xAB && g_p[1] == 0xCD;
    g_p += 2; ok = ok && strcmp((const char*)g_p, "Scr") == 0; g_p += 4;
    ok = ok && get32() == 2;                                      // two game-persistent dead entities, map order
    ok = ok && get32() == 3 && get32() == 0 && get32() == 0x3333;
    ok = ok && get32() == 1 && get32() == 5 && get32() == 0x1111;
    ok = ok && g_p == &os.Bytes[0] + os.Bytes.size();
    if (ok) printf("00521620_TEST PASS\n");
    else printf("00521620_TEST FAIL\n");
    return ok ? 0 : 1;
}