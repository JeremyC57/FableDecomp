#pragma optimize("t", on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
extern void __fastcall DestructFirstMember_009a4400(void* member);
extern void __fastcall DestructSecondMember_006e46f0(void* member);
void* __fastcall CData_CompositeScalarDeletingDestructor(void* data, int, unsigned int flags)
{
    DestructFirstMember_009a4400(static_cast<unsigned char*>(data) + 36);
    DestructSecondMember_006e46f0(static_cast<unsigned char*>(data) + 28);
    ((CResource*)(data))->CResource::~CResource();
    if (flags & 1)
        operator delete(data);
    return data;
}