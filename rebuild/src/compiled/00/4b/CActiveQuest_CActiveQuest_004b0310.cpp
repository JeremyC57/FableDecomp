// CActiveQuest::CActiveQuest(long id, const CCharString& questName, CScriptInfo* scriptInfo, CScriptBase* script,
// CScriptDataBase* scriptData, bool loadedResources) -- constructor: id and script info, the script and its data
// adopted by counted pointers, no regions, persistent, the resources flag and the quest name.
// Leaf: ??0CActiveQuest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B0310, length 213 bytes, `ret 18h`.
// Callees: operator new @ 0x00BFEA1A (two 12-byte count blocks with the per-type destroy functions 0x004B6AD0
// (CScriptBase) and 0x004B6AE0 (CScriptDataBase)), CCharString copy ctor @ 0x0099EC30.
// Layout: Ego_r.pdb CActiveQuest with the retail 12-byte vector (members after Regions sit 4 lower): ID +0x00,
// PScriptInfo +0x04, PScript +0x08, Regions +0x10, MarkerPosition +0x1c (not initialised), Persistent +0x24,
// LoadedResources +0x25, PScriptData +0x28, QuestName +0x30.
// Shape: the landed CCountedPointer(T*) constructor (0x008C5980) inlined twice: object stored, then for a non-null
// object a null-checked block {1, destroy, object re-read through this}; the name copy is duplicated into all three
// tails of the second pointer's branches.
void* operator new(unsigned int size);              // 0x00BFEA1A

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);          // 0x0099EC30
};

void __fastcall DestroyScript(void* object);        // 0x004B6AD0
void __fastcall DestroyScriptData(void* object);    // 0x004B6AE0

template<void (__fastcall* Destroy)(void*)> struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy), m_pObject(object) { }
};

template<class T, void (__fastcall* Destroy)(void*)> struct CCountedPointer {
    T* m_pObject;                                   // +0
    SCountBlock<Destroy>* m_pCount;                 // +4
    CCountedPointer(T* object) : m_pObject(object)
    {
        if (object)
            m_pCount = new SCountBlock<Destroy>(m_pObject);
        else
            m_pCount = 0;
    }
};

struct CScriptInfo;
struct CScriptBase;
struct CScriptDataBase;
struct CRegion;

struct CRegionVector {                              // retail vector<const CRegion*> (first, last, end)
    const CRegion** m_first;
    const CRegion** m_last;
    const CRegion** m_end;
    CRegionVector() : m_first(0), m_last(0), m_end(0) { }
};

struct C2DCoordI { long X, Y; };

struct CActiveQuest {
    long         ID;                                // +0x00
    CScriptInfo* PScriptInfo;                       // +0x04
    CCountedPointer<CScriptBase, DestroyScript> PScript;            // +0x08
    CRegionVector Regions;                          // +0x10
    C2DCoordI    MarkerPosition;                    // +0x1c
    bool         Persistent;                        // +0x24
    bool         LoadedResources;                   // +0x25
    CCountedPointer<CScriptDataBase, DestroyScriptData> PScriptData; // +0x28
    CCharString  QuestName;                         // +0x30
    CActiveQuest(long id, const CCharString& questName, CScriptInfo* scriptInfo, CScriptBase* script,
                 CScriptDataBase* scriptData, bool loadedResources);
};

CActiveQuest::CActiveQuest(long id, const CCharString& questName, CScriptInfo* scriptInfo, CScriptBase* script,
                           CScriptDataBase* scriptData, bool loadedResources)
    : ID(id)
    , PScriptInfo(scriptInfo)
    , PScript(script)
    , Regions()
    , Persistent(true)
    , LoadedResources(loadedResources)
    , PScriptData(scriptData)
    , QuestName(questName)
{
}