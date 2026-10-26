// CCreatureAction_CastWillEnflameSpell::PerformAction() -- if the creature has the type-0x95 (enflame spell) component, raise its cast flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BA370, length 80 bytes. Vtable slot 3 (PerformAction).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (`add ecx,8`: PCreature at +8, this is never kept), CVectorMap::LowerBound
// @ 0x0040F020 (the inlined GetInterface lookup for type 0x95 = bit 21 of the bitset dword at +0x30; landed shape of 0x00849320).
// Shape: one `push ecx` frame for the lookup key; the component rides in esi; the byte at +0xd of the component is set.
// Retail CCreatureActionBase is 0xa8 bytes (PDB 0xb4), so `this` is modelled locally.
class CTCBase;

struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

class CThingCreatureBase { public:                 // engine/CThingCreatureBase.h exists but is not used: only the interface lookup is modelled
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCEnflameSpell {                    // the type-0x95 component (name descriptive)
    unsigned char _pad0[0x0d];
    unsigned char m_cast;                   // +0x0d
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

class CCreatureAction_CastWillEnflameSpell { public:   // engine/CCreatureAction_CastWillEnflameSpell.h exists but is not used: retail CCreatureActionBase is 0xa8 bytes, the PDB build's 0xb4
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
};

void CCreatureAction_CastWillEnflameSpell::PerformAction()
{
    CTCEnflameSpell* pSpell;
    if (PCreature.GetPItem()->GetInterface(&pSpell, 0x95))
        pSpell->m_cast = 1;
}