// CTCTavernBar::OnInitialActivate() -- fill every free "jug empty" action point of the bar with a new tavern jug object owned by the bar
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BA550, 865 bytes, 8-aligned ebp frame, `ret`.
// Callees: LowerBound @ 0x0040F020 (the bar's interface 0x2e), CTCNamedUserActionPoints::GetFreeNamedActionPoint @ 0x007FF190,
//   CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, CTCBase::GetWorld @ 0x00686A70 / 0x00686D50, CDefinitionManager::
//   GetDefGlobalIndexFromName @ 0x009AD410, the world's level getter @ 0x00449960, CThingObject::Create @ 0x00703210,
//   CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor, the Jugs vector reallocating insert @ 0x007B99E0, the graphic
//   SetRHSet vslot +0x12c, CThing::AddTC @ 0x004C9D60, CTCOwnedEntity::SetPOwner @ 0x007E8590.
// Layout: Ego_r.pdb CTCTavernBar Jugs +0x0c (12-byte retail vector of 8-byte intelligent pointers); the jug's interface
//   0xbb component RH set +0x11 / position +0x29 (packed), interface 0x32 owner pointer +0x0c.
#include <new>
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public:
    C3DVector Forward, Up;
    CRightHandedSet() {}
    CRightHandedSet(float fx, float fy, float fz, float ux, float uy, float uz)
    {
        Forward.X = fx; Forward.Y = fy; Forward.Z = fz;
        Up.X = ux; Up.Y = uy; Up.Z = uz;
    }
};

class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* PData;                                 // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();                                 // vtable 0x01237F1C
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};

class CTCParamsBase;
class CThing;
struct CGraphic {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void SetVisible(bool visible);              // +0x90
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72();
    virtual void v73(); virtual void v74();
    virtual void SetRHSet(const CRightHandedSet& rh);   // +0x12c
};
class CThing;
class CTCNamedUserActionPoints { public:
    bool GetFreeNamedActionPoint(CThing* user, C3DVector* pos, CRightHandedSet* rh, CCharString name, long* index, long flags,
                                 bool reserve);         // 0x007FF190
};
class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetActive(bool active);                // +0x70
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CGraphic*     Graphic;                              // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char _flags91;                             // +0x91
    const C3DVector& GetPos() const;               // 0x004C73D0
    CTCBase* AddTC(const CCharString& name, bool flag, CTCParamsBase* params);   // 0x004C9D60
    template <class T>
    __forceinline void GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
        }
    }
};
class CThingObject : public CThing { public:
    template <class T>
    __forceinline void GetInterfaceFast(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            CInterfaceMap::SEntry* last = m_interfaces.last;
            CInterfaceMap::SEntry* first = m_interfaces.first;
            long count = last - first;
            while (count > 0) {
                long half = count / 2;
                if (first[half].type < type) {
                    first += half + 1;
                    count -= half + 1;
                }
                else
                    count = half;
            }
            *out = (T*)((first == last || type < first->type) ? last : first)->component;
        }
    }
};

struct CTCOwnedEntityA {                                // interface 0x32
    unsigned char _pad00[0x0c];
    CIntelligentPointer<const CThing> POwner;                 // +0x0c
};
class CTCOwnedEntity { public:                                 // interface 0x33
    void SetPOwner(CThing* owner);                      // 0x007E8590
};
#pragma pack(push, 1)
struct CTCPlacedEntity {                                // interface 0xbb
    unsigned char   _pad00[0x11];
    CRightHandedSet RHSet;                              // +0x11
    C3DVector       Position;                           // +0x29
};
#pragma pack(pop)

class CMap;
class CDefinitionManager { public: long GetDefGlobalIndexFromName(const CCharString& name); };   // 0x009AD410
struct CWorldA {
    unsigned char _pad[0x10];
    CDefinitionManager* DefinitionManager;
    CDefinitionManager* GetDefinitionManager() const { return DefinitionManager; }
};
struct CLevel { unsigned char _pad[0x28]; CMap* Map; };
struct CWorldB { CLevel* GetLevel(); };                 // 0x00449960
CThingObject* __fastcall CreateThingObject(long defIndex, const C3DVector& pos, CMap* map, const CRightHandedSet* rh,
                                           unsigned long flags, const CCharString& name);   // 0x00703210
struct SScalarTag { SScalarTag() { } };
template <class T> struct CPtrVector {
    T* first;
    T* last;
    T* end;
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x007B99E0
    void push_back(const T& value)
    {
        if (last != end) {
            new (last) T(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
struct CBaseIntelligentPointerC : CBaseIntelligentPointer { };
template <class T> struct CIntelligentPointerC : CBaseIntelligentPointer {
    virtual void v00();                                 // vtable 0x0125A858
    CIntelligentPointerC(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointerC(const CIntelligentPointerC& other) { SetPItem(other.GetPItem()); }
};
void* GrowVectorStorage_7ba550(void* vector, unsigned long count);
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void CPtrVector<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    T* p = (T*)GrowVectorStorage_7ba550(this, count);
    if (p)
        new (p) T(value);
}
#pragma auto_inline(on)

class CTCTavernBar { public:
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[4];
    CPtrVector<CIntelligentPointerC<CThingObject> > Jugs;   // +0x0c
    CWorldA* GetWorld();                                // 0x00686A70
    CWorldB* GetWorldB();                               // 0x00686D50
    void OnInitialActivate();
};

void CTCTavernBar::OnInitialActivate()
{
    CTCNamedUserActionPoints* points;
    ParentThing->GetInterface(0x2e, &points);
    C3DVector pos;
    CRightHandedSet rh;
    long index;
    for (;;) {
        CThing* parent = ParentThing;
        if (!points->GetFreeNamedActionPoint(parent, &pos, &rh, "ACTION_POINT_JUGEMPTY", &index, 0, true))
            break;
        long defIndex = GetWorld()->GetDefinitionManager()->GetDefGlobalIndexFromName("OBJECT_VILLAGE_TAVERN_JUG");
        CMap* map = GetWorldB()->GetLevel()->Map;
        CThingObject* jug = CreateThingObject(defIndex, pos, map, 0, 0, "");
        Jugs.push_back(CIntelligentPointerC<CThingObject>(jug));
        jug->Graphic->SetRHSet(rh);
        CTCPlacedEntity* placed;
        jug->GetInterfaceFast(0xbb, &placed);
        placed->RHSet = rh;
        placed->Position = pos;
        jug->_flags91 |= 2;
        jug->AddTC("CTCCreatedEntity", false, 0);
        CTCOwnedEntityA* created;
        jug->GetInterfaceFast(0x32, &created);
        created->POwner = CIntelligentPointer<const CThing>(ParentThing);
        jug->AddTC("CTCOwnedEntity", false, 0);
        CTCOwnedEntity* owned;
        jug->GetInterfaceFast(0x33, &owned);
        owned->SetPOwner(ParentThing);
    }
}