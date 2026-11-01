// Behaviour test for 00c61cf0: CPKVoiceHandle::GetEventManager -- context (from the emitter) -> device -> voice table
#include <cstdio>
struct CSoundEventManager { public: int tag; };
class CPKVoiceDevice { public: unsigned char _pad[0x38]; CSoundEventManager* m_pVoiceTable; };
class CPKVoiceContext { public: void* m_00; CPKVoiceDevice* m_pDevice; };
class CAmbientEmitter { public: CPKVoiceContext* m_pContext; };
class CPKVoiceHandle {
public:
    CPKVoiceContext* GetContext(CAmbientEmitter* emitter) { return emitter->m_pContext; }
    CSoundEventManager* GetEventManager(CAmbientEmitter* emitter) { return GetContext(emitter)->m_pDevice->m_pVoiceTable; }
};
int main()
{
    CSoundEventManager t; CPKVoiceDevice d; d.m_pVoiceTable = &t; CPKVoiceContext c; c.m_pDevice = &d; CAmbientEmitter e; e.m_pContext = &c;
    CPKVoiceHandle h;
    int ok = h.GetEventManager(&e) == &t;
    std::printf(ok ? "00c61cf0_TEST PASS\n" : "00c61cf0_TEST FAIL\n");
    return ok ? 0 : 1;
}
