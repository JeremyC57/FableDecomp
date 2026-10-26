// CCreatureAction_TrollGroundPound::PerformAction() -- with the troll's ground-pound sub-def: create its trap object at the
// troll (owned by the troll's player) and make the trap spare the troll and fire at once; create its second object at the
// troll and hand its driver the troll; and, for a positive obstruction radius, create an obstruction there and pass its
// driver the def's +0x48 value
// Leaf: ?PerformAction@CCreatureAction_TrollGroundPound
// Layout: CThing interface bitset +0x20, map +0x44, PTCDriver +0x5c (the cached driver component), def +0x70, player +0x90.
// Shape: GetSubDef 0x008C79B0 sits in this TU (same-TU noinline stand-in), as does the LowerBound stand-in (0x0040F020),
//   so the key word, the def word and the out pointers share frame slots; each position is a block-scoped copy.
class CTCBase;
class C3DVector { public: float X, Y, Z; };
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CTrollGroundPoundDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long          RefCount;                           // +0x04
    unsigned char _pad08[0x3c - 0x08];
    long          TrapDefIndex;                       // +0x3c
    long          ObjectDefIndex;                     // +0x40
    float         ObstructionRadius;                  // +0x44
    long          ObstructionValue;                   // +0x48
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CTrollGroundPoundDef>* out);   // 0x008C79B0
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CTrollGroundPoundDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
void* FindSubDef(CDefClassBase* def);
#pragma auto_inline(off)
#pragma auto_inline(on)
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                  // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                      // +0x44
    unsigned char  _pad4c[0x5c - 0x4c];
    CTCBase*       PTCDriver;                         // +0x5c the cached driver component
    unsigned char  _pad60[0x70 - 0x60];
    CDefClassBase* Def;                               // +0x70
    unsigned char  _pad74[0x90 - 0x74];
    char           PlayerNumber;                      // +0x90
    CDefClassBase* GetDef() const { return Def; }
    const C3DVector& GetPosition() const;             // 0x004C73D0
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline bool GetDriver(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            *ppOut = (T*)PTCDriver;
            return true;
        }
        return false;
    }
};
class CRightHandedSet;
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* set,
                                     unsigned long flags, const CCharString& name);   // 0x00703210
__forceinline CThing* CreateThingObject(const long& defIndex, const C3DVector& position, const CThing* creature, const CCharString& name)
{
    return CreateThingObject(defIndex, &position, creature->PlayerNumber, 0, 0, name);
}
CThing* __fastcall CreateThing(long defIndex, const C3DVector* position);             // 0x0080C460
CThing* __fastcall CreateObstruction(const C3DVector* a, const C3DVector* b, float radius);   // 0x007FD4B0
class CTCTrapBase { public:                                  // interface 0x7D
    void SetThingAsImmune(CThing* thing);             // 0x0078839A
    void ManualTrigger();                             // 0x00788388
};
struct CTCDriver41 {                                  // driver of interface 0x41
    void SetOwner(CThing* owner);                     // 0x0080CBF0
};
struct CTCDriverED {                                  // driver of interface 0xED
    void SetValue(long value);                        // 0x007FD3D0
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
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;                // +0x08
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_TrollGroundPound : public CCreatureActionBase { public:
    void PerformAction();
};

void CCreatureAction_TrollGroundPound::PerformAction()
{
    CDefPointer<CTrollGroundPoundDef> def;
    if (GetCreature()->GetDef()->GetSubDef(&def)) {
        if (def.Object->TrapDefIndex) {
            C3DVector position = GetCreature()->GetPosition();
            CThing* trap = CreateThingObject(def.Object->TrapDefIndex, position, GetCreature(), "");
            CTCTrapBase* trapBase;
            trap->GetInterface(&trapBase, 0x7d);
            trapBase->SetThingAsImmune(GetCreature());
            trapBase->ManualTrigger();
        }
        if (def.Object->ObjectDefIndex) {
            C3DVector position = GetCreature()->GetPosition();
            CThing* object = CreateThing(def.Object->ObjectDefIndex, &position);
            CTCDriver41* driver;
            object->GetDriver(&driver, 0x41);
            driver->SetOwner(GetCreature());
        }
        if (def.Object->ObstructionRadius > 0.0001f) {
            C3DVector position = GetCreature()->GetPosition();
            CThing* obstruction = CreateObstruction(&position, &position, def.Object->ObstructionRadius);
            if (obstruction) {
                CTCDriverED* driver;
                obstruction->GetDriver(&driver, 0xed);
                driver->SetValue(def.Object->ObstructionValue);
            }
        }
    }
}