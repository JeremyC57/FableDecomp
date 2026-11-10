#include <cstdio>
struct CSoundPlayer_00a3e480 {
    long m_state; long m_lastId;
    virtual long GetState(long id) { m_lastId = id; return m_state; }
};
struct CSoundSystem_00a3e480 {
    CSoundPlayer_00a3e480* m_player;
    virtual CSoundPlayer_00a3e480* GetPlayer() { return m_player; }
};
struct CSoundManagerLH {
    CSoundSystem_00a3e480* m_pSystem;
    virtual bool vfunc_49(long id)
    {
        if (m_pSystem) {
            if (m_pSystem->GetPlayer()->GetState(id) == 2)
                return true;
        }
        return false;
    }
};
int main()
{
    CSoundPlayer_00a3e480 p; p.m_state = 2; p.m_lastId = 0;
    CSoundSystem_00a3e480 s; s.m_player = &p;
    CSoundManagerLH m; m.m_pSystem = 0;
    bool ok = !m.vfunc_49(5);
    m.m_pSystem = &s;
    ok = ok && m.vfunc_49(9) && p.m_lastId == 9;
    p.m_state = 1;
    ok = ok && !m.vfunc_49(3);
    if (!ok) { std::printf("00a3e480_TEST FAIL\n"); return 1; }
    std::printf("00a3e480_TEST PASS\n");
    return 0;
}