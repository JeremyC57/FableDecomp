// CGameJoystickManager::CGameJoystickManager() -- CBase root, then the main-player joystick device number (+4) and its "set" flag (+8) zeroed, then the vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049BB60, length 26 bytes.
// Callees: CBase::CBase @ 0x0099A2F0. Vtable 0x01238B54 (CGameJoystickManager, rtti-vtables.tsv).
// Shape: the two zero stores sit BEFORE the vtable store, so they are plain initialiser-list members (member objects with
// inline constructors would be scheduled after it). Member names from engine/CGameJoystickManager.h (the generated header is a
// POD snapshot, so the polymorphic class is modelled locally with the same offsets).
// Leaf: ??0CGameJoystickManager
struct CBase {
    virtual ~CBase();
    CBase();                                        // 0x0099A2F0
};

class CGameJoystickManager : public CBase { public:
    long m_MainPlayerJoystickDeviceNumber;          // +0x04
    bool m_MainPlayerJoystickDeviceNumberSet;       // +0x08
    CGameJoystickManager();
};

CGameJoystickManager::CGameJoystickManager()
    : m_MainPlayerJoystickDeviceNumber(0), m_MainPlayerJoystickDeviceNumberSet(false)
{
}