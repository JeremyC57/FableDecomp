// Grade: DIFFER(485v501) -- level loop: retail tests info->Valid and then the info pointer (both kept; the out-of-range path jumps straight to the increment, base reloaded for the element) while ours drops the null test after the dereference; the "\r\n" argument is pushed before the level-UID call in ours, after it in retail
// NThingManager::CEntityRuntimePersistence::SaveToGameStateScript(CCharString&) -- append the game-persistent entity scripts (one START_THING_LEVEL block per valid level, named by the world map's level UID) and then every game-persistent dead thing's UID and section CRC as a text-persist block
// Leaf: SaveToGameStateScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051EDA0, length 501 bytes, `ret 4`.
// Callees: virtual slot 13 of the object at +0x24 of the global @ 0x013B86A0 (the world map), CWorldMap level-UID getter
//   0x004FCE00, the unsigned-to-string helper 0x0099F8B0 and operator+(const char*, const CCharString&) 0x0099F690,
//   operator+(const CCharString&, const char*) 0x0099F600 (all __fastcall, hidden result in ecx), CCharString::operator+=
//   (const CCharString&) 0x0099F0A0 and (const char*) 0x0099F100, ~CCharString 0x0099EAE0, CPersistContext(CCharString*)
//   0x009BAE10, CPersistContext::Transfer<unsigned __int64> 0x0044BB00 and <unsigned long> 0x004106F0 (name, value, default).
// Layout: PDB CEntityRuntimePersistence with the retail 12-byte CArray: LevelPersistentEntityScripts +0,
//   GamePersistentEntityScripts +0xc, PersistedDeadEntities (STLport map header pointer) +0x18; CEntityScriptInfo 0x18
//   bytes (Script +0, SaveData +4, UncompressedSize +0x10, Valid +0x14); CPersistedDeadEntity (UID +0, GamePersistent +8,
//   SectionNameCRC +0xc) as the map value's second at node +0x18.
struct CCharString {
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
    CCharString& operator+=(const char* text);          // 0x0099F100
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
CCharString __fastcall GFUIntToCharString(unsigned long value);                  // 0x0099F8B0

struct CPersistContext {
    unsigned char _data[0x2c];
    CPersistContext(CCharString* stringToSaveTo);                                // 0x009BAE10
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};

struct CWorldMap {
    unsigned long GetLevelUID(long level);              // 0x004FCE00
};

struct CWorldProvider {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap* GetWorldMap();                   // slot 13
};

struct SGameState_013b86a0 {
    unsigned char   _pad[0x24];
    CWorldProvider* m_pWorldProvider;                   // +0x24
};
extern SGameState_013b86a0* g_pGameState_013b86a0;      // 0x013B86A0

template <class T> struct CIndexedArray {
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    T* Get(unsigned long index) { return index < Size() ? &First[index] : 0; }
};

namespace NThingManager {
struct CEntityRuntimePersistence {
    struct CEntityScriptInfo {
        CCharString   Script;                           // +0x00
        unsigned char SaveData[0xc];                    // +0x04 CArray<unsigned char>
        long          UncompressedSize;                 // +0x10
        bool          Valid;                            // +0x14
    };
    struct CPersistedDeadEntity {
        unsigned __int64 UID;                           // +0x00
        bool             GamePersistent;                // +0x08
        unsigned long    SectionNameCRC;                // +0x0c
    };
    struct SDeadNode {                                  // STLport _Rb_tree_node<pair<const unsigned __int64, CPersistedDeadEntity> >
        long                 Color;                     // +0x00
        SDeadNode*           Parent;                    // +0x04
        SDeadNode*           Left;                      // +0x08
        SDeadNode*           Right;                     // +0x0c
        unsigned __int64     Key;                       // +0x10
        CPersistedDeadEntity Value;                     // +0x18
    };
    CIndexedArray<CEntityScriptInfo> LevelPersistentEntityScripts;   // +0x00
    CIndexedArray<CEntityScriptInfo> GamePersistentEntityScripts;    // +0x0c
    SDeadNode*                       PersistedDeadEntitiesHeader;    // +0x18
    void SaveToGameStateScript(CCharString& script);
};
}

using namespace NThingManager;

static inline CEntityRuntimePersistence::SDeadNode* NextNode(CEntityRuntimePersistence::SDeadNode* node)
{
    if (node->Right != 0) {
        node = node->Right;
        while (node->Left != 0)
            node = node->Left;
    } else {
        CEntityRuntimePersistence::SDeadNode* parent = node->Parent;
        while (node == parent->Right) {
            node = parent;
            parent = parent->Parent;
        }
        if (node->Right != parent)
            node = parent;
    }
    return node;
}

void CEntityRuntimePersistence::SaveToGameStateScript(CCharString& script)
{
    CWorldMap* worldMap = g_pGameState_013b86a0->m_pWorldProvider->GetWorldMap();
    for (unsigned long i = 0; i < GamePersistentEntityScripts.Size(); ++i) {
        CEntityScriptInfo* info = GamePersistentEntityScripts.Get(i);
        if (info->Valid && info) {
            script += "START_THING_LEVEL " + GFUIntToCharString(worldMap->GetLevelUID(i)) + "\r\n";
            script += info->Script;
            script += "END_THING_LEVEL\r\n";
        }
    }
    script += "\r\nSTART_DEAD_THINGS\r\n";
    CPersistContext context(&script);
    for (SDeadNode* node = PersistedDeadEntitiesHeader->Left; node != PersistedDeadEntitiesHeader; node = NextNode(node)) {
        if (node->Value.GamePersistent) {
            script += "START_NEW_DEAD_THING\r\n";
            context.Transfer("UID", node->Value.UID, (unsigned __int64)0);
            context.Transfer("SectionNameCRC", node->Value.SectionNameCRC, (unsigned long)0);
            script += "END_NEW_DEAD_THING\r\n";
        }
    }
    script += "END_DEAD_THINGS\r\n\r\n";
}
