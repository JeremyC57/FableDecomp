// CTCBattleCharge::OnInitialActivate() -- give the owner's creature mode manager (interface 0x31) mode 0x23 when it has one; then, for a live player creature (thing type 1), suspend the owner's animation, otherwise reset the charge
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A7370, length 114 bytes (+14 int3 padding). Vtable slot 6 of CTCBattleCharge
// (0x0126CBFC; catalog "vfunc_6"; slot 6 is the CTCBase OnInitialActivate slot).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface), CTCCreatureModeManager::AddMode
// @ 0x0073AE10 (result dropped), the owner's suspend-animation setter @ 0x00833A10 (catalog "CThingAnimatable::SetSuspend
// Animation", called with true, false), the charge's own reset @ 0x007A7000 (catalog "CTCAssassinRush::UpdateDirectionOverride").
// Shape: the owner in edi for both tests (ParentThing re-read for the setter call); the GetInterface result is used straight from
// the iterator; `mov eax,1` shared by the dead-bit test, the type compare and the `true` push; separate returns per arm.
// Layout: retail CThing: +0x10 thing type (1 = player creature), +0x20 component bitset (0x31 = bit 17 of the dword at +0x24),
// +0x44 interface map, +0x91 bit 0 Dead.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char m_type;                       // +0x10
    unsigned char _pad11[0xf];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_dead : 1;                   // +0x91 bit 0
    unsigned char m_91_1 : 7;
    void SetSuspendAnimation(bool suspend, bool immediate);   // 0x00833A10
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
class CTCCreatureModeManager { public:
    bool AddMode(unsigned long mode);           // 0x0073AE10
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCBattleCharge : public CTCBase {
public:
    virtual void OnInitialActivate();
    void Reset_007a7000();                               // 0x007A7000
};

void CTCBattleCharge::OnInitialActivate()
{
    CThing* thing = ParentThing;
    CTCCreatureModeManager* modes;
    if (thing->GetInterface(&modes, 0x31))
        modes->AddMode(0x23);
    if (!thing->m_dead && thing->m_type == 1)
        ParentThing->SetSuspendAnimation(true, false);
    else
        Reset_007a7000();
}