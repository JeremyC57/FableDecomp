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
namespace NCameraModes {
struct CBase {                                         // RTTI base of every camera mode
    virtual ~CBase();                                  // 0x0056E0E0 (Ego_r ??1CBase@NCameraModes@@UAE@XZ)
};
}
void* __fastcall CZTargetNew_CompositeScalarDeletingDestructor(void* targetNew, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(targetNew) + 164))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    ((NCameraModes::CBase*)targetNew)->NCameraModes::CBase::~CBase();   // the base destructor (namepass CAM2)
    if (flags & 1)
        operator delete(targetNew);
    return targetNew;
}