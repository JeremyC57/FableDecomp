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
extern void __fastcall CTCCreatureItemLevitationNymph_Destruct(void* nymph);
void* __fastcall CTCCreatureItemLevitationNymph_CompositeScalarDeletingDestructor(void* nymph, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(nymph) + 40))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    CTCCreatureItemLevitationNymph_Destruct(nymph);
    if (flags & 1)
        operator delete(nymph);
    return nymph;
}