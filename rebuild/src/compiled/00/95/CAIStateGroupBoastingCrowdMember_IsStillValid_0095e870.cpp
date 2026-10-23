// CAIStateGroup_BoastingCrowdMember::IsStillValid() -- still valid while the boaster is alive, its crowd component (interface 0x74) is in an active state (neither 0 nor 10) and that component's audience target is alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095E870, length 134 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times on the boaster pointer at +0x14, once on
// the component's pointer at +0x24), the interface-map lower bound @ 0x004365B0 (a second instantiation of the
// 0x0040F020 CVectorMap::LowerBound; interface 0x74 = bitset word 3 bit 20 -> `test dword ptr [eax+2Ch],100000h`).
// Shape: nested ifs with one `return 1` (`mov eax,1`: an int-width result, not bool) and the shared
// `xor eax,eax` tail; the dead flag is bit 0 of the thing's byte at +0x91; the boaster pointer's address is
// formed once into esi and re-fetched for every use; the lookup is the landed GetInterface out-parameter
// template whose bool is tested.
// Model: local (no engine header for this class); the boaster pointer is a CIntelligentPointer<CThing>.
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
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

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CTCBoastingCrowd {                       // interface 0x74
    unsigned char _pad[0x24];
    CBaseIntelligentPointer PAudienceTarget;    // +0x24
    long State;                                 // +0x2c
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_BoastingCrowdMember : CAIStateGroupBase {
    CBaseIntelligentPointer PBoaster;           // +0x14
    long IsStillValid();
};

long CAIStateGroup_BoastingCrowdMember::IsStillValid()
{
    if (PBoaster.GetPItem() != 0 && !PBoaster.GetPItem()->IsDead()) {
        CTCBoastingCrowd* crowd;
        if (PBoaster.GetPItem()->GetInterface(&crowd, 0x74)) {
            if (crowd->State != 0 && crowd->State != 10) {
                CThing* target = crowd->PAudienceTarget.GetPItem();
                if (target != 0 && !target->IsDead())
                    return 1;
            }
        }
    }
    return 0;
}