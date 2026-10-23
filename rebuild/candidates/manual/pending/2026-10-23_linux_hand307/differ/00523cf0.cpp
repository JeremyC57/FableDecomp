// Grade: DIFFER(785v799) -- call sequence and both save paths in place; residue: retail's hero path calls the vector grow directly (push_back on the known-empty vector folded), the thing vector is built after the list walk's zero register, the map count is a vector size() through &maps->vector (add eax,0x20), and frame 0x44 vs ours 0x4c
// CThingManager::RuntimeSaveGamePersistentEntitiesInLoadedMaps(bool binary) -- for every loaded world map (index 1 on),
// gather the game-state-persisted live things on that map (only the hero when the save is hero-only), then save them
// either as a binary block (memory output stream, region save) or as script text, into the runtime persistence record
// for the map; the text path also grows the per-map record vector and marks the map's record as text-saved
// Leaf: ?RuntimeSaveGamePersistentEntitiesInLoadedMaps@CThingManager@@AAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00523CF0, `ret 4`.
// Callees: the world-map manager's slot 10 (+0x28, is-map-loaded), CPlayerManager::GetMainPlayer 0x00449970,
//   CPlayer::GetHeroCreature 0x00487DC0, CThing::IsGameStatePersisted 0x004C8280, the thing vector's grow 0x00436AC0,
//   CMemoryDataOutputStream ctor 0x00996AF0 / reserve 0x00996BC0 / PeekData 0x00995E70 / GetSize 0x00994B40 / dtor
//   0x00996B30, SavePersistentEntitiesInRegions 0x00522160, the text save 0x00521EB0, CCharString 0x0099E4B0 /
//   0x0099EBF0 / reserve 0x0099EF90 / fast-extend 0x0099EF70 / dtor 0x0099EAE0, CEntityRuntimePersistence
//   SetGamePersistentSaveData 0x00523860 / text setter 0x005237F0, the record vector's resize 0x0052C6A0, free 0x00BFEA14.
// Layout: retail CThingManager +0x18 thing list head, +0x24 settings (+0x102 hero-only byte), +0x28 world-map manager
//   (map vector +0x20/+0x24, 0x48-byte elements), +0x9a saving flag, +0xa0 runtime persistence (record vector +0xc,
//   0x18-byte records: +0x10 long, +0x14 byte); CThing dead flag +0x91 bit 0, map index short +0x8e.
//   g_pPlayerManager 0x013B86A0 (+0x1c player manager).
#include <new>
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    void Reserve(long size);                            // 0x0099EF90
    void SetUseFastExtend(bool fast);                   // 0x0099EF70
};
class CThing;
struct SScalarTag { SScalarTag() { } };
struct CThingVector {
    CThing** first;
    CThing** last;
    CThing** end;
    CThingVector() : first(0), last(0), end(0) { }
    ~CThingVector() { if (first) free(first); }
    void InsertGrow(CThing** where, CThing* const& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00436AC0
    void push_back(CThing* const& value)
    {
        if (last != end) {
            new (last) CThing*(value);
            ++last;
        } else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
struct CCharStringVector {
    CCharString* first;
    CCharString* last;
    CCharString* end;
    CCharStringVector() : first(0), last(0), end(0) { }
    ~CCharStringVector()
    {
        for (CCharString* p = first; p != last; ++p)
            p->~CCharString();
        if (first)
            free(first);
    }
};
struct CMemoryDataOutputStream {
    unsigned char _body[0x18];
    CMemoryDataOutputStream();                          // 0x00996AF0
    ~CMemoryDataOutputStream();                         // 0x00996B30
    void Reserve(long size);                            // 0x00996BC0
    const void* PeekData();                             // 0x00995E70
    unsigned long GetSize();                            // 0x00994B40
};
class CThing {
public:
    unsigned char _pad00[0x8e];
    short         MapIndex;                             // +0x8e
    unsigned char _pad90;
    unsigned char Dead : 1;                             // +0x91 bit 0
    bool IsGameStatePersisted();                        // 0x004C8280
};
struct CPlayer { CThing* GetHeroCreature(); };          // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };    // 0x00449970
struct CPlayerManagerOwner { unsigned char _pad[0x1c]; CPlayerManager* Players; };   // +0x1c
extern CPlayerManagerOwner* g_pPlayerManagerOwner;      // 0x013B86A0
struct CWorldMapEntry { unsigned char _body[0x48]; };
struct CWorldMapManager {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04();
    virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(); virtual void m09();
    virtual bool IsMapLoaded(long map);                 // slot 10 (+0x28)
    unsigned char _pad04[0x20 - 0x04];
    CWorldMapEntry* MapsStart;                          // +0x20
    CWorldMapEntry* MapsEnd;                            // +0x24
    long GetNoMaps() const { return MapsEnd - MapsStart; }
};
struct CThingListNode { CThingListNode* Next; CThingListNode* Prev; CThing* Thing; };
struct CSaveSettings { unsigned char _pad[0x102]; bool HeroOnly; };   // +0x102
struct SRuntimeRecord { unsigned char _pad[0x10]; long Data; bool TextSaved; unsigned char _pad15[3]; };
struct CRuntimeRecordVector {
    SRuntimeRecord* first;
    SRuntimeRecord* last;
    SRuntimeRecord* end;
    unsigned long size() const { return last - first; }
    void resize(unsigned long size);                    // 0x0052C6A0
};
template<class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
struct CEntityRuntimePersistence {
    unsigned char        _pad00[0xc];
    CRuntimeRecordVector Records;                       // +0x0c
    void SetGamePersistentSaveData(long map, const void* data, unsigned long size);   // 0x00523860
    void SetGamePersistentSaveText(long map, const CCharString& text);               // 0x005237F0
};
class CThingManager {
public:
    unsigned char      _pad00[0x18];
    CThingListNode*    Things;                          // +0x18
    unsigned char      _pad1c[0x24 - 0x1c];
    CSaveSettings*     Settings;                        // +0x24
    CWorldMapManager*  WorldMaps;                       // +0x28
    unsigned char      _pad2c[0x9a - 0x2c];
    bool               Saving;                          // +0x9a
    unsigned char      _pad9b[0xa0 - 0x9b];
    CEntityRuntimePersistence RuntimePersistence;       // +0xa0
    void SavePersistentEntitiesInRegions(CMemoryDataOutputStream& out, long mode, CThingVector& things, CCharStringVector& names);   // 0x00522160
    void SaveEntitiesAsText(CCharString& out, long mode, CThingVector& things, CCharStringVector& names);   // 0x00521EB0
    void RuntimeSaveGamePersistentEntitiesInLoadedMaps(bool binary);
};

void CThingManager::RuntimeSaveGamePersistentEntitiesInLoadedMaps(bool binary)
{
    for (long map = 1; map < WorldMaps->GetNoMaps(); ++map) {
        if (!WorldMaps->IsMapLoaded(map))
            continue;
        CThingVector things;
        if (Settings->HeroOnly) {
            CThing* hero = g_pPlayerManagerOwner->Players->GetMainPlayer()->GetHeroCreature();
            if (hero && !hero->Dead && hero->MapIndex == map)
                things.push_back(hero);
        } else {
            for (CThingListNode* node = Things->Next; node != Things; node = node->Next) {
                CThing* thing = node->Thing;
                if (thing->IsGameStatePersisted() && thing && !thing->Dead && thing->MapIndex == map)
                    things.push_back(thing);
            }
        }
        CCharStringVector names;
        Saving = true;
        if (binary) {
            CMemoryDataOutputStream out;
            out.Reserve(0x2000);
            SavePersistentEntitiesInRegions(out, 2, things, names);
            CEntityRuntimePersistence& persistence = RuntimePersistence;
            persistence.SetGamePersistentSaveData(map, out.PeekData(), out.GetSize());
            persistence.SetGamePersistentSaveText(map, CCharString(""));
        } else {
            CCharString text;
            text.Reserve(0x8000);
            text.SetUseFastExtend(true);
            SaveEntitiesAsText(text, 2, things, names);
            CEntityRuntimePersistence& persistence = RuntimePersistence;
            persistence.SetGamePersistentSaveText(map, text);
            persistence.Records.resize(Max(persistence.Records.size(), (unsigned long)(map + 1)));
            SRuntimeRecord& record = persistence.Records.first[map];
            record.Data = 0;
            record.TextSaved = true;
        }
        Saving = false;
    }
}
