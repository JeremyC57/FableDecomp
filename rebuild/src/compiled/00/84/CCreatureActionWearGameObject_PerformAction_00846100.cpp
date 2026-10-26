// CCreatureAction_WearGameObject::PerformAction() -- put on the thing the creature carries in its primary slot: fetch
//   its object def; when the creature has attachable appearance modifiers and the def has an appearance-modifier sub
//   def, take off every worn modifier that conflicts with its slot (dropping each as a thing at the head position, a
//   random +-0.2 X/Y offset, pushed by that offset and 0.15 up); then kill the carried thing and wear its def
// Leaf: PerformAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00846100, length 687 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, LowerBound @ 0x004365B0 (const) / 0x0040F020,
//   CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, GetPDefinitionManager @ 0x00692910 / GetPWorld @ 0x00692940 /
//   GetPlayer @ 0x00692930, CDefinitionManager::GetDef @ 0x0042B040, GetSubDef @ 0x0057FF92, modifiers: conflicting
//   list @ 0x00706B50, RemoveAppearanceModifier @ 0x00706370, AddAppearanceModifier @ 0x00706880, list::remove
//   @ 0x0084ACE0, ~list @ 0x0042AB73, CThing::Kill(bool) @ 0x004C9B80, CTCPhysicsBase::ApplyAcceleration @ 0x006B0210,
//   CWorld random seed @ 0x0049D810, GFFloatRandom @ 0x004978A0, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//   GetHeadPos @ 0x00661B40, CThingObject::Create @ 0x00703210 (__fastcall).
// Layout: same local models as the landed CCreatureAction_Undress::PerformAction (0x00846410); thing +0x60 physics TC,
//   +0x8c def index (word).
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
    ~CLongList();                                            // 0x0042AB73
    bool empty() const { return _M_node->_M_next == _M_node; }
    long& back() { return _M_node->_M_prev->_M_data; }
    void remove(const long& value);                          // 0x0084ACE0
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
    long RefCount;                                           // +4
};
class CAppearanceModifierDef : public CDefBase { public:
    unsigned char _pad08[0x40 - 8];
    long          Slot;                                      // +0x40
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
void RemoveListValue(CLongList* list, long value);           // stand-in body helper (not graded here)
#pragma auto_inline(off)
__declspec(noinline) void CLongList::remove(const long& value)
{
    RemoveListValue(this, value);
}
#pragma auto_inline(on)
struct CRandomSource {
    unsigned long* GetRandomSeed();                          // 0x0049D810
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
    static CThingObject* __fastcall Create(long defIndex, const C3DVector* position, long player,
                                           CRightHandedSet* orientation, unsigned long flags,
                                           const CCharString& name);   // 0x00703210
};

class CTCHeroAttachableAppearanceModifiers { public:
    CLongList GetConflictingModifiers(long slot, long type);  // 0x00706B50
    void RemoveAppearanceModifier(long def);                 // 0x00706370
    void AddAppearanceModifier(long def);                    // 0x00706880
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
    const SEntry* LowerBound(const long* type) const;        // 0x004365B0
};
class CThing;
class CTCCarrying { public:
    CThing* GetThingInPrimarySlot();                         // 0x006A1440
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x8c - 0x4c];
    unsigned short DefIndex;                                 // +0x8c
    C3DVector GetHeadPos(bool world);                        // 0x00661B40
    void Kill(bool removeNow);                               // 0x004C9B80
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
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
    CDefPointer<CThingObjectDef> def;
    GetPDefinitionManager()->GetDef(item->DefIndex, &def);
    CTCHeroAttachableAppearanceModifiers* modifiers;
    if (GetCreature()->GetInterface(&modifiers, 0x5e)) {
        CDefPointer<CAppearanceModifierDef> modifier;
        if (def->GetSubDef(&modifier)) {
            CLongList worn = modifiers->GetConflictingModifiers(modifier->Slot, 2);
            while (!worn.empty()) {
                long id = worn.back();
                modifiers->RemoveAppearanceModifier(id);
                worn.remove(id);
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
                    thing = CThingObject::Create(id, &position, player, 0, 0, name);
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