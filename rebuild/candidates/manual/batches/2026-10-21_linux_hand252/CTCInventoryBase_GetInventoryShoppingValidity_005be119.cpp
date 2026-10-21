// CTCInventoryBase::GetInventoryShoppingValidity() -- the shop is valid while the shopkeeper lives; when it does, its shop's keeper creature (if alive) finishes any action it may, and with the hero alive the keeper is turned to face the hero
// Leaf: GetInventoryShoppingValidity
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BE119, 363 bytes, plain `ret`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (`leave`, `push 53h ; pop edi`).
// Callees: IS_THING_ALIVE<CThing*> @ 0x0040DCE0 (twice), IS_THING_ALIVE @ 0x005C05C0 (the keeper creature) and
//   @ 0x0043DBD0 (the hero), all `__fastcall` on the pointer's address; std::bitset<>::test @ 0x00410DE0 and
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, types 0x53 and 2),
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50, IsFreeToFinishAction @ 0x006621C0, FinishCurrentAction @ 0x00662650,
//   CTCBase::GetPlayerManager @ 0x00686D40, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroThing
//   @ 0x00487DC0, CThing::GetPosition @ 0x004C73D0, _CIfmod @ 0x00BFEB28, the type-2 component's virtual slot 66
//   (+0x108, set the facing angle).
// Data: 1/(2*pi) @ 0x01237F08, 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// Shape: same-TU `auto_inline(off)` stand-ins for the three IS_THING_ALIVE instantiations and LowerBound let VC7.1 pack
//   the frame as retail (creature in esi, the hero in the creature's word); the difference vector is a `const C2DVector&`
//   bound to a temporary (keeps retail's `fld st(1)` copies around fpatan); the type-2 lookup ignores its bool (an absent interface reads the uninitialised out word, which shares the key
//   slot); the facing angle is atan2(dx, dy) / 2pi wrapped into [0, 1) as the landed 0x00751970.
// Layout: retail CTCInventoryBase PShopKeeper (CThing*) +0xB8; the shop component (type 0x53) holds the keeper
//   creature's intelligent pointer at +0x10; the type-2 component's X / Y at +0x0C / +0x10; retail CThing bitset +0x20,
//   interface map +0x44.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <math.h>
#pragma intrinsic(atan2, fmod)

class CTCBase;
struct CPresenceBits { bool test(long kind); };            // bitset<>::test 0x00410DE0
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
struct C3DVector { float X, Y, Z; };
struct C2DVector { float X, Y; C2DVector(float x, float y) : X(x), Y(y) { } };
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20 std::bitset of interface types
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing {
public:
    bool IsFreeToFinishAction();                            // 0x006621C0
    void FinishCurrentAction();                             // 0x00662650
};
class CThingHero : public CThing { };
bool __fastcall IS_THING_ALIVE(CThing* const& thing);                  // 0x0040DCE0
bool __fastcall IS_THING_ALIVE(CThingCreatureBase* const& thing);      // 0x005C05C0
bool __fastcall IS_THING_ALIVE(CThingHero* const& thing);              // 0x0043DBD0

// Same-TU stand-ins: with these bodies visible VC7.1 sees that the pointer addresses and the lookup key do not
// escape, and packs the frame as retail did.
bool IsAlive(const CThing* thing);
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* lo = first;
    SEntry* hi = last;
    while (lo < hi) {
        SEntry* mid = lo + (hi - lo) / 2;
        if (mid->type < *type)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}
bool __fastcall IS_THING_ALIVE(CThing* const& thing) { return thing != 0 && IsAlive(thing); }
bool __fastcall IS_THING_ALIVE(CThingCreatureBase* const& thing) { return thing != 0 && IsAlive(thing); }
bool __fastcall IS_THING_ALIVE(CThingHero* const& thing) { return thing != 0 && IsAlive(thing); }
#pragma auto_inline(on)

struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThingCreatureBase* GetPItem() const;                   // 0x00A01B50
};
struct CTCShop {                                            // interface 0x53
    unsigned char _pad00[0x10];
    CBaseIntelligentPointer PKeeper;                        // +0x10
};
struct CTCFacing {                                          // interface 2
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65();
    virtual void SetFacingAngle(float angle);               // slot 66 (+0x108)
    unsigned char _pad04[0x0c - 0x04];
    float X;                                                // +0x0C
    float Y;                                                // +0x10
};
struct CPlayer { CThingHero* GetHeroThing(); };             // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };        // 0x00449970

struct CTCInventoryBase {
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04 CTCBase
    unsigned char _pad008[0xb8 - 0x08];
    CThing*       PShopKeeper;                              // +0xB8

    CPlayerManager* GetPlayerManager() const;               // 0x00686D40 (CTCBase)
    bool GetInventoryShoppingValidity();
};

bool CTCInventoryBase::GetInventoryShoppingValidity()
{
    bool valid = IS_THING_ALIVE(PShopKeeper);
    if (valid && IS_THING_ALIVE(PShopKeeper)) {
        CThing* keeper = PShopKeeper;
        CTCShop* shop;
        if (keeper->GetInterface(&shop, 0x53)) {
            CThingCreatureBase* creature = shop->PKeeper.GetPItem();
            if (IS_THING_ALIVE(creature)) {
                if (creature->IsFreeToFinishAction())
                    creature->FinishCurrentAction();
                CThingHero* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
                if (IS_THING_ALIVE(hero)) {
                    CTCFacing* facing;
                    creature->GetInterface(&facing, 2);
                    const C3DVector& target = hero->GetPosition();
                    const C2DVector& difference = C2DVector(target.X - facing->X, target.Y - facing->Y);
                    float angle = atan2(difference.X, difference.Y) * 0.15915494f;
                    if (angle < 0.0f || angle >= 1.0f) {
                        angle = (float)fmod((double)angle, 1.0);
                        if (angle < 0.0f)
                            angle += 1.0f;
                    }
                    facing->SetFacingAngle(angle);
                }
            }
        }
    }
    return valid;
}
