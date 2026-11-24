// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CTCInventory::IsSaveMenuReceivingInput() -- scans each inventory screen's COMPONENT_SAVE entries for input states 1 or 2
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058376C, 169 bytes. The two-state query and list lifetime are kept inline to reproduce both exits.
// Callees: STLport list<entry*> default ctor 0x0042ABAF and dtor 0x0042ABCA (manifest "CActiveFile::OnReadFinished" is a
//   propagated twin), CCharString(const char*, long) 0x0099EBF0 for the by-value name argument, the screen's virtual
//   slot 5 (+0x14) entry search, and the entry's second-base virtual slot 2 (+0x08) state query.
// Local model: the CTCInventory screen vector sits at +0x0c (retail); no engine/ header declares these members.
// Leaf: ?IsSaveMenuReceivingInput@CTCInventory@@QBE_NXZ
#pragma optimize("y", off)
#pragma optimize("s", on)
// NF156 (2026-11-20, q4776): the local CEntryList / CListAllocator / CMenuEntry / CStateQuery / CComponentBase / CMenuScreen /
//   CCharString models replaced by the shared headers: the list is STLport std::list<NUISystem::CComponent*> (ctor 0x0042ABAF /
//   ~list 0x0042ABCA out of line, the 0x00583815 define set), the screen search is NUISystem::CComponent::FindAllChildren (slot 5,
//   engine/NUISystem_CComponent.h), and the entry's second-base slot 2 is CObserver::IsObservingEvent (the CObserver sub-object at
//   +4; the stand-in called it IsInState(1) / (2)). Was ??1CEntryList@@QAE@XZ / ??0CEntryList@@QAE@ABU?$CListAllocator@PAUCMenuEntry@@@@@Z.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
// NF241 (2026-11-22, the list<CComponent*> family pass): the list calls bind _List_base<CComponent*>'s constructor 0x0042ABAF and
//   destructor 0x0042ABCA (was FABLE_RETAIL_STLP_LIST_DEFAULT_CTOR_NOINLINE + _DTOR_OUTOFLINE, binding list's own names there; retail
//   0x0042B6A6 / 0x0042B6D2 are list's size-optimised wrappers, a `call 0x0042ABAF` and a 5-byte `jmp 0x0042ABCA`). Same bytes.
#define FABLE_RETAIL_STLP_LIST_BASE_VISIBLE_NOINLINE
#include "stlport/fable_containers.h"
#include "engine/NUISystem_CComponent.h"
using NUISystem::CComponent;
struct CScreenVector {
    CComponent** first; CComponent** last;
    unsigned long size() const { return (unsigned long)(last - first); }
    CComponent*& operator[](unsigned long n) const { return first[n]; }
};
class CTCInventory { public:
    unsigned char _pad[0x0c];
    CScreenVector Screens;
    bool IsSaveMenuReceivingInput() const;
};
bool CTCInventory::IsSaveMenuReceivingInput() const
{
    unsigned long i = 0;
    for (; i < Screens.size(); ++i) {
        std::list<CComponent*> entries;
        Screens[i]->FindAllChildren(CCharString("COMPONENT_SAVE", -1), entries);
        for (std::list<CComponent*>::iterator it = entries.begin(); it != entries.end(); ++it) {
            CComponent* entry = *it;
            if (entry) {
                NUISystem::CObserver* query = entry;                 // second base, +0x04
                if (query->IsObservingEvent(NUISystem::UI_EVENT_DOWN) || query->IsObservingEvent(NUISystem::UI_EVENT_LEFT)) {
                    return true;
                }
            }
        }
    }
    return false;
}