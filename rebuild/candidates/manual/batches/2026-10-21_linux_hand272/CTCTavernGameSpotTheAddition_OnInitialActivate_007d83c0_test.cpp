// Behaviour test for CTCTavernGameSpotTheAddition::OnInitialActivate (0x007d83c0): base activation, game-position action point, game piece, first live linked thing of the cover type remembered with position/frame, cover laid unless the view is showing.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CTCBase;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Forward; C3DVector Up; };
static int g_strings;
struct CCharString {
    const char* m_data;
    CCharString(const char* text, long length = -1) : m_data(text) { (void)length; ++g_strings; }
    CCharString(const CCharString& other) : m_data(other.m_data) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
struct CDefBase {
    int destroyed;
    long RefCount;
    virtual void Destroy() { ++destroyed; }
    virtual ~CDefBase() {}
};
struct CSpotTheAdditionDef : CDefBase {
    long CoverThingType;
    long GamePieceType;
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
static CSpotTheAdditionDef* g_def;
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CSpotTheAdditionDef>* def) const;
};
CSpotTheAdditionDef* FindSubDef(const CDefClassBase*) { if (g_def) ++g_def->RefCount; return g_def; }
bool CDefClassBase::GetSubDef(CDefPointer<CSpotTheAdditionDef>* out) const
{
    CSpotTheAdditionDef* def = FindSubDef(this);
    out->Object = def;
    return def != 0;
}
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
    const SEntry* LowerBound(const long* type) const { const SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CFrameSource { CRightHandedSet RH; const CRightHandedSet& GetRHSet() { return RH; } };
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    CFrameSource* Frame;
    CDefClassBase* DefClass;
    unsigned short DefIndex;
    unsigned char Dead : 1;
    C3DVector Pos;
    const C3DVector& GetPosition() const { return Pos; }
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
struct CBaseIntelligentPointer {
    CThing* Item;
    CThing* GetPItem() const { return Item; }
};
struct CLinkNode {
    CLinkNode* Next;
    CLinkNode* Prev;
    CBaseIntelligentPointer Item;
};
struct CTCLinkedThings {
    CLinkNode* Head;
};
static int g_points; static const char* g_pointName;
struct CTCNamedUserActionPoints {
    bool GetFreeNamedActionPoint(CThing* user, C3DVector* pos, CRightHandedSet* rh, CCharString name, long* index,
                                 long flags, bool reserve)
    { ++g_points; g_pointName = name.m_data; pos->X = 5; rh->Up.Z = 1; *index = 3; (void)user; (void)flags; (void)reserve; return true; }
};
struct CGameView { bool Showing; bool IsShowing() { return Showing; } };
struct CTCTavernGameSpotTheAddition {
    CThing* ParentThing;
    CThing* Cover;
    void* GamePiece;
    C3DVector CoverPosition;
    CRightHandedSet CoverRHSet;
    long CoverState;
    CRightHandedSet GamePositionRHSet;
    C3DVector GamePositionPos;
    long GamePositionActionPointIndex;
    int baseCalls; long pieceType; CGameView view; float coverT; int covers;
    void OnInitialActivateBase() { ++baseCalls; }
    void* CreateGamePiece(long type) { pieceType = type; return &view; }
    CGameView* GetGameView() { return &view; }
    void PositionCover(float t) { coverT = t; ++covers; }
    void OnInitialActivate();
};

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
                CoverPosition = t->GetPosition();
                CoverRHSet = t->Frame->GetRHSet();
                if (!GetGameView()->IsShowing())
                    PositionCover(1.0f);
                break;
            }
        }
    }
}

int main()
{
    int fails = 0;
    static CSpotTheAdditionDef def; def.RefCount = 1; def.destroyed = 0; def.CoverThingType = 7; def.GamePieceType = 42; g_def = &def;
    static CDefClassBase defClass;
    static CTCNamedUserActionPoints points; static CTCLinkedThings links;
    static CThing owner, wrongType, deadCover, cover;
    memset(owner.m_presentBits, 0, sizeof(owner.m_presentBits));
    owner.m_presentBits[1] = (1UL << (46 - 32)) | (1UL << (0x21 - 32));
    CInterfaceMap::SEntry entries[2] = { { 0x21, (CTCBase*)&links }, { 46, (CTCBase*)&points } };
    owner.m_interfaces.first = entries; owner.m_interfaces.last = entries + 2; owner.DefClass = &defClass;
    static CFrameSource frame; frame.RH.Forward.X = 0.25f; frame.RH.Up.Z = 0.5f;
    wrongType.Dead = 0; wrongType.DefIndex = 6;
    deadCover.Dead = 1; deadCover.DefIndex = 7;
    cover.Dead = 0; cover.DefIndex = 7; cover.Pos.X = 1; cover.Pos.Y = 2; cover.Pos.Z = 3; cover.Frame = &frame;
    static CLinkNode head, n0, n1, n2, n3;
    head.Next = &n0; n0.Next = &n1; n1.Next = &n2; n2.Next = &n3; n3.Next = &head;
    n0.Item.Item = 0; n1.Item.Item = &wrongType; n2.Item.Item = &deadCover; n3.Item.Item = &cover;
    links.Head = &head;
    CTCTavernGameSpotTheAddition game; memset(&game, 0, sizeof(game));
    game.ParentThing = &owner; game.CoverState = 9;
    game.OnInitialActivate();
    if (game.baseCalls != 1 || g_points != 1 || strcmp(g_pointName, "DUMMY_OBJECT_GAME_POSITION") != 0) ++fails;
    if (game.GamePositionPos.X != 5 || game.GamePositionActionPointIndex != 3 || game.GamePositionRHSet.Up.Z != 1) ++fails;
    if (game.pieceType != 42 || game.GamePiece != &game.view || game.CoverState != 0) ++fails;
    if (game.Cover != &cover || game.CoverPosition.Z != 3 || game.CoverRHSet.Forward.X != 0.25f) ++fails;
    if (game.covers != 1 || game.coverT != 1.0f) ++fails;
    if (def.RefCount != 1 || def.destroyed != 0 || g_strings != 0) ++fails;
    // view already showing: cover found but not laid; no links component: cover stays null
    game.view.Showing = true; game.OnInitialActivate();
    if (game.covers != 1 || game.Cover != &cover) ++fails;
    owner.m_presentBits[1] &= ~(1UL << (0x21 - 32)); game.OnInitialActivate();
    if (game.Cover != 0 || game.baseCalls != 3) ++fails;
    if (fails) { printf("007d83c0_TEST FAIL %d\n", fails); return 1; }
    printf("007d83c0_TEST PASS\n");
    return 0;
}
