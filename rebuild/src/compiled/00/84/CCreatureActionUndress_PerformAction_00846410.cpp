// CCreatureAction_Undress::PerformAction() -- take every type-2 appearance modifier the creature wears whose object def's appearance-modifier sub def is removable, then for each (last first) take it off and drop the item as a thing at the head position, offset by a random +-0.2 in X/Y, and give it that offset (with 0.15 up) as its velocity
// Leaf: ?PerformAction@CCreatureAction_Undress@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00846410, length 580 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020, malloc (CRT
//          import thunk) @ 0x00BFEA0E, CTCHeroAttachableAppearanceModifiers::GetAppearanceModifierList(list<long>&,
//          EHeroAttachableAppearanceModifierType) const @ 0x00706800 / RemoveAppearanceModifier(long) @ 0x00706370, CCreatureActionBase::GetPDefinitionManager
//          @ 0x00692910 / GetPWorld @ 0x00692940 / GetPlayer @ 0x00692930, CDefinitionManager::GetDef(long,
//          CDefPointer*) @ 0x0042B040, CDefClassBase::GetSubDef(CDefPointer*) @ 0x0057FF92, CWorld random seed
//          @ 0x0049D810, GFFloatRandom @ 0x004978A0 (__fastcall), free @ 0x00BFEA14, CCharString @ 0x0099EBF0 / ~
//          @ 0x0099EAE0, CThingCreatureBase::GetHeadPos(bool) @ 0x00661B40, CThingObject::Create @ 0x00703210
//          (__fastcall), thing velocity setter @ 0x006A4DC0, ~list<long> @ 0x0042AB73.
// Name fix (agentNF 2026-11-10): the worn list is the shared STLport 4.6 list<long>; ~_List_base 0x0042AB73, GetAppearanceModifierList
//   0x00706800 and RemoveAppearanceModifier 0x00706370 bound under their PDB spellings (were ??1CLongList@@, GetModifiers(CLongList&),
//   AddAppearanceModifier_00706370; names: Ego_r/FableWin name sets, landed 0x005B4EBA / 0x007066A0 bindings).
// Shape: GetDef/GetSubDef get same-TU noinline stand-ins (they write only their out pointer), so the def pointers stay
//   in ebp/ebx across loop 1 (ebx is pushed around that loop only). The STLport list<long> ctor zero-inits its node
//   member, then links a fresh 12-byte sentinel through a local before storing it. The item offset is a 12-byte
//   C3DVector with only X/Y assigned (keeps retail's frame); the name, player, offset and position live in a block of
//   their own so the "" CCharString dies right after Create.
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_POPBACK
#include "stlport/fable_containers.h"   // malloc 0x00BFEA0E / free 0x00BFEA14 (MSVCR71 import thunks) through allocator<_List_node<long> >
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CRightHandedSet;
inline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(b.X + a.X, b.Y + a.Y, a.Z); }
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

// STLport 4.6 list<long> (stlport/fable_containers.h): the constructor is expanded here, the _List_base destructor is called out of
// line (??1?$_List_base@JV?$allocator@J@std@@@std@@QAE@XZ 0x0042AB73): declared as an explicit specialisation, no body.
template<> std::_List_base<long, std::allocator<long> >::~_List_base();

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
    long RefCount;                                           // +4
};
class CAppearanceModifierDef : public CDefBase { public:
    unsigned char _pad08[0x44 - 8];
    bool          IsRemoveable;                              // +0x44
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
class CThingObjectDef : public CDefBase { public:
    bool GetSubDef(CDefPointer<CAppearanceModifierDef>* out);   // 0x0057FF92
};
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CThingObjectDef>* out);   // 0x0042B040
};
CThingObjectDef* FindThingObjectDef(long index);                       // stand-in: the def lookup (not graded here)
CAppearanceModifierDef* FindAppearanceModifierDef(const CThingObjectDef* def);   // stand-in: the sub def lookup
#pragma auto_inline(off)
// Same-TU bodies of the two def getters (retail instantiates them in this TU): they write only their out pointer.
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CThingObjectDef>* out)
{
    CThingObjectDef* def = FindThingObjectDef(index);
    out->Object = def;
    return def != 0;
}
__declspec(noinline) bool CThingObjectDef::GetSubDef(CDefPointer<CAppearanceModifierDef>* out)
{
    CAppearanceModifierDef* def = FindAppearanceModifierDef(this);
    out->Object = def;
    return def != 0;
}
class CWorld { public:
    unsigned long* GetWorldSeed() const;   // 0x0049D810 (Ego_r.pdb ?GetWorldSeed@CWorld@@QBEPAKXZ)
};
#pragma auto_inline(on)
struct CRandomSource {
};
class CWorld;
class CPlayer { public:
    unsigned char _pad00[0x28];
    long          ID;                                        // +0x28
};
class CThingObject { public:
    void SetVelocity(const C3DVector& velocity);             // 0x006A4DC0
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player, const CRightHandedSet* orientation,
                                           unsigned long flags, const CCharString& name);   // 0x00703210 (Ego_r/FableWin ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z)
};

enum EHeroAttachableAppearanceModifierType { };      // Ego_r enum (value 2 used here)
class CTCHeroAttachableAppearanceModifiers { public:
    void GetAppearanceModifierList(std::list<long>& out, EHeroAttachableAppearanceModifierType type) const;   // 0x00706800
    void RemoveAppearanceModifier(long def);                 // 0x00706370
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    C3DVector GetHeadPos(bool world);                 // 0x00661B40
    FABLE_FORCEINLINE CTCBase* FindTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return it->second;
    }
    template <class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->end() || interfaceType < it->first ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    FABLE_FORCEINLINE bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;                       // +0x08
    CThing* GetCreature() const { return PCreature.GetPItem(); }
    CDefinitionManager* GetPDefinitionManager() const;       // 0x00692910
    CPlayer* GetPlayer() const;                              // 0x00692930
    CRandomSource* GetPWorld() const;                        // 0x00692940
};
struct CCreatureAction_Undress : CCreatureActionBase {
    virtual void PerformAction();
};

void CCreatureAction_Undress::PerformAction()
{
    CTCHeroAttachableAppearanceModifiers* modifiers;
    if (!GetCreature()->GetInterface(&modifiers, 0x5e))
        return;
    std::list<long> worn;
    modifiers->GetAppearanceModifierList(worn, (EHeroAttachableAppearanceModifierType)2);
    for (std::list<long>::iterator it = worn.begin(); it != worn.end(); ) {
        CDefPointer<CThingObjectDef> def;
        CDefPointer<CAppearanceModifierDef> modifier;
        if (GetPDefinitionManager()->GetDef(*it, &def) && def->GetSubDef(&modifier) && !modifier->IsRemoveable)
            it = worn.erase(it);
        else
            ++it;
    }
    while (!worn.empty()) {
        long item = worn.back();
        modifiers->RemoveAppearanceModifier(item);
        C3DVector velocity;
        velocity.X = 0.2f - GFFloatRandom(0.4f, ((CWorld*)(GetPWorld()))->GetWorldSeed());
        velocity.Y = 0.2f - GFFloatRandom(0.4f, ((CWorld*)(GetPWorld()))->GetWorldSeed());
        velocity.Z = 0.15f;
        CThingObject* thing;
        {
            CCharString name("");
            long player = GetPlayer()->ID;
            C3DVector offset;
            offset.X = velocity.X;
            offset.Y = velocity.Y;
            C3DVector position = GetCreature()->GetHeadPos(true) + offset;
            thing = CThingObject::Create(item, position, player, 0, 0, name);
        }
        if (thing)
            thing->SetVelocity(velocity);
        worn.pop_back();
    }
}