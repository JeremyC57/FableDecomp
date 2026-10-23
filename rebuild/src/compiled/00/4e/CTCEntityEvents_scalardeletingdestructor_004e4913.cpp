#pragma optimize("s", on)
extern void __fastcall DestructMember_004e1bbb(void* member);
extern void __fastcall CTCEntityEvents_Destruct(void* events);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCEntityEvents_CompositeScalarDeletingDestructor(void* events, int, unsigned int flags)
{
    DestructMember_004e1bbb(static_cast<unsigned char*>(events) + 12);
    CTCEntityEvents_Destruct(events);
    if (flags & 1)
        OperatorDelete(events);
    return events;
}