// Behaviour test for 00c61cf0: CPKVoiceHandle::GetTable -- context (from the emitter) -> device -> voice table
#include <cstdio>
class CPKVoiceTable { public: int tag; };
class CPKVoiceDevice { public: unsigned char _pad[0x38]; CPKVoiceTable* m_pVoiceTable; };
class CPKVoiceContext { public: void* m_00; CPKVoiceDevice* m_pDevice; };
class CPKEmitter { public: CPKVoiceContext* m_pContext; };
class CPKVoiceHandle {
public:
    CPKVoiceContext* GetContext(CPKEmitter* emitter) { return emitter->m_pContext; }
    CPKVoiceTable* GetTable(CPKEmitter* emitter) { return GetContext(emitter)->m_pDevice->m_pVoiceTable; }
};
int main()
{
    CPKVoiceTable t; CPKVoiceDevice d; d.m_pVoiceTable = &t; CPKVoiceContext c; c.m_pDevice = &d; CPKEmitter e; e.m_pContext = &c;
    CPKVoiceHandle h;
    int ok = h.GetTable(&e) == &t;
    std::printf(ok ? "00c61cf0_TEST PASS\n" : "00c61cf0_TEST FAIL\n");
    return ok ? 0 : 1;
}
