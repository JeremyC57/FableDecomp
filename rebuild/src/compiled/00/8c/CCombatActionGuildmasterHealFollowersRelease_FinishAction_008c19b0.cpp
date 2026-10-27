// CCombatAction_Guildmaster_HealFollowersRelease::FinishAction() -- remove creature mode 0x3C on the owner (through its creature-mode manager component), then the base combat-action finish
// Leaf: ?FinishAction@CCombatAction_Guildmaster_HealFollowersRelease@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C19B0, 82 bytes, thiscall, plain `ret`.
// Identity (proven): CCombatAction_Guildmaster_HealFollowersRelease vtable 0x012869BC slot 4 (FinishAction); Ego_r.pdb ?FinishAction@CCombatAction_Guildmaster_HealFollowersRelease@@UAEXXZ (donor 0x008F99C0).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020,
//   CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0, CCombatActionBase::FinishAction() 0x00859A20.
// Shape (family K040, representative 0x008C19B0): `if (GetCreature()->GetInterface(&modes, 0x31)) modes->RemoveMode(0x3C);` (the
//   landed CCreatureAction_FinishFireballSpell::FinishAction 0x008BAC40 GetInterface model: the bitset word test, the lower
//   bound and its end / greater check), then `CCombatActionBase::FinishAction();`.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
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
class CTCCreatureModeManager { public:                // interface 0x31
    void RemoveMode(unsigned long mode);              // 0x0073ABA0
    bool AddMode(unsigned long mode);                 // 0x0073AE10
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CCreatureActionBase { public:                   // retail sizeof 0xa8
    virtual ~CCreatureActionBase();
    virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();                      // slot 4
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;            // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCombatActionBase : public CCreatureActionBase { public:      // retail sizeof 0x114
    unsigned char _pad0a8[0x114 - 0xa8];
    virtual void FinishAction();                      // 0x00859A20
};
class CCombatAction_Guildmaster_HealFollowersRelease : public CCombatActionBase {
public:
    virtual void FinishAction();
};
void CCombatAction_Guildmaster_HealFollowersRelease::FinishAction()
{
    CTCCreatureModeManager* modes;
    if (GetCreature()->GetInterface(&modes, 0x31))
        modes->RemoveMode(0x3C);
    CCombatActionBase::FinishAction();
}