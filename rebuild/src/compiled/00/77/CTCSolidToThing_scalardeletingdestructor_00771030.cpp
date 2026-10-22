#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCSolidToThing_Destruct(void* thing);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCSolidToThing_CompositeScalarDeletingDestructor(void* thing, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(thing) + 12);
    CTCSolidToThing_Destruct(thing);
    if (flags & 1)
        OperatorDelete(thing);
    return thing;
}