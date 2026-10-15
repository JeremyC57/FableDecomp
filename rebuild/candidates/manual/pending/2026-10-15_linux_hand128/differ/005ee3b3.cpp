// CTCInventoryAbilities::GetMiscThingsDef() -- look up the "UI_MISC_THINGS_DEF" definition's index and return a counted pointer to that def
// Grade: DIFFER(111v101) -- (1) `lea eax,[ebp-4]` scheduled before `push 1` (retail after it); (2) after the add-ref
//   `inc [eax+4]` the lane re-reads the def local from its frame slot for the destructor (`mov eax,[ebp-4] ; dec [eax+4] ;
//   cmp [eax+4],0`) where retail keeps it in ecx with one shared null test (`inc [ecx+4] ; dec [ecx+4] ; jne`): the
//   local's address escaped to GetDef and the lane treats the add-ref store as aliasing it. Tried: one outer def local
//   (112v101, no frame-slot sharing), this sibling-block form (111v101, best), AddRef/Release as members of the def
//   (111v101, still re-read).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE3B3, length 101 bytes, `ret 4` (hidden result pointer; `this` in ecx
//   unused).
// Identity: called with ecx = the component and a frame result slot from CTCInventoryAbilities code (0x005EEEB5,
//   0x005F1B69, ... ; 0x005EE418 is CTCInventoryAbilities::FrameUpdate), so it is a member of that TU's class; the
//   catalog label "CCreatureAction_CastStrengthMultiStrikeProper" is propagated. Ego_r.pdb names it
//   `?GetMiscThingsDef@CTCInventoryAbilities@@ABE?BV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@XZ`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CGameDefinitionManager::Get @ 0x0044C6B0 (returns
//          the global manager), CDefinitionManager::GetDefGlobalIndex(const CCharString&, bool) @ 0x009AD390 (manifest
//          label "ActualToUniqueIndex" is propagated), CCharString::~CCharString @ 0x0099EAE0,
//          CDefinitionManager::GetDef<T>(long, CDefPointer<T>*) @ 0x0042B312.
// Shape: a size-optimised frame-pointer TU (`optimize("y",off)` + `("s",on)`: `push ebp ; mov ebp,esp ... leave`,
//   `and dword ptr [ebp-4],0`); the name string lives in its own block and its frame word is reused by the def
//   pointer local (zeroed by its inline constructor); `return def;` copy-constructs the hidden result (add-ref) and the
//   local's destructor releases -- one shared null test with both `inc`/`dec [ecx+4]` kept, slot 1 on zero.
// Leaf: GetMiscThingsDef
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CDefBase {                           // counted game definition: vtable + reference count
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long m_refCount;                        // +4
};

template <class T>
struct CDefPointer {
    T* m_pDef;
    __forceinline CDefPointer() : m_pDef(0) { }
    __forceinline CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) ++m_pDef->m_refCount; }
    __forceinline ~CDefPointer() { if (m_pDef) { if (--m_pDef->m_refCount == 0) m_pDef->Destroy(); } }
};

struct CUIMiscThingsDef : CDefBase { };

struct CDefinitionManager {
    long GetDefGlobalIndex(const CCharString& name, bool mustExist);          // 0x009AD390
    template <class T> bool GetDef(long index, CDefPointer<T>* result);        // 0x0042B312 (one instantiation)
};

struct CGameDefinitionManager {
    static CDefinitionManager* Get();       // 0x0044C6B0
};

struct CTCInventoryAbilities {
    CDefPointer<CUIMiscThingsDef> GetMiscThingsDef() const;
};

CDefPointer<CUIMiscThingsDef> CTCInventoryAbilities::GetMiscThingsDef() const
{
    long index;
    {
        CCharString name("UI_MISC_THINGS_DEF", -1);
        index = CGameDefinitionManager::Get()->GetDefGlobalIndex(name, true);
    }
    {
        CDefPointer<CUIMiscThingsDef> def;      // sibling block: shares the string's frame word
        CGameDefinitionManager::Get()->GetDef(index, &def);
        return def;
    }
}
