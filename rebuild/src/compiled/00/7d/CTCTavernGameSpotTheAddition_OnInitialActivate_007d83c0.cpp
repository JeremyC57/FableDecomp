// CTCTavernGameSpotTheAddition::OnInitialActivate() -- run the tavern-game activation, find the owner's free game-position action point, create the game piece named by the sub def, then look through the owner's linked things (component 0x21) for the first live one whose type matches the def's cover type: remember it with its position and frame, and lay the cover over the board unless the game is already showing
// Retail: Fable.exe @ 0x007D83C0, 432 bytes (plain `ret`, no EH frame).
// Shape: the sub-def GetSubDef is a same-TU auto_inline(off) stand-in (def stays in edi, release without re-reading); the linked-things lookup is a const GetInterface (LowerBound 0x4365B0); the cover frame comes from vslot 72 on the thing's +0x60 object.
// Callees: CTCTavernGame::OnInitialActivate 0x005EA650, CDefClassBase::GetSubDef<T> 0x007D9A80, LowerBound 0x0040F020 / 0x004365B0 (const),
//   CCharString ctor 0x0099EBF0, CTCNamedUserActionPoints::GetFreeNamedActionPoint 0x007FF190, CTCTavernGame::CreateGamePiece 0x005EA9B0,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0, 0x00686D60 (game view), PositionCover 0x007D7F10
class CTCBase;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Forward; C3DVector Up; };
class CCharString { public:
    void* m_data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
struct CSpotTheAdditionDef : CDefBase {                 // descriptive
    unsigned char _pad08[0x2c - 0x08];
    long CoverThingType;                                // +0x2c
    unsigned char _pad30[0x64 - 0x30];
    long GamePieceType;                                 // +0x64
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CSpotTheAdditionDef>* def) const;   // 0x007D9A80
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CFrameSource { virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b(); virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v1a(); virtual void v1b(); virtual void v1c(); virtual void v1d(); virtual void v1e(); virtual void v1f(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v2a(); virtual void v2b(); virtual void v2c(); virtual void v2d(); virtual void v2e(); virtual void v2f(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v3a(); virtual void v3b(); virtual void v3c(); virtual void v3d(); virtual void v3e(); virtual void v3f(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); 
    virtual const CRightHandedSet& GetRHSet(); };   // vslot 72 (+0x120)
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CFrameSource* Frame;                                // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CDefClassBase* DefClass;                            // +0x70
    unsigned char _pad74[0x8c - 0x74];
    unsigned short DefIndex;                            // +0x8c
    unsigned char _pad8e[0x91 - 0x8e];
    unsigned char Dead : 1;                             // +0x91 bit0
    const C3DVector& GetPos() const;               // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
    template<class T> __forceinline bool GetInterfaceC(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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
struct CLinkNode {
    CLinkNode* Next;                                    // +0x00
    CLinkNode* Prev;                                    // +0x04
    CIntelligentPointer<CThing> Item;                       // +0x08
};
struct CTCLinkedThings {                                // component 0x21 (descriptive)
    unsigned char _pad00[0x0c];
    CLinkNode* Head;                                    // +0x0c (STLport list sentinel)
};
class CTCNamedUserActionPoints { public:
    bool GetFreeNamedActionPoint(CThing* user, C3DVector* pos, CRightHandedSet* rh, CCharString name, long* index,
                                 long flags, bool reserve);   // 0x007FF190
};
struct CGameView { virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
                   virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual bool IsShowing(); };
#pragma pack(push, 1)
struct CTCTavernGameSpotTheAddition {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0xf8 - 0x08];
    CThing* Cover;                                      // +0xf8
    unsigned char _padfc[0x104 - 0xfc];
    void* GamePiece;                                    // +0x104
    unsigned char _pad108[0x11c - 0x108];
    C3DVector CoverPosition;                            // +0x11c
    CRightHandedSet CoverRHSet;                         // +0x128
    unsigned char _pad140[0x144 - 0x140];
    long CoverState;                                    // +0x144
    unsigned char _pad148[0x14d - 0x148];
    CRightHandedSet GamePositionRHSet;                  // +0x14d
    C3DVector GamePositionPos;                          // +0x165
    unsigned char _pad171[0x174 - 0x171];
    long GamePositionActionPointIndex;                  // +0x174
    void OnInitialActivateBase();                       // 0x005EA650
    void* CreateGamePiece(long type);                   // 0x005EA9B0
    CGameView* GetGameView();                           // 0x00686D60
    void PositionCover(float t);                        // 0x007D7F10
    void OnInitialActivate();
};
#pragma pack(pop)
CSpotTheAdditionDef* FindSubDef(const CDefClassBase* defClass);
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CSpotTheAdditionDef>* out) const
{
    CSpotTheAdditionDef* def = FindSubDef(this);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)

void CTCTavernGameSpotTheAddition::OnInitialActivate()
{
    OnInitialActivateBase();
    CDefPointer<CSpotTheAdditionDef> def;
    ParentThing->DefClass->GetSubDef(&def);
    CThing* thing = ParentThing;
    CTCNamedUserActionPoints* points = 0;
    thing->GetInterface(&points, 46);
    points->GetFreeNamedActionPoint(thing, &GamePositionPos, &GamePositionRHSet, CCharString("DUMMY_OBJECT_GAME_POSITION"),
                                    &GamePositionActionPointIndex, 0, true);
    GamePiece = CreateGamePiece(def->GamePieceType);
    CoverState = 0;
    Cover = 0;
    CTCLinkedThings* links;
    if (ParentThing->GetInterfaceC(&links, 0x21)) {
        for (CLinkNode* node = links->Head->Next; node != links->Head; node = node->Next) {
            CThing* t = node->Item.GetPItem();
            if (t && !t->Dead && t->DefIndex == def->CoverThingType) {
                Cover = t;
                CoverPosition = t->GetPos();
                CoverRHSet = t->Frame->GetRHSet();
                if (!GetGameView()->IsShowing())
                    PositionCover(1.0f);
                break;
            }
        }
    }
}