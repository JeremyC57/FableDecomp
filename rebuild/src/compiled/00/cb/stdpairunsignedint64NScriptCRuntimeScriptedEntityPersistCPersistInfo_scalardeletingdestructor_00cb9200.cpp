// std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>::`scalar deleting destructor'(unsigned int) -- destroy the persist-script string of a scripted-entity persist record, free the pair when asked
// Leaf: ??_G?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@QAEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB9200, 33 bytes (no vptr store: non-polymorphic ??_G).
// Identity: FableWin.pdb ??_G?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@QAEPAXI@Z; Ego_r layout
//   (UID uint64, PersistScript CCharString at pair +0x10); neighbour pair ctor 0x00CB90F0.
// Shape: `delete entry;` on the non-polymorphic pair emits the scalar deleting destructor.
#include "stlport/fable_containers.h"
#include "fable_string.h"

namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo {
    public:
        unsigned __int64 UID;                           // +0x00
        CCharString      PersistScript;                 // +0x08
    };
};
}

typedef std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> CPersistInfoEntry;
void DeletePersistInfoEntry(CPersistInfoEntry* entry)
{
    delete entry;
}