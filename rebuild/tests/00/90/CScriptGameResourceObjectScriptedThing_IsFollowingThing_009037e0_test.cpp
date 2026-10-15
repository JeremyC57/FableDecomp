// Behaviour test for 0x009037E0 (CScriptGameResourceObjectScriptedThing::IsFollowingThing): true only
// when flag bit 0 is set and the current state-group type is 2.
#include <cstdio>

struct CThing_Retail {
    unsigned char m_flags;
    long m_stateGroupType;
    long GetCurrentStateGroupType() { return m_stateGroupType; }
};
struct CIntelligentPointer_Retail {
    CThing_Retail* m_pItem;
    CThing_Retail* GetPItem() { return m_pItem; }
};
struct CScriptGameResourceObjectScriptedThing {
    CIntelligentPointer_Retail PThing;
    bool IsFollowingThing();
};

bool CScriptGameResourceObjectScriptedThing::IsFollowingThing()
{
    CIntelligentPointer_Retail* thing = &PThing;
    if ((thing->GetPItem()->m_flags & 1) && thing->GetPItem()->GetCurrentStateGroupType() == 2)
        return true;
    return false;
}

int main()
{
    CThing_Retail t;
    CScriptGameResourceObjectScriptedThing s;
    s.PThing.m_pItem = &t;
    t.m_flags = 1; t.m_stateGroupType = 2;
    if (!s.IsFollowingThing()) { std::printf("009037e0_TEST FAIL\n"); return 1; }
    t.m_stateGroupType = 3;
    if (s.IsFollowingThing()) { std::printf("009037e0_TEST FAIL\n"); return 1; }
    t.m_flags = 0xfe; t.m_stateGroupType = 2;
    if (s.IsFollowingThing()) { std::printf("009037e0_TEST FAIL\n"); return 1; }
    std::printf("009037e0_TEST PASS\n");
    return 0;
}