// CAIStateGroup_LightHomeCandle::ShouldBeLit() -- a villager's candle should be lit while the time of day lies in its village def's lighting window
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00946690, length 157 bytes (+ int3 padding). Vtable slot 15 of
// CAIStateGroup_LightHomeCandle (0x0128BDBC).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (the const instantiation of the inlined interface lookup, type
// 0x23 = CTCVillageMember, `test byte ptr [eax+24h],8`), CTCVillageMember::GetPVillage @ 0x007EE540 (landed name; manifest
// "IS_THING_ALIVE<..>" is propagated) x2, CAIStateGroupBase::GetPThingWorld @ 0x008FCFA0, the time-of-day test
// Game_IsTimeOfDayInRange @ 0x006BB390 on the world's +0x1c object, the village def's virtual slot 1 (destroy).
//
// Shape: the const out-parameter GetInterface (result ignored, the miss arm reads the never-written local from
// the key slot) followed by `if (villager && villager->GetPVillage())`; the village's def pointer (+0x94) is
// copied into a pointer local with a null-tested add-ref (`inc [esi+4]`) and released BY HAND without a null
// test (`dec [esi+4] ; jne ; call [vt+4]`); the world is a named local taken before the def fields are read and
// the time manager comes through an inline accessor (`mov eax,[eax+1Ch]` ... `mov ecx,eax`); the bool rides in
// bl across the release. Field names of the village def window are descriptive.
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};

class CVillageDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    unsigned char _pad[0xc4];                   // +0x08..+0xcb
    long LightOnTime;                           // +0xcc (descriptive)
    long LightOffTime;                          // +0xd0 (descriptive)
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};

class CTCVillage { public:
    unsigned char _pad[0x94];
    CVillageDef* PDef;                          // +0x94
};

class CTCVillageMember { public:                            // interface type 0x23
public:
    CTCVillage* GetPVillage() const;   // 0x007EE540
};

class CGameTimeManager { public:
    bool IsTimeOfDayInRange(long start, long end);   // 0x006BB390
};

class CWorld { public:
    unsigned char _pad[0x1c];
    CGameTimeManager* m_pTimeManager;           // +0x1c
    CGameTimeManager* GetPTimeManager() { return m_pTimeManager; }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    CWorld* GetPThingWorld();                   // 0x008FCFA0
};

class CAIStateGroup_LightHomeCandle : public CAIStateGroupBase { public:
    unsigned char PCandle[8];                   // +0x14 CIntelligentPointer<CThing>
    virtual bool ShouldBeLit();
};

bool CAIStateGroup_LightHomeCandle::ShouldBeLit()
{
    CTCVillageMember* villager;
    PBrain->PCreature->GetInterface(&villager, 0x23);
    if (villager && villager->GetPVillage()) {
        CVillageDef* def = villager->GetPVillage()->PDef;
        if (def)
            def->AddRef();
        CWorld* world = GetPThingWorld();
        bool lit = world->GetPTimeManager()->IsTimeOfDayInRange(def->LightOnTime, def->LightOffTime);
        def->Release();
        return lit;
    }
    return false;
}