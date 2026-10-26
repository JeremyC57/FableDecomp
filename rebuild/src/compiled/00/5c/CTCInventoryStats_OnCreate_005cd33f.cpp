// CTCInventoryStats::OnCreate() -- register the stats screen with the hero-information screen manager, point the inventory at the owner's CCreatureDef StatsScreenInventory def, run the base OnCreate and cache the owner's hero-experience (TC 0x68) and hero-stats (TC 4) components
// Grade: RELOCATION_MATCH -- the real GetSubDef body (size 246 -> 244) plus the GetInterface entry pick written as the
// ternary `((it == map->last || typeId < it->type) ? map->last : it)->component` (retry 10b20 lever): the two reads of
// map->last outweigh `this` in the esi/edi tie, giving retail this=edi (edi pushed at entry) and thingDef/owner=esi.
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CD33F, length 244 bytes, plain `ret`. Frame-pointer size TU: optimize("y",off) +
// ("s",on).
// Callees: CPlayerGui::GetPHeroInfoScreensManager() const @ 0x00437CD0 on g_pPlayerGui (0x013B8790), NHeroInformationScreens::
// CManager::RegisterScreen(const CBase*) @ 0x0066EB11, CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>*)
// @ 0x00485815, CTCInventoryBase::SetPDef(long) @ 0x005BDB4D, CTCInventoryBase::OnCreate() @ 0x005C0121, std::bitset::test
// @ 0x00410DE0 on the owner's UsingComponents (+0x20), the const component-map LowerBound @ 0x004365B0, the defs' slot 1 on
// release.
// Shape: the landed CTCInventoryExperience::OnCreate (0x0057713F) / CTCInventoryTrade::OnCreate (0x0059C8F2) def dance (the
// null-safe conversion to the CBase secondary base, the owner def add-ref'd and released by hand around GetSubDef, the sub-def
// released without a null test at the end); each component is the landed GetInterface bitset-test + const LowerBound shape
// with the owner re-read, the size-optimised `push 68h ; pop ebx` constant serving the test and the key.
// Layout: CThing PDef +0x70; CCreatureDef StatsScreenInventory retail +0x90 (PDB +0xA0); retail CTCInventoryBase 0x130 bytes
// with CBase at +0x130; CTCInventoryStats CreaturesKilled +0x134 (12-byte map), pMannequin +0x140, UseMannequin +0x144,
// lastCategoryIdx +0x148, pTCHeroExperience +0x14c, pTCHeroStats +0x150 (PDB minus 0x18).
#pragma optimize("y", off)
#pragma optimize("s", on)
__forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    void Release() { if (DecRefToZero(m_refCount)) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
class CCreatureDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x90 - 0x08];
    long StatsScreenInventory;                  // +0x90 (PDB +0xA0)
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefSubRef { long DefIndex; };
class CDefinitionManager { public:
    CDefPointer<CDefPointeeBase> GetDef(long index);    // 0x009ADA10 (hidden result)
};
class CDefClassBase : public CDefPointeeBase { public:
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual const CDefSubRef* FindSubDef(const CCharString& typeName);   // slot 14 (+0x38)
    unsigned char _pad08[0x1c - 0x08];
    CDefinitionManager* DefManager;                     // +0x1c
    bool GetSubDef(CDefPointer<CCreatureDef>* out);     // 0x00485815
};
// 0x00485815 CDefClassBase::GetSubDef<CCreatureDef>, the retail body: look the sub def up by its class name, fetch it from
// the definition manager and assign it into *out (addref/release); only *out is written, so the caller's def word does
// not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCreatureDef>* out)
{
    const CDefSubRef* sub = FindSubDef(CCharString("CCreatureDef"));
    if (!sub)
        return false;
    CCreatureDef* def;
    {
        CDefPointer<CDefPointeeBase> fetched = DefManager->GetDef(sub->DefIndex);
        def = (CCreatureDef*)fetched.Object;
        if (def)
            ++def->m_refCount;
        if (fetched.Object) {
            fetched.Object->Release();
            fetched.Object = 0;
        }
    }
    if (out->Object != def) {
        if (out->Object) {
            out->Object->Release();
            out->Object = 0;
        }
        out->Object = def;
        if (def)
            ++def->m_refCount;
    }
    if (def)
        def->Release();
    return true;
}
#pragma auto_inline(on)
class CTCBase;
struct CPresenceBits {
    bool test(long bit) const;                  // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
class CThing { public:
    unsigned char _pad00[0x20];
    CPresenceBits UsingComponents;              // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;            // +0x70
    template <class T> __forceinline void GetInterface(long typeId, T** out) const
    {
        if (UsingComponents.test(typeId)) {
            long type = typeId;
            const CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || typeId < it->type) ? map->last : it)->component;
        }
    }
};
namespace NHeroInformationScreens {
class CBase { public:                                  // vtable 0x0123A3E0 (its vptr is the +0x130 word)
    virtual void v00();
};
class CManager { public:
    void RegisterScreen(const CBase* screen);   // 0x0066EB11
};
}
class CPlayerGui { public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790
class CTCHeroExperience;
class CTCHeroStats;
struct CTCInventoryBase_Stats {                 // retail 0x130 bytes
    virtual void OnCreate();                    // slot 4
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x130 - 0x08];
    void SetPDef(long globalDefIndex);          // 0x005BDB4D
};
class CTCInventoryStats : public CTCInventoryBase_Stats, public NHeroInformationScreens::CBase {
public:
    unsigned char      CreaturesKilled[0xc];    // +0x134 map<unsigned long, long>
    CThing*            pMannequin;              // +0x140
    bool               UseMannequin;            // +0x144
    long               lastCategoryIdx;         // +0x148
    CTCHeroExperience* pTCHeroExperience;       // +0x14c
    CTCHeroStats*      pTCHeroStats;            // +0x150
    virtual void OnCreate();
};

void CTCInventoryStats::OnCreate()
{
    g_pPlayerGui->GetPHeroInfoScreensManager()->RegisterScreen(this);
    CDefClassBase* thingDef = ParentThing->PDef.Object;
    CDefPointer<CCreatureDef> def;
    def.Object = 0;
    if (thingDef)
        ++thingDef->m_refCount;
    thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    SetPDef(def->StatsScreenInventory);
    CTCInventoryBase_Stats::OnCreate();
    ParentThing->GetInterface(0x68, &pTCHeroExperience);
    ParentThing->GetInterface(4, &pTCHeroStats);
    def.Object->Release();
}