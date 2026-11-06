// CContextSensitiveItemDef::IsValidOverride(const CThing& thing, long slot, const CTCInventory* inventory) const -- whether a context-sensitive item applies now: per item Type a health / stamina deficit, a targeted creature that can be pick-pocketed / lock-picked / bought from / recruited / dismissed / apologised to, a deed or gift the target would react to, a digging spot in range or water to fish, or a trophy held in the primary slot
// Grade: DIFFER(2684v2576) -- frame 0x3c v 0x30: retail homes the out pointers in the dead parameter slots [ebp+0Ch] / [ebp+10h] (ours [ebp+8]) and keeps 0 in edi for the whole body (`push edi ; xor edi,edi ; cmp esi,edi`), ours tests against an immediate; the case-5 sub-case true / false epilogue copies merge to different instances (VC7.1 keeps the first, retail the last); case 8's box centre converts Y first (the out-of-line C2DBoxI ctor takes the coord by reference); 378 lines, 11 attempts
// Next idea: the edi = 0 register first (an early zero-initialised local the cases share), then the epilogue order per sub-case (return v break)
// Leaf: ?IsValidOverride@CContextSensitiveItemDef@@QBE_NABVCThing@@JPBVCTCInventory@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00850A80, 2519 bytes + two switch tables (0x00851458 Type 2..9, 0x00851478 ExtraData1 1..6),
//   `ret 0Ch`; Ego_r.pdb ?IsValidOverride@CContextSensitiveItemDef@@QBE_NABVCThing@@JPBVCTCInventory@@@Z.
// Callees: CVectorMap LowerBound 0x0040F020 / const 0x004365B0, CTCTargeting vtable slot 33 (+0x84, the targeted creature),
//   CThingCreatureBase::IsPickPocketable 0x00665E90, CTCDoor::IsLockPickable 0x00730AE0, CTCFollowed::IsCreatureValidFollower
//   0x006D4E60 / IsFollower 0x006D4D70, CTCGuard::PlayerIsCriminalAndCanApologise 0x007B2BF0,
//   CContextSensitiveItemDef::TargetCreatureLikelyToReactToDeed 0x008508B0 (same TU), CThingCreatureBase::GetCreatureType
//   0x00662000, CTCWife::IsCourtableAndNotBlockingIt 0x006CF810 / IsInLoveWithHero 0x006CF900,
//   CTCHeroOpinionDeedLog::PeekPDeedEffectsDef 0x006C5630, CGameDefinitionManager::Get 0x0044C6B0,
//   CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C, CDefClassBase::GetSubDef<CGiftDef> 0x006D3EF0 / <CTrophyDef> 0x005DA300,
//   CThing::GetPos 0x004C73D0, __ftol2 0x00BFEA70, C2DBoxI(const C2DCoordI&, long) 0x00434740,
//   CThingSearchTools::GetAllThingsInRoughArea<CThing, vector<CThing*>, CThingFilter_HasTC> 0x00851490,
//   CTCCarriedActionUseSpade::IsThingAbleToDig 0x007F0300, CTCCarriedActionUseFishingRod::IsThingAbleToFish 0x007EFF80,
//   CTCCarrying::GetThingInPrimarySlot 0x006A1440, IS_THING_ALIVE<CThing*> 0x00436380 (out of line here), free 0x00BFEA14.
// Retail-asm: 0x008511A6..0x008511D5 `fld [ebp-0Ch] ; fadd [ebp-10h] ; fistp dword ptr [ebp+0Ch] ; fild ; fsub ; fstp ; mov eax ;
//   cmp eax ; jne ; dec dword ptr [ebp+0Ch]` = the engine's GFFloatToLongCeil(3.0f), through the shared helper
//   engine/GFFloatToLongCeil.h (bare dword fistp: no C++ spelling emits it).
// Layout: CContextSensitiveItemDef Type +0x28, ExtraData1 +0x34, ExtraData2 +0x38 (Ego_r.pdb = retail); CThing UsingComponents
//   +0x20, Components +0x44, ThingTypeFlags +0x6C, DefGlobalIndex +0x8C, Dead +0x91 bit 0; CThingPhysical MaxHealth +0xB0 / Health
//   +0xB4; CThingAICreature RespondingToFollowAndWait +0x1C9 bit 2 / CanBeCourted bit 3; CTCHeroStats Stamina +0x58 / MaxStamina
//   +0x5C; CTCStockItem Stealable +0x0D; CTCCreatureOpinionOfHero InFindAGuardAndYellForHelp +0x343; CTCWife ReceivedWeddingRing
//   +0x91; CGiftDef IsWeddingRing +0x2C; CTCDiggingSpot Enabled +0x0C / Hidden +0x0D; CTCTrophy MinigameInProgress +0x14;
//   CTCInventory NumberOfCSOpinionExpressions +0x1D8 / LastCSOpinionExpression +0x1E4 / LastButOneCSOpinionExpression +0x1F0;
//   COpinionDeedEffectsDef ReactionRestrictToTarget +0x7C bit 1.
#include "engine/GFFloatToLongCeil.h"
#include "stlport/fable_containers.h"
class CTCBase;
enum ETCInterfaceType { };
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

enum EOpinionDeedType { };
enum ECreatureType { };
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
class C2DCoordI { public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(const C2DCoordI& centre, long radius);   // 0x00434740
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad54[0x6c - 0x54];
    unsigned long ThingTypeFlags;               // +0x6c
    unsigned char _pad70[0x8c - 0x70];
    unsigned short DefGlobalIndex;              // +0x8c
    unsigned char _pad8e[0x91 - 0x8e];
    unsigned char Dead : 1;                     // +0x91 bit 0
    unsigned char _pad91 : 7;
    unsigned char _pad92[0xb0 - 0x92];
    float MaxHealth;                            // +0xb0 (CThingPhysical)
    float Health;                               // +0xb4
    const C3DVector& GetPos() const;            // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out, long type)
    {
        if (HasTC(type)) {
            long key = type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)key);
            *out = (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    template <class T> __forceinline bool PeekTC(const T** out, long type) const
    {
        if (HasTC(type)) {
            long key = type;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)key);
            *out = (const T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public:
    bool IsPickPocketable() const;              // 0x00665E90
    ECreatureType GetCreatureType() const;      // 0x00662000
};
class CThingAICreature : public CThingCreatureBase { public:
    unsigned char _padb8[0x1c9 - 0xb8];
    bool AllowedToFollowHero : 1;               // +0x1c9
    bool RegionFollowingOverriddenFromScript : 1;
    bool RespondingToFollowAndWait : 1;
    bool CanBeCourted : 1;
    bool CanBeMarried : 1;
};
class CThingPlayerCreature : public CThingCreatureBase { };
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);     // 0x00436380 (folded body)
class CTCTargeting { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThingAICreature* GetPTargetedCreature() const;   // slot 33 (+0x84)
};
class CTCHeroStats { public:
    unsigned char _pad00[0x58];
    long Stamina;                               // +0x58
    long MaxStamina;                            // +0x5c
};
class CTCDoor { public:
    bool IsLockPickable() const;                // 0x00730AE0
};
class CTCStockItem { public:
    unsigned char _pad00[0x0c];
    bool ForSale;                               // +0x0c
    bool Stealable;                             // +0x0d
};
class CTCFollowed { public:
    bool IsCreatureValidFollower(const CThingAICreature& creature, bool* reason) const;   // 0x006D4E60
    bool IsFollower(const CThing* thing) const; // 0x006D4D70
};
class CTCCreatureOpinionOfHero { public:
    unsigned char _pad00[0x343];
    bool InFindAGuardAndYellForHelp;            // +0x343
};
class CTCGuard { public:
    bool PlayerIsCriminalAndCanApologise();     // 0x007B2BF0
};
class CTCWife { public:
    unsigned char _pad00[0x91];
    bool ReceivedWeddingRing;                   // +0x91
    bool IsCourtableAndNotBlockingIt() const;   // 0x006CF810
    bool IsInLoveWithHero() const;              // 0x006CF900
    bool HasReceivedWeddingRing() const { return ReceivedWeddingRing; }
};
class CTCDiggingSpot { public:
    unsigned char _pad00[0x0c];
    bool Enabled;                               // +0x0c
    bool Hidden;                                // +0x0d
};
class CTCCarrying { public:
    CThing* GetThingInPrimarySlot() const;      // 0x006A1440
};
class CTCTrophy { public:
    unsigned char _pad00[0x14];
    bool MinigameInProgress;                    // +0x14
};
class CTCInventory { public:
    unsigned char _pad00[0x1d8];
    std::vector<long> NumberOfCSOpinionExpressions;   // +0x1d8
    std::vector<long> LastCSOpinionExpression;        // +0x1e4
    std::vector<long> LastButOneCSOpinionExpression;  // +0x1f0
};
class CTCCarriedActionUseSpade { public:
    static bool __fastcall IsThingAbleToDig(const CThingCreatureBase& creature);     // 0x007F0300
};
class CTCCarriedActionUseFishingRod { public:
    static bool __fastcall IsThingAbleToFish(const CThingCreatureBase& creature);    // 0x007EFF80
};
class CDefPointeeBase { public:
    virtual void v000();
    virtual void DestroyDef() const;            // slot 1
    void Release() const
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
    mutable long m_refCount;                    // +4
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};
class COpinionDeedEffectsDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x7c - 8];
    bool CanBeHeard : 1;                        // +0x7c
    bool ReactionRestrictToTarget : 1;          // +0x7c bit 1
};
class CTCHeroOpinionDeedLog { public:
    CDefPointer<const COpinionDeedEffectsDef> PeekPDeedEffectsDef(EOpinionDeedType type) const;   // 0x006C5630
};
class CGiftDef;
class CTrophyDef;
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x006D3EF0 <CGiftDef> / 0x005DA300 <CTrophyDef>
};
class CGiftDef : public CDefClassBase { public:
    unsigned char _pad08[0x2c - 8];
    bool IsWeddingRing;                         // +0x2c
};
class CTrophyDef : public CDefClassBase { };
class CThingBaseDef : public CDefClassBase { };
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042AF3C
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
class CThingFilter_HasTC { public:
    CThingFilter_HasTC(ETCInterfaceType type) : Type(type) { }
private:
    ETCInterfaceType Type;
};
class CThingSearchTools { public:
    template <class T, class C, class F> void GetAllThingsInRoughArea(const C2DBoxI& box, const F& filter, C& out) const;   // 0x00851490
};
class CWorld { public:
    unsigned char _pad00[0x20];
    CThingSearchTools* PThingSearchTools;       // +0x20
};
class CMainGameComponent { public:
    unsigned char _pad00[0x24];
    CWorld* PWorld;                             // +0x24
    static CMainGameComponent* PMainGameComponent;   // 0x013B86A0
};

// Stand-in bodies (EXTRA_RULES chunk 422): the callees are out of line in retail, GetAllThingsInRoughArea 0x00851490 in this TU;
// same-TU noinline bodies that write only their out parameters, so VC7.1 knows the caller's def pointers and vector are not
// captured (the landed 0x008DDAD0 / 0x007A9FE0 models).
void* Find_StandIn(const void* owner, long index);          // stand-in callee
CThing* FindThing_StandIn(const C2DBoxI& box, long index);  // stand-in callee
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = Find_StandIn(this, 0);
    if (!def)
        return false;
    out->Object = (const T*)def;
    return true;
}
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    void* def = Find_StandIn(this, index);
    if (!def)
        return false;
    out->Object = (const T*)def;
    return true;
}
template <class T, class C, class F> __declspec(noinline) void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& box, const F& filter, C& out) const
{
    for (long index = 0; ; ++index) {
        CThing* thing = FindThing_StandIn(box, index);
        if (!thing)
            break;
        out.push_back(thing);
    }
}
#pragma auto_inline(on)

class CContextSensitiveItemDef { public:
    unsigned char _pad00[0x28];
    long Type;                                  // +0x28 EContextSensitiveType
    long Priority;                              // +0x2c
    long SlotIndex;                             // +0x30
    long ExtraData1;                            // +0x34
    long ExtraData2;                            // +0x38
    bool TargetCreatureLikelyToReactToDeed(CThingPlayerCreature* player, CThingAICreature* target,
                                           EOpinionDeedType deed) const;   // 0x008508B0
    bool IsValidOverride(const CThing& thing, long slot, const CTCInventory* inventory) const;
};

bool CContextSensitiveItemDef::IsValidOverride(const CThing& thing, long slot, const CTCInventory* inventory) const
{
    if (&thing == 0 || thing.Dead)
        return false;
    switch (Type) {
    case 2:
        return true;
    case 3:
        if (thing.MaxHealth - thing.Health >= ExtraData1 || ExtraData2 == 0 || ExtraData2 >= thing.Health)
            return true;
        break;
    case 4: {
        const CTCHeroStats* stats;
        if (thing.PeekTC(&stats, 4) && stats->MaxStamina - stats->Stamina >= ExtraData1)
            return true;
        break;
    }
    case 5: {
        const CTCTargeting* targeting;
        thing.PeekTC(&targeting, 8);
        if (targeting == 0)
            return false;
        CThingAICreature* creature = targeting->GetPTargetedCreature();
        if (creature == 0 || thing.HasTC(0x84))
            return false;
        switch (ExtraData1) {
        case 1:
            if ((creature->ThingTypeFlags & 8) && creature->IsPickPocketable())
                return true;
            return false;
        case 2: {
            const CTCDoor* door;
            if (creature->PeekTC(&door, 0x26) && door->IsLockPickable())
                return true;
            return false;
        }
        case 3: {
            const CTCStockItem* stock;
            if (creature->PeekTC(&stock, 0x54) && stock->Stealable)
                return true;
            return false;
        }
        case 4: {
            if (!(creature->ThingTypeFlags & 0x10) || !creature->RespondingToFollowAndWait)
                return false;
            const CTCFollowed* followed;
            thing.PeekTC(&followed, 0xbc);
            if (followed != 0 && followed->IsCreatureValidFollower(*creature, 0) && !followed->IsFollower(creature))
                return true;
            return false;
        }
        case 5: {
            if (!(creature->ThingTypeFlags & 0x10) || !creature->RespondingToFollowAndWait)
                return false;
            const CTCFollowed* followed;
            thing.PeekTC(&followed, 0xbc);
            if (followed != 0 && followed->IsFollower(creature))
                return true;
            return false;
        }
        case 6: {
            if (!(creature->ThingTypeFlags & 0x10))
                return false;
            const CTCCreatureOpinionOfHero* opinion;
            if (creature->PeekTC(&opinion, 0x4f) && opinion->InFindAGuardAndYellForHelp)
                return true;
            CTCGuard* guard;
            if (creature->GetTC(&guard, 0xb4) && guard->PlayerIsCriminalAndCanApologise())
                return true;
            return false;
        }
        }
        return false;
    }
    case 6: {
        const CTCTargeting* targeting;
        thing.PeekTC(&targeting, 8);
        if (targeting == 0)
            return false;
        CThingAICreature* creature = targeting->GetPTargetedCreature();
        if (creature != 0) {
            if (!(creature->ThingTypeFlags & 0x10) || !creature->HasTC(0x4f) || creature->HasTC(0xb4))
                return false;
            if (!TargetCreatureLikelyToReactToDeed((CThingPlayerCreature*)&thing, creature, (EOpinionDeedType)ExtraData2))
                return false;
            if (inventory != 0 && slot > 0) {
                long count = inventory->NumberOfCSOpinionExpressions[ExtraData1];
                if (count > 1) {
                    if (inventory->LastCSOpinionExpression[ExtraData1] == slot)
                        return false;
                    if (count > 2 && inventory->LastButOneCSOpinionExpression[ExtraData1] == slot)
                        return false;
                }
            }
            if (ExtraData1 == 4 || ExtraData1 == 5)
                return true;
            if (creature->GetCreatureType() != 1 && (ExtraData1 == 2 || ExtraData1 == 3))
                return true;
            if (!creature->CanBeCourted)
                return false;
            if (ExtraData1 != 0 && ExtraData1 != 1)
                return false;
            const CTCWife* wife;
            if (ExtraData1 == 1 && creature->PeekTC(&wife, 0xd7) && !wife->IsCourtableAndNotBlockingIt())
                return false;
            return true;
        }
        long deed = ExtraData2;
        if (deed <= 0 || deed >= 0x5c)
            return false;
        const CTCHeroOpinionDeedLog* deedLog;
        if (thing.PeekTC(&deedLog, 0x51) && deedLog != 0 &&
            !deedLog->PeekPDeedEffectsDef((EOpinionDeedType)deed)->ReactionRestrictToTarget)
            return true;
        return false;
    }
    case 7: {
        const CTCTargeting* targeting;
        if (!thing.PeekTC(&targeting, 8))
            return false;
        CThingAICreature* creature = targeting->GetPTargetedCreature();
        if (creature == 0 || !(creature->ThingTypeFlags & 0x10) || !creature->HasTC(0x4f) || creature->HasTC(0xb4))
            return false;
        if (!TargetCreatureLikelyToReactToDeed((CThingPlayerCreature*)&thing, creature, (EOpinionDeedType)ExtraData2))
            return false;
        if (creature->GetCreatureType() != 1 && (ExtraData1 == 2 || ExtraData1 == 0))
            return true;
        if (!creature->CanBeCourted || ExtraData1 != 1)
            return false;
        const CTCWife* wife;
        if (!creature->PeekTC(&wife, 0xd7) || !wife->IsCourtableAndNotBlockingIt())
            return false;
        CDefPointer<const CThingBaseDef> def;
        if (!CGameDefinitionManager::Get().GetDef(slot, &def))
            return false;
        CDefPointer<const CGiftDef> gift;
        if (def->GetSubDef(&gift) && gift->IsWeddingRing && (!wife->IsInLoveWithHero() || wife->HasReceivedWeddingRing()))
            return false;
        return true;
    }
    case 8: {
        C3DVector pos = thing.GetPos();
        switch (ExtraData1) {
        case 1:
            return CTCCarriedActionUseFishingRod::IsThingAbleToFish((const CThingCreatureBase&)thing);
        case 2: {
            long radius = GFFloatToLongCeil(3.0f);
            const C3DVector& centre = thing.GetPos();
            C2DBoxI box(C2DCoordI((long)centre.X, (long)centre.Y), radius);
            std::vector<CThing*> spots;
            CMainGameComponent::PMainGameComponent->PWorld->PThingSearchTools->GetAllThingsInRoughArea<CThing>(box,
                CThingFilter_HasTC((ETCInterfaceType)0xef), spots);
            for (std::vector<CThing*>::iterator it = spots.begin(); it != spots.end(); ++it) {
                const CThing* spotThing = *it;
                const CTCDiggingSpot* spot;
                if (spotThing->PeekTC(&spot, 0xef) && spot->Enabled && !spot->Hidden) {
                    C3DVector spotPos = spotThing->GetPos();
                    const C3DVector& myPos = thing.GetPos();
                    float dx = spotPos.X - myPos.X;
                    float dy = spotPos.Y - myPos.Y;
                    if (dx * dx + dy * dy < 9.0f && CTCCarriedActionUseSpade::IsThingAbleToDig((const CThingCreatureBase&)thing))
                        return true;
                }
            }
            return false;
        }
        }
        return false;
    }
    case 9: {
        if (slot <= 0)
            return false;
        CDefPointer<const CThingBaseDef> def;
        CGameDefinitionManager::Get().GetDef(slot, &def);
        CDefPointer<const CTrophyDef> trophy;
        if (!def->GetSubDef(&trophy))
            return false;
        const CTCCarrying* carrying;
        if (!thing.PeekTC(&carrying, 0x46))
            return false;
        CThing* carried = carrying->GetThingInPrimarySlot();
        if (!IS_THING_ALIVE(carried) || carried->DefGlobalIndex != slot)
            return false;
        const CTCTrophy* trophyTC;
        if (!carried->PeekTC(&trophyTC, 0x39))
            return false;
        return trophyTC->MinigameInProgress;
    }
    }
    return false;
}
