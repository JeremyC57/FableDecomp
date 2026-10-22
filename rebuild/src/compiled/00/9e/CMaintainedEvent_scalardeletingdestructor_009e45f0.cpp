#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CMaintainedEvent_Destruct(void* event);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CMaintainedEvent_CompositeScalarDeletingDestructor(void* event, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(event) + 4);
    CMaintainedEvent_Destruct(event);
    if (flags & 1)
        OperatorDelete(event);
    return event;
}