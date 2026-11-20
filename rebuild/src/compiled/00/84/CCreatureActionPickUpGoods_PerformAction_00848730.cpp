// CCreatureAction_PickUpGoods::PerformAction() -- when the goods thing is alive: put it in the creature's carry slot named by its carryable def and push a new CModeCarryingGoodsStand(creature) mode
// Leaf: ?PerformAction@CCreatureAction_PickUpGoods@@QAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00848730, 275 bytes (288 with padding), virtual PerformAction.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PGoods +0xa8, PCreature +0x08), CVectorMap::LowerBound 0x0040F020
//   (inlined GetInterface: 0x46 CTCCarrying, bit 6 of +0x28; 0x31 CTCCreatureModeManager, bit 17 of +0x24),
//   CDefClassBase::GetSubDef<CCarryableDef> 0x005CC10E, CTCCarrying::PutThingInCarrySlot 0x006A1F80, operator new
//   0x00BFEA1A (0x74 bytes), NCreatureMode::CModeCarryingGoodsStand(CThing*) 0x0073FCE0 (landed), CCountedPointer<CModeBase>
//   (CModeBase*) 0x00718A00 (built in the argument area), CTCCreatureModeManager::AddMode 0x0073ACC0.
// Layout: retail CCreatureActionBase is 0xa8 bytes: the goods pointer is this class's first member (+0xa8); CThing +0x70
//   the def, +0x91 dead bit; CCarryableDef +0x28 the carry slot.
// Shape: the GetPlateFromCupboard 0x00848500 model: both component lookups unchecked (the miss path reads the out
//   variable's uninitialised slot), the carryable def a CDefPointer local (DecRefToZero release, null test folded), the
//   new mode built through the owner pointer's inline `operator CThing*()`; the GetSubDef and LowerBound same-TU stand-ins
//   carry retail's LTCG knowledge (def held in ebx, key and def share one frame word).
// Naming (NF144, 2026-11-20; q6408): the local empty `struct CModeBase { };` and the local CTCCreatureModeManager replaced by the shared
//   engine/CTCCreatureModeManager.h (ADDMODEPOINTER, nested CModeBase); 0x0073ACC0 now binds the Ego_r.pdb
//   ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z (was ...QAE_NV?$CCountedPointer@UCModeBase@@@@@Z) and
//   0x00718A00 the FableWin.pdb ??0?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@QAE@PAVCModeBase@CTCCreatureModeManager@@@Z (was
//   ??0?$CCountedPointer@UCModeBase@@@@QAE@PAUCModeBase@@@Z). The header-less mode class keeps its token and its constructor spelling, now on
//   the shared CModeBase (the landed 0x0096DBC0 shape), its pad resized so sizeof stays the retail allocation.
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#include "fable_compiler.h"
class CThing;
#include "engine/C3DVector.h"
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCARRYABLEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE   // agentNF25b: the GetSubDef<CCarryableDef> template body (0x005CC10E) kept out of line, visible to the TU
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CCarryableDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/ETCInterfaceType.h"                        // the shared enum (CTCCreatureModeManager.h needs TCI_ENTITY_MODE_MANAGER)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad54[0x1c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x90 - 0x74];
    signed char m_player;                             // +0x90
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CBaseObject* GetPItem() const;                         // 0x00A01B50
    operator CThing*() const { return ((CThing*)GetPItem()); }
};
class CTCCarrying { public:
    void PutThingInCarrySlot(CThing& thing, long slot, bool immediately);         // 0x006A1F80 (Ego_r.pdb + FableWin.pdb: CThing&)
};
#include "engine/CTCCreatureModeManager.h"
#include "engine/CCountedPointer.h"
struct CModeCarryingGoodsStand : CTCCreatureModeManager::CModeBase {
    unsigned char _pad[0x74 - 0x1c];
    CModeCarryingGoodsStand(CThing* owner);           // 0x0073FCE0
    virtual unsigned long GetType() const;
};
class CCreatureActionBase { public:                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};
class CCreatureAction_PickUpGoods : public CCreatureActionBase { public:
    CBaseIntelligentPointer PGoods;                   // +0xa8
    void PerformAction();
};

void CCreatureAction_PickUpGoods::PerformAction()
{
    CThing* goods = ((CThing*)PGoods.GetPItem());
    if (goods && !(goods->m_flags91 & 1)) {
        CTCCarrying* carrying;
        ((CThing*)PCreature.GetPItem())->GetInterface(&carrying, 0x46);
        CDefPointer<const CCarryableDef> carryDef;
        goods->m_pDef->GetSubDef(&carryDef);
        carrying->PutThingInCarrySlot(*goods, carryDef->ActiveCarrySlot, true);
        CTCCreatureModeManager* modes;
        ((CThing*)PCreature.GetPItem())->GetInterface(&modes, 0x31);
        modes->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(new CModeCarryingGoodsStand(PCreature)));
    }
}