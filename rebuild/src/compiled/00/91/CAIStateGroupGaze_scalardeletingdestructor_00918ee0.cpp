#pragma optimize("t", on)
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};
void* __fastcall CAIStateGroup_Gaze_CompositeScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(stateGroup) + 20))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    ((CCharString*)(static_cast<unsigned char*>(stateGroup) + 12))->~CCharString();
    ((CBaseClass*)(stateGroup))->CBaseClass::~CBaseClass();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}