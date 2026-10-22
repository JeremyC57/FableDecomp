// CGameScriptThingManager::CreateScriptThing(CThing* thing) -- return the script thing for a game thing: when the list already holds an entry with the thing's unique id, refresh that CGameScriptThing (slot 71) and wrap it; otherwise create a CGameScriptThing for the current world and the thing, append {uid, counted pointer} to the list and wrap that
// Grade: DIFFER(538v531) -- control flow, calls and the found path match; the not-found path's frame differs: retail keeps an extra unused 8-byte slot between `result` (+14h) and the counted pointer (+28h) (sub esp,20h) and uses esi as its zero register; ours packs the pointer below `result` (sub esp,18h, ebp zero)
// Leaf: CreateScriptThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00704860, length 531 bytes (`ret 8`: hidden result + thing).
// Callees: CBaseClass ctor/dtor 0x0099A2D0 / 0x0099A2E0, operator new 0x00BFEA1A, CGameScriptThing::CGameScriptThing
//   (CWorld*, CThing*) 0x008D0250, CCountedPointer<CGameScriptThing>::Reset 0x007045F0 (landed), malloc 0x00BFEA0E (list
//   node), CScriptThing's by-value counted-pointer setter 0x007046D0 (landed as "SetAnimationMemoryPool"), operator
//   delete 0x00BFE9BC. Globals: g_pMainGameComponent 0x013B86A0 (PWorld +0x24). CScriptThing vtable 0x01238C8C.
// Layout: as the landed CGameScriptThingManager ctor 0x00704A80 (retail 0x10 bytes: the STLport list sentinel +0x00,
//   NullThing +0x04; Ego_r.pdb says 0x18). CScriptThingInfo is the PDB record {ulong64 ThingUID, CCountedPointer}.
//   CThing::UniqueID at +0x18 (Ego_r.pdb).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* block);             // 0x00BFE9BC
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

struct CWorld;
struct CMainGameComponent {
    unsigned char _pad00[0x24];
    CWorld*       PWorld;                          // +0x24
    CWorld* GetWorld() const { return PWorld; }
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

struct CThing {
    unsigned char _pad00[0x18];
    unsigned __int64 UniqueID;                     // +0x18
};
struct CGameScriptThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70();
    virtual void Refresh();                        // slot 71 (+0x11C)
    unsigned char _body[0x44];
    CGameScriptThing(CWorld* world, CThing* thing);   // 0x008D0250
};

struct SCountBlock {
    long m_references;                             // +0x00
    void (__fastcall* m_pfnDestroy)(CGameScriptThing*);   // +0x04
    CGameScriptThing* m_pObject;                   // +0x08
};
struct CCountedPointer {                           // CCountedPointer<CGameScriptThing>: {Data, Info}
    CGameScriptThing* Data;
    SCountBlock*      Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
    }
    void Reset(CGameScriptThing* object);          // 0x007045F0
};

struct CBaseClass {                                // vtable 0x01231710
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                         // 0x0099A2E0
};
struct SScriptCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
struct CScriptPImp {
    void* Data;
    SScriptCountBlock* Info;
    CScriptPImp() : Data(0), Info(0) { }
    CScriptPImp(const CScriptPImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->m_references; }
    ~CScriptPImp()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThing : CBaseClass {                 // vtable 0x01238C8C, 12 bytes
    CScriptPImp PImp;                              // +0x04
    CScriptThing() { }
    virtual ~CScriptThing() { }
    void SetImp(CCountedPointer imp);              // 0x007046D0
};

struct CScriptThingInfo {                          // Ego_r.pdb, 16 bytes
    unsigned __int64 ThingUID;                     // +0x00
    CCountedPointer  PScriptThing;                 // +0x08
    CScriptThingInfo(const unsigned __int64& uid, const CCountedPointer& p) : ThingUID(uid), PScriptThing(p) { }
};
struct SScriptThingInfoNode {
    SScriptThingInfoNode* m_pNext;
    SScriptThingInfoNode* m_pPrev;
    CScriptThingInfo      m_data;
};
struct CScriptThingInfoList {                      // STLport list: the sentinel pointer only
    SScriptThingInfoNode* m_pHead;
    __forceinline void PushBack(unsigned __int64 uid, const CCountedPointer& p)
    {
        SScriptThingInfoNode* position = m_pHead;
        SScriptThingInfoNode* node = (SScriptThingInfoNode*)malloc(sizeof(SScriptThingInfoNode));
        new (&node->m_data) CScriptThingInfo(uid, p);
        node->m_pNext = position;
        node->m_pPrev = position->m_pPrev;
        position->m_pPrev->m_pNext = node;
        position->m_pPrev = node;
    }
};

struct CGameScriptThingManager {
    CScriptThingInfoList ScriptThings;             // +0x00
    CScriptThing         NullThing;                // +0x04
    CScriptThing CreateScriptThing(CThing* thing);
};

CScriptThing CGameScriptThingManager::CreateScriptThing(CThing* thing)
{
    for (SScriptThingInfoNode* node = ScriptThings.m_pHead->m_pNext; node != ScriptThings.m_pHead; node = node->m_pNext) {
        if (node->m_data.ThingUID == thing->UniqueID) {
            node->m_data.PScriptThing.Data->Refresh();
            CScriptThing result;
            result.SetImp(node->m_data.PScriptThing);
            return result;
        }
    }
    {
        CScriptThing result;
        CCountedPointer imp;
        imp.Reset(new CGameScriptThing(g_pMainGameComponent->GetWorld(), thing));
        ScriptThings.PushBack(thing->UniqueID, imp);
        result.SetImp(imp);
        return result;
    }
}
