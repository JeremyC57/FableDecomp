// CCreatureAction_HeroLoadRangedWeapon::PerformAction() -- when the ranged weapon lives and is analogue-loading, create one
// shot of the weapon's ammo (its created-shot def) at the hero for the hero's player, keep it in PAmmo, and put a live
// carryable (0x47) ammo thing into the hero's carry slot for the weapon's category
// Leaf: ?PerformAction@CCreatureAction_HeroLoadRangedWeapon
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Layout: retail CCreatureActionBase is 0xa8 (Ego_r.pdb +0xc), so PRangedWeapon +0xa8 and PAmmo +0xb0 (PDB +0xb4/+0xbc);
//   the shot def's object def index at +0x7c; CThing interface bitset +0x20, map +0x44, player +0x90.
// Shape: IS_THING_ALIVE<> are out-of-line template instances (0x004CC340, 0x0078B2A0); the object is created through an
//   inline wrapper taking the position thing and the owner (both GetPItem calls before the pushes); CIntelligentPointer
//   <CThingObject>(CThing*) is out of line (0x0065E133) and its operator= inline, reading the new item into a named local
//   before SetPItem (so &PAmmo is formed after the GetPItem call as retail); GetInterface stores the component as ONE ternary
//   `*ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;` (fixes the esi/ebx/edi permutation).
class CTCBase;
#include "engine/C3DVector.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CDefPointeeBase { public:
    virtual void v000();
    virtual void DestroyDef() const;                  // slot 1
    void Release() const
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
    mutable long m_refCount;                          // +4
};
struct CShotDef : CDefPointeeBase {
    unsigned char _pad08[0x7c - 8];
    long          ObjectDefIndex;                     // +0x7c
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<const CShotDef>* out);   // 0x0078B230
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                   // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad54[0x3c];
    char          PlayerNumber;                       // +0x90
    const C3DVector& GetPos() const;             // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingObject : public CThing {};
class CRightHandedSet;
CThingObject* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* set,
                                           unsigned long flags, const CCharString& name);   // 0x00703210
__forceinline CThingObject* CreateThingObject(long defIndex, const CThing* owner, const CThing* at, const CCharString& name)
{
    return CreateThingObject(defIndex, &at->GetPos(), owner->PlayerNumber, 0, 0, name);
}
class CTCWeapon { public:                                    // interface 0x48
    bool IsAnalogueLoading_005da8f0();                         // 0x005DA8F0
    long GetCategoryIdentifier();                     // 0x005DA920
    long GetCreatedShotDefGlobalIndex();              // 0x005DA750
};
class CTCCarrying { public:                                  // interface 0x46
    void PutThingInCarrySlot(CThing* thing, long slot, bool show);   // 0x006A1F80
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                        // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
    CBaseObject* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CBaseObject*a0);                      // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item);                     // out of line (CThingObject: 0x0065E133)
    __forceinline CIntelligentPointer& operator=(T* item)
    {
        CIntelligentPointer pointer(item);
        CThing* p = ((CThing*)pointer.GetPItem());
        SetPItem((CBaseObject*)p);
        return *this;
    }
};
template<class T> bool __fastcall IS_THING_ALIVE(const T& pointer);   // 0x004CC340 / 0x0078B2A0
class CCreatureActionBase { public:                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;            // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CDefinitionManager* GetPDefinitionManager() const;   // 0x00692910
    CThing* GetCreature() const { return ((CThing*)PCreature.GetPItem()); }
};
class CCreatureAction_HeroLoadRangedWeapon : public CCreatureActionBase { public:
    CIntelligentPointer<CThing>       PRangedWeapon;  // +0xa8
    CIntelligentPointer<CThingObject> PAmmo;          // +0xb0
    void PerformAction();
};

void CCreatureAction_HeroLoadRangedWeapon::PerformAction()
{
    CTCWeapon* weapon;
    if (IS_THING_ALIVE(PRangedWeapon) && ((CThing*)PRangedWeapon.GetPItem())->GetInterface(&weapon, 0x48)) {
        CTCCarrying* carrying;
        if (GetCreature()->GetInterface(&carrying, 0x46) && weapon->IsAnalogueLoading_005da8f0()) {
            long category = weapon->GetCategoryIdentifier();
            long shot = weapon->GetCreatedShotDefGlobalIndex();
            if (category >= 0 && shot >= 0) {
                CDefPointer<const CShotDef> def;
                GetPDefinitionManager()->GetDef(shot, &def);
                long objectDef = def->ObjectDefIndex;
                PAmmo = CreateThingObject(objectDef, GetCreature(), GetCreature(), "");
                if (IS_THING_ALIVE(PAmmo) && ((CThing*)PAmmo.GetPItem())->HasTC(0x47))
                    carrying->PutThingInCarrySlot(((CThing*)PAmmo.GetPItem()), category, true);
            }
        }
    }
}