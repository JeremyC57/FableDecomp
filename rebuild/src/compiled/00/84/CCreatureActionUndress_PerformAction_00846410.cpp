// CCreatureAction_Undress::PerformAction() -- take every type-2 appearance modifier the creature wears whose object def's appearance-modifier sub def is removable, then for each (last first) take it off and drop the item as a thing at the head position, offset by a random +-0.2 in X/Y, and give it that offset (with 0.15 up) as its velocity
// Leaf: ?PerformAction@CCreatureAction_Undress
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00846410, length 580 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020, malloc (CRT
//          import thunk) @ 0x00BFEA0E, CTCHeroAttachableAppearanceModifiers::GetModifiers(list<long>&, long) @ 0x00706800 /
//          AddAppearanceModifier(long) @ 0x00706370 (that is: remove it), CCreatureActionBase::GetPDefinitionManager
//          @ 0x00692910 / GetPWorld @ 0x00692940 / GetPlayer @ 0x00692930, CDefinitionManager::GetDef(long,
//          CDefPointer*) @ 0x0042B040, CDefClassBase::GetSubDef(CDefPointer*) @ 0x0057FF92, CWorld random seed
//          @ 0x0049D810, GFFloatRandom @ 0x004978A0 (__fastcall), free @ 0x00BFEA14, CCharString @ 0x0099EBF0 / ~
//          @ 0x0099EAE0, CThingCreatureBase::GetHeadPos(bool) @ 0x00661B40, CThingObject::Create @ 0x00703210
//          (__fastcall), thing velocity setter @ 0x006A4DC0, ~list<long> @ 0x0042AB73.
// Shape: GetDef/GetSubDef get same-TU noinline stand-ins (they write only their out pointer), so the def pointers stay
//   in ebp/ebx across loop 1 (ebx is pushed around that loop only). The STLport list<long> ctor zero-inits its node
//   member, then links a fresh 12-byte sentinel through a local before storing it. The item offset is a 12-byte
//   C3DVector with only X/Y assigned (keeps retail's frame); the name, player, offset and position live in a block of
//   their own so the "" CCharString dies right after Create.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E (MSVCR71 import thunk)
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
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

struct SLongListNode {
    SLongListNode* _M_next;
    SLongListNode* _M_prev;
    long           _M_data;
};
struct CLongList {                                           // STLport list<long> (retail 4 bytes: the sentinel)
    SLongListNode* _M_node;
    CLongList() : _M_node(0)
    {
        SLongListNode* node = (SLongListNode*)malloc(sizeof(SLongListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~CLongList();                                            // 0x0042AB73
    bool empty() const { return _M_node->_M_next == _M_node; }
    SLongListNode* erase(SLongListNode* pos)
    {
        SLongListNode* next = pos->_M_next;
        SLongListNode* prev = pos->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(pos);
        return next;
    }
    long& back() { return _M_node->_M_prev->_M_data; }
    void pop_back()
    {
        SLongListNode* last = _M_node->_M_prev;
        SLongListNode* next = last->_M_next;
        SLongListNode* prev = last->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(last);
    }
};

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
#pragma auto_inline(on)
struct CRandomSource {
    unsigned long* GetRandomSeed();                          // 0x0049D810
};
class CWorld;
class CPlayer { public:
    unsigned char _pad00[0x28];
    long          ID;                                        // +0x28
};
class CThingObject { public:
    void SetVelocity(const C3DVector& velocity);             // 0x006A4DC0
    static CThingObject* __fastcall Create(long defIndex, const C3DVector* position, long player,
                                           CRightHandedSet* orientation, unsigned long flags,
                                           const CCharString& name);   // 0x00703210
};

class CTCHeroAttachableAppearanceModifiers { public:
    void GetModifiers(CLongList& out, long type);            // 0x00706800
    void AddAppearanceModifier_00706370(long def);                    // 0x00706370
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    C3DVector GetHeadPos(bool world);                 // 0x00661B40
    __forceinline CTCBase* FindTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return it->second;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->last || interfaceType < it->first ? map->last : it)->second;
            return true;
        }
        return false;
    }
    __forceinline bool HasTC(long interfaceType) const
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
    CLongList worn;
    modifiers->GetModifiers(worn, 2);
    for (SLongListNode* it = worn._M_node->_M_next; it != worn._M_node; ) {
        CDefPointer<CThingObjectDef> def;
        CDefPointer<CAppearanceModifierDef> modifier;
        if (GetPDefinitionManager()->GetDef(it->_M_data, &def) && def->GetSubDef(&modifier) && !modifier->IsRemoveable)
            it = worn.erase(it);
        else
            it = it->_M_next;
    }
    while (!worn.empty()) {
        long item = worn.back();
        modifiers->AddAppearanceModifier_00706370(item);
        C3DVector velocity;
        velocity.X = 0.2f - GFFloatRandom(0.4f, GetPWorld()->GetRandomSeed());
        velocity.Y = 0.2f - GFFloatRandom(0.4f, GetPWorld()->GetRandomSeed());
        velocity.Z = 0.15f;
        CThingObject* thing;
        {
            CCharString name("");
            long player = GetPlayer()->ID;
            C3DVector offset;
            offset.X = velocity.X;
            offset.Y = velocity.Y;
            C3DVector position = GetCreature()->GetHeadPos(true) + offset;
            thing = CThingObject::Create(item, &position, player, 0, 0, name);
        }
        if (thing)
            thing->SetVelocity(velocity);
        worn.pop_back();
    }
}