// CCreatureAction_ReleaseStrengthBerserk::FrameUpdate() -- the combat-action FrameUpdate, then when the creature has a CTCBerserk component read its four super-rumble values, clamp each to [0, 1] and rumble the creature's joystick (type 4) with them as a looping rumble
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E6500, length 395 bytes (+ int3 padding).
// Callees: CCombatActionBase::FrameUpdate @ 0x00859D80 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature +8),
// CVectorMap::LowerBound @ 0x0040F020 (inlined tested GetInterface 0x84 = CTCBerserk, bit 4 of +0x30), CTCBerserk::GetSuperRumble
// (float&, float&, float&, float&) @ 0x0078BE70, GFGetSystemManager @ 0x009A4EC0 (`__fastcall`, the input manager at +0x58),
// CThing::GetJoystickDeviceNumber @ 0x004C7CA0, the joystick device's virtual slot 17 (+0x44: Rumble(type, const params&, flag)).
// Globals: 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Shape: four float locals (the fourth shares the lookup key's frame word), each clamped `if (v < 0) v = 0; else if (v > 1) v = 1;`
// (`test ah,5 ; jp` / `test ah,41h ; jne`); the joystick entries are 8-byte {device, id} pairs at the input manager's +0x2c; the
// rumble parameters are a 20-byte {a, b, c, d, true} temporary built by an inline ctor in the argument area's frame slot.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];            // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                // +0x44
    long GetJoystickDeviceNumber();             // 0x004C7CA0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCBerserk {                             // interface type 0x84
    void GetSuperRumble(float& a, float& b, float& c, float& d);   // 0x0078BE70
};
struct CRumbleParams {                          // 20 bytes
    float Strength;
    float Duration;
    float Delay;
    float Falloff;
    bool  Looping;
    CRumbleParams(float strength, float duration, float delay, float falloff, bool looping)
        : Strength(strength), Duration(duration), Delay(delay), Falloff(falloff), Looping(looping) { }
};
struct CJoystickDevice {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void Rumble(long type, const CRumbleParams& params, long flag);   // slot 17 (+0x44)
};
struct CJoystickEntry {                         // 8 bytes
    CJoystickDevice* Device;
    long             Id;
};
struct CInputManager {
    unsigned char   _pad00[0x2c];
    CJoystickEntry* Joysticks;                  // +0x2c (vector begin)
    CJoystickDevice* GetJoystick(long n) const { return Joysticks[n].Device; }
};
struct CSystemManager {
    unsigned char  _pad00[0x58];
    CInputManager* InputManager;                // +0x58
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +4
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void FrameUpdate();                 // 0x00859D80
    unsigned char _pad04[4];                    // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0x114 - 0x10];         // +0x10
};
struct CCreatureAction_ReleaseStrengthBerserk : CCombatActionBase {
    virtual void FrameUpdate();
};

static __forceinline void Clamp01(float& v)
{
    if (v < 0.0f)
        v = 0.0f;
    else if (v > 1.0f)
        v = 1.0f;
}

void CCreatureAction_ReleaseStrengthBerserk::FrameUpdate()
{
    CCombatActionBase::FrameUpdate();
    CTCBerserk* berserk;
    if (PCreature.GetPItem()->GetInterface(&berserk, 0x84)) {
        float a, b, c, d;
        berserk->GetSuperRumble(a, b, c, d);
        Clamp01(a);
        Clamp01(b);
        Clamp01(c);
        Clamp01(d);
        CInputManager* input = GFGetSystemManager()->InputManager;
        input->GetJoystick(PCreature.GetPItem()->GetJoystickDeviceNumber())->Rumble(4, CRumbleParams(a, b, c, d, true), 0);
    }
}
