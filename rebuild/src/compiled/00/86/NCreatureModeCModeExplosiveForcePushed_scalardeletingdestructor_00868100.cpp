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
extern void __fastcall DestructSecondMember(void* member);
extern void __fastcall CModeExplosiveForcePushed_Destruct(void* mode);
void* __fastcall CModeExplosiveForcePushed_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(mode) + 40))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    DestructSecondMember(static_cast<unsigned char*>(mode) + 28);
    CModeExplosiveForcePushed_Destruct(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}