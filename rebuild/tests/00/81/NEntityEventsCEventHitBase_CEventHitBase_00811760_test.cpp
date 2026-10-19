#include <stdio.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#endif
#include <string.h>
// Behaviour test for NEntityEvents::CEventHitBase::CEventHitBase(const CHitParameters&) (0x00811760).
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];       // +0x20
    struct CInterfaceMap {
        struct SEntry { long type; void* component; };
        SEntry* first;
        SEntry* last;
        SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
    } m_interfaces;                       // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_dead : 1;             // +0x91 bit 0
    unsigned char m_91_1 : 7;
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

struct CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* Info;
    CBaseObject* GetPItem() const { return (CBaseObject*)Info; }
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct CTCWeaponHolder_77 {                         // TC 0x77
    CThing* weapon;
    CThing* GetPWeapon() { return weapon; }
};

class CHitParameters {
public:
    unsigned char _pad00[0x58];
    CIntelligentPointer<CThing> PThingHitting;      // +0x58
    bool attr4; CThing* setWeapon;
    CHitParameters() : attr4(false), setWeapon(0) { PThingHitting.Info = 0; }
    CHitParameters(const CHitParameters& o) : attr4(o.attr4), setWeapon(0) { PThingHitting.Info = o.PThingHitting.Info; }
    bool HasDamageAttribute4() const { return attr4; }
    void SetWeapon(CThing* w) { setWeapon = w; }
};

template<class T> struct CCountedPointer {
    T* Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Reset(T* data) { Data = data; Info = (void*)1; }
    T* operator->() const { return Data; }
};

namespace NEntityEvents {
class __declspec(novtable) CEventBase {
public:
    virtual void v00() {}
    long WorldFrame;                                // +0x04
    CEventBase() : WorldFrame(0) {}
};
class CEventHitBase : public CEventBase {
public:
    CCountedPointer<CHitParameters> PHitParams;     // +0x08
    virtual void v00() {}
    CEventHitBase(const CHitParameters& params);
};
}

NEntityEvents::CEventHitBase::CEventHitBase(const CHitParameters& params)
{
    PHitParams.Reset(new CHitParameters(params));
    do {
        if (!PHitParams->HasDamageAttribute4())
            break;
        CThing* hitter = PHitParams->PThingHitting.GetPItem();
        if (!hitter || hitter->m_dead || !hitter->HasTC(0x77))
            break;
        CTCWeaponHolder_77* holder = hitter->GetTC<CTCWeaponHolder_77>(0x77);
        CThing* weapon = holder->GetPWeapon();
        if (weapon && !weapon->m_dead)
            PHitParams->SetWeapon(holder->GetPWeapon());
    } while (0);
}

using namespace NEntityEvents;
int main()
{
    int ok = 1;
    CThing hitter, weapon; memset(&hitter, 0, sizeof(hitter)); memset(&weapon, 0, sizeof(weapon));
    CTCWeaponHolder_77 holder; holder.weapon = &weapon;
    CThing::CInterfaceMap::SEntry e[1] = { { 0x77, &holder } }; hitter.m_interfaces.first = e; hitter.m_interfaces.last = e + 1;
    CHitParameters p; p.PThingHitting.Info = &hitter;
    { CEventHitBase ev(p); if (ev.WorldFrame != 0 || ev.PHitParams.Data == 0 || ev.PHitParams.Data->setWeapon != 0 || ev.PHitParams.Data->PThingHitting.Info != &hitter) ok = 0; }
    p.attr4 = true;
    { CEventHitBase ev(p); if (ev.PHitParams.Data->setWeapon != 0) ok = 0; }          // no TC 0x77
    hitter.m_presentBits[0x77 >> 5] |= 1UL << (0x77 & 31);
    { CEventHitBase ev(p); if (ev.PHitParams.Data->setWeapon != &weapon) ok = 0; }
    weapon.m_dead = 1;
    { CEventHitBase ev(p); if (ev.PHitParams.Data->setWeapon != 0) ok = 0; }
    weapon.m_dead = 0; hitter.m_dead = 1;
    { CEventHitBase ev(p); if (ev.PHitParams.Data->setWeapon != 0) ok = 0; }
    hitter.m_dead = 0; holder.weapon = 0;
    { CEventHitBase ev(p); if (ev.PHitParams.Data->setWeapon != 0) ok = 0; }
    if (ok) { printf("00811760_TEST PASS\n"); return 0; }
    printf("00811760_TEST FAIL\n"); return 1;
}