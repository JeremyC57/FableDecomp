// NHeroStatModifierEvent::CHitThing::CHitThing(CThing* thing) -- the event base keeps the thing (+4, stored before the vtable), then this class's vtable (0x01272FA0), the flag at +8 cleared, the word at +0xc cleared, the flag at +0x10 set and the scale at +0x14 = 1.0f
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083AE30, length 37 bytes.
// Shape: `mov eax,ecx` first (the constructor returns `this`), the +4 store before the vtable (a non-polymorphic base holds it), `ret 4`.
struct CThing;
struct CThingEventBase_0083ae30 {                  // the non-polymorphic base at +4: the thing
    CThing* m_pThing;
    CThingEventBase_0083ae30(CThing* thing) : m_pThing(thing) { }
};
namespace NHeroStatModifierEvent {
struct CHitThing : CThingEventBase_0083ae30 {      // vtable 0x01272FA0
    virtual void v00();
    bool  m_8;                                     // +0x08
    long  m_c;                                     // +0x0c
    bool  m_10;                                    // +0x10
    float m_scale;                                 // +0x14
    CHitThing(CThing* thing);
};

CHitThing::CHitThing(CThing* thing)
    : CThingEventBase_0083ae30(thing), m_8(false), m_c(0), m_10(true), m_scale(1.0f)
{
}
}