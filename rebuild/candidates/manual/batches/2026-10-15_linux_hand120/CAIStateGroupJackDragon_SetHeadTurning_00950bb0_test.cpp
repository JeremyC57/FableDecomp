// Behaviour test for CAIStateGroup_JackDragon::SetHeadTurning (0x00950BB0): only modes of type 0x4e or
// 0x47 get the head-turning flag; other modes are left alone.
#include <cstdio>

struct CTCBase { int dummy; };

// Sorted {type, component} vector map searched by LowerBound (0x0040F020 in retail).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* p = first;
        while (p != last && p->type < *type) ++p;
        return p;
    }
};

struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    CThing() : m_flags91(0)
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_interfaces.first = m_interfaces.last = 0;
    }
    void SetBit(long t) { m_presentBits[t >> 5] |= 1UL << (t & 31); }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
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

struct CAIBrain { CThing* PCreature; };

struct CMode {
    long m_type;
    bool m_headTurning;
    virtual ~CMode() {}
    virtual long GetModeType() { return m_type; }
};
struct CTCCreatureModeManager : CTCBase {
    CMode* m_pCurrent;
    CMode* GetCurrentMode() { return m_pCurrent; }
};
struct CAIStateGroup_JackDragon {
    CAIBrain* PBrain;
    void SetHeadTurning(bool enable)
    {
        CTCCreatureModeManager* modes;
        PBrain->PCreature->GetInterface(&modes, 0x31);
        if (modes->GetCurrentMode()->GetModeType() == 0x4e)
            modes->GetCurrentMode()->m_headTurning = enable;
        else if (modes->GetCurrentMode()->GetModeType() == 0x47)
            modes->GetCurrentMode()->m_headTurning = enable;
    }
};

int main()
{
    CMode mode; mode.m_type = 0x4e; mode.m_headTurning = false;
    CTCCreatureModeManager mgr; mgr.m_pCurrent = &mode;
    CInterfaceMap::SEntry e[1]; e[0].type = 0x31; e[0].component = &mgr;
    CThing creature; creature.m_interfaces.first = e; creature.m_interfaces.last = e + 1; creature.SetBit(0x31);
    CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_JackDragon g; g.PBrain = &brain;
    g.SetHeadTurning(true); bool ok = mode.m_headTurning == true;
    mode.m_type = 0x47; g.SetHeadTurning(false); ok = ok && mode.m_headTurning == false;
    mode.m_type = 0x10; g.SetHeadTurning(true); ok = ok && mode.m_headTurning == false;
    if (!ok) { std::printf("00950bb0_TEST FAIL\n"); return 1; }
    std::printf("00950bb0_TEST PASS\n");
    return 0;
}
