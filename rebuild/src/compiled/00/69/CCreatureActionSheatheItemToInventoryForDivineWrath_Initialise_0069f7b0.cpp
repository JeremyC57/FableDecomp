// CCreatureAction_SheatheItemToInventory_ForDivineWrath::Initialise() -- attach the spell component after sheathing.
// Leaf: ?Initialise@CCreatureAction_SheatheItemToInventory_ForDivineWrath@@UAEXXZ
// Identity: proven: vtable RTTI class/PDB Initialise; own literal0x0123c280="CTCDivineWrath"; base call0x69F480; PCreature+8; AddTC0x4C9D60.
// Layout: engine/CCreatureActionBase.h has a data-member vptr and no virtual Initialise/typed PCreature; the local polymorphic base preserves the retail +8 member.
// Layout: engine/CCreatureAction_SheatheItemToInventory_ForDivineWrath.h flattens the polymorphic base and omits typed PCreature; retail lea this+8 targets CBaseIntelligentPointer::GetPItem. The model preserves that real base/member chain.
#define CCreatureAction_SheatheItemToInventory_ForDivineWrath CCreatureAction_SheatheItemToInventory_ForDivineWrath_HeaderLayout
#include "engine/CCreatureAction_SheatheItemToInventory_ForDivineWrath.h"
#undef CCreatureAction_SheatheItemToInventory_ForDivineWrath
class CCharStringData;
class CCharString { public: CCharStringData* PStringData; CCharString(const char*, long = -1); ~CCharString(); };
class CTCBase;
class CTCParamsBase;
class CThing { public: CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*); };
class CThingCreatureBase : public CThing { };
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer(); CBaseObject* GetPItem() const; CBaseObjectPointer* PData; };
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); } };
class CCreatureActionBase { public:
virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
virtual void Initialise();
unsigned char _pad04[4]; CIntelligentPointer<CThingCreatureBase> PCreature;
};
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase { public: virtual void Initialise(); };
class CCreatureAction_SheatheItemToInventory_ForDivineWrath : public CCreatureAction_SheatheItemToInventory { public: virtual void Initialise(); };
void CCreatureAction_SheatheItemToInventory_ForDivineWrath::Initialise()
{
    CCreatureAction_SheatheItemToInventory::Initialise();
    PCreature.GetPItem()->AddTC("CTCDivineWrath", true, 0);
}