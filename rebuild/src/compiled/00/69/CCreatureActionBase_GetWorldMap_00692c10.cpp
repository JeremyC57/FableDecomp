// CCreatureActionBase::GetWorldMap() -- the world map of the world the acting creature lives in
// Leaf: ?GetWorldMap@CCreatureActionBase@@IAEAAVCWorldMap@@XZ
// Retail: Fable.exe @ 0x00692C10, 22 bytes; Ego_r.pdb ?GetWorldMap@CCreatureActionBase@@IAEAAVCWorldMap@@XZ.
// PCreature (+0x08, CIntelligentPointer<CThingCreatureBase>) -> CBaseIntelligentPointer::GetPItem 0x00A01B50, then the
// thing's out-of-line world getter 0x004C7AA0 (`[g_pThingManager 0x013B8A1C]+0x24`, CThing::GetWorld; manifest name
// CViewBase::GetDisplayEngine is ICF-propagated), then CWorld::GetWorldMap, virtual slot 12 (+0x30), as a tail jump.
#define CCreatureActionBase CCreatureActionBaseFields
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
class CBaseObject;
class CWorldMap;
class CWorld {
public:
    virtual void _s00(); virtual void _s01(); virtual void _s02(); virtual void _s03();
    virtual void _s04(); virtual void _s05(); virtual void _s06(); virtual void _s07();
    virtual void _s08(); virtual void _s09(); virtual void _s10(); virtual void _s11();
    virtual CWorldMap& GetWorldMap();                    // slot 12 (+0x30)
};
class CCreatureActionBase;
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
}