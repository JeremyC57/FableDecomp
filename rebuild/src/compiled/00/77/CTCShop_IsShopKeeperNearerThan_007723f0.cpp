// CTCShop::IsShopKeeperNearerThan(float distance) const -- true when both the shop keeper and the player's controlled hero are alive and the keeper lies within `distance` of the hero on the XY plane
// Leaf: ?IsShopKeeperNearerThan@CTCShop@@QBE_NM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007723F0, length 153 bytes (+ padding), `ret 4`.
// PDB: ?IsShopKeeperNearerThan@CTCShop@@QBE_NM@Z (Ego_r.pdb and FableWin.pdb agree: const, returns bool). The
//   `mov eax,1 / xor eax,eax` tails are VC7.1's code for a bool `return a && b && c;` (an if/return-true form gives
//   `mov al,1`), so the earlier attempt's `long` return was a misreading.
// Callees: CTCBase::PeekPlayerManager() const 0x00686D50 (protected, const CPlayerManager&), CPlayerManager::GetMainPlayer()
//   const 0x00449960 (const CPlayer&), CPlayer::PeekPControlledCreature() const 0x00487DD0 (const CThingPlayerCreature*),
//   CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (x3; PDB returns CBaseObject*), CThing::GetPos() const 0x004C73D0
//   (hero first, then keeper). All spellings from the Ego_r/FableWin PDB name sets and the landed callers' bindings.
// Layout: retail CTCShop: CTCBase (engine/CTCBase.h via the define pattern: ParentThing +4, ToBeRemoved +8, ..), StockState +0x0c,
//   PShopKeeper (CIntelligentPointer<CThingCreatureBase>, vptr + PData) +0x10; CThing dead flag +0x91 bit 0.
// Shape: three levers. (1) ONE bool expression `return IS_THING_ALIVE(keeper) && IS_THING_ALIVE(hero) && near;` --
//   with an `if (..) return 1; return 0;` VC7.1 gives the hero ebp at entry and shrink-wraps edi for the keeper; the
//   expression form gives retail's hero-in-edi and the late `push ebp` for the keeper. (2) the ONE generic __fastcall
//   IS_THING_ALIVE<T>(const T&) of the PDB (`thing != 0 && !thing->Dead`): over CIntelligentPointer its operator T* and
//   operator-> each call GetPItem (the two retail calls); the two instantiations mangle exactly as FableWin.pdb's
//   ??$IS_THING_ALIVE@PBVCThingPlayerCreature@@@@YI_N.. and ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingCreatureBase@@@@@@YI_N.. (3) the XY distance as a
//   C2DVector temporary's inline squared length (`X * X + Y * Y`) -- named dx/dy locals square dy first (`fld st(0)`),
//   the temporary's members give retail's `fld st(1) ; fmul st(2)` twice. The distance helper must be __forceinline
//   (plain inline is called out of line) and takes the two things, so the keeper's GetPItem runs before the hero's GetPos.
//   GFWorld2DDistanceSmallerThan(const C3DVector&, const C3DVector&, float, bool) of the PDB takes positions, which would
//   evaluate the keeper's position first; the helper here is descriptive (`IsWithin2DDistance`).
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetSquaredLength() const { return X * X + Y * Y; }
};
class CBaseObject;
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                         // +0x91 bit 0
    const C3DVector& GetPos() const;                // 0x004C73D0
};
class CThingCreatureBase : public CThing { };
class CThingPlayerCreature : public CThingCreatureBase { };
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                  // 0x00A01B50
private:
    void* PData;                                    // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    operator T*() const { return GetPItem(); }
    T* operator->() const { return GetPItem(); }
};
template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && !thing->Dead;
}
__forceinline bool IsWithin2DDistance(const CThing* a, const CThing* b, float distance)
{
    const C3DVector& pa = a->GetPos();
    const C3DVector& pb = b->GetPos();
    return C2DVector(pb.X - pa.X, pb.Y - pa.Y).GetSquaredLength() < distance * distance;
}
#define CPlayer CPlayer_PDB
#include "engine/CPlayer.h"
#undef CPlayer
class CPlayer : public CPlayer_PDB { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
#include "engine/CPlayerManager.h"                  // GetMainPlayer() const 0x00449960
#define CTCBase CTCBase_PDB
#include "engine/CTCBase.h"                          // vptr (as void*), ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
#undef CTCBase
class CTCBase : public CTCBase_PDB {
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50
};
class CTCShop : public CTCBase { public:
    long StockState;                                // +0x0c
    CIntelligentPointer<CThingCreatureBase> PShopKeeper;   // +0x10
    bool IsShopKeeperNearerThan(float distance) const;
};

bool CTCShop::IsShopKeeperNearerThan(float distance) const
{
    const CThingPlayerCreature* hero = PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    return IS_THING_ALIVE(PShopKeeper) && IS_THING_ALIVE(hero) && IsWithin2DDistance(hero, PShopKeeper.GetPItem(), distance);
}