// CThingPhysicalSwitch::`vector_deleting_destructor' @ 0x00529ca0
// Standard MSVC deleting destructor:
//   push esi; mov esi,ecx; call <dtor>; test [esp+8],1; je +; mov ecx,esi; call <delete>; mov eax,esi; ret 4

class CThingPhysicalSwitch { public:
    void *vfptr;
};

// Base/class destructor forwarded to (direct call, this in ecx).
void __fastcall CThingPhysicalSwitch_dtor(CThingPhysicalSwitch *physicalSwitch);
// Fable operator-delete wrapper: pointer passed in ecx (thiscall-style).
void __fastcall CThingPhysicalSwitch_del(void *p);

void * __fastcall CThingPhysicalSwitch_scalar_deleting_destructor(
    CThingPhysicalSwitch *physicalSwitch, int /*edx*/, unsigned int flags)
{
    CThingPhysicalSwitch_dtor(physicalSwitch);
    if (flags & 1)
        CThingPhysicalSwitch_del(physicalSwitch);
    return physicalSwitch;
}