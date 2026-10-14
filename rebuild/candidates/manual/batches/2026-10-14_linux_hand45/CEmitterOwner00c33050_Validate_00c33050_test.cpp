#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[8] = ""; static void* g_owner = 0;
struct CEmitterOwner_00c33050;
struct CValidatable_00c33050 { virtual void Validate() { std::strcat(g_seq, "V"); } };
struct CEmitter_00c33050 { void EditFrameUpdate(CEmitterOwner_00c33050* o) { std::strcat(g_seq, "E"); g_owner = o; } };
struct CEmitterOwner_00c33050 { long m_0; CValidatable_00c33050* m_pTarget; CEmitter_00c33050* m_pEmitter; void Validate() { m_pTarget->Validate(); m_pEmitter->EditFrameUpdate(this); } };
int main() {
    CEmitterOwner_00c33050 o; CValidatable_00c33050 v; CEmitter_00c33050 e; o.m_pTarget = &v; o.m_pEmitter = &e; o.Validate(); if (std::strcmp(g_seq, "VE") || g_owner != &o) { std::printf("00c33050_TEST FAIL\n"); return 1; }
    std::printf("00c33050_TEST PASS\n"); return 0;
}
