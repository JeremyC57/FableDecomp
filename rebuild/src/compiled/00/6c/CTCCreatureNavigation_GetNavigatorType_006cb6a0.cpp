// CTCCreatureNavigation::GetNavigatorType() const -- the class type id of the current navigator
// Leaf: ?GetNavigatorType@CTCCreatureNavigation@@QBEKXZ
// Retail: Fable.exe @ 0x006CB6A0, 8 bytes: `mov eax,[ecx+0Ch] ; movzx eax,byte ptr [eax+10h] ; ret`.
// Identity (proven): Ego_r.pdb names ?GetNavigatorType@CTCCreatureNavigation@@QBEKXZ and ?GetType@CClass@@QBEKXZ (CClass::Type,
// a byte at +0x10; CNavigatorBase derives from CClass per the Fable.exe RTTI). Callers (0x00665A9F, 0x0091277B, 0x00913F75,
// 0x00914422, 0x00914694) take interface 7 from the thing's interface map, call this, compare the result with a navigator
// type global (0x0129CB44) and then call the landed CTCCreatureNavigation::SetNavigatorToType 0x006CBA80 with the same
// global. The manifest label "CIsHitEventBlocked::operator()" is propagated.
// Layout: CTCCreatureNavigation::PNavigator +0x0C (Ego_r, as the landed SetNavigatorToType); engine/CClass.h (Type +0x10).
#define CClass DonorCClass
#include "engine/CClass.h"
#undef CClass
class CClass : public DonorCClass { public:          // engine/CClass.h: Type +0x10
    unsigned long GetType() const { return Type; }
};
class CNavigatorBase : public CClass { public:
};
class CThing;
class CTCBase { public:                             // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
};
class CTCCreatureNavigation : public CTCBase { public:
    CNavigatorBase* PNavigator;                     // +0x0C
    unsigned long GetNavigatorType() const;
};

unsigned long CTCCreatureNavigation::GetNavigatorType() const
{
    return PNavigator->GetType();
}