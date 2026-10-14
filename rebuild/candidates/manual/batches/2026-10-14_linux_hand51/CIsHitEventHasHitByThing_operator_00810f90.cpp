// CIsHitEventHasHitByThing::operator()(const SHitEvent* event) -- true when the event's record (+8) still points at a live thing (the intelligent pointer at +0x50, GetPItem @ 0x00A01B50; +0x91 bit 0 clear)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00810F90, length 48 bytes.
// Shape: `this` unused, `test byte ptr [eax+91h],1`, `mov al,1` / `xor al,al`, `ret 4`.
struct CThing_Retail {
    unsigned char _pad[0x91];
    unsigned char m_91_0 : 1;                      // +0x91 bit 0
    unsigned char m_91_1 : 7;
};
struct CIntelligentPointer_Retail {
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem() const;               // 0x00A01B50
};
struct SHitRecord_00810f90 {
    unsigned char              _pad[0x50];
    CIntelligentPointer_Retail m_hitBy;            // +0x50
    CThing_Retail* GetHitBy() const { return m_hitBy.GetPItem(); }   // `lea ecx,[eax+50h]`: an inline accessor on the record
};
struct SHitEvent_00810f90 {
    unsigned char        _pad[8];
    SHitRecord_00810f90* m_pRecord;                // +0x08
};
struct CIsHitEventHasHitByThing {
    bool operator()(const SHitEvent_00810f90* event);
};

bool CIsHitEventHasHitByThing::operator()(const SHitEvent_00810f90* event)
{
    CThing_Retail* thing = event->m_pRecord->GetHitBy();
    if (thing && !thing->m_91_0)
        return true;
    return false;
}
