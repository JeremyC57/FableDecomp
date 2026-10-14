// CThingStateCounter_00c3eaa0::Count(CThing* thing) -- raise one of three counters by the thing's state: +0x1d0 when virtual slot 4 says it is not conscious, else +0x1cc when slot 5 holds, else +0x1c8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3EAA0, length 96 bytes.
// The chunk label "CIsThingConscious::operator()" is propagated onto both the increment and the decrement body; named by effect. Shape: the thing in edi, `this` in esi, three `inc eax` tails, `ret 4`.
struct CThing_Retail {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual bool IsConscious();                    // slot 4 (+0x10)
    virtual bool IsAlert();                        // slot 5 (+0x14)
};
struct CThingStateCounter_00c3eaa0 {
    unsigned char _pad[0x1c8];
    long m_other;                                  // +0x1c8
    long m_alert;                                  // +0x1cc
    long m_unconscious;                            // +0x1d0
    void Count(CThing_Retail* thing);
};

void CThingStateCounter_00c3eaa0::Count(CThing_Retail* thing)
{
    if (!thing->IsConscious())
        ++m_unconscious;
    else if (thing->IsAlert())
        ++m_alert;
    else
        ++m_other;
}
