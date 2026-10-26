// NCreatureMode::CModeStrafe::FrameUpdate() -- refresh the weapon state, then for a creature with a projectile-charge component clamp the charge fraction to [0, 1] as PullBackAmount, cube it into RumbleParam and rumble the creature's joystick with it
// Grade: RELOCATION_MATCH -- the joystick device is reached through an inline CInputManager::GetJoystick(n) accessor: the device then loads straight into ecx for the virtual Rumble call and the parameter strength is stored before `push 0`, like retail
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EA220, 226 bytes (`ret`; vtable 0x01268588 slot 3).
// Callees: CModeStrafe::UpdateWeaponState 0x008E9950; CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x004365B0 (the const
// instantiation: inlined const GetInterface, type 0x81 = bit 1 of +0x30); the component's charge-fraction getter 0x00789A70
// (float in st0; manifest "CMap::DrawGetSeaLevel" is propagated); the pow intrinsic _CIpow 0x00BFEB40; CThing::GetJoystickDeviceNumber
// 0x004C7CA0; the joystick device's virtual slot 17 (+0x44: rumble(type, const params&, flag)).
// Globals: the input manager 0x013B8388 (joystick entries {device, ..} 8 bytes each, begin pointer at +0x2c); float constants
// 0.0f 0x0122DEDC and 1.0f 0x0122DED8, the double 3.0 0x012368F0.
// Layout (PDB CModeStrafe 0x70, matches retail): ParentThing +0x04, RumbleParam +0x44, PullBackAmount +0x48.
// Shape: the fraction is a float local clamped with `if (v < 0) v = 0; else if (v > 1) v = 1;` (`test ah,5 ; jp` /
// `test ah,41h ; jne`); the rumble parameters are a 20-byte temporary {strength, 0.5, 0, 0.4, false} built by an inline ctor
// from the re-read RumbleParam.
extern "C" double __cdecl pow(double x, double y);
#pragma intrinsic(pow)

class CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;  // 0x004365B0 (the const instantiation)
};

class CThing { public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];       // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;           // +0x44
    long GetJoystickDeviceNumber();        // 0x004C7CA0
    template<class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCProjectileCharge {               // interface type 0x81
    float GetChargeFraction() const;       // 0x00789A70
};

struct CRumbleParams {                     // 20 bytes
    float Strength;
    float Duration;
    float Delay;
    float Falloff;
    bool  Looping;
    CRumbleParams(float strength) : Strength(strength), Duration(0.5f), Delay(0.0f), Falloff(0.4f), Looping(false) { }
};

struct CJoystickDevice {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void Rumble(long type, const CRumbleParams& params, long flag);   // slot 17 (+0x44)
};

struct CJoystickEntry {                    // 8 bytes
    CJoystickDevice* Device;
    long             Id;
};

class CInputManager { public:
    unsigned char   _pad00[0x2c];
    CJoystickEntry* Joysticks;             // +0x2c (vector begin)
    CJoystickDevice* GetJoystick(long n) const { return Joysticks[n].Device; }
};

extern CInputManager* g_pInputManager;     // 0x013B8388

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
};

namespace NCreatureMode {

struct CModeStrafe : CModeBase {           // vtable 0x01268588
    unsigned char _members[0x28];          // AnimMovementDistance .. AimingCenterFrame (+0x1c..+0x43)
    float         RumbleParam;             // +0x44
    float         PullBackAmount;          // +0x48
    void UpdateWeaponState();              // 0x008E9950
    void FrameUpdate();
};

void CModeStrafe::FrameUpdate()
{
    UpdateWeaponState();
    const CTCProjectileCharge* charge;
    if (ParentThing->GetInterface(&charge, 0x81)) {
        float fraction = charge->GetChargeFraction();
        if (fraction < 0.0f)
            fraction = 0.0f;
        else if (fraction > 1.0f)
            fraction = 1.0f;
        PullBackAmount = fraction;
        RumbleParam = (float)pow(fraction, 3.0);
        g_pInputManager->GetJoystick(ParentThing->GetJoystickDeviceNumber())->Rumble(2, CRumbleParams(RumbleParam), 0);
    }
}

}