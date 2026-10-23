#pragma optimize("s",on)
// CTCParticleAttachmentTracker::GetClassTypeName() const -- calls CTCParticleAttachmentTracker::StatGetClassTypeName @ 0x004D5B5E into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d5b5e(); };
extern "C" T* __stdcall CTCParticleAttachmentTracker_GetClassTypeName(T* result) { result->Init_004d5b5e(); return result; }