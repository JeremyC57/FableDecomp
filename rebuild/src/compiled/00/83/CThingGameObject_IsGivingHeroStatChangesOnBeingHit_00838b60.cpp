// CThingGameObject::IsGivingHeroStatChangesOnBeingHit() -- the per-object override (flags +0xC8 bit 2) when its "set" bit (bit 3) is on, else the def's GiveHeroStatChangesOnBeingHit bit (byte +0x43 bit 4) read through a counted copy of the def pointer (+0x70)
// Leaf: IsGivingHeroStatChangesOnBeingHit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00838B60, length 54 bytes (+ int3 padding). Twin of 0x00838B20.
// Callees: the def's virtual slot 1 (`call [eax+4]`, destroy at reference count zero).
// Layout: local model. engine/CThingGameObject.h agrees on the retail flag byte at +0xC8 (the landed setters
// 0x008389A0 / 0x008389C0 write bits 0/1 and 2/3) but holds only padding at +0x70 (the PBaseDef def pointer, the
// retail CThing's head is 0x10 narrower than the PDB's +0x80), so the members are modelled locally.
// Shape: the def pointer is copied into a local CDefPointer (copy constructor `if (p) ++p->count` = `inc dword ptr
// [ecx+4]`), the bit is read, and the destructor releases with a load/dec/store (no null test) -- the landed
// CTCWeapon::IsAnalogueLoading (0x005D9270) shape; the bitfield value rides in bl across the release.
struct CThingGameObjectDef {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long          m_refCount;                       // +0x04
    unsigned char _pad08[0x3b];
    unsigned char _bits43_0 : 3;
    unsigned char AddToComboMultiplierOnHit : 1;    // +0x43 bit 3
    unsigned char GiveHeroStatChangesOnBeingHit : 1;// +0x43 bit 4
};

struct CDefPointer {
    CThingGameObjectDef* m_p;
    CDefPointer(const CDefPointer& other) : m_p(other.m_p) { if (m_p) ++m_p->m_refCount; }
    ~CDefPointer()
    {
        long count = m_p->m_refCount - 1;
        m_p->m_refCount = count;
        if (count == 0)
            m_p->Destroy();
    }
    CThingGameObjectDef* operator->() const { return m_p; }
};

class CThingGameObject { public:
    unsigned char _pad00[0x70];
    CDefPointer   PBaseDef;                         // +0x70
    unsigned char _pad74[0x54];
    unsigned char AddToComboMultiplierOnHitOverride : 1;        // +0xc8 bit 0
    unsigned char AddToComboMultiplierOnHitOverrideSet : 1;     // +0xc8 bit 1
    unsigned char GiveHeroStatChangesOnBeingHitOverride : 1;    // +0xc8 bit 2
    unsigned char GiveHeroStatChangesOnBeingHitOverrideSet : 1; // +0xc8 bit 3
    bool IsGivingHeroStatChangesOnBeingHit() const;
};

bool CThingGameObject::IsGivingHeroStatChangesOnBeingHit() const
{
    if (GiveHeroStatChangesOnBeingHitOverrideSet)
        return GiveHeroStatChangesOnBeingHitOverride;
    CDefPointer def = PBaseDef;
    return def->GiveHeroStatChangesOnBeingHit;
}