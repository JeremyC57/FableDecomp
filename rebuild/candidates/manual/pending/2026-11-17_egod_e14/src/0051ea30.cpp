// CThingManager::FillRenderListsWithAllEntities() -- mark every thing of each registered thing type (1..15) as rendered this frame (flags 0xF), so the render lists pick them all up
// Leaf: ?FillRenderListsWithAllEntities@CThingManager@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051EA30, 88 bytes, __thiscall, plain `ret`; only caller CThingManager::SetUpdateAndRenderAllFlag
//   0x0051EA90 (stores UpdateAndRenderAllFlag +0x99, then calls this row). Was labelled _gapscan::sub_0051ea30.
// Identity (proven, two sources): Ego_d.pdb thing_manager.obj lists CThingManager::FillRenderListsWithAllEntities directly before
//   SetUpdateAndRenderAllFlag (retail 0x0051EA90, the next function); Ego_r.pdb names ?FillRenderListsWithAllEntities@CThingManager@@AAEXXZ
//   (0x00528AA0) directly before ?SetUpdateAndRenderAllFlag@CThingManager@@QAEX_N@Z (0x00528B10); FableWin.pdb has it too (0x01D32810).
// Locals (Ego_d): long c1 { const CCharString& name { std::list<CClass*>& list { iterator it { CThing* pthing } } } }, declared here in
//   those scopes.
// Fidelity: `name` (CClassFactory::GetTypeName; Ego_d type const CCharString&, the only inline CClassFactory accessor returning one) is
//   listed by Ego_d but unused in retail (FableWin's /Od body is 108 bytes longer than this TU's: debug-only use). It emits no instruction
//   of its own but is load-bearing: without it VC7.1 keeps ClassTypeInfo's start in a register across the Used test (DIFFER 86v88);
//   with it the start is re-read after the test as retail does (`mov eax,[ecx+4]` at 0x0051EA54).
// Callees: CThing::SetAsRenderedThisFrame(unsigned long) 0x004C7590 (reloc_bindings; `push 0Fh`, ecx = the node's thing). The CClassFactory
//   accessors IsClassTypeRegistered / GetTypeName / GetTypeList(unsigned long) and CThingManager::GetTypeList(unsigned char) (both PDBs) are
//   inline in retail (overlay opt-ins FABLE_RETAIL_CCLASSFACTORY_TYPEACCESSORS / FABLE_RETAIL_CTHINGMANAGER_FILLRENDERLISTS).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCLASSFACTORY_TYPEACCESSORS
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_FILLRENDERLISTS
#define FABLE_RETAIL_CTHING
#include "stlport/fable_containers.h"
#include "engine/CThingManager.h"
#include "engine/CThing.h"

void CThingManager::FillRenderListsWithAllEntities()
{
    for (long c1 = 1; c1 < 16; ++c1)
    {
        if (IsClassTypeRegistered(c1))
        {
            const CCharString& name = GetTypeName(c1);
            {
                std::list<CClass*>& list = GetTypeList(c1);
                for (std::list<CClass*>::iterator it = list.begin(); it != list.end(); ++it)
                {
                    CThing* pthing = static_cast<CThing*>(*it);
                    pthing->SetAsRenderedThisFrame(0xF);
                }
            }
        }
    }
}
