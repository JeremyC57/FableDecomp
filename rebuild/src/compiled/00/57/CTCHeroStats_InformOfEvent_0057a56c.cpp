// CTCHeroStats::InformOfEvent(const NHeroStatModifierEvent::CBase& event) -- lets the hero-stat modifier event apply itself to these stats (event's first virtual, given `this`)
// Retail: Fable.exe @ 0x0057A56C, 14 bytes, `ret 4`. Callers (CEventDied::OnAdd 0x00811569, CThingGameObject::OnHit
// 0x0083A7AE) look up the hero's stats component and pass a stack NHeroStatModifierEvent::CKnockedThingUnconscious
// (vtable 0x01272334, RTTI base NHeroStatModifierEvent::CBase). The event's slot-0 method name is descriptive.
// Shape: size TU: under `optimize("s", on)` VC7.1 keeps the virtual call as `call [edx] ; ret 4` (under /O2 it tail-jumps).
#pragma optimize("s", on)
class CTCHeroStats;
namespace NHeroStatModifierEvent {
class CBase {
public:
    virtual void ApplyTo(CTCHeroStats* stats) const = 0;
};
}
class CTCHeroStats {
public:
    void InformOfEvent(const NHeroStatModifierEvent::CBase& event);
};

void CTCHeroStats::InformOfEvent(const NHeroStatModifierEvent::CBase& event)
{
    event.ApplyTo(this);
}