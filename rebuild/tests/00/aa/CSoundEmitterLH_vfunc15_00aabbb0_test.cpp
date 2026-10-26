#include <cstdio>
#include <new>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

static int g_fail = 0;
static void Check(bool ok) { if (!ok) g_fail = 1; }

class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
class CSoundPlayer {
public:
    int   Calls;
    long  LastHandle;
    float LastX, LastY, LastZ;
    CSoundPlayer() : Calls(0), LastHandle(0), LastX(0), LastY(0), LastZ(0) { }
    virtual ~CSoundPlayer() { }
    virtual void SetPosition(long handle, const C3DVector& p) { ++Calls; LastHandle = handle; LastX = p.X; LastY = p.Y; LastZ = p.Z; }
};
class CSoundSystem {
public:
    CSoundPlayer Player;
    virtual ~CSoundSystem() { }
    virtual CSoundPlayer* GetPlayer() { return &Player; }
};
struct CSoundManagerLH { CSoundSystem* m_pSystem; };
struct CActiveSound { long m_handle; bool m_playing; bool m_stopping; };
struct SSoundNode { SSoundNode* next; SSoundNode* prev; CActiveSound* sound; void* count; };
class CSoundEmitterLH {
public:
    CSoundManagerLH* m_pManager;
    C3DVector        m_position;
    SSoundNode*      m_pSounds;
    bool             m_active;
    CSoundEmitterLH() : m_position(0.0f, 0.0f, 0.0f), m_pSounds(0), m_active(false) { }
    // the row: vtable slot 15 -- move the emitter and every sound that is playing and not stopping
    void SetPosition(const C3DVector& position)
    {
        if (m_pManager->m_pSystem && m_active) {
            m_position = position;
            for (SSoundNode* node = m_pSounds->next; node != m_pSounds; node = node->next) {
                CActiveSound* sound = node->sound;
                if (sound->m_playing && !sound->m_stopping) {
                    C3DVector soundPosition(m_position.X, m_position.Y, m_position.Z);
                    m_pManager->m_pSystem->GetPlayer()->SetPosition(sound->m_handle, soundPosition);
                }
            }
        }
    }
};
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    CSoundSystem system;
    CSoundManagerLH manager;
    manager.m_pSystem = &system;
    CActiveSound sounds[3] = { { 11, true, false }, { 12, true, true }, { 13, false, false } };
    SSoundNode head;
    SSoundNode nodes[3];
    head.next = &nodes[0];
    head.prev = &nodes[2];
    for (int k = 0; k < 3; ++k) {
        nodes[k].sound = &sounds[k];
        nodes[k].count = 0;
        nodes[k].next = k < 2 ? &nodes[k + 1] : &head;
        nodes[k].prev = k > 0 ? &nodes[k - 1] : &head;
    }
    CSoundEmitterLH e;
    e.m_pManager = &manager;
    e.m_pSounds = &head;
    e.SetPosition(C3DVector(1.0f, 2.0f, 3.0f));
    Check(Near(e.m_position.X, 0.0f) && system.Player.Calls == 0);
    e.m_active = true;
    e.SetPosition(C3DVector(1.0f, 2.0f, 3.0f));
    Check(Near(e.m_position.X, 1.0f) && Near(e.m_position.Z, 3.0f));
    Check(system.Player.Calls == 1 && system.Player.LastHandle == 11);
    Check(Near(system.Player.LastX, 1.0f) && Near(system.Player.LastY, 2.0f) && Near(system.Player.LastZ, 3.0f));
    sounds[1].m_stopping = false;
    e.SetPosition(C3DVector(-4.0f, 5.0f, 6.5f));
    Check(system.Player.Calls == 3 && system.Player.LastHandle == 12 && Near(system.Player.LastZ, 6.5f));
    manager.m_pSystem = 0;
    e.SetPosition(C3DVector(9.0f, 9.0f, 9.0f));
    Check(Near(e.m_position.X, -4.0f) && system.Player.Calls == 3);

    if (g_fail) {
        std::printf("00aabbb0_TEST FAIL\n");
        return 1;
    }
    std::printf("00aabbb0_TEST PASS\n");
    return 0;
}