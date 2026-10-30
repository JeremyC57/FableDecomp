// CBaseIntelligentPointer::SetPItem(CBaseObject*) -- point at an object: share its record (count up) or create one and hang it on the object
// Leaf: ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01B90.
// Identity: Ego_r.pdb + FableWin.pdb ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z.
// Callees: operator new / delete (the record), the inlined ClearData 0x00A01B20.
// CBaseObject (vtable 0x0129A7D8): vptr + CBaseObjectPointer* IntelligentPointer (+0x04), Ego_r.pdb.
class CBaseObject;
class CBaseObjectPointer {                             // Ego_r.pdb, 8 bytes
public:
    CBaseObject*  object;                              // +0x00 the observed object, 0 once it died
    unsigned long refcount;                            // +0x04 intelligent pointers sharing this record
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CBaseIntelligentPointer {                        // vtable 0x0129C95C
public:
    virtual ~CBaseIntelligentPointer();                // 0x00A01C10
    void SetPItem(CBaseObject* item);                  // 0x00A01B90
private:
    void ClearData() const                             // 0x00A01B20 (private const in Ego_r), inlined here
    {
        if (PData != 0) {
            PData->refcount--;
            if (PData->refcount == 0 && PData->object == 0)
                delete PData;
            PData = 0;
        }
    }
    mutable CBaseObjectPointer* PData;                 // +0x04
};

void CBaseIntelligentPointer::SetPItem(CBaseObject* item)
{
    if (PData != 0 && PData->object == item && item != 0)
        return;
    ClearData();
    if (item != 0) {
        CBaseObjectPointer* data = item->IntelligentPointer;
        if (data != 0) {
            PData = data;
            data->refcount++;
            return;
        }
        PData = new CBaseObjectPointer;
        PData->object = item;
        PData->refcount = 1;
        item->IntelligentPointer = PData;
    }
}
