// Grade: DIFFER(276v270) -- only the final def release: ours `mov eax,[esp+10h] ; mov edx,[eax+4] ; dec edx ; mov [eax+4],edx`, retail `mov ecx,[esp+10h] ; dec dword ptr [ecx+4]` (known def-release load-register gap)
// CTCInventoryMap::ConstructRegionDescriptionMapKeys(NUISystem::CComponent* parent, CRegion* region, bool pc) -- creates the map-keys container component (PC or console layout) and adds it under the parent
// Retail 0x005FB7E0, 272 bytes (+ int3), `ret 0Ch`, esp frame (speed TU); `this` and `region` are unused (the
// retail body keeps only the container part: an unused CWideString local and the UI misc-things def fetch survive).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall), CWideString() 0x0099AED0 / ~CWideString
// 0x0099B510, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CGameDefinitionManager::Get
// 0x0044C6B0 (__fastcall), CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString&, long) const
// 0x009AD390 (manifest "COptimisedPrimitive::ActualToUniqueIndex" is propagated), CDefinitionManager::GetDef<T>
// (long, CDefPointer<T>*) 0x0042B312, CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D,
// CCountedPointer<CComponent>::Reset(CComponent*) 0x00429C6F, the parent's virtual slot 59 (+0xEC, AddChild(const
// CCountedPointer<CComponent>&)), operator delete 0x00BFE9BC (the count block).
// Shape: the class name converts implicitly (built first, then re-addressed with `lea`); the container is an if/else
// of two complete Reset(CreateComponent(...)) statements whose implicit CCharString temporaries live in the dead `pc`
// argument slot, tail-merged by VC7.1 after the literal push; the counted pointer's inline destructor releases the
// count block ({refs, __fastcall destroy, object}) and zeroes both words; the def pointer releases through the
// pointee's inline Release() (ecx) with no zero store.
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CWideString {
    void* m_pStringData;
    CWideString();                                     // 0x0099AED0
    ~CWideString();                                    // 0x0099B510
};
void operator delete(void* p);                         // 0x00BFE9BC

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                            // slot 1
    long m_refCount;                                   // +0x04
    void Release()
    {
        if (DecRefToZero(m_refCount))
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
};
struct CUIMiscThingsDef : CDefPointeeBase {};
struct CDefinitionManager {
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    template <class T> bool GetDef(long globalIndex, CDefPointer<T>* out);                     // 0x0042B312
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};

struct SCountBlock {
    long  m_references;                                // +0
    void (__fastcall* m_pfnDestroy)(void*);            // +4: per-type deleter, object in ecx
    void* m_pObject;                                   // +8
};
template <class T> struct CCountedPointer {
    T*           m_pObject;                            // +0
    SCountBlock* m_pCount;                             // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    ~CCountedPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
    void Reset(T* object);                             // 0x00429C6F
};

namespace NUISystem {
struct CComponent {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void AddChild(const CCountedPointer<CComponent>& child);   // slot 59 (+0xEC)
};
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance(); // 0x0041E5F2
};
}
struct CRegion;

struct CTCInventoryMap {
    void ConstructRegionDescriptionMapKeys(NUISystem::CComponent* parent, CRegion* region, bool pc);
};

void CTCInventoryMap::ConstructRegionDescriptionMapKeys(NUISystem::CComponent* parent, CRegion* region, bool pc)
{
    NUISystem::CFrontEndManager* manager = NUISystem::CFrontEndManager::GetInstance();
    CWideString text;
    CDefPointer<CUIMiscThingsDef> miscDef;
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(index, &miscDef);
    CCountedPointer<NUISystem::CComponent> container;
    if (pc)
        container.Reset(manager->CreateComponent("PC_MAP_KEYS_CONTAINER", false));
    else
        container.Reset(manager->CreateComponent("UI_MAP_KEYS_CONTAINER", false));
    parent->AddChild(container);
}
