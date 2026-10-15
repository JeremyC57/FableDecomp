// CCreatureAction_UseLamp::PerformAction() -- toggle the light of the object being used: if the target exists, is not flagged (bit 0 of +0x91) and has a CTCLight (interface type 0x27), switch the light to the opposite of its current state
// Grade: DIFFER(108v106) -- the CTCLight pointer lands in eax and is copied to ecx after `sete cl ; push ecx`; retail loads it straight into ecx and builds the bool in dl
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00847FB0, length 106 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PObjectToUse at +0xa8, three times, the member address kept in esi),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x27 = bit 7 of the bitset dword at +0x24:
// VC reads the byte and tests its sign, `mov cl,[eax+24h] ; test cl,cl ; jns`), CTCLight::SetActive(bool) @ 0x006412B0.
// Shape: the three guards are early returns; `!light->Active` on a bool member gives `test dl,dl ; sete dl ; push edx`.
// Layout: PObjectToUse is the CIntelligentPointer<CThing> at +0xa8 (Ego_r.pdb +0xb4, retail CCreatureActionBase is 0xa8 bytes);
// CTCLight's active byte at +0x1c. Modelled locally (the generated headers flatten the bases).
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                // +0x91 (bit 0 tested here)
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
struct CTCLight {
    unsigned char _pad[0x1c];
    bool Active;                            // +0x1c
    void SetActive(bool active);            // 0x006412B0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 4];
};

struct CCreatureAction_UseLamp : CCreatureActionBase {
    CBaseIntelligentPointer PObjectToUse;   // +0xa8 CIntelligentPointer<CThing>
    void PerformAction();
};

void CCreatureAction_UseLamp::PerformAction()
{
    if (PObjectToUse.GetPItem() == 0)
        return;
    if (PObjectToUse.GetPItem()->m_flags91 & 1)
        return;
    CTCLight* light;
    if (PObjectToUse.GetPItem()->GetInterface(&light, 0x27))
        light->SetActive(!light->Active);
}
