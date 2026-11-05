// CTCCombat::SetBlocked(CThing* blocker) -- when the blocking thing is a creature-type thing with interface 48, that component's +0x50 frame counter is set to two seconds of frames; then the parent creature is deregistered from the combat manager
// Leaf: ?SetBlocked@CTCCombat_Methods@@QAEXPAVCThing@@@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00734050, length 96 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, __ftol2 @ 0x00BFEA70,
// CCombatManager::DeregisterCombatant(CThingCreatureBase*) @ 0x006EA430 (landed). Global: the frame rate
// long @ 0x01375550 (15).
// Layout = engine/CTCCombat.h: CombatManager +0x20, ParentCreature +0x24. CThing: the thing-type byte
// +0x10, the interface presence bitset +0x20 (type 48 -> word 1 bit 16: `test dword ptr [eax+24h],10000h`),
// the sorted interface map +0x44.
// Shape: `type == 2 && GetInterface(48, &component)` with the landed out-parameter GetInterface
// template (0x007504E0): the key temporary is homed in the consumed argument slot (`lea eax,[esp+0Ch]`),
// the map pointer is a named local (esi); `(long)(CMainGameComponent::GetConstantFPS() * 2.0f)` is `fild ; fadd st0,st0 ;
// __ftol2`; the deregistration is unconditional.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCBase { public:
    unsigned char _pad[0x50];
    long          m_frameCounter;           // +0x50
};

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

#include "engine/CTCCombat.h"

class CThingCreatureBase;
class CCombatManager { public:
    void DeregisterCombatant_006ea430(CThingCreatureBase* creature);   // 0x006EA430
};

struct CTCCombat_Methods : CTCCombat {
    void SetBlocked(CThing* blocker);
};

void CTCCombat_Methods::SetBlocked(CThing* blocker)
{
    CTCBase* component;
    if (blocker->GetType() == 2 && blocker->GetTC(TCI_COMBAT, &component))
        component->m_frameCounter = (long)(CMainGameComponent::GetConstantFPS() * 2.0f);
    CombatManager->DeregisterCombatant_006ea430(ParentCreature);
}