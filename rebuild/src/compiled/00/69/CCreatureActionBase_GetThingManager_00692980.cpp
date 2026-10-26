#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
struct Sub; 
class CThingManager; extern CThingManager* g_thingmgr;
CThingManager* __fastcall GetThingManager(CCreatureActionBase* action)
{
    ((CBaseIntelligentPointer*)(reinterpret_cast<Sub*>(reinterpret_cast<char*>(action) + 8)))->GetPItem();
    return g_thingmgr;
}