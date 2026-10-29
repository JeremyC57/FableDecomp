// CTCFishingSpot::RemoveParticles() -- kill each live particle thing (the two splash particles and the fish particle), clear its pointer, then mark the spot disabled
// Leaf: RemoveParticles
//
// Retail 0x007B01C0, 320 bytes incl. padding (plain `ret`). Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CThing::Kill(bool) @ 0x004C9B80, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::SetPItem @ 0x00A01B90,
// ::~CBaseIntelligentPointer @ 0x00A01C10; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Layout = Ego_r.pdb CTCFishingSpot (0x28): PParticle1 +0xc, PParticle2 +0x14, PParticleFish +0x1c, Enabled +0x24 (as the
// landed ctor 0x007B0150); retail CThing dead bit +0x91.
// Shape: one `__forceinline` per-pointer helper expanded three times (the member address held in esi): IS_THING_ALIVE calls
// GetPItem for the null and the dead test, Kill gets a fresh GetPItem, and the clear is `p = CIntelligentPointer<CThing>(0)`
// -- a frame temporary (base ctor, vtable store, SetPItem(0)) copied through the inline `T* item = other.GetPItem();
// SetPItem(item);` assignment and destroyed by the base destructor (CIntelligentPointer declares no destructor).
class CBaseObject;

class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Flags91;                                  // +0x91 bit 0 Dead
    void Kill(bool immediately);                            // 0x004C9B80
};

struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                             // 0x00A01C10
    CBaseObject* GetPItem() const;                               // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        T* item = ((CThing*)other.GetPItem());
        SetPItem((CBaseObject*)item);
        return *this;
    }
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->Flags91 & 1))
            return true;
    }
    return false;
}

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCFishingSpot : public CTCBase { public:                           // vtable 0x0126D51C
    CIntelligentPointer<CThing> PParticle1;                 // +0x0c
    CIntelligentPointer<CThing> PParticle2;                 // +0x14
    CIntelligentPointer<CThing> PParticleFish;              // +0x1c
    bool                        Enabled;                    // +0x24
    static __forceinline void RemoveParticle(CIntelligentPointer<CThing>& particle)
    {
        if (IS_THING_ALIVE(particle)) {
            ((CThing*)particle.GetPItem())->Kill(true);
            particle = CIntelligentPointer<CThing>(0);
        }
    }
    void RemoveParticles();
};

void CTCFishingSpot::RemoveParticles()
{
    RemoveParticle(PParticle1);
    RemoveParticle(PParticle2);
    RemoveParticle(PParticleFish);
    Enabled = false;
}