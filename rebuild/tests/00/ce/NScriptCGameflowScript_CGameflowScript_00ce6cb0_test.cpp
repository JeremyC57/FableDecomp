#include <cstdio>
#include <cstddef>
// Behaviour model of CVtable_12c3fa4::CVtable_12c3fa4(CScriptBase*, long) @ 0x00CE6CB0.
class CScriptBase { public: virtual void v00() {} CScriptBase() {} };
struct SScriptState { long m_stage, m_frame, m_counter; void* m_pThingA; void* m_pThingB; void* m_pThingC;
    SScriptState() : m_stage(0), m_frame(0), m_counter(0), m_pThingA(0), m_pThingB(0), m_pThingC(0) {} };
struct CScriptOwnedBase : CScriptBase { unsigned char _scriptBody[0x3c]; long m_argument; CScriptBase* m_pOwner;
    CScriptOwnedBase(CScriptBase* pOwner, long argument) : CScriptBase(), m_argument(argument), m_pOwner(pOwner) {} };
struct CVtable_12c3fa4 : CScriptOwnedBase { unsigned char _pad[4]; SScriptState m_state; virtual void v00() {}
    CVtable_12c3fa4(CScriptBase* pOwner, long argument) : CScriptOwnedBase(pOwner, argument), m_state() {} };
int main()
{
    CScriptBase owner; CVtable_12c3fa4 s(&owner, 7);
    bool ok = s.m_argument == 7 && s.m_pOwner == &owner && s.m_state.m_stage == 0 && s.m_state.m_counter == 0 && s.m_state.m_pThingA == 0 && s.m_state.m_pThingC == 0;
    if (sizeof(void*) == 4) ok = ok && offsetof(CVtable_12c3fa4, m_argument) == 0x40 && offsetof(CVtable_12c3fa4, m_state) == 0x4c;
    if (!ok) { std::printf("00ce6cb0_TEST FAIL\n"); return 1; }
    std::printf("00ce6cb0_TEST PASS\n");
    return 0;
}