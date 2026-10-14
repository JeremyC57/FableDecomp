// CIsHitEventHasHitByThing_008d43c0::operator()(const SHitEvent* event) -- true when the event's hitting thing (its record's intelligent pointer at +0x58, 0x00810FD0) is live (+0x91 bit 0 clear) and its 16-bit id (+0x8c) is the one this functor holds (+0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D43C0, length 48 bytes.
// The chunk label is shared with 0x00810F90 (a different test); named by address. Shape: `movzx eax,word ptr [eax+8Ch] ; cmp eax,[esi]`, `ret 4`.
struct CThing_Retail {
    unsigned char  _pad[0x8c];
    unsigned short m_id;                           // +0x8c
    unsigned char  _pad8e[3];
    unsigned char  m_91_0 : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
};
struct SHitEvent_008d43c0 {
    CThing_Retail* GetHitThing() const;            // 0x00810FD0 (`this->m_pRecord->m_hitBy.GetPItem()`; the manifest label is propagated)
};
struct CIsHitEventHasHitByThing_008d43c0 {
    long m_id;                                     // +0x00
    bool operator()(const SHitEvent_008d43c0* event);
};

bool CIsHitEventHasHitByThing_008d43c0::operator()(const SHitEvent_008d43c0* event)
{
    CThing_Retail* thing = event->GetHitThing();
    if (thing && !thing->m_91_0 && thing->m_id == m_id)
        return true;
    return false;
}