#pragma optimize("t", on)
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();   // 0x00693EF0
};
extern void __fastcall DestructMember_0099b510(void* member);
void* __fastcall CCreatureAction_Read_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructMember_0099b510(static_cast<unsigned char*>(action) + 168);
    ((CCreatureActionBase*)(action))->CCreatureActionBase::~CCreatureActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}