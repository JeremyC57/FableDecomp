// CCreatureAction_WearGameObject::PerformAction() -- put on the thing the creature carries in its primary slot: fetch
//   its object def; when the creature has attachable appearance modifiers and the def has an appearance-modifier sub
//   def, take off every worn modifier that conflicts with its slot (dropping each as a thing at the head position, a
//   random +-0.2 X/Y offset, pushed by that offset and 0.15 up); then kill the carried thing and wear its def
// Leaf: PerformAction
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00846100, length 687 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, LowerBound @ 0x004365B0 (const) / 0x0040F020,
//   CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, GetPDefinitionManager @ 0x00692910 / GetPWorld @ 0x00692940 /
//   GetPlayer @ 0x00692930, CDefinitionManager::GetDef @ 0x0042B040, GetSubDef @ 0x0057FF92, modifiers: conflicting
//   list @ 0x00706B50, RemoveAppearanceModifier @ 0x00706370, AddAppearanceModifier @ 0x00706880, list::remove
//   @ 0x0084ACE0, ~list @ 0x0042AB73, CThing::Kill(bool) @ 0x004C9B80, CTCPhysicsBase::ApplyAcceleration @ 0x006B0210,
//   CWorld random seed @ 0x0049D810, GFFloatRandom @ 0x004978A0, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//   GetHeadPos @ 0x00661B40, CThingObject::Create @ 0x00703210 (__fastcall).
// Name fix (agentNF 2026-11-10): `worn` is the shared STLport 4.6 list<long>; ~_List_base 0x0042AB73, list<long>::remove 0x0084ACE0
//   (header opt-in FABLE_RETAIL_STLP_LIST_REMOVE_DECL, real _list.c body as a same-TU explicit specialisation) and
//   GetAppearanceModifierListWithCoversBodyAreaFlags 0x00706B50 (Ego_r name set, catalog) bound under their real spellings (were
//   ??1CLongList@@, ?remove@CLongList@@, ?GetConflictingModifiers@..).
// Layout: same local models as the landed CCreatureAction_Undress::PerformAction (0x00846410); thing +0x60 physics TC,
//   +0x8c def index (word).
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REMOVE_DECL
#include "stlport/fable_containers.h"
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

// STLport 4.6 list<long> (stlport/fable_containers.h): returned by value into `worn`; the _List_base destructor is called out of
// line (??1?$_List_base@JV?$allocator@J@std@@@std@@QAE@XZ 0x0042AB73), declared here as an explicit specialisation, no body.
template<> std::_List_base<long, std::allocator<long> >::~_List_base();

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
    long RefCount;                                           // +4
};
class CAppearanceModifierDef : public CDefBase { public:
    unsigned char _pad08[0x40 - 8];
    long          Slot;                                      // +0x40
};
// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template <class T> class CDefPointer { public:
    typename CDefNonConst<T>::Type* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    typename CDefNonConst<T>::Type* operator->() const { return Object; }
};
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0057FF92 GetSubDef<CAppearanceModifierDef> (Ego_r.pdb member template)
};
class CThingObjectDef : public CDefClassBase { public:
};
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042B040 GetDef<CThingObjectDef> (Ego_r.pdb member template)
};
CThingObjectDef* FindThingObjectDef(long index);                       // stand-in: the def lookup (not graded here)
CAppearanceModifierDef* FindAppearanceModifierDef(const CDefClassBase* def);   // stand-in: the sub def lookup
#pragma auto_inline(off)
// Same-TU bodies of the two def getters (retail instantiates them in this TU): they write only their out pointer.
template <> __declspec(noinline) bool CDefinitionManager::GetDef<CThingObjectDef>(long index, CDefPointer<const CThingObjectDef>* out) const
{
    CThingObjectDef* def = FindThingObjectDef(index);
    out->Object = def;
    return def != 0;
}
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<const CAppearanceModifierDef>* out) const
{
    CAppearanceModifierDef* def = FindAppearanceModifierDef(this);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// STLport 4.6 _list.c list::remove, the same-TU instance retail calls out of line (0x0084ACE0, ?remove@?$list@JV?$allocator@J@std@@@std@@QAEXABJ@Z).
template<> void std::list<long>::remove(const long& __val)
{
    iterator __first = begin();
    iterator __last = end();
    while (__first != __last) {
        iterator __next = __first;
        ++__next;
        if (__val == *__first) erase(__first);
        __first = __next;
    }
}
class CWorld { public:
    unsigned long* GetWorldSeed() const;   // 0x0049D810 (Ego_r.pdb ?GetWorldSeed@CWorld@@QBEPAKXZ)
};
#pragma auto_inline(on)
struct CRandomSource {
};
class CPlayer { public:
    unsigned char _pad00[0x28];
    long          ID;                                        // +0x28
};
class CTCPhysicsBase { public:
    void ApplyAcceleration(C3DVector* acceleration);         // 0x006B0210
};
class CThingObject { public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PPhysics;                                // +0x60
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player, const CRightHandedSet* orientation,
                                           unsigned long flags, const CCharString& name);   // 0x00703210 (Ego_r/FableWin ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z)
};

enum EHeroAttachableAppearanceModifierType { };      // Ego_r enum (value 2 used here)
class CTCHeroAttachableAppearanceModifiers { public:
    std::list<long> GetAppearanceModifierListWithCoversBodyAreaFlags(long coversBodyAreaFlags, EHeroAttachableAppearanceModifierType type) const;   // 0x00706B50
    void RemoveAppearanceModifier(long def);                 // 0x00706370
    void AddAppearanceModifier(long def);                    // 0x00706880
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing;
class CTCCarrying { public:
    CThing* GetThingInPrimarySlot() const;                         // 0x006A1440
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad54[0x38];
    unsigned short DefIndex;                                 // +0x8c
    C3DVector GetHeadPos(bool world);                        // 0x00661B40
    void Kill(bool removeNow);                               // 0x004C9B80
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
    template <class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->end() || interfaceType < it->first ? map->end() : it)->second;
            return true;
        }
        return false;
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
struct CCreatureAction_WearGameObject : CCreatureActionBase {
    virtual void PerformAction();
};

void CCreatureAction_WearGameObject::PerformAction()
{
    CTCCarrying* carrying;
    static_cast<const CThing*>(GetCreature())->GetInterface(&carrying, 0x46);
    CThing* item = carrying->GetThingInPrimarySlot();
    if (item == 0 || item->DefIndex == 0)
        return;
    CDefPointer<const CThingObjectDef> def;
    GetPDefinitionManager()->GetDef(item->DefIndex, &def);
    CTCHeroAttachableAppearanceModifiers* modifiers;
    if (GetCreature()->GetInterface(&modifiers, 0x5e)) {
        CDefPointer<const CAppearanceModifierDef> modifier;
        if (def->GetSubDef(&modifier)) {
            std::list<long> worn = modifiers->GetAppearanceModifierListWithCoversBodyAreaFlags(modifier->Slot, (EHeroAttachableAppearanceModifierType)2);
            while (!worn.empty()) {
                long id = worn.back();
                modifiers->RemoveAppearanceModifier(id);
                worn.remove(id);
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
                    thing = CThingObject::Create(id, position, player, 0, 0, name);
                }
                if (thing)
                    thing->PPhysics->ApplyAcceleration(&velocity);
            }
        }
    }
    item->Kill(true);
    if (GetCreature()->GetInterface(&modifiers, 0x5e))
        modifiers->AddAppearanceModifier(item->DefIndex);
}