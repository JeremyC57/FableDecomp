#pragma optimize("s", on)
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
void* __fastcall CIntelligentPointer_ScalarDeletingDestructor(void* pointer, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(pointer))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    if (flags & 1)
        operator delete(pointer);
    return pointer;
}