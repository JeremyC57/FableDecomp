// CInputProcessGameBase::IsInputEventToBeSentToThisInputProcess(const CInputEvent& event) const -- a joystick event (device 1) counts only when the player uses that joystick device number; every other device is accepted
// Leaf: ?IsInputEventToBeSentToThisInputProcess@CInputProcessGameBase@@MBE_NABVCInputEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687DB0, length 48 bytes. CInputProcessGameBase vftable 0x0125BE98 slot 8, and slot 8 of the
// 67 family tables that inherit it (NF477 audit: 68 `.rdata` dwords, no call or jump).
// Identity (NF478, r467b, NAME_FIXES_QUEUE nf477): Ego_r.pdb + FableWin.pdb ?IsInputEventToBeSentToThisInputProcess@CInputProcessGameBase@@MBE_NABVCInputEvent@@@Z
// (protected virtual const), the override of CAInputProcess slot 8 (0x00A0D400); declared by engine/CInputProcessGameBase.h FABLE_RETAIL_INPUTPROCESS_VTABLE.
// Was the made-up ?IsEventFromPlayerDevice@CInputProcessMain@@QAE_NABVCInputEvent@@@Z on a local class model (CInputProcessMain's own
// vftable 0x0123758C holds it at slot 8 only by inheritance). No body change.
// Callees: CInputEvent::GetDevice @ 0x00A03B50 (+0x20), CInputEvent::GetDeviceNumber @ 0x00A03B60 (+0x24; both labelled "CWorld::DrawGetEnvironment"),
// CPlayer::IsUsingJoystickDeviceNumber(long) @ 0x004874F0 (MyPlayer +0x18, engine/CInputProcessGameBase.h).
// Shape: the event reference in edi for both accessor calls, `cmp eax,1 ; jne` to the `mov al,1` tail, the bool of the player call
// passed through al, `ret 4`.
class CInputEvent { public:
    long GetDevice() const;                         // 0x00A03B50
    long GetDeviceNumber() const;                   // 0x00A03B60
};
class CPlayer { public:
    bool IsUsingJoystickDeviceNumber(long deviceNumber) const;   // 0x004874F0
};
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessGameBase.h"

bool CInputProcessGameBase::IsInputEventToBeSentToThisInputProcess(const CInputEvent& event) const
{
    if (event.GetDevice() == 1)
        return MyPlayer->IsUsingJoystickDeviceNumber(event.GetDeviceNumber());
    return true;
}
