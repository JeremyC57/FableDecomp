// CTCBoss::CountEffects() const -- count the boss's effect things that still exist and are not dead
//
// Retail 0x007AEF10, 78 bytes. Callee: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (twice
// per element). Effects is the retail 12-byte vector<CIntelligentPointer<CThing>> {first, last, end}
// at +0x3c (Ego_r.pdb +0x40 behind a 16-byte HelperCreatures vector; retail HelperCreatures is at
// +0x30, see the twin 0x007AEEC0). An element is the 8-byte intelligent pointer (vptr + link), so
// size() is `sar 3`. The Dead flag is CThing +0x91 bit 0 (PDB +0xa1).
// Shape: count in ebx, index in ebp (an unsigned index: `jbe` entry test, `jb` loop test), size()
// re-read from the members after the calls; the element is bound to a reference local once per
// iteration (`lea edi,[ecx+ebp*8]`) so both GetPItem calls reuse it.

class CThing
{ public:
    unsigned char _pad_0x00[0x91];
    unsigned char Dead : 1;                         // +0x91 bit 0
};

class CBaseObject;

struct CBaseIntelligentPointer
{
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer
{ public:
    T* Get() const { return (T*)GetPItem(); }
};

template <class T>
struct CRetailVector                                // retail std::vector: {first, last, end}
{
    T* _First;
    T* _Last;
    T* _End;
    unsigned int size() const { return _Last - _First; }
    const T& operator[](unsigned int i) const { return _First[i]; }
};

class CThingAICreature;

#pragma pack(push, 1)
class CTCBoss
{ public:
    void*         __vftable;                        // +0x00 (CTCBase)
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];                   // +0x08 (CTCBase flags)
    void*         PBossDef;                         // +0x0c CDefPointer<const CBossDef>
    long          GenericCounter;                   // +0x10
    long          StrikesSinceHit;                  // +0x14
    long          currentBossPhase;                 // +0x18
    long          NextWeapon;                       // +0x1c
    bool          BCountStrikes;                    // +0x20
    bool          EnablePhaseChanges;               // +0x21
    unsigned char PhaseStartPos[0xc];               // +0x22 C3DVector
    unsigned char _pad_0x2e[0x2];                   // +0x2e
    CRetailVector<CIntelligentPointer<CThingAICreature> > HelperCreatures;   // +0x30
    CRetailVector<CIntelligentPointer<CThing> >           Effects;           // +0x3c

    long CountEffects();
};
#pragma pack(pop)

long CTCBoss::CountEffects()
{
    long count = 0;
    for (unsigned int i = 0; i < Effects.size(); ++i)
    {
        const CIntelligentPointer<CThing>& effect = Effects[i];
        if (effect.Get() && !effect.Get()->Dead)
            ++count;
    }
    return count;
}