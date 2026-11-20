#ifndef FABLE_ENGINE_CVSYNCCALLBACKTIMER_H
#define FABLE_ENGINE_CVSYNCCALLBACKTIMER_H
// New header (hand3c2, row 0x009BF7E0 CDisplayManager::CreateDevice): CVsyncCallbackTimer, the object CDisplayManager owns through
// boost::scoped_ptr (VsyncCallbackTimer +0x294). Ego_d.pdb LF_CLASS 0xB3F7 / field list 0x60C01: private `Device` (IDirect3DDevice9*)
// at offset 0, then FreeQueoryObjects / PendingQueories (std::list<CPendingQuery>), PendingFrame, FinishedFrame; public
// ?SetDevice@CVsyncCallbackTimer@@QAEXPAUIDirect3DDevice9@@@Z, one 21-byte source line in Ego_d (the member store), inline in retail
// (0x009BFA0B `mov edx,[ebp+294h]` ... 0x009BFA23 `mov [edx],eax`). Only the first member is modelled: the lists are VC10 containers
// in Ego_d (16 bytes each there) and no retail row reads past offset 0 yet, so the size is not asserted and the class must not be
// constructed or embedded from this header.
struct IDirect3DDevice9;
class CVsyncCallbackTimer {
public:
    void SetDevice(IDirect3DDevice9* device) { Device = device; }
private:
    IDirect3DDevice9* Device;                                            // +0x00
};
#endif
