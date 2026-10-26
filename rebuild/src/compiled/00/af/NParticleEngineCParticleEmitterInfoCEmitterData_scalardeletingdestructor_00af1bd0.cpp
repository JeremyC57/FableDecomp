#pragma optimize("t", on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
extern void __fastcall DestructFirstMember_00ae94a0(void* member);
extern void __fastcall DestructSecondMember_009a4400(void* member);
void* __fastcall CEmitterData_CompositeScalarDeletingDestructor(void* emitter, int, unsigned int flags)
{
    DestructFirstMember_00ae94a0(static_cast<unsigned char*>(emitter) + 36);
    DestructSecondMember_009a4400(static_cast<unsigned char*>(emitter) + 28);
    ((CResource*)(emitter))->CResource::~CResource();
    if (flags & 1)
        operator delete(emitter);
    return emitter;
}