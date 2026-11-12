#ifndef FABLE_ENGINE_CFACTIONMANAGER_H
#define FABLE_ENGINE_CFACTIONMANAGER_H
// Original PDB complete member set; native CVectorMap16-byte layout.
#include "engine/CVectorMap.h"
#include "engine/CCountedPointer.h"
class CFaction;
class CFactionManager {
public:
#ifdef FABLE_RETAIL_CFACTIONMANAGER_CTOR
    // Opt-in (chunk 2051, row 0x004A6E30 CWorld::Init): Ego_r.pdb ??0CFactionManager@@QAE@ABVCWorld@@@Z, retail 0x006B97E0. Declaration only (implicit destructor:
    // retail expands it in place, 0x004A70D8).
    CFactionManager(const class CWorld& world);                    // 0x006B97E0
#endif
    CFaction* GetPFaction(const CCharString& name) const;
private:
    typedef CVectorMap<unsigned long,CCountedPointer<CFaction>,CKeyPairCompareLess<unsigned long,CCountedPointer<CFaction> > > TFactionMap;
    TFactionMap Factions;
};
FABLE_STATIC_ASSERT(sizeof(CFactionManager)==16);
#endif
