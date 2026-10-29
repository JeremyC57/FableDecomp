// CThingObject::WorldUpdate() -- vtable slot 19: after the game-object update, count down the die-off timer (unless held by TC 0x47 or a shot TC), kill the object when its health is gone or its die-off time is up, else run its own update and the shot component's
// Leaf: ?WorldUpdate@CThingObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00704150, length 215 bytes, `ret`, bool in al; slot 19 of vtable 0x01264DCC =
//   CThingObject (rtti-vtables.tsv; CThingGameObject's slot 19 is 0x00838BA0, landed callers name it WorldUpdate).
// Callees: CThingGameObject::WorldUpdate @ 0x00838BA0 (qualified base call), CInterfaceMap::LowerBound const @ 0x004365B0
//   (TC 0x47) and non-const @ 0x0040F020 (TC 0x77), CThing::Kill(bool) @ 0x004C9B80, the object's own update @ 0x007034D0
//   (manifest label propagated; name unproven), CTCShotDefinitionBase::Update @ 0x007C21A0 (engine_api.tsv).
// Float pool: 0x0122DEDC = 0.0f.
// Layout (retail = Ego_r.pdb - 0x18 for the CThingObject members): CThingPhysical Health +0xb4; CThingObject PDef +0xd0,
//   DieOffTimer +0xd4, DiesOff +0xd8; retail CThing interface bitset +0x20 (TC 0x47 = word +0x28 bit 7, TC 0x77 = word
//   +0x2c bit 23), interface map +0x44; the TC 0x47 component's hold word +0x10 (class not identified).
// Shape: DiesOff read once into a bool local (bl); the TC 0x47 test is the out-parameter const GetInterface (component left in eax), the shot TC through the
//   non-const one; the update path is the nested success path returning false, the kill block laid out last.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const; // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CTCDieOffHold_47 {                       // the component behind ETCInterfaceType 0x47
    unsigned char _pad00[0x10];
    long HoldCount;                             // +0x10
};
struct CTCShotDefinitionBase {
    void Update();                              // 0x007C21A0
};

class CThing {
public:
    void Kill(bool immediately);                // 0x004C9B80
};

class CThingGameObject : public CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0xb4 - 0x4c];
    float Health;                               // +0xb4 (CThingPhysical)
    bool WorldUpdate();                         // 0x00838BA0 (slot 19)
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return (T*)it->second;
    }
    template<class T> __forceinline bool GetInterface(const T** out, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *out = (const T*)it->second;
            return true;
        }
        return false;
    }
};

class CThingObjectDef;
class CThingObject : public CThingGameObject {
public:
    unsigned char _padb8[0xd0 - 0xb8];
    const CThingObjectDef* PDef;                // +0xd0
    long DieOffTimer;                           // +0xd4
    bool DiesOff;                               // +0xd8
    void UpdateObject_007034d0();               // 0x007034D0
    __forceinline bool IsDieOffHeld() const
    {
        const CTCDieOffHold_47* hold;
        return GetInterface(&hold, 0x47) && hold->HoldCount != 0;
    }
    bool WorldUpdate();
};

bool CThingObject::WorldUpdate()
{
    if (CThingGameObject::WorldUpdate())
        return true;
    bool diesOff = DiesOff;
    if (diesOff) {
        if (!IsDieOffHeld() && !HasTC(0x77))
            --DieOffTimer;
    }
    if (!(Health <= 0.0f) && (!diesOff || DieOffTimer > 0)) {
        UpdateObject_007034d0();
        if (HasTC(0x77))
            GetTC<CTCShotDefinitionBase>(0x77)->Update();
        return false;
    }
    Kill(true);
    return true;
}