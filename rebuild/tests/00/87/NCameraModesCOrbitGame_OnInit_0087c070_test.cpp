// Behaviour test for 0x0087C070 NCameraModes::COrbitGame::OnInit (source body copied; the out-of-line callees stubbed).
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CInterfaceMap {                               // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                  // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;            // 0x004C73D0
};
class CTCTavernGame { public:
    void*   __vftable;
    CThing* ParentThing;                             // +0x04 (CTCBase)
};
class CTCGamePlayer { public:
    CTCTavernGame* GetGame() const;                  // 0x005EDC90
};
class CPlayer { public:
    CThing* GetHeroThing() const;                    // 0x00487DC0
};
class CWorld { public:
    CPlayer* GetMainPlayer() const;                  // 0x0049D9D0
};
namespace NCameraModes {
class CBase;
}
class CGameCameraManager { public:                          // retail offsets
    unsigned char        _pad000[0x98];
    NCameraModes::CBase* CurrentMode;                // +0x98
};
namespace NCameraModes {
class CBase { public:
    virtual void v00();
    float               DefaultFOV;                  // +0x04
    float               DefaultRollAngle;            // +0x08
    float               DefaultObservedPositionZOffset;   // +0x0c
    unsigned char       LookAt[0x30];                // +0x10 CPhysicsState
    CGameCameraManager* CameraManager;               // +0x40
    void*               PCameraModeDef;              // +0x44
    void*               SubComponents[3];            // +0x48
    unsigned char       GoTo[0x30];                  // +0x54 CPhysicsState
    float               CameraFOV;                   // +0x84
    float               RollAngle;                   // +0x88
    float               ObservedPositionZOffset;     // +0x8c
    unsigned char       _pad90[0xa4 - 0x90];
    const CWorld& GetWorld() const;                  // 0x0084D050
};
struct COrbitGame : CBase {
    C3DVector Focus;                                 // +0xa4
    float     FocusDistance;                         // +0xb0
    float     ZAngle;                                // +0xb4
    float     XAngle;                                // +0xb8
    virtual void OnInit();
};
void COrbitGame::OnInit()
{
    CThing* hero = GetWorld().GetMainPlayer()->GetHeroThing();
    CTCGamePlayer* player;
    hero->GetInterface(&player, 0xd0);
    if (player && player->GetGame()) {
        Focus = player->GetGame()->ParentThing->GetPosition();
        Focus.Z += 1.0;
        FocusDistance = 1.0f;
        ZAngle = 0.0f;
        XAngle = 0.134f;
        const CBase* mode = CameraManager->CurrentMode;
        DefaultFOV = 0.2f;
        ObservedPositionZOffset = mode->ObservedPositionZOffset;
    }
}
}

static CWorld g_world; static CPlayer g_player; static CThing g_hero; static CTCGamePlayer g_gamePlayer;
static CTCTavernGame g_game; static CThing g_gameThing; static C3DVector g_pos;
static CInterfaceMap::SEntry g_entries[3];
static int g_haveGame = 1;
const CWorld& NCameraModes::CBase::GetWorld() const { return g_world; }
CPlayer* CWorld::GetMainPlayer() const { return &g_player; }
CThing* CPlayer::GetHeroThing() const { return &g_hero; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
CTCTavernGame* CTCGamePlayer::GetGame() const { return g_haveGame ? &g_game : 0; }
const C3DVector& CThing::GetPosition() const { return g_pos; }
void NCameraModes::CBase::v00() {}
static float Abs(float x) { return x < 0 ? -x : x; }
int main()
{
    int ok = 1;
    memset(&g_hero, 0, sizeof g_hero);
    g_hero.m_presentBits[0xd0 >> 5] = 1UL << (0xd0 & 31);
    g_entries[0].type = 4;    g_entries[0].component = 0;
    g_entries[1].type = 0xd0; g_entries[1].component = (CTCBase*)&g_gamePlayer;
    g_entries[2].type = 0xff; g_entries[2].component = 0;
    g_hero.m_interfaces.first = g_entries; g_hero.m_interfaces.last = g_entries + 3;
    g_game.ParentThing = &g_gameThing;
    g_pos.X = 10.0f; g_pos.Y = -4.0f; g_pos.Z = 2.5f;
    CGameCameraManager manager; memset(&manager, 0, sizeof manager);
    NCameraModes::COrbitGame previous; memset((char*)&previous + sizeof(void*), 0, sizeof previous - sizeof(void*));
    previous.ObservedPositionZOffset = 0.75f;
    manager.CurrentMode = &previous;
    NCameraModes::COrbitGame mode; memset((char*)&mode + sizeof(void*), 0, sizeof mode - sizeof(void*));
    mode.CameraManager = &manager;
    mode.OnInit();
    if (Abs(mode.Focus.X - 10.0f) > 1e-4f || Abs(mode.Focus.Y + 4.0f) > 1e-4f || Abs(mode.Focus.Z - 3.5f) > 1e-4f) ok = 0;
    if (Abs(mode.FocusDistance - 1.0f) > 1e-6f || mode.ZAngle != 0.0f || Abs(mode.XAngle - 0.134f) > 1e-6f) ok = 0;
    if (Abs(mode.DefaultFOV - 0.2f) > 1e-6f || Abs(mode.ObservedPositionZOffset - 0.75f) > 1e-6f) ok = 0;
    NCameraModes::COrbitGame idle; memset((char*)&idle + sizeof(void*), 0, sizeof idle - sizeof(void*));
    idle.CameraManager = &manager;
    g_haveGame = 0;
    idle.OnInit();                                  // no game: nothing changes
    if (idle.DefaultFOV != 0.0f || idle.FocusDistance != 0.0f || idle.ObservedPositionZOffset != 0.0f) ok = 0;
    printf("0087c070_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}