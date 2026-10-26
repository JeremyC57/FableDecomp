#pragma optimize("t", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructFirstMember_007099e0(void* member);
extern void __fastcall DestructSecondMember_0070a7f0(void* member);
void* __fastcall CTCTextureDecal_CompositeScalarDeletingDestructor(void* decal, int, unsigned int flags)
{
    DestructFirstMember_007099e0(static_cast<unsigned char*>(decal) + 16);
    DestructSecondMember_0070a7f0(static_cast<unsigned char*>(decal) + 12);
    ((CTCBase*)(decal))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(decal);
    return decal;
}