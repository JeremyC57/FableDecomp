// CThingAICreature::GetSleepTimes(long& start, long& end) -- the creature's sleep window by age (child/adult/elderly) from its village's def, or from the definition manager's default village def when it has no village
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00833390, length 349 bytes (`ret 8`).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (the const copy; inlined GetTC on the
//   village-member interface 0x23), CTCVillageMember::GetPVillage @ 0x007EE540 (manifest "IS_THING_ALIVE<...>" is
//   propagated; landed callers 0x007B6A80 / 0x00833710 name it), CThing::GetDefinitionManager @ 0x004C7990 (propagated
//   "CViewBase::GetDisplayEngine"), the village def's virtual slot 1 (destroy at refcount zero).
// Layout: retail CThing interface bitset +0x20 (bit 0x23 = byte +0x24 bit 3), CVectorMap +0x44; CThingCreatureBase PDef
//   +0xE0 (landed ConstructFromParams 0x00830360) whose +0xC4 word is the creature's age (1 child, 2 adult, 3 elderly:
//   it selects the PDB CVillageDef SleepChild/SleepAdult/SleepElderly pairs); retail CTCVillage PDef +0x94 (landed
//   CTCCrateStack::vfunc_6 0x007B6A80); the village def's sleep pairs sit at +0xB4/+0xBC/+0xC4 (PDB +0xD4.. less the
//   eight 4-byte-narrower STLport vectors); the definition manager's +0xD0 def carries the same pairs 4 bytes lower
//   (+0xB0/+0xB8/+0xC0) -- descriptive model, its PDB name is unconfirmed.
// Shape: `CDefPointer<const CVillageDef> def = village->PDef;` (null-tested add-ref) is released without a null test
//   after the switch reads through it; every case RETURNS (retail duplicates the release and the epilogue per case),
//   cases written 1, 2, 3, default (default = the adult pair, merged with case 2); the fallback def comes from a named
//   manager local so the def pointer is loaded before the age (ecx/eax as retail).
class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long m_refCount;                                        // +4
};

class CVillageDef : public CDefPointeeBase { public:
    unsigned char _pad08[0xb4 - 0x08];
    long SleepAdultStartTime;                               // +0xb4
    long SleepAdultEndTime;                                 // +0xb8
    long SleepChildStartTime;                               // +0xbc
    long SleepChildEndTime;                                 // +0xc0
    long SleepElderlyStartTime;                             // +0xc4
    long SleepElderlyEndTime;                               // +0xc8
};

struct CDefaultVillageTimes {                               // the definition manager's +0xD0 def (descriptive)
    unsigned char _pad00[0xb0];
    long SleepAdultStartTime;                               // +0xb0
    long SleepAdultEndTime;                                 // +0xb4
    long SleepChildStartTime;                               // +0xb8
    long SleepChildEndTime;                                 // +0xbc
    long SleepElderlyStartTime;                             // +0xc0
    long SleepElderlyEndTime;                               // +0xc4
};

class CDefinitionManager { public:
    unsigned char _pad00[0xd0];
    CDefaultVillageTimes* DefaultVillageDef;                // +0xd0
};

template <class T>
class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (--Object->m_refCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

class CTCVillage { public:
    unsigned char           _pad00[0x94];
    CDefPointer<CVillageDef> PDef;                          // +0x94 CDefPointer<const CVillageDef>
};

class CTCVillageMember { public:
    CTCVillage* GetPVillage() const;                        // 0x007EE540
};

class CThingCreatureDef { public:
    unsigned char _pad00[0xc4];
    long Age;                                               // +0xc4 (1 child, 2 adult, 3 elderly)
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    CDefinitionManager* GetDefinitionManager();             // 0x004C7990
};

class CThingAICreature : public CThing { public:
    unsigned char      _pad4c[0xe0 - 0x4c];
    CThingCreatureDef* PDef;                                // +0xe0 CDefPointer<CThingCreatureDef const>::Object
    void GetSleepTimes(long& start, long& end);
};

void CThingAICreature::GetSleepTimes(long& start, long& end)
{
    if (HasTC(0x23)) {
        CTCVillage* village = GetTC<CTCVillageMember>(0x23)->GetPVillage();
        if (village) {
            CDefPointer<CVillageDef> def = village->PDef;
            switch (PDef->Age) {
            case 1:
                start = def->SleepChildStartTime;
                end = def->SleepChildEndTime;
                return;
            case 2:
                start = def->SleepAdultStartTime;
                end = def->SleepAdultEndTime;
                return;
            case 3:
                start = def->SleepElderlyStartTime;
                end = def->SleepElderlyEndTime;
                return;
            default:
                start = def->SleepAdultStartTime;
                end = def->SleepAdultEndTime;
                return;
            }
        }
    }
    CDefinitionManager* manager = GetDefinitionManager();
    CDefaultVillageTimes* def = manager->DefaultVillageDef;
    switch (PDef->Age) {
    case 1:
        start = def->SleepChildStartTime;
        end = def->SleepChildEndTime;
        return;
    case 2:
        start = def->SleepAdultStartTime;
        end = def->SleepAdultEndTime;
        return;
    case 3:
        start = def->SleepElderlyStartTime;
        end = def->SleepElderlyEndTime;
        return;
    default:
        start = def->SleepAdultStartTime;
        end = def->SleepAdultEndTime;
        return;
    }
}