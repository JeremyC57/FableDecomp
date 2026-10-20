// CTCBulletTime::ImmediateCancel() -- end bullet time at once: drop the slowdown, stop the looping sound, remove the screen filter, kill the looping particle effect, mark the cutscene cancel and remove the component from the thing
// Leaf: ImmediateCancel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079D020, length 239 bytes (+ int3 padding).
// Callees: CTCBase::GetWorld @ 0x00686A70, the bullet-time manager's RemoveBulletTimeSlowdown(unsigned long key) @ 0x004C6140
// (world +0x68, keyed by this component), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for
// the sound emitter, type 0x10 = bit 16 of CThing+0x20), the emitter's StopSample @ 0x0066ADD0, the spell-effect interpolator's
// filter removal @ 0x006B83F0 (world +0x18 -> +0x1964; descriptive), CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 /
// SetPItem 0x00A01B90 / dtor 0x00A01C10 (temporary vtable 0x0125A26C = CIntelligentPointer<NParticleEngine::CParticleEmitter>),
// NParticleEngine::CParticleEmitter::Kill @ 0x00AEE8C0, CThing::RemoveTC(long) @ 0x004C9840 (0x92 = CTCBulletTime).
// Layout: engine/CTCBulletTime.h (retail offsets: m_tpLoopingEffect +0x30, LoopSound +0x38, ScreenFilterKey +0x50,
// Pausable +0x54, CutSceneCancel +0x56).
// Shape: the sound stop is the in-class inline member whose other call (in Pause, 0x0079CF30) is emitted out of line at
// 0x0079CEE0: its by-value handle is loaded before the presence test; the slowdown removal goes through an inline CWorld
// forwarder, the world for the filter removal is a named local fetched before the arguments; the effect kill is the Pause shape (one-bit `unsigned short` dead flag, temporary from 0).
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
    void RemoveTC(long interfaceType);          // 0x004C9840
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
struct CBulletTimeManager {
    void RemoveBulletTimeSlowdown(unsigned long key);   // 0x004C6140
};
struct CWorld {
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                 // +0x18
    unsigned char _pad1c[0x68 - 0x1c];
    CBulletTimeManager* PBulletTimeManager;     // +0x68
    void RemoveBulletTimeSlowdown(unsigned long key) { PBulletTimeManager->RemoveBulletTimeSlowdown(key); }
};
struct CTCSoundEmitter {                        // interface type 0x10
    void StopSample(unsigned long sampleID);    // 0x0066ADD0
};

struct CTCBulletTimeImpl : CTCBulletTime {
    CThing* GetParentThing() const { return (CThing*)f4; }
    CIntelligentPointer<NParticleEngine::CParticleEmitter>& LoopingEffect()
    {
        return *(CIntelligentPointer<NParticleEngine::CParticleEmitter>*)m_tpLoopingEffect;
    }
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
    __forceinline void StopSound(unsigned long sound)   // out of line at 0x0079CEE0
    {
        CTCSoundEmitter* emitter;
        if (GetParentThing()->GetInterface(&emitter, 0x10))
            emitter->StopSample(sound);
    }
    void ImmediateCancel();
};

void CTCBulletTimeImpl::ImmediateCancel()
{
    GetWorld()->RemoveBulletTimeSlowdown((unsigned long)this);
    StopSound(LoopSound);
    CWorld* world = GetWorld();
    world->PEnvironment->SpellEffects->RemoveFilter_006b83f0(0, ScreenFilterKey);
    CIntelligentPointer<NParticleEngine::CParticleEmitter>& effect = LoopingEffect();
    if (effect.GetPItem() && !effect.GetPItem()->IsDead()) {
        effect.GetPItem()->Kill();
        effect = CIntelligentPointer<NParticleEngine::CParticleEmitter>(0);
    }
    CutSceneCancel = true;
    Pausable = false;
    GetParentThing()->RemoveTC(0x92);
}