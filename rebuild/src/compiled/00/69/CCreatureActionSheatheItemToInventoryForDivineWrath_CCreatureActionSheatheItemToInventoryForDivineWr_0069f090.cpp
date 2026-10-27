// CCreatureAction_SheatheItemToInventory_ForDivineWrath::CCreatureAction_SheatheItemToInventory_ForDivineWrath(CThingCreatureBase& creature, const CCharString& itemName, CCreatureActionBase* parentAction, long priority) -- construct the CCreatureAction_SheatheItemToInventory base with the same four arguments, then install this class's vtable
// Leaf: ??0CCreatureAction_SheatheItemToInventory_ForDivineWrath@@QAE@AAVCThingCreatureBase@@ABVCCharString@@PAVCCreatureActionBase@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069f090, 42 bytes, thiscall, returns this, `ret 10h`.
// Identity (proven): it stores vtable 0x0125CA7C (RTTI .?AVCCreatureAction_SheatheItemToInventory_ForDivineWrath@@, direct base CCreatureAction_SheatheItemToInventory) after calling the base constructor 0x0069EF80 with its four arguments; Ego_r.pdb ??0CCreatureAction_SheatheItemToInventory_ForDivineWrath@@QAE@AAVCThingCreatureBase@@ABVCCharString@@PAVCCreatureActionBase@@J@Z; caller CTCSpecialAbilities::SetupDivineWrath 0x0072CD00.
// Callee: CCreatureAction_SheatheItemToInventory::CCreatureAction_SheatheItemToInventory 0x0069EF80.
// Shape (family L037): the four stack arguments re-pushed in reverse, base ctor call, `mov [esi], vtable`.
class CThingCreatureBase;
class CCharString;
class CCreatureActionBase;
class CCreatureAction_SheatheItemToInventory {                                              // (members not touched here)
public:
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase& creature, const CCharString& itemName, CCreatureActionBase* parentAction, long priority);   // 0x0069EF80
    virtual ~CCreatureAction_SheatheItemToInventory();
};
class CCreatureAction_SheatheItemToInventory_ForDivineWrath : public CCreatureAction_SheatheItemToInventory {
public:
    CCreatureAction_SheatheItemToInventory_ForDivineWrath(CThingCreatureBase& creature, const CCharString& itemName, CCreatureActionBase* parentAction, long priority);
};
CCreatureAction_SheatheItemToInventory_ForDivineWrath::CCreatureAction_SheatheItemToInventory_ForDivineWrath(CThingCreatureBase& creature, const CCharString& itemName, CCreatureActionBase* parentAction, long priority)
    : CCreatureAction_SheatheItemToInventory(creature, itemName, parentAction, priority)
{
}