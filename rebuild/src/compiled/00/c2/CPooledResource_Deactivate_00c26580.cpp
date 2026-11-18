// CPooledResource::Deactivate(CSoundEventManager* manager) -- mark the pooled sound resource inactive (state 2) and remove it from the
//   manager's event set (CSoundEventSet m_events +0x2C, PKCTRList<CPooledResource> virtual slot 8 Remove), tail-called
// Leaf: ?Deactivate@CPooledResource@@QAEXPAUCSoundEventManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C26580, thiscall `ret 4`: `mov word ptr [ecx+74h],2`, then the event set's slot-8 call with `this`.
// Identity: DESCRIPTIVE (libpk has no PDB): the counterpart of the header's inline Activate (state 1, m_events.Add, slot 9; its twin at
//   0x00C26560); libpk/CPooledResource.h FABLE_RETAIL_CPOOLEDRESOURCE_DEACTIVATE (NF63) declares it; vtable 0x012B3FDC is CPooledResource's.
// NF70 (round 7, land1117fu q10729): was catalogued NScript::CIsThingAlive::operator() (a propagated script label) over local Inner /
//   CIsThingAlive stand-ins with a forced member-pointer emission; now the descriptive libpk member on the shared headers.
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_STREAMED_PASS23
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_EVENT_POSITION_PASS23
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_CHECK_EXISTING_PASS23
#define FABLE_RETAIL_CPOOLEDRESOURCE_DEACTIVATE
#include "libpk/CPooledResource.h"
#include "libpk/CSoundEventManager.h"

void CPooledResource::Deactivate(CSoundEventManager* manager)
{
    m_state = 2;
    manager->m_events.Remove(this);
}
