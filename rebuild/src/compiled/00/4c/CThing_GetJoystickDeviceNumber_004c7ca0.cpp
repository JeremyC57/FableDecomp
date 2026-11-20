// CThing::GetJoystickDeviceNumber @ 0x004c7ca0
// Leaf: ?GetJoystickDeviceNumber@CThing@@QBEJXZ
// NF171 (2026-11-20, q11922 / q11924): in-place const on the row's existing local CThing model: Ego_r.pdb (0x004C9930) + FableWin.pdb
//   (0x01CFA2E0) ?GetJoystickDeviceNumber@CThing@@QBEJXZ (const), the spelling engine/CThing.h declares and 6 of the 7 landed referencing rows
//   bind (the 7th, 0x00892DF0, is respelled in the same hand-back). Was QAEJXZ. The full port onto the CThing / CThingManager /
//   CPlayerManager / CPlayer headers stays at NF153's DIFFER(31v29) (see NF171 NOTES).
// Naming wave 18: the global stand-in g_inputHolder (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).

struct CInputProvider {
    long GetDeviceNumber();          // 0x487510 __fastcall -> long
};

class CInputManager { public:
    CInputProvider* GetProvider(int deviceIndex); // 0x449910 __fastcall(byte arg)
};

class CThingManager { public:
    char pad[0x30];
    CInputManager* mgr;   // +0x30
};


class CThing {
private:
    static CThingManager* PCurrentThingManager;      // 0x013B8A1C ?PCurrentThingManager@CThing@@0PAVCThingManager@@A (private static)
public:
    static CThingManager* __fastcall GetPCurrentThingManager() { return PCurrentThingManager; }   // Ego_r / FableWin ?GetPCurrentThingManager@CThing@@SIPAVCThingManager@@XZ (inline)
private: public:
    char pad[0x90];
    signed char joystickDeviceNumber; // +0x90
    long GetJoystickDeviceNumber() const;
};

long CThing::GetJoystickDeviceNumber() const
{
    return CThing::GetPCurrentThingManager()->mgr->GetProvider(this->joystickDeviceNumber)->GetDeviceNumber();
}