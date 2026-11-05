// CThingPlayerCreature::UpdatePlayerControl() -- updates the thing's scripted-control component (interface 0x1f) when it is flagged or its script task is not running, then performs the controlled move
// Leaf: ?UpdatePlayerControl@CThingPlayerCreature@@AAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe 0x006AD970, 88 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the inlined interface lookup;
// type 0x1f = bit 31 of the bitset dword at +0x20, `test eax,eax ; jns`), the component's script-task
// probe 0x007122C0 (labelled CScriptTask_IsRunning), CTCScriptedControl::Update 0x00713910,
// CThingPlayerCreature::ControlledMove 0x006AD700.
// Shape: the landed out-parameter GetInterface template (0x008386E0 model, the key temporary in the
// `push ecx` slot); the component flag byte at +0x18 short-circuits the probe; the controlled move
// always runs (`this` kept in edi).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCScriptedControl { public:                              // interface type 0x1f component
    unsigned char _pad00[0x18];
    bool          m_forceUpdate;                         // +0x18
    bool IsScriptTaskRunning();                          // 0x007122C0
    void Update();                                       // 0x00713910
};

class CThingPlayerCreature : public CThing { protected: void ControlledMove(); private:                               // 0x006AD700
    void UpdatePlayerControl();
};

void CThingPlayerCreature::UpdatePlayerControl()
{
    CTCScriptedControl* control;
    if (GetTC(TCI_SCRIPTED_CONTROL, &control))
    {
        if (control->m_forceUpdate || !control->IsScriptTaskRunning())
            control->Update();
    }
    ControlledMove();
}