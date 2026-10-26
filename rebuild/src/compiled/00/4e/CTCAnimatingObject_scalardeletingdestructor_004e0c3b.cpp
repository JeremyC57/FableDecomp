#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_004db68f(void* member);
void* __fastcall CTCAnimatingObject_CompositeScalarDeletingDestructor(void* animatingObject, int, unsigned int flags)
{
    DestructMember_004db68f(static_cast<unsigned char*>(animatingObject) + 20);
    ((CTCBase*)(animatingObject))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(animatingObject);
    return animatingObject;
}