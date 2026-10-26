#include <stdio.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif
#include <string.h>
// Behaviour test for NCreatureMode::CModeStrafe::FrameUpdate (0x008EA220).
#include <math.h>

class CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};

class CThing { public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];       // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;           // +0x44
    long pad;
    long GetJoystickDeviceNumber() { return 1; }
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
    float f;
    float GetChargeFraction() const { return f; }
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
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {} virtual void v007() {} virtual void v008() {} virtual void v009() {}
    virtual void v010() {} virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {} virtual void v016() {}
    int calls; long t; float st, du; long fl;
    virtual void Rumble(long type, const CRumbleParams& p, long flag) { ++calls; t = type; st = p.Strength; du = p.Duration; fl = flag; }
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

CInputManager* g_pInputManager;

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00() {}
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
};

namespace NCreatureMode {

struct CModeStrafe : CModeBase {           // vtable 0x01268588
    unsigned char _members[0x28];          // AnimMovementDistance .. AimingCenterFrame (+0x1c..+0x43)
    float         RumbleParam;             // +0x44
    float         PullBackAmount;          // +0x48
    int updates;
    void UpdateWeaponState() { ++updates; }
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
        RumbleParam = (float)pow((double)fraction, 3.0);
        g_pInputManager->GetJoystick(ParentThing->GetJoystickDeviceNumber())->Rumble(2, CRumbleParams(RumbleParam), 0);
    }
}

}

using namespace NCreatureMode;
int main()
{
    int ok = 1;
    CThing owner; memset(&owner, 0, sizeof(owner));
    CTCProjectileCharge charge; charge.f = 0.5f;
    CInterfaceMap::SEntry e[1] = { { 0x81, (CTCBase*)&charge } }; owner.m_interfaces.first = e; owner.m_interfaces.last = e + 1;
    CJoystickDevice dev0, dev1; dev0.calls = 0; dev1.calls = 0;
    CJoystickEntry joys[2] = { { &dev0, 0 }, { &dev1, 1 } };
    CInputManager im; im.Joysticks = joys; g_pInputManager = &im;
    CModeStrafe m; m.ParentThing = &owner; m.updates = 0; m.RumbleParam = 9; m.PullBackAmount = 9;
    m.FrameUpdate();
    if (m.updates != 1 || dev1.calls != 0 || m.RumbleParam != 9) ok = 0;
    owner.m_presentBits[0x81 >> 5] |= 1UL << (0x81 & 31);
    m.FrameUpdate();
    if (dev1.calls != 1 || dev0.calls != 0 || m.PullBackAmount != 0.5f || m.RumbleParam != 0.125f || dev1.t != 2 || dev1.st != 0.125f || dev1.du != 0.5f || dev1.fl != 0) ok = 0;
    charge.f = 3.0f; m.FrameUpdate();
    if (m.PullBackAmount != 1.0f || m.RumbleParam != 1.0f) ok = 0;
    charge.f = -2.0f; m.FrameUpdate();
    if (m.PullBackAmount != 0.0f || m.RumbleParam != 0.0f || dev1.calls != 3) ok = 0;
    if (ok) { printf("008ea220_TEST PASS\n"); return 0; }
    printf("008ea220_TEST FAIL\n"); return 1;
}