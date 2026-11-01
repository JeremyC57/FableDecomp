// CThing::GetJoystickDeviceNumber @ 0x004c7ca0
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
    long GetJoystickDeviceNumber();
};

long CThing::GetJoystickDeviceNumber()
{
    return CThing::GetPCurrentThingManager()->mgr->GetProvider(this->joystickDeviceNumber)->GetDeviceNumber();
}