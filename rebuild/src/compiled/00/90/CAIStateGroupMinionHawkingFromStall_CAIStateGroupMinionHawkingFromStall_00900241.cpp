// CAIStateGroup_MinionHawkingFromStall::CAIStateGroup_MinionHawkingFromStall() -- base CAIStateGroupBase(), NextHawkFrame = 0, own vtable, the Customer intelligent pointer built inline
// Leaf: ??0CAIStateGroup_MinionHawkingFromStall@@QAE@XZ
// Naming wave 32 (family aisg): the base is engine/CAIStateGroupBase.h's retail branch through the RTTI base CAIStateGroup_WorkBase<CAIStateGroupBase> (was ??0CAIStateGroupBase_Calls@@QAE@XZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00900241, length 40 bytes.
// Callees: CAIStateGroupBase::CAIStateGroupBase @ 0x008FD255, CBaseIntelligentPointer::
// CBaseIntelligentPointer @ 0x00A01B10 (the member's base). Vtables: 0x0128BF48 =
// CAIStateGroup_MinionHawkingFromStall, 0x0123E7D8 = CIntelligentPointer (rtti-vtables.tsv). The
// manifest's "~CCombatAbilityBlockDefBase" label is propagated; this is a constructor.
// Layout = Ego_r.pdb: CAIStateGroupBase is 0x14 bytes (engine/CAIStateGroupBase.h's retail branch; RTTI base
// CAIStateGroup_WorkBase<CAIStateGroupBase>), NextHawkFrame +0x14, Customer (CIntelligentPointer<CThingCreatureBase>) +0x18;
// size 0x20. `and dword ptr [esi+14h],0` marks the size-optimised TU; the long's zero store is
// scheduled above the derived vtable store, the member's constructor (out-of-line base call, then
// its vtable) below it.
#pragma optimize("s", on)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255
template <class T> class CAIStateGroup_WorkBase : public T { };     // RTTI CAIStateGroup_WorkBase<CAIStateGroupBase> (no ctor of its own)

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

class CThingCreatureBase;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {      // vtable 0x0123E7D8
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CAIStateGroup_MinionHawkingFromStall : public CAIStateGroup_WorkBase<CAIStateGroupBase> {   // own vtable 0x0128BF48
public:
    long                NextHawkFrame;                      // +0x14
    CIntelligentPointer<CThingCreatureBase> Customer;                           // +0x18
    CAIStateGroup_MinionHawkingFromStall();
};

CAIStateGroup_MinionHawkingFromStall::CAIStateGroup_MinionHawkingFromStall()
    : NextHawkFrame(0)
    , Customer()
{
}