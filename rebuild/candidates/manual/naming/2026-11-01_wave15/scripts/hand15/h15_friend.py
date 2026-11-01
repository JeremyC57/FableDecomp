# family decisions (hand): DECISIONS_W14_RESOLVED item 2 -- CThing::GetWorld 0x004C7AA0 is protected in both PDBs
# (?GetWorld@CThing@@IAEAAVCWorld@@XZ) and CCreatureActionBase reaches it from its own protected members, so retail granted
# access: `friend class CCreatureActionBase;` in CThing (no byte effect). 0x00692940 is Ego_r ?GetWorld@CCreatureActionBase@@
# IAEAAVCWorld@@XZ and 0x00692C10 ?GetWorldMap@CCreatureActionBase@@IAEAAVCWorldMap@@XZ: the stand-in classes
# CCreatureActionWorldCalls / CCreatureActionBase_Methods / CThingWorldCalls are respelled to the PDB members. The engine
# CCreatureActionBase.h layout carries no member functions, so it is included under the accepted `#define ...Fields`
# pattern and the PDB class derives from it (no layout change).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import recode, edit
recode('00692940', 'src', '#include "engine/CCreatureActionBase.h"', '''#define CCreatureActionBase CCreatureActionBaseFields
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CWorld.h"
class CCreatureActionBase;
class CThing : public CThingFields {
    friend class CCreatureActionBase;   // Access: friend (PDB IAE; retail-consistent, not attested)
protected:
    CWorld& GetWorld();                 // 0x004C7AA0 reads global main component 013B8A1C+24 (Ego_r.pdb protected)
};
class CBaseObject;
struct CBaseObjectPointer;
struct CBaseIntelligentPointer {virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;CBaseObject* GetPItem()const;}; // native8-byte intelligent pointer, actual PDB fields; GetPItem 00A01B50
class CCreatureActionBase : public CCreatureActionBaseFields {
protected:
    CWorld& GetWorld();                 // 0x00692940 (Ego_r.pdb ?GetWorld@CCreatureActionBase@@IAEAAVCWorld@@XZ)
};
CWorld& CCreatureActionBase::GetWorld(){return ((CThing*)reinterpret_cast<const CBaseIntelligentPointer*>(PCreature)->GetPItem())->GetWorld();}''',
    leaf='?GetWorld@CCreatureActionBase@@IAEAAVCWorld@@XZ')
recode('00692c10', 'src', 'class CThing {', '''class CCreatureActionBase;
class CThing {
    friend class CCreatureActionBase;                    // Access: friend (PDB IAE; retail-consistent, not attested)
protected:
    CWorld& GetWorld();                                  // 0x004C7AA0 (out of line; Ego_r.pdb protected)
};
class CThingCreatureBase : public CThing { };
class CBaseObjectPointer;
class CBaseIntelligentPointer {                          // Ego_r.pdb: 8 bytes, PData +0x04
public:
    void*               _pad00;
    CBaseObjectPointer* PData;                           // +0x04
    CBaseObject* GetPItem() const;                       // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
};
class CCreatureActionBase : public CCreatureActionBaseFields {
protected:
    CWorldMap& GetWorldMap();                            // 0x00692C10 (Ego_r.pdb ?GetWorldMap@CCreatureActionBase@@IAEAAVCWorldMap@@XZ)
};
CWorldMap& CCreatureActionBase::GetWorldMap()
{
    return (*(CIntelligentPointer<CThingCreatureBase>*)PCreature)->GetWorld().GetWorldMap();
}''', leaf='?GetWorldMap@CCreatureActionBase@@IAEAAVCWorldMap@@XZ')
edit('00692c10', 'src', '#include "engine/CCreatureActionBase.h"\n', '#define CCreatureActionBase CCreatureActionBaseFields\n#include "engine/CCreatureActionBase.h"\n#undef CCreatureActionBase\n')
