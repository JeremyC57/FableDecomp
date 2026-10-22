#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCCreatureHitNotification_Destruct(void* notification);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCreatureHitNotification_CompositeScalarDeletingDestructor(void* notification, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(notification) + 12);
    CTCCreatureHitNotification_Destruct(notification);
    if (flags & 1)
        OperatorDelete(notification);
    return notification;
}