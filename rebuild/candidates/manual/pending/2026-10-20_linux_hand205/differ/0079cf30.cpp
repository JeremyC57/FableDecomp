// CTCBulletTime::Pause() -- pause a pausable bullet time once: stop its looping sound, kill the looping particle effect, drop the screen filter and clear the time-control flag of the interface-0x46 component
// Grade: DIFFER(ours v retail) -- register only: the interface-0x46 lookup key address is formed in eax (lea eax,[esp+4]; push eax) where retail uses ecx; everything else matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079CF30, length 228 bytes (+ int3 padding).
// Callees: the bullet-time sound stop @ 0x0079CEE0 (on `this`, the LoopSound handle; manifest "CCombatSequence_WillEnflame::OnStart"
// is propagated), CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (the
// cleared temporary's vtable 0x0125A26C is CIntelligentPointer<NParticleEngine::CParticleEmitter>), NParticleEngine::
// CParticleEmitter::Kill @ 0x00AEE8C0, CTCBase::GetWorld @ 0x00686A70, the spell-effect interpolator's filter removal
// @ 0x006B83F0 (world +0x18 -> +0x1964; descriptive), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined
// GetInterface for type 0x46 = bit 6 of CThing+0x28).
// Layout: engine/CTCBulletTime.h (retail offsets: m_tpLoopingEffect +0x30, LoopSound +0x38, ScreenFilterKey +0x50,
// Pausable +0x54, Paused +0x55); the typed views of its byte members are inline accessors.
// Shape: the emitter test is an inline alive check on the one-bit `unsigned short` field at bit 5 of +0x40; the filter removal
// is an inline helper that fetches the interpolator before the arguments are pushed; the pointer is cleared
// with an intelligent-pointer temporary built from 0 whose item is read before the member address is formed.
#include "engine/CTCBulletTime.h"
struct CTCBaseInterface;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 UsingComponents
    CInterfaceMap m_interfaces;                 // +0x44 Components
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
struct CTCTimeControlled {                      // interface type 0x46
    unsigned char _pad00[0x1c];
    bool m_flag1c;                              // +0x1c
};
namespace NParticleEngine {
struct CParticleEmitter {
    unsigned char _pad00[0x40];
    unsigned short Flags40 : 5;                 // +0x40
    unsigned short Dead : 1;                    // +0x40 bit 5
    unsigned short Flags40High : 10;
    bool IsDead() const { return Dead; }
    void Kill();                                // 0x00AEE8C0
};
}
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointed-to item
    void* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    NParticleEngine::CParticleEmitter* GetPItem() const;   // 0x00A01B50
    void SetPItem(NParticleEngine::CParticleEmitter* item); // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0125A26C for <CParticleEmitter>
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        T* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
struct CEnvironmentSpellEffects {
    void RemoveFilter_006b83f0(long mode, void* key);   // 0x006B83F0 (descriptive)
};
struct CEnvironment {
    unsigned char _pad00[0x1964];
    CEnvironmentSpellEffects* SpellEffects;     // +0x1964
};
struct CWorld {
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                 // +0x18
};

struct CTCBulletTimeImpl : CTCBulletTime {
    CThing* GetParentThing() const { return (CThing*)f4; }
    CIntelligentPointer<NParticleEngine::CParticleEmitter>& LoopingEffect()
    {
        return *(CIntelligentPointer<NParticleEngine::CParticleEmitter>*)m_tpLoopingEffect;
    }
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
    void StopSound_0079cee0(unsigned long sound);   // 0x0079CEE0
    __forceinline void RemoveScreenFilter()
    {
        CEnvironment* environment = GetWorld()->PEnvironment;
        environment->SpellEffects->RemoveFilter_006b83f0(0, ScreenFilterKey);
    }
    void Pause();
};

void CTCBulletTimeImpl::Pause()
{
    if (!Pausable || Paused)
        return;
    Paused = true;
    StopSound_0079cee0(LoopSound);
    CIntelligentPointer<NParticleEngine::CParticleEmitter>& effect = LoopingEffect();
    if (effect.GetPItem() && !effect.GetPItem()->IsDead()) {
        effect.GetPItem()->Kill();
        effect = CIntelligentPointer<NParticleEngine::CParticleEmitter>(0);
    }
    RemoveScreenFilter();
    CTCTimeControlled* timeControlled;
    if (GetParentThing()->GetInterface(&timeControlled, 0x46))
        timeControlled->m_flag1c = false;
}
