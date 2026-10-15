// Behaviour test for CSecretWhisperingFriend::Init (0x00EC0B00): the interface gets (Me, 1), StartPos becomes Me's position.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CScriptThing {
    C3DVector m_pos;
    virtual ~CScriptThing() {}
    virtual const C3DVector& GetPos() const { return m_pos; }
};
struct CGameScriptInterfaceBase {
    const CScriptThing* m_thing; long m_flag;
    virtual ~CGameScriptInterfaceBase() {}
    virtual void Slot477(const CScriptThing& thing, long flag) { m_thing = &thing; m_flag = flag; }
};
struct CSecretWhisperingFriend {
    CGameScriptInterfaceBase* Game;
    CScriptThing Me;
    C3DVector StartPos;
    void Init()
    {
        Game->Slot477(Me, 1);
        StartPos = Me.GetPos();
    }
};
int main()
{
    CGameScriptInterfaceBase game; game.m_thing = 0; game.m_flag = 0;
    CSecretWhisperingFriend f;
    f.Game = &game;
    f.Me.m_pos.X = 10.0f; f.Me.m_pos.Y = -2.0f; f.Me.m_pos.Z = 0.5f;
    f.StartPos.X = f.StartPos.Y = f.StartPos.Z = 0.0f;
    f.Init();
    if (game.m_thing != &f.Me || game.m_flag != 1 || f.StartPos.X != 10.0f || f.StartPos.Y != -2.0f || f.StartPos.Z != 0.5f) { std::printf("00ec0b00_TEST FAIL\n"); return 1; }
    std::printf("00ec0b00_TEST PASS\n");
    return 0;
}