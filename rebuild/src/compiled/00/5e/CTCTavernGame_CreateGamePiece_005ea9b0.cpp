// CTCTavernGame::CreateGamePiece(long defIndex) -- create a game-piece object at the owner's position, give it the owned-entity TCs pointing back at the owner
// Leaf: CreateGamePiece
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA9B0, length 520 bytes, `ret 4`; called by the landed
// CTCTavernGameCoinBase::AddNewCoin (0x008EFA00).
// Callees: CTCBase::GetWorld @ 0x00686D50, CWorld::GetMap @ 0x00449700 (catalog label "DrawGetEnvironment"),
// CThing::GetPosition @ 0x004C73D0, CThingObject::Create (fastcall) @ 0x00703210, CCharString(const char*, long)
// @ 0x0099EBF0 / ~ 0x0099EAE0, CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem
// 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10, CTCOwnedEntity::SetPOwner @ 0x007E8590.
// Layout: retail CThing interface bitset +0x20 / map +0x44, the bitfield byte +0x91 and graphic +0x60; the
// interface-0x32 component keeps its owner CIntelligentPointer<CThing> at +0x0c; the interface-0x5b component its flag
// byte at +0x59.
// Shape: each lookup is the `T* p; GetInterface(type, &p);` out-parameter form whose miss path reads the never-written
// local (it shares the dead defIndex argument slot with the key and the string temporaries).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet {
    C3DVector Forward, Up;
    CRightHandedSet(float fx, float fy, float fz, float ux, float uy, float uz)
    {
        Forward.X = fx; Forward.Y = fy; Forward.Z = fz;
        Up.X = ux; Up.Y = uy; Up.Z = uz;
    }
};

struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* PData;                                 // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();                                 // vtable 0x01237F1C
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};

struct CTCParamsBase;
struct CThing;
struct CGraphic {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void SetVisible(bool visible);              // +0x90
};
struct CThing {
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
    const C3DVector& GetPosition() const;               // 0x004C73D0
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
struct CThingObject : CThing { };

struct CTCOwnedEntityA {                                // interface 0x32
    unsigned char _pad00[0x0c];
    CIntelligentPointer<CThing> POwner;                 // +0x0c
};
struct CTCOwnedEntity {                                 // interface 0x33
    void SetPOwner(CThing* owner);                      // 0x007E8590
};
struct CTCInterface5B {                                 // interface 0x5b
    unsigned char _pad00[0x59];
    unsigned char Flags;                                // +0x59
};

struct CMap;
CThingObject* __fastcall CreateThingObject(long defIndex, const C3DVector& pos, CMap* map, const CRightHandedSet& rh,
                                           unsigned long flags, const CCharString& name);   // 0x00703210
struct CWorld { CMap* GetMap(); };                      // 0x00449700

struct CTCTavernGame {
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04
    CWorld* GetWorld();                                 // 0x00686D50
    CThingObject* CreateGamePiece(long defIndex);
};

CThingObject* CTCTavernGame::CreateGamePiece(long defIndex)
{
    CMap* map = GetWorld()->GetMap();
    C3DVector pos = ParentThing->GetPosition();
    CRightHandedSet rh(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    CThingObject* piece = CreateThingObject(defIndex, pos, map, rh, 0, "");
    piece->Graphic->SetVisible(false);
    piece->_flags91 |= 2;
    piece->AddTC("TCOwnedEntityA", false, 0);
    CTCOwnedEntityA* owned;
    piece->GetInterface(0x32, &owned);
    owned->POwner = CIntelligentPointer<CThing>(ParentThing);
    piece->AddTC("TCOwnedEntity", false, 0);
    CTCOwnedEntity* entity;
    piece->GetInterface(0x33, &entity);
    entity->SetPOwner(ParentThing);
    CTCInterface5B* tc5b;
    piece->GetInterface(0x5b, &tc5b);
    tc5b->Flags |= 2;
    piece->SetActive(false);
    return piece;
}