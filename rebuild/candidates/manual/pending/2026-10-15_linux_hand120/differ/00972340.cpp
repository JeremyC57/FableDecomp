// Grade: DIFFER(ours v retail) -- 168v142: the lane gives the GetInterface key temporary and the sub-def local separate frame slots (sub esp,8; retail shares one `push ecx` slot), and with the extra slot the releases go through esi (`mov esi,[eax+4] ; add eax,4 ; dec esi ; mov [eax],esi`) instead of `dec dword ptr [ecx+4]`
// CAIStateGroup_JackOfBladesStage1::HasHitsCausedRush() -- true when the boss's hit counter has reached the will-power indicator def's rush threshold (a zero threshold never rushes)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00972340, length 142 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined interface lookup, type 0xad = CTCBoss, `test ah,20h` on the
// presence dword at +0x34), CDefClassBase::GetSubDef<CAICreatureWillPowerIndicatorDef> @ 0x008C33E0 (on the
// creature's def at +0x70), the sub-def's virtual slot 1 (destroy) on the last release.
//
// Shape: the creature is a named local (edi); the out-parameter GetInterface template with its result
// ignored (miss arm reads the never-written boss local from the key slot); the sub-def pointer is a
// zero-initialised local in the same frame slot, filled by GetSubDef(&def), and released BY HAND on the
// pointer local (`if (--def->m_refCount == 0) def->Destroy();`, no null test) on both exits: the zero
// threshold exit returns false, the other keeps `boss->m_hitCount == def->HitsToRush` in bl across the
// release (`cmp [esi+10h],eax ; sete bl`). Member names of the boss/def fields are descriptive.
// Tried: release on the address-taken local (re-read from the frame each use, 166), this pointer copy (168; the
// lane spills into the wave-18 `mov esi,[eax+4] ; add eax,4 ; dec esi` release), a Release() member + hits local (168).
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CAICreatureWillPowerIndicatorDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    unsigned char _pad[0x38];                   // +0x08..+0x3f
    long HitsToRush;                            // +0x40 (descriptive)
};

struct CDefClassBase {
    bool GetSubDef(CAICreatureWillPowerIndicatorDef** out);   // 0x008C33E0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x24];                 // +0x4c..+0x6f
    CDefClassBase* PDef;                        // +0x70
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

struct CTCBoss {                                // interface type 0xad
    unsigned char _pad[0x10];
    long m_hitCount;                            // +0x10 (cleared by JackOfBladesBase::Initialise)
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
};

struct CAIStateGroup_JackOfBladesStage1 : CAIStateGroupBase {
    bool HasHitsCausedRush();
};

bool CAIStateGroup_JackOfBladesStage1::HasHitsCausedRush()
{
    CThing* creature = PBrain->PCreature;
    CTCBoss* boss;
    creature->GetInterface(&boss, 0xad);
    CAICreatureWillPowerIndicatorDef* subDef = 0;
    creature->PDef->GetSubDef(&subDef);
    CAICreatureWillPowerIndicatorDef* def = subDef;
    if (def->HitsToRush == 0) {
        if (--def->m_refCount == 0)
            def->Destroy();
        return false;
    }
    bool rushed = boss->m_hitCount == def->HitsToRush;
    if (--def->m_refCount == 0)
        def->Destroy();
    return rushed;
}
