#pragma optimize("t", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_00783ff0(void* member);
void* __fastcall CTCBoastingCrowdControl_CompositeScalarDeletingDestructor(void* control, int, unsigned int flags)
{
    DestructMember_00783ff0(static_cast<unsigned char*>(control) + 16);
    ((CTCBase*)(control))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(control);
    return control;
}