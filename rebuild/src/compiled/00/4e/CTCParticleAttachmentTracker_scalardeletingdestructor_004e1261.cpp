#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCParticleAttachmentTracker_Destruct(void* tracker);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCParticleAttachmentTracker_CompositeScalarDeletingDestructor(void* tracker, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(tracker) + 12);
    CTCParticleAttachmentTracker_Destruct(tracker);
    if (flags & 1)
        OperatorDelete(tracker);
    return tracker;
}