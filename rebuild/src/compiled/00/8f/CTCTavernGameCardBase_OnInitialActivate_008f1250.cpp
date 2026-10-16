// CTCTavernGameCardBase::OnInitialActivate() -- find the owner's free "DUMMY_OBJECT_GAME_POSITION" named action point (position, frame, index) through its named-action-points component, then the tavern-game activation
// Leaf: ?OnInitialActivate@CTCTavernGameCardBase
// Retail: Fable.exe @ 0x008F1250, 119 bytes; vtable slot 6 (the catalog's "vfunc_6" = CTCBase::OnInitialActivate) of both
//         CTCTavernGameCardSorting (0x0126F5B4) and CTCTavernGameCardPairs (0x0126F6FC), body in the card-base TU, so the
//         catalog's CTCTavernGameCardSorting label is the inheriting class: this is CTCTavernGameCardBase's.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined GetInterface, interface 46 = bit 14 of the
//          bitset word at CThing+0x24); CCharString::CCharString(const char*, long) 0x0099EBF0 (built in the by-value
//          argument slot); CTCNamedUserActionPoints::GetFreeNamedActionPoint(CThing*, C3DVector*, CRightHandedSet*,
//          CCharString, long*, long, bool) 0x007FF190; CTCTavernGame::OnInitialActivate() 0x005EA650 (non-virtual base call).
//          String 0x0126F7F4 "DUMMY_OBJECT_GAME_POSITION".
// Layout (retail CTCTavernGameCardBase, PDB minus 0x18): GamePositionRHSet +0xf0, GamePositionPos +0x108,
//         GamePositionActionPointIndex +0x114. Retail CThing: bitset +0x20, interface map +0x44.
// Shape: `T* p = 0; GetInterface(&p, 46)` preloads the zero in edi; the CCharString argument has a declared copy ctor so
//        it is constructed in place (`push ecx ; mov ecx,esp`).
class CTCBase;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { float m[6]; };

struct CCharString {
    void* m_data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
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
};

struct CTCNamedUserActionPoints {
    bool GetFreeNamedActionPoint(CThing* user, C3DVector* pos, CRightHandedSet* rh, CCharString name, long* index,
                                 long flags, bool reserve);   // 0x007FF190
};

struct CTCTavernGame {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    void OnInitialActivateBase();               // 0x005EA650 (CTCTavernGame::OnInitialActivate)
};

struct CTCTavernGameCardBase : CTCTavernGame {
    unsigned char   _pad_0x08[0xe8];
    CRightHandedSet GamePositionRHSet;          // +0xf0
    C3DVector       GamePositionPos;            // +0x108
    long            GamePositionActionPointIndex;   // +0x114
    void OnInitialActivate();
};

void CTCTavernGameCardBase::OnInitialActivate()
{
    CThing* thing = ParentThing;
    CTCNamedUserActionPoints* points = 0;
    thing->GetInterface(&points, 46);
    points->GetFreeNamedActionPoint(thing, &GamePositionPos, &GamePositionRHSet, CCharString("DUMMY_OBJECT_GAME_POSITION"),
                                    &GamePositionActionPointIndex, 0, true);
    OnInitialActivateBase();
}