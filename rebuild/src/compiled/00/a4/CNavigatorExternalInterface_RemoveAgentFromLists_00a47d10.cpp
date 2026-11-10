// CNavigatorExternalInterface::RemoveAgentFromLists(const CSteeringAgent* agent) -- erase every occurrence of the steering agent from each registered agent vector
// Leaf: ?RemoveAgentFromLists@CNavigatorExternalInterface@@SIXPBVCSteeringAgent@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A47D10, length 97 bytes (+15 int3 pad), static `__fastcall` (agent in ecx, plain `ret`).
// Name: Ego_r.pdb `?RemoveAgentFromLists@CNavigatorExternalInterface@@SIXPBVCSteeringAgent@@@Z` (name set; FableWin.pdb has the
//   same class and its RemoveAgentListFromLists sibling, landed at 0x00A47D80).
// Global: CNavigatorExternalInterface::AgentLists 0x013D2834 (Ego_r.pdb `dynamic initializer for 'CNavigatorExternalInterface::AgentLists'`),
//   an STLport list<vector<const CSteeringAgent*>*>; opt-in FABLE_RETAIL_CNAVIGATOREXTERNALINTERFACE_AGENTLISTS declares it.
// Callees: memmove 0x00BFEAE6 (the inlined STLport vector::erase's trivial copy).
#define FABLE_RETAIL_CNAVIGATOREXTERNALINTERFACE_AGENTLISTS
#include "engine/CNavigatorExternalInterface.h"

void FABLE_FASTCALL CNavigatorExternalInterface::RemoveAgentFromLists(const CSteeringAgent* agent)
{
    for (std::list<std::vector<const CSteeringAgent*>*>::iterator list = AgentLists.begin(); list != AgentLists.end(); ++list) {
        for (std::vector<const CSteeringAgent*>::iterator it = (*list)->begin(); it != (*list)->end(); ++it) {
            if (*it == agent) {
                // Step back before erasing, so the loop's ++it lands on the element that slid into the hole. Erasing a named
                // copy of the old position keeps VC7.1 from strength-reducing `position + 1` into a second induction register:
                // retail copies the position (`mov edx,esi ; lea ecx,[edx+4] ; sub esi,4`, 0x00A47D37) and keeps agent in ebp.
                std::vector<const CSteeringAgent*>::iterator position = it;
                --it;
                (*list)->erase(position);
            }
        }
    }
}