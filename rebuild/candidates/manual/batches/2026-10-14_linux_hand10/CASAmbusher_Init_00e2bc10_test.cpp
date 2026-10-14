#include <cstdio>
// Model of CAS_Ambusher::Init @ 0x00E2BC10: TransferState(&m_state, block) then TransferState(block, &m_state).
struct S { int v; };
struct Owner {
    S block; int calls; S* to[2]; S* from[2];
    virtual S* GetPStateBlock() { return &block; }
    virtual void TransferState(S* t, S* f) { to[calls] = t; from[calls] = f; ++calls; t->v = f->v; }
};
struct CAS_Ambusher { Owner* m_pOwner; S m_state; void Init() { m_pOwner->TransferState(&m_state, m_pOwner->GetPStateBlock()); m_pOwner->TransferState(m_pOwner->GetPStateBlock(), &m_state); } };
int main()
{
    Owner o; o.block.v = 42; o.calls = 0; CAS_Ambusher a; a.m_pOwner = &o; a.m_state.v = 0;
    a.Init();
    if (o.calls != 2 || o.to[0] != &a.m_state || o.from[0] != &o.block || o.to[1] != &o.block || o.from[1] != &a.m_state || a.m_state.v != 42) { std::printf("00e2bc10_TEST FAIL\n"); return 1; }
    std::printf("00e2bc10_TEST PASS\n"); return 0;
}
