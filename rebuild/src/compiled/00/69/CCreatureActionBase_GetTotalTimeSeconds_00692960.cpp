#include "engine/CCreatureActionBase.h"

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
struct CreaturePointerStorage;
struct TimeConverter { float Convert(int ticks); };

struct CCreatureActionBaseVirtuals {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6();
    virtual int GetTotalTimeTicks() const;
};

float __fastcall CCreatureActionBase_GetTotalTimeSeconds(const CCreatureActionBase* action) {
    TimeConverter* converter = ((TimeConverter*)(((CBaseIntelligentPointer*)(reinterpret_cast<CreaturePointerStorage*>(const_cast<unsigned char*>(action->PCreature))))->GetPItem()));
    const CCreatureActionBaseVirtuals* virtuals =
        reinterpret_cast<const CCreatureActionBaseVirtuals*>(action);
    return converter->Convert(virtuals->GetTotalTimeTicks());
}
