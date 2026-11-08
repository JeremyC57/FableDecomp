// CTCCupboard::UpdateAnimation() -- while the cupboard is animating, advance its animation by one frame (1 / fps seconds) and stop once the animation has (to within epsilon) reached its end
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00749B40, 163 bytes (plain `ret`).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined lookup of CTCAnimationComplex, interface 0x5a:
// bit 26 of the +0x28 word), CTCAnimationComplex::GetAnimation @ 0x0070B460, CAnimateThing::UpdateFrameLogic(const
// CAnimFrameTime&) @ 0x0070E8B0 (as the landed twin CTCCreatureHive::UpdateAnimation 0x0077D190). Globals: CMainGameComponent::GetConstantFPS()
// @ 0x01375550, 1.0f @ 0x0122DED8, DELTA_NOTIONAL_ZERO @ 0x0129BA3C.
// Layout: Ego_r.pdb CTCCupboard Animating +0x0c; the animation's play fraction at +0x1c (named descriptively).
// Shape: unlike the hive twin the animation is fetched (an accessor returning an unset local when the component is absent,
// over a one-ternary GetTC, so
// the component lands straight in ecx and the absent arm reads the key's frame word)
// and the 12-byte frame-time record is zero-constructed before the Animating test; the seconds are stored inside;
// `1.0f - eps < fraction` is `fld 1.0 ; fsub eps ; fcomp [esi+1Ch] ; test ah,5 ; jp`.
#include "fable_compiler.h"
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
extern const float DELTA_NOTIONAL_ZERO;     // 0x0129BA3C (1e-4f)
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    FABLE_FORCEINLINE CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (it == map->end() || interfaceType < it->first ? map->end() : it)->second;
    }
    FABLE_FORCEINLINE CTCBase* GetTCIfPresent(long interfaceType)
    {
        CTCBase* tc;                                    // deliberately unset when absent (retail reads a dead slot)
        if (HasTC(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
};
struct CAnimFrameTime {                                 // 12-byte UpdateFrameLogic argument
    float Seconds;
    long  Reserved1;
    long  Reserved2;
    CAnimFrameTime() : Seconds(0), Reserved1(0), Reserved2(0) { }
};
class CAnimateThing { public:
    unsigned char _pad00[0x1c];
    float PlayFraction;                                 // +0x1c
    void UpdateFrameLogic(const CAnimFrameTime& time);  // 0x0070E8B0
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();                      // 0x0070B460
};
class CTCCupboard { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    bool Animating;                                     // +0x0c
    void UpdateAnimation();
};

void CTCCupboard::UpdateAnimation()
{
    CAnimateThing& anim = ((CTCAnimationComplex*)ParentThing->GetTCIfPresent(0x5a))->GetAnimation();
    CAnimFrameTime time;
    if (Animating) {
        time.Seconds = 1.0f / CMainGameComponent::GetConstantFPS();
        anim.UpdateFrameLogic(time);
        if (1.0f - DELTA_NOTIONAL_ZERO < anim.PlayFraction)
            Animating = false;
    }
}