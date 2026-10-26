#pragma optimize("t", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_004df88a(void* member);
void* __fastcall CTCSolidToThing_CompositeScalarDeletingDestructor(void* thing, int, unsigned int flags)
{
    DestructMember_004df88a(static_cast<unsigned char*>(thing) + 12);
    ((CTCBase*)(thing))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(thing);
    return thing;
}