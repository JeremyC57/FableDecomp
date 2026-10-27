// CThingCreatureBase::GetPCombatTypeDef() const -- the creature's combat type def: the combat component's own def when it has one, else the def named by the creature def's CombatTypeDef index, else "COMBAT_TYPE_NULL"
// Grade: DIFFER(268v268) -- 268v268 register allocation only: the name-lookup arm pushes &def/&name through eax/ecx (retail ecx/edx) and the closing copy-out + release stores the incremented count before `mov eax,edx` and calls through eax (ours edx); everything else matches Also tried here: STLport bitset::test HasTC (kept), AddRef as ++m_refCount, Release as --m_refCount == 0, the DecRefToZero(long&) idiom (266 bytes, worse).
// Leaf: ?GetPCombatTypeDef@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00664D40, 266 bytes (+6 int3), `ret 4` (CDefPointer returned through the hidden pointer).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the inlined GetInterface for CTCCombat, type
//   0x30 = bit 16 of the interface bitset word at +0x24), CThing::GetDefinitionManager @ 0x004C7990, CDefinitionManager::
//   GetDef<CCombatTypeDef>(long, CDefPointer&) @ 0x00669640 and (const CCharString&, CDefPointer&) @ 0x00669770 (both
//   landed), CCharString(const char*, long) @ 0x0099EBF0 ("COMBAT_TYPE_NULL" @ 0x0125ABA0), ~CCharString @ 0x0099EAE0, the
//   def's virtual slot 1 (+4) at a zero count.
// Layout: CThing interface bitset +0x20, interface map +0x44; retail CThingCreatureBase PDef +0xE0 (the landed ctor
//   0x00666090); CThingCreatureDef CombatTypeDef +0x124 (PDB +0x12C); CTCCombat PCombatTypeDef +0x28 (PDB).
// Shape: the result local is zeroed first; the out-parameter GetInterface (component in eax); the def comes back through a
//   `__forceinline` by-value getter returning a named local (AddRef/Release as `m = m +- 1` members) into a
//   local (getter temporary +1, local +1, temporary -1 with its zero test kept), and is returned from there (+1 into the
//   caller's slot, -1); the fall-back lookups fill the local, whose copy-out and release share one null test.
extern "C" int __cdecl puts(const char* text);              // 0x00BFEAF2 (MSVCR71 import thunk)
extern "C" __declspec(noreturn) void __cdecl abort(void);   // 0x00BFEAEC
namespace std {
template <unsigned int _Nb>
class bitset {                                              // STLport 4.6 bitset<274>: 9 words, test() range-checked (no exceptions)
public:
    enum { _Words = (_Nb + 31) / 32 };
    unsigned long _M_w[_Words];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static unsigned int _S_whichbit(unsigned int __pos) { return __pos % 32; }
    static unsigned long _S_maskbit(unsigned int __pos) { return 1UL << _S_whichbit(__pos); }
    unsigned long _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
    bool _Unchecked_test(unsigned int __pos) const { return (_M_getword(__pos) & _S_maskbit(__pos)) != 0; }
    bool test(unsigned int __pos) const
    {
        if (__pos >= _Nb) {
            puts("bitset");
            abort();
        }
        return _Unchecked_test(__pos);
    }
};
}
struct CDefPointeeBase {                // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();             // slot 1
    long m_refCount;                    // +0x04
    void AddRef() { m_refCount = m_refCount + 1; }
    void Release() { m_refCount = m_refCount - 1; if (m_refCount == 0) Destroy(); }
};
struct CCombatTypeDef : CDefPointeeBase { };
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            Object->AddRef();
    }
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<CCombatTypeDef>& out);          // 0x00669640
    bool GetDef(const CCharString& name, CDefPointer<CCombatTypeDef>& out);   // 0x00669770
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callees of the stand-in bodies below
void* FindDef_CDefinitionManager_GetDef_1();
// Stand-in bodies (the real ones are out of line in retail, in this module): they write the out slot only inline, so
// VC7.1 knows the def pointer's address does not escape the call (the landed 0x00848730 / 0x0083C1E0 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CCombatTypeDef>& out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
__declspec(noinline) bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<CCombatTypeDef>& out)
{
    void* def = FindDef_CDefinitionManager_GetDef_1();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x004365B0
};
struct CTCCombat {
    unsigned char _pad00[0x28];
    CDefPointer<CCombatTypeDef> PCombatTypeDef;     // +0x28
    __forceinline CDefPointer<CCombatTypeDef> GetPCombatTypeDef() const
    {
        CDefPointer<CCombatTypeDef> def = PCombatTypeDef;
        return def;
    }
};
struct CThingCreatureDef {
    unsigned char _pad00[0x124];
    long CombatTypeDef;                     // +0x124
};
struct CThingCreatureBase {
    unsigned char _pad0[0x20];
    std::bitset<274> UsingComponents;         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0xe0 - 0x4c];
    CThingCreatureDef* PDef;                // +0xe0 CDefPointer<CThingCreatureDef const>::Object
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990
    __forceinline bool HasTC(long interfaceType) const
    {
        return UsingComponents.test(interfaceType);
    }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        CInterfaceMap* map = (CInterfaceMap*)&m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (UsingComponents.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = (CInterfaceMap*)&m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    CDefPointer<CCombatTypeDef> GetPCombatTypeDef() const;
};

CDefPointer<CCombatTypeDef> CThingCreatureBase::GetPCombatTypeDef() const
{
    CDefPointer<CCombatTypeDef> def;
    CTCCombat* combat;
    if (GetInterface(&combat, 0x30)) {
        CDefPointer<CCombatTypeDef> combatDef = combat->GetPCombatTypeDef();
        if (combatDef.Object)
            return combatDef;
    }
    if (PDef->CombatTypeDef)
        GetDefinitionManager()->GetDef(PDef->CombatTypeDef, def);
    else
        GetDefinitionManager()->GetDef("COMBAT_TYPE_NULL", def);
    return def;
}
