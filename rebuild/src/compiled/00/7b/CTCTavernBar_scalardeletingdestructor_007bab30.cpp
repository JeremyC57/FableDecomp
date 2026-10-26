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
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructSecondMember_007b9830(void* member);
void* __fastcall CTCTavernBar_CompositeScalarDeletingDestructor(void* bar, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(bar) + 24))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    DestructSecondMember_007b9830(static_cast<unsigned char*>(bar) + 12);
    ((CTCBase*)(bar))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(bar);
    return bar;
}