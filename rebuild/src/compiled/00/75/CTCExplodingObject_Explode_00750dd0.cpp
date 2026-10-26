// CTCExplodingObject::Explode() -- spawn the def's explosion thing at the parent's position, hand its driver component the radius / damage values, then kill the parent thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00750DD0, length 79 bytes.
// Callees: CThing::GetPosition() @ 0x004C73D0 (`&[this+60h]->+0Ch`), CreateThing(long defIndex, const C3DVector&)
// @ 0x0080C460 (`__fastcall`: index in ecx, position in edx), CThing::Kill(bool) @ 0x004C9B80.
// Layout: PDB CTCExplodingObject over CTCBase: MaxDamage +0xc, Radius +0x10, FireDamage +0x14, PDef +0x24 (the def's
// explosion type at +0x2c). The spawned thing's driver component is cached at +0x5c behind bit 1 of the flag byte
// at +0x28 (the landed GetTCDriver 0x007512C0); its record takes Radius at +0xc and +0x10, MaxDamage +0x14, FireDamage +0x18.
// Shape: the def pointer is read into a local (edi) BEFORE the position call; the out-parameter GetTCDriver template
// inlined into a `push ecx` frame word that is read UNINITIALISED on the absent path (`mov eax,[esp+8]`: the
// caller does not test the result, as the GOTCHAS 24l note on GetInterface); the four copies in source order
// (Radius re-read after the first store through the component).
#include "engine/CTCBase.h"

class C3DVector { public: float x, y, z; };

struct CExplosionDriver {                          // the spawned thing's driver component
    unsigned char _pad[0xc];
    float m_innerRadius;                           // +0x0c
    float m_outerRadius;                           // +0x10
    float m_maxDamage;                             // +0x14
    long  m_fireDamage;                            // +0x18
};

class CThing { public:
    unsigned char     _pad0[0x28];
    unsigned char     m_driverFlags;               // +0x28 bit 1: the driver component is cached
    unsigned char     _pad1[0x5c - 0x29];
    CExplosionDriver* m_pDriver;                   // +0x5c
    const C3DVector& GetPosition();                // 0x004C73D0
    void Kill(bool immediately);                   // 0x004C9B80
    bool GetTCDriver(CExplosionDriver** out)
    {
        if (m_driverFlags & 2) {
            *out = m_pDriver;
            return true;
        }
        return false;
    }
};

CThing* __fastcall CreateThing(long defIndex, const C3DVector& position);   // 0x0080C460

class CExplodingObjectDef { public:
    unsigned char _pad[0x2c];
    long ExplosionType;                            // +0x2c
};

class CTCExplodingObject : public CTCBase { public:
    float                MaxDamage;                // +0x0c
    float                Radius;                   // +0x10
    long                 FireDamage;               // +0x14
    long                 CountdownTimer;           // +0x18
    bool                 TriggeredOnCreatureProximity; // +0x1c
    float                TriggerRadius;            // +0x20
    CExplodingObjectDef* PDef;                     // +0x24 CDefPointer<CExplodingObjectDef const>
    void Explode();
};

void CTCExplodingObject::Explode()
{
    CExplodingObjectDef* def = PDef;
    CThing* explosion = CreateThing(def->ExplosionType, ParentThing->GetPosition());
    CExplosionDriver* driver;
    explosion->GetTCDriver(&driver);
    driver->m_innerRadius = Radius;
    driver->m_outerRadius = Radius;
    driver->m_fireDamage = FireDamage;
    driver->m_maxDamage = MaxDamage;
    ParentThing->Kill(true);
}