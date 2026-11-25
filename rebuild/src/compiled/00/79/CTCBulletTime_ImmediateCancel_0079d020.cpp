// CTCBulletTime::ImmediateCancel() -- end bullet time at once: drop the slowdown, stop the looping sound, remove the screen filter, kill the looping particle effect, mark the cutscene cancel and remove the component from the thing
// Leaf: ImmediateCancel
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079D020, length 239 bytes (+ int3 padding).
// Callees: CTCBase::GetWorld @ 0x00686A70, the bullet-time manager's RemoveBulletTimeSlowdown(unsigned long key) @ 0x004C6140
// (world +0x68, keyed by this component), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for
// the sound emitter, type 0x10 = bit 16 of CThing+0x20), the emitter's StopSample @ 0x0066ADD0,
// CEnvironmentSpellEffectInterpolator::ScreenFilterFadeOut @ 0x006B83F0 (world +0x18 -> +0x1964), CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 /
// SetPItem 0x00A01B90 / dtor 0x00A01C10 (temporary vtable 0x0125A26C = CIntelligentPointer<NParticleEngine::CParticleEmitter>),
// NParticleEngine::CParticleEmitter::Kill @ 0x00AEE8C0, CThing::RemoveTC(long) @ 0x004C9840 (0x92 = CTCBulletTime).
// Layout: engine/CTCBulletTime.h (retail offsets: m_tpLoopingEffect +0x30, LoopSound +0x38, ScreenFilterKey +0x50,
// Pausable +0x54, CutSceneCancel +0x56).
// Shape: the sound stop is the in-class inline member whose other call (in Pause, 0x0079CF30) is emitted out of line at
// 0x0079CEE0: its by-value handle is loaded before the presence test; the slowdown removal goes through an inline CWorld
// forwarder, the world for the filter removal is a named local fetched before the arguments; the effect kill is the Pause shape (one-bit `unsigned short` dead flag, temporary from 0).
#include "fable_compiler.h"
#define CTCBulletTime CTCBulletTimeLayout
#include "engine/CTCBulletTime.h"
#undef CTCBulletTime
class CWorld;

class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCBulletTime : public CTCBase, public CTCBulletTimeLayout
{
};
struct CTCBaseInterface;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCTimeControlled {                      // interface type 0x46
    unsigned char _pad00[0x1c];
    bool m_flag1c;                              // +0x1c
};
namespace NParticleEngine {
class CParticleEmitter { public:
    unsigned char _pad00[0x40];
    unsigned short Flags40 : 5;                 // +0x40
    unsigned short Dead : 1;                    // +0x40 bit 5
    unsigned short Flags40High : 10;
    bool IsDead() const { return Dead; }
    void Kill();                                // 0x00AEE8C0
};
}
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x0125A26C for <CParticleEmitter>
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        T* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
// NF122 (2026-11-20, q6936): 0x006B83F0 is CEnvironmentSpellEffectInterpolator::ScreenFilterFadeOut(float time, void* handle)
//   (Ego_r.pdb 0x006F4390 + FableWin.pdb 0x0246D190; CGameScriptInterface::ScreenFilterFadeOut 0x00890200 jumps there), declared by
//   engine/CEnvironmentSpellEffectInterpolator.h (FADEOUT); was the descriptive CEnvironmentSpellEffects::RemoveFilter_006b83f0(long, void*).
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FADEOUT
#include "engine/CEnvironmentSpellEffectInterpolator.h"
class CEnvironment { public:
    unsigned char _pad00[0x1964];
    CEnvironmentSpellEffectInterpolator* SpellEffects;     // +0x1964
};
class CBulletTimeManager { public:
    // NF108 (2026-11-19, q5099): Ego_r.pdb 0x004C8B80 + FableWin.pdb 0x01D28090 ?RemoveBulletTimeSlowdownMultiplier@CBulletTimeManager@@QAEXPAX@Z
    //   (the key is the caller's `this` as void*; was RemoveBulletTimeSlowdown(unsigned long)).
    void RemoveBulletTimeSlowdownMultiplier(void* key);   // 0x004C6140
};
class CWorld { public:
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                 // +0x18
    unsigned char _pad1c[0x68 - 0x1c];
    CBulletTimeManager* PBulletTimeManager;     // +0x68
    void RemoveBulletTimeSlowdownMultiplier(void* key) { PBulletTimeManager->RemoveBulletTimeSlowdownMultiplier(key); }
};
struct CTCSoundEmitter {                        // interface type 0x10
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x0066ADD0 ?StopAttachedSound@CTCSoundPlayer@@QAEXK@Z (was ?StopSample@CTCSoundEmitter@@QAEXK@Z)
class CTCSoundPlayer { public: void StopAttachedSound(unsigned long); };
struct CTCBulletTimeImpl : CTCBulletTime {
    CThing* GetParentThing() const { return (CThing*)f4; }
    CIntelligentPointer<NParticleEngine::CParticleEmitter>& LoopingEffect()
    {
        return *(CIntelligentPointer<NParticleEngine::CParticleEmitter>*)m_tpLoopingEffect;
    }
    FABLE_FORCEINLINE void StopSound(unsigned long sound)   // out of line at 0x0079CEE0
    {
        CTCSoundEmitter* emitter;
        if (GetParentThing()->GetTC(TCI_SOUND_PLAYER, &emitter))
            reinterpret_cast<CTCSoundPlayer*>(emitter)->StopAttachedSound(sound);
    }
    void ImmediateCancel();
};

void CTCBulletTimeImpl::ImmediateCancel()
{
    GetWorld().RemoveBulletTimeSlowdownMultiplier(this);
    StopSound(LoopSound);
    CWorld* world = (&GetWorld());
    world->PEnvironment->SpellEffects->ScreenFilterFadeOut(0.0f, ScreenFilterKey);
    CIntelligentPointer<NParticleEngine::CParticleEmitter>& effect = LoopingEffect();
    if (effect.GetPItem() && !effect.GetPItem()->IsDead()) {
        effect.GetPItem()->Kill();
        effect = CIntelligentPointer<NParticleEngine::CParticleEmitter>(0);
    }
    CutSceneCancel = true;
    Pausable = false;
    GetParentThing()->RemoveTC((ETCInterfaceType)(0x92));
}