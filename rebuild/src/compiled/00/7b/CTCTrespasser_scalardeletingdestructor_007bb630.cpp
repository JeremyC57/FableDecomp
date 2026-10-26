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
extern void __fastcall DestructFirstMember_005142e0(void* member);
void* __fastcall CTCTrespasser_CompositeScalarDeletingDestructor(void* trespasser, int, unsigned int flags)
{
    DestructFirstMember_005142e0(static_cast<unsigned char*>(trespasser) + 56);
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(trespasser) + 45))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    ((CTCBase*)(trespasser))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(trespasser);
    return trespasser;
}