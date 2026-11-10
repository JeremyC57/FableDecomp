#ifndef FABLE_ENGINE_CFACTIONMANAGER_H
#define FABLE_ENGINE_CFACTIONMANAGER_H
// Original PDB complete member set; native CVectorMap16-byte layout.
#include "engine/CVectorMap.h"
#include "engine/CCountedPointer.h"
class CFaction;
class CFactionManager {
public:
    CFaction* GetPFaction(const CCharString& name) const;
private:
    typedef CVectorMap<unsigned long,CCountedPointer<CFaction>,CKeyPairCompareLess<unsigned long,CCountedPointer<CFaction> > > TFactionMap;
    TFactionMap Factions;
};
FABLE_STATIC_ASSERT(sizeof(CFactionManager)==16);
#endif
