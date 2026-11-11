// CTCTavernGameCardBase::OnInitialActivate() -- find the owner's free "DUMMY_OBJECT_GAME_POSITION" named action point (position, frame, index) through its named-action-points component, then the tavern-game activation
// Leaf: ?OnInitialActivate@CTCTavernGameCardBase
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
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
#include "engine/C3DVector.h"
class CRightHandedSet { public: float m[6]; };

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCNamedUserActionPoints { public:
    bool GetFreeNamedActionPoint(CThing* user, C3DVector* pos, CRightHandedSet* rh, CCharString name, long* index,
                                 long flags, bool reserve);   // 0x007FF190
};

class CTCTavernGame { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    virtual void OnInitialActivate();           // 0x005EA650 ?OnInitialActivate@CTCTavernGame@@UAEXXZ (vtable slot 6; Ego_r/FableWin)
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
    thing->GetTC(TCI_NAMED_USER_ACTION_POINTS, &points);
    points->GetFreeNamedActionPoint(thing, &GamePositionPos, &GamePositionRHSet, CCharString("DUMMY_OBJECT_GAME_POSITION"),
                                    &GamePositionActionPointIndex, 0, true);
    CTCTavernGame::OnInitialActivate();
}