// CThingObject::`vector deleting destructor'
// Retail 0x00704280. MSVC VC7.1 compiler-emitted vector-deleting-destructor thunk.

struct CThingObject
{
    void Destroy();   // scalar destructor body at 0x7042a0 (thiscall, this in ecx)
    void Free();      // custom deallocator at 0x4c73b0 (thiscall, this in ecx)
};

void* __fastcall CThingObject_scalar_deleting_destructor(
    CThingObject* thingObject, int edx, unsigned int flags)
{
    thingObject->Destroy();
    if (flags & 1)
        thingObject->Free();
    return thingObject;
}