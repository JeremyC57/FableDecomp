// CThingPhysicalSwitch::`vector_deleting_destructor' @ 0x00529ca0
// Standard MSVC deleting destructor:
//   push esi; mov esi,ecx; call <dtor>; test [esp+8],1; je +; mov ecx,esi; call <delete>; mov eax,esi; ret 4
// Naming (NF460, queue nf453): the callee 0x0083D6D0 is CThingPhysicalSwitch::~CThingPhysicalSwitch, ??1CThingPhysicalSwitch@@UAE@XZ (Ego_r +
//   FableWin; that row's Leaf), called non-virtually (was the free stand-in CThingPhysicalSwitch_dtor). Same bytes (NF456 b1u recipe).

class CThingPhysicalSwitch { public:
    virtual ~CThingPhysicalSwitch();                // 0x0083D6D0
};

// Base/class destructor forwarded to (direct call, this in ecx).
// Fable operator-delete wrapper: pointer passed in ecx (thiscall-style).
void __fastcall CThingPhysicalSwitch_del(void *p);

void * __fastcall CThingPhysicalSwitch_scalar_deleting_destructor(
    CThingPhysicalSwitch *physicalSwitch, int /*edx*/, unsigned int flags)
{
    physicalSwitch->CThingPhysicalSwitch::~CThingPhysicalSwitch();
    if (flags & 1)
        CThingPhysicalSwitch_del(physicalSwitch);
    return physicalSwitch;
}