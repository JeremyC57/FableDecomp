// CDefinitionManager::LoadDef(unsigned long globalIndex) -- make sure instantiated def `globalIndex` is loaded: an already-loaded def is only touched in the resource list; otherwise decompress its data, create it through its class's factory, load and persist it from the stream, size it and add it to the resource list (with progress markers)
// Leaf: LoadDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AD6E0, length 524 bytes, `ret 4`.
// Callees: CResource::ListRemove @ 0x009FBFB0, CResource::ListInsert @ 0x009FBF90, CCharString::CCharString(const char*,
//          long) @ 0x0099EBF0, NProgressDisplay::DisplayProgress(const CCharString&, float, bool, bool) @ 0x009E9F40
//          (`__fastcall`, the first bool in dl), CCharString::~CCharString @ 0x0099EAE0, CDataInputStream::CDataInputStream()
//          @ 0x00993B20, CDefinitionManager::DecompressDefinitionData(CMemoryDataInputStream*, ulong) @ 0x009AD5F0,
//          CDefStringTable::GetString(long) @ 0x009D49B0 (the global string table @ 0x013CA828), the class-info lookup
//          @ 0x009AD2E0, CResourceList::MakeFreeSpace @ 0x009FC4F0, CResourceList::FrameEnd @ 0x009FC150,
//          CPersistContext::CPersistContext(CDataInputStream*, bool) @ 0x009BAA30, CResource::SetResourceSize
//          @ 0x009FBF20, CResourceList::AddResource @ 0x009FBFE0, ~CMemoryDataInputStream @ 0x00993B10.
// Layout: CDefinitionManager is a CResourceList at +0 (MakeFreeSpace/FrameEnd/AddResource on `this`), InstantiatedDefs
//   +0xa4 (the landed GetPDefFromGlobalIndex 0x009ADE10), the persist flag byte +0xc0; CInstantiatedDefInfo {ClassName,
//   InstantiationName, ClassIndex, PDef +0xc}; CResource / CResourceList as the landed CLipSyncDataBank::GetLipSyncData
//   0x00A42FC0 (inline Touch); the def's owner/global index at +0x1c/+0x20; CMemoryDataInputStream vtable 0x0123675C.
// Shape: the three progress markers convert their literals to CCharString temporaries before the pushes; the factory
//   result replaces the def pointer by an inline raw assignment (release through the def's inline Release() member --
//   written inline at the use it loads the count back --, null store, store); the class name is a named block-scoped
//   CCharString local (passing the GetString result straight drops the `lea`).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}

struct CResourceList;
struct CResource {
    virtual void v00();
    long           IVRefCount;                               // +4
    CResourceList* ResourceList;                             // +8
    CResource*     PrevResource;                             // +0xc
    CResource*     NextResource;                             // +0x10
    unsigned long  ResourceSize;                             // +0x14
    unsigned long  LastUsedFrame;                            // +0x18
    void ListRemove();                                       // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next);       // 0x009FBF90
    void SetResourceSize(unsigned long size);                // 0x009FBF20
    void Touch();
};
struct CResourceList {
    long          m_pad0;                                    // +0
    CResource     Head;                                      // +4
    unsigned long ResourceCount;                             // +0x20
    long          AllocatedMemory;                           // +0x24
    long          MaximumMemory;                             // +0x28
    unsigned long CurrentFrame;                              // +0x2c
    bool MakeFreeSpace(unsigned long size);                  // 0x009FC4F0
    void FrameEnd();                                         // 0x009FC150
    void AddResource(CResource* resource);                   // 0x009FBFE0
};
inline void CResource::Touch()
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}

struct CDataInputStream {
    virtual ~CDataInputStream();
    unsigned long StreamPos;                                 // +0x04
    long          StreamSize;                                // +0x08
    void*         CurrentSourcePtr;                          // +0x0c
    unsigned long SourceChunkStreamPos;                      // +0x10
    long          SourceChunkBytesRemaining;                 // +0x14
    CDataInputStream();                                      // 0x00993B20
};
struct CMemoryDataInputStream : CDataInputStream {           // vtable 0x0123675C
    const unsigned char* Data;                               // +0x18
    CMemoryDataInputStream() : Data(0) { }
    virtual ~CMemoryDataInputStream();                       // 0x00993B10
};
struct CPersistContext {
    unsigned char _body[0x2c];
    CPersistContext(CDataInputStream* stream, bool flag);    // 0x009BAA30
};

struct CDefinitionManager;
struct CDefClassBase : CResource {
    CDefinitionManager* Owner;                               // +0x1c
    unsigned long       GlobalIndex;                         // +0x20
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void SetInstantiationName(long name);            // slot 0x2c
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17();
    virtual void Persist(CPersistContext& context);          // slot 0x48
    virtual void v19(); virtual void v20(); virtual void v21();
    virtual void Load(CDataInputStream& stream);             // slot 0x58
    void Release() { if (--IVRefCount == 0) v01(); }
};
struct CDefPointer {
    CDefClassBase* Object;
    CDefPointer& operator=(CDefClassBase* p)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = p;
        return *this;
    }
    CDefClassBase* operator->() const { return Object; }
};
struct CInstantiatedDefInfo {
    long          ClassName;                                 // +0x00
    long          InstantiationName;                         // +0x04
    unsigned long ClassIndex;                                // +0x08
    CDefPointer   PDef;                                      // +0x0c
};
struct CDefClassInfo {
    long ClassIndex;                                         // +0x00
    CDefClassBase* (*Create)();                              // +0x04 factory
};

struct CDefStringTable {
    CCharString GetString(long id) const;                    // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                     // 0x013CA828

struct CDefinitionManager : CResourceList {
    unsigned char          _pad30[0xa4 - 0x30];
    CInstantiatedDefInfo** InstantiatedDefs;                 // +0xa4
    unsigned char          _pada8[0xc0 - 0xa8];
    bool                   PersistFlag;                      // +0xc0
    void DecompressDefinitionData(CMemoryDataInputStream* stream, unsigned long globalIndex);   // 0x009AD5F0
    CDefClassInfo* FindClassInfo(const CCharString& className);   // 0x009AD2E0
    void LoadDef(unsigned long globalIndex);
};

void CDefinitionManager::LoadDef(unsigned long globalIndex)
{
    CInstantiatedDefInfo* info = InstantiatedDefs[globalIndex];
    CDefClassBase* def = info->PDef.Object;
    if (def) {
        def->Touch();
        return;
    }
    NProgressDisplay::DisplayProgress("CDefinitionManager::LoadDef 1", -1.0f, false, false);
    CMemoryDataInputStream stream;
    DecompressDefinitionData(&stream, globalIndex);
    CDefClassInfo* classInfo;
    {
        CCharString className = g_DefStringTable.GetString(info->ClassName);
        classInfo = FindClassInfo(className);
    }
    if (classInfo) {
        unsigned long size = stream.StreamSize + 0x25;
        MakeFreeSpace(size);
        FrameEnd();
        CDefClassBase* created = classInfo->Create();
        info->PDef = created;
        created->SetInstantiationName(info->InstantiationName);
        created->Owner = this;
        created->GlobalIndex = globalIndex;
        created->Load(stream);
        NProgressDisplay::DisplayProgress("CDefinitionManager::LoadDef 2", -1.0f, false, false);
        CPersistContext context(&stream, PersistFlag);
        info->PDef->Persist(context);
        info->PDef->SetResourceSize(size);
        AddResource(info->PDef.Object);
        info->PDef->Touch();
        NProgressDisplay::DisplayProgress("CDefinitionManager::LoadDef 3", -1.0f, false, false);
    }
}
