#include <cstdio>
#include <cstring>
static char g_seq[32] = "";
class CPlayer { public: unsigned char _pad[0x220]; unsigned char m_firstPersonControl; };
class CPlayerManager { public: CPlayer* m_pCurrent; CPlayer* GetCurrentPlayer() { return m_pCurrent; } };
struct SCameraRecord_0068ea10 { float m_values[14]; float GetAngleXY() const { std::strcat(g_seq, "A"); return m_values[6] + m_values[7]; } };
class CAIGameCameraBase { public: virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { } virtual void v06() { } virtual float GetRelativeMovementAngleXY() { std::strcat(g_seq, "C"); return 2.5f; } };
class CWorld { public:
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { }
    virtual SCameraRecord_0068ea10 GetCameraRecord() { std::strcat(g_seq, "R"); SCameraRecord_0068ea10 r; std::memset(&r, 0, sizeof r); r.m_values[6] = 1.0f; r.m_values[7] = 0.5f; return r; }
    unsigned char _pad04[0x08]; CPlayerManager* PlayerManager; unsigned char _pad10[0x24]; CAIGameCameraBase* PCurrentGameCamera;
    CPlayer* GetCurrentPlayer() { return PlayerManager->GetCurrentPlayer(); }
};
struct CInputProcessGameBase { virtual void v00() { } unsigned char _pad04[0x10]; CWorld* World; void* MyPlayer; void* DefinitionManager; void* Component; void* PGamePlayerInterface; };
class CInputProcessCombat : public CInputProcessGameBase { public:
    float GetCurrentRelativeMovementAngleXY() { if (World->GetCurrentPlayer()->m_firstPersonControl) return World->GetCameraRecord().GetAngleXY(); return World->PCurrentGameCamera->GetRelativeMovementAngleXY(); }
};
int main()
{
    CWorld w; CPlayerManager pm; CPlayer p; CAIGameCameraBase cam; pm.m_pCurrent = &p; w.PlayerManager = &pm; w.PCurrentGameCamera = &cam; CInputProcessCombat c; c.World = &w;
    p.m_firstPersonControl = 1; if (c.GetCurrentRelativeMovementAngleXY() != 1.5f || std::strcmp(g_seq, "RA")) { std::printf("0068ea10_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; p.m_firstPersonControl = 0; if (c.GetCurrentRelativeMovementAngleXY() != 2.5f || std::strcmp(g_seq, "C")) { std::printf("0068ea10_TEST FAIL\n"); return 1; }
    std::printf("0068ea10_TEST PASS\n");
    return 0;
}