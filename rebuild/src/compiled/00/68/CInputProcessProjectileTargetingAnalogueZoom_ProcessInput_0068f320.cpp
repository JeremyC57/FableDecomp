// CInputProcessProjectileTargetingAnalogueZoom::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- for a live hero: the analogue action 0x54 zooms the projectile-targeting camera by its Y value / 255, presses of actions 0x6e / 0x6f zoom by +-196/255; a zoom marks the input handled
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068F320, length 323 bytes, `ret 8`.
// Callees: CPlayer::GetHeroCreature @ 0x00487DC0 (MyPlayer +0x18), CInputProcessGameBase::GetEventGameActionVector(long,
//   const CInputEvent&, C2DVector*) @ 0x00687D00 (landed name), IsEventGameAction @ 0x00687CF0, IsEventButtonPressed
//   @ 0x00687A90, the component map's LowerBound @ 0x0040F020, the zoom component's AdjustZoom(float) @ 0x00789A10
//   (adds delta * k to +0x40 and clamps it to [0,1]; manifest "ZoomControl_Update"), CProcessedInput::SetHandled @ 0x00A0D390.
//   Float pool: 1/255 @ 0x01231724; the +-196/255 deltas are pushed as immediates.
// Layout: `this` is engine/CInputProcessGameBase.h (MyPlayer +0x18); CThing dead flag bit 0 of +0x91, component bitset +0x20
//   (0x81 = word 4 bit 1, read as `test al,2` on +0x30) and map +0x44 as in the landed CInputProcessBetting 0x00691560.
//   Component 0x81's class is unconfirmed (named CTCProjectileZoom here).
// Shape: GetTC is a `__forceinline` `T* c = 0; if (HasTC(k)) c = Find(k); return c;` inlined at each of the three zoom
//   calls (the `xor ecx,ecx` preload each time); its key shares the dead `event` argument slot.
#include "engine/CInputProcessGameBase.h"
struct C2DVector {
    float x, y;
};
class CInputEvent;
class CProcessedInput {
public:
    void SetHandled();                             // 0x00A0D390
};
struct CTCProjectileZoom {
    void AdjustZoom(float delta);                  // 0x00789A10
};
struct STCEntry {                                  // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long               m_kind;
    CTCProjectileZoom* m_pTC;
};
struct CTCMap {                                    // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x0040F020
    __forceinline CTCProjectileZoom* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[9];                     // +0x20: which component kinds the thing holds
    CTCMap        m_tcs;                           // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char m_dead : 1;                      // +0x91 bit 0
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCProjectileZoom* GetTC(long kind)
    {
        CTCProjectileZoom* tc = 0;
        if (HasTC(kind))
            tc = m_tcs.Find(kind);
        return tc;
    }
};
class CPlayer {
public:
    CThing* GetHeroCreature();                     // 0x00487DC0
};
class CInputProcessProjectileTargetingAnalogueZoom : public CInputProcessGameBase {
public:
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    bool GetEventGameActionVector(long action, const CInputEvent& event, C2DVector* vector);   // 0x00687D00
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessProjectileTargetingAnalogueZoom::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CThing* hero = MyPlayer->GetHeroCreature();
    if (!hero || hero->m_dead)
        return;
    C2DVector zoom;
    if (GetEventGameActionVector(0x54, event, &zoom))
        hero->GetTC(0x81)->AdjustZoom(zoom.y * (1.0f / 255.0f));
    else if (IsEventGameAction(0x6e, event)) {
        if (!IsEventButtonPressed(event))
            return;
        hero->GetTC(0x81)->AdjustZoom(196.0f / 255.0f);
    } else if (IsEventGameAction(0x6f, event) && IsEventButtonPressed(event))
        hero->GetTC(0x81)->AdjustZoom(-196.0f / 255.0f);
    else
        return;
    processed.SetHandled();
}