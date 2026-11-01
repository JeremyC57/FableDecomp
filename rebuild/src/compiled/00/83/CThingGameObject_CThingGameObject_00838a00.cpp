// CThingGameObject::~CThingGameObject() -- take the object off the thing manager's game-object list, destroy PThingStandingOn, then run ~CThingPhysical
// Leaf: ??1CThingGameObject@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00838A00, 40 bytes, thiscall, tail `jmp` to the base destructor.
// Identity (proven): the body stores vtable 0x01244F7C = .?AVCThingGameObject@@ (RTTI COL base array CThingGameObject -> CThingPhysical
//   -> CThing -> CClass -> CBaseObject -> CBaseClass) and chains to 0x006A5D50, which stores the CThingPhysical vtable 0x01244E84 (so it is
//   ~CThingPhysical); its callers are the ??_G of CThingGameObject (0x005298C0, vtable 0x01244F7C slot 0) and the derived destructors
//   (12 sites). Name: Ego_r.pdb ??1CThingGameObject@@UAE@XZ (landed callers already bind this spelling at 0x00529BA0).
// Callees: CThingManager::RemoveFromGameObjectList(CThingGameObject&) 0x0051DFF0 (an empty `ret 4` in the release build: Ego_r donor
//   order AddToPhysicalList, AddToGameObjectList, AddToRenderList, RemoveFromPhysicalList, RemoveFromGameObjectList, RemoveFromRenderList
//   = retail 0x0051DF60, 0x0051DF70, AddToRenderList 0x0051DF80, 0x0051DFE0 (called only by ~CThingPhysical), 0x0051DFF0 (called only
//   here), RemoveFromRenderList 0x0051E000), CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (PThingStandingOn +0xC0),
//   CThingPhysical::~CThingPhysical 0x006A5D50. Global: CThing::PCurrentThingManager 0x013B8A1C (private static, read through the
//   protected inline accessor CThing::GetThingManager()).
// Layout: engine/CThingGameObject.h (retail STLport sizes): PThingStandingOn +0xC0, the AddToComboMultiplierOnHit.. bitfield byte +0xC8,
//   sizeof 0xD0; engine/CThingPhysical.h gives sizeof CThingPhysical 0xC0. Both headers flatten the vptr, so the polymorphic chain is a
//   local model sized from them (the CThingDeadCreature 0x00835FD0 include pattern).
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define CThingPhysical CThingPhysicalFields
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObjectFields
#include "engine/CThingGameObject.h"
#undef CThingGameObject
class CThingGameObject;
class CThingManager {
public:
    void RemoveFromGameObjectList(CThingGameObject& thing);      // 0x0051DFF0
};
class CCharString;
class CClass : public CBaseObject {
public:
    virtual void Save(CCharString&);
};
class CThing : public CClass {
protected:
    CThingManager& GetThingManager() { return *PCurrentThingManager; }   // Ego_r / FableWin ?GetThingManager@CThing@@IAEAAVCThingManager@@XZ (inline)
private:
    static CThingManager* PCurrentThingManager;                  // 0x013B8A1C ?PCurrentThingManager@CThing@@0PAVCThingManager@@A
};
typedef char CThingPhysicalExtent[sizeof(CThingPhysicalFields) == 0xC0 ? 1 : -1];
class CThingPhysical : public CThing {
public:
    virtual ~CThingPhysical();                                   // 0x006A5D50
private:
    unsigned char _pad_members[sizeof(CThingPhysicalFields) - sizeof(CClass)];
};
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                                   // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                          // 0x00A01C10
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CThingGameObject : public CThingPhysical {
public:
    virtual ~CThingGameObject();
    CIntelligentPointer<CThing> PThingStandingOn;                // +0xC0
    unsigned char AddToComboMultiplierOnHit : 1;                 // +0xC8
    unsigned char AddToComboMultiplierOnHitOverrideSet : 1;
    unsigned char GiveHeroStatChangesOnBeingHit : 1;
    unsigned char GiveHeroStatChangesOnBeingHitOverrideSet : 1;
};

CThingGameObject::~CThingGameObject()
{
    GetThingManager().RemoveFromGameObjectList(*this);
}