// CCreatureAction_UseLamp::PerformAction() -- toggle the light of the object being used: if the target exists, is not flagged (bit 0 of +0x91) and has a CTCLight (interface type 0x27), switch the light to the opposite of its current state
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- HasTC + the ternary GetTC model on a GetPItem() thing local: the light loads straight into ecx and the bool builds in dl
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00847FB0, length 106 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PObjectToUse at +0xa8, three times, the member address kept in esi),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x27 = bit 7 of the bitset dword at +0x24:
// VC reads the byte and tests its sign, `mov cl,[eax+24h] ; test cl,cl ; jns`), CTCLight::SetActive(bool) @ 0x006412B0.
// Shape: the three guards are early returns; `!light->Active` on a bool member gives `test dl,dl ; sete dl ; push edx`.
// Layout: PObjectToUse is the CIntelligentPointer<CThing> at +0xa8 (Ego_r.pdb +0xb4, retail CCreatureActionBase is 0xa8 bytes);
// CTCLight's active byte at +0x1c. Modelled locally (the generated headers flatten the bases).
class CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                // +0x91 (bit 0 tested here)
    template<class T> __forceinline T* GetTC(long wanted)        // the landed 0x00584157 lookup: the entry pick as one ternary, the component read once
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
class CTCLight { public:
    unsigned char _pad[0x1c];
    bool Active;                            // +0x1c
    void SetActive(bool active);            // 0x006412B0
};
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 4];
};

class CCreatureAction_UseLamp : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PObjectToUse;   // +0xa8 CIntelligentPointer<CThing>
    void PerformAction();
};

void CCreatureAction_UseLamp::PerformAction()
{
    if (PObjectToUse.GetPItem() == 0)
        return;
    if (PObjectToUse.GetPItem()->m_flags91 & 1)
        return;
    CThing* object = PObjectToUse.GetPItem();
    if (object->HasTC(0x27)) {
        CTCLight* light = object->GetTC<CTCLight>(0x27);
        light->SetActive(!light->Active);
    }
}