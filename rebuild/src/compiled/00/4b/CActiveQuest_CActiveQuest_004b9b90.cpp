// CActiveQuest::~CActiveQuest() -- destructor: release the quest name, the script-data pointer, the region list and the script pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B9B90, length 113 bytes (+ int3 padding).
// Callees: CCharString::~CCharString() @ 0x0099EAE0, operator delete @ 0x00BFE9BC (count blocks), free @ 0x00BFEA14 (the vector buffer).
// Label correction: the manifest's "CEditFractal::~CEditFractal" is a propagated name. The 0x34-byte object (allocated with `push 34h` in
// CQuestManager::ActivateQuests 0x004B3CE0 and built by CActiveQuest::CActiveQuest 0x004B0310; its __fastcall deleter 0x004BAEF0 is
// stored in the count blocks) is the PDB CActiveQuest with a 12-byte retail vector: ID +0x00, PScriptInfo +0x04, PScript (CCountedPointer)
// +0x08, Regions (vector<const CRegion*>) +0x10, MarkerPosition +0x1c, Persistent/LoadedResources +0x24/+0x25, PScriptData +0x28,
// QuestName +0x30. The PDB CEditFractal (PNoise +0, PFractal +8, EditWorld +0x10, ...) does not fit the destructor.
// Shape: no vtable store (non-polymorphic; its `??_G` at 0x004BA740 comes from a `delete`); the members are destroyed in reverse order with
// one zero register (edi): each counted pointer is `if (block) { --count; if (count == 0) { deleter(object); delete block; } }` re-reading
// the block after the decrement, then object = 0, block = 0; the STLport vector frees its buffer when non-null.
// Leaf: ??1CActiveQuest
extern "C" void __cdecl free(void* block);   // 0x00BFEA14

struct SCountBlock {                         // the shared count block of CCountedPointer: count, per-type deleter, object
    long  m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*);  // +4 (called with the object in ecx)
    void* m_pObject;                         // +8
};

template <class T>
struct CCountedPointer {                     // {object, count block}
    T*           m_pObject;
    SCountBlock* m_pCount;
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

class CCharString {
public:
    ~CCharString();                          // 0x0099EAE0
    struct CCharStringData* PStringData;
};

template <class T>
struct CRetailVector {                       // STLport vector of pointers: 12 bytes; the base destructor frees the buffer
    T* m_start;
    T* m_finish;
    T* m_endOfStorage;
    ~CRetailVector()
    {
        if (m_start)
            free(m_start);
    }
};

struct CScriptInfo;
struct CScriptBase;
struct CScriptDataBase;
struct CRegion;

struct C2DCoordI {
    long X, Y;
};

class CActiveQuest {
public:
    long                             ID;               // +0x00
    CScriptInfo*                     PScriptInfo;      // +0x04
    CCountedPointer<CScriptBase>     PScript;          // +0x08
    CRetailVector<const CRegion*>    Regions;          // +0x10
    C2DCoordI                        MarkerPosition;   // +0x1c
    bool                             Persistent;       // +0x24
    bool                             LoadedResources;  // +0x25
    CCountedPointer<CScriptDataBase> PScriptData;      // +0x28
    CCharString                      QuestName;        // +0x30
    ~CActiveQuest();
};

CActiveQuest::~CActiveQuest()
{
}