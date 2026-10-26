// CTCDRegionExit::FrameUpdate() -- while the exit is active and connected: when the live hero walks inside the exit radius
// move it through the exit; inside the message radius start the letterbox and show the destination region's name, and on
// leaving it end the letterbox and finish the region name; re-arm the waiting flags once the hero is outside the radii
// Leaf: FrameUpdate
//
// Retail 0x007796B0, 598 bytes (plain `ret`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCBase::GetPlayerManager @ 0x00686D40, CPlayerManager::GetMainPlayer
// @ 0x00449970, CPlayer::GetHeroThing @ 0x00487DC0, CThing::GetPosition @ 0x004C73D0 (twice), CDisplayEngine::StartLetterBoxMode
// (float, float, float) @ 0x00434BA0 / EndLetterBox @ 0x004349D0 (g_pMainGameComponent 0x013B86A0 +0x28), CTCBase::GetWorld
// const @ 0x00686A80 (byte +0xdf), CVectorMap::LowerBound @ 0x0040F020 (CTCRegionDisplay 0x8e on the hero),
// CTCDRegionExit::GetDisplayName @ 0x00779260, CTCRegionDisplay::SetCurrentAreaName @ 0x00639BA0 / FinishShowingRegionName
// @ 0x00639930, CTCDRegionExit::MoveHeroThroughExit @ 0x00779080; g_pPlayerGui 0x013B8790 (byte +0xf6), epsilon 0x0129BA3C.
// Layout = Ego_r.pdb CTCDRegionExit (packed, as the landed MoveHeroThroughExit 0x00779080); CThing dead bit +0x91, +0xb4 float.
class C3DVector { public: float X, Y, Z; };
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                                  // +0x91 bit 0
    unsigned char _pad92[0xb4 - 0x92];
    float         Health;                                    // +0xb4
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
class CBaseIntelligentPointer {
public:
    virtual void v00();
    void* PData;
    CThing* GetPItem() const;                                // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CPlayer { public: CThing* GetHeroThing(); };                  // 0x00487DC0
class CPlayerManager { public: CPlayer* GetMainPlayer(); };         // 0x00449970
class CCharString { public: void* PStringData; };
class CDisplayEngine { public:
    void StartLetterBoxMode(float a, float b, float aspect); // 0x00434BA0
    void EndLetterBox();                                     // 0x004349D0
};
class CMainGameComponent { public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* DisplayEngine;                           // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;             // 0x013B86A0
class CPlayerGui { public:
    unsigned char _pad00[0xf6];
    bool          Busy;                                      // +0xf6
};
extern CPlayerGui* g_pPlayerGui;                             // 0x013B8790
class CWorld { public:
    unsigned char _pad00[0xdf];
    bool          ShowRegionNames;                           // +0xdf
};
class CTCRegionDisplay { public:
    void SetCurrentAreaName(const CCharString& name, float time, bool a, bool b, bool c);   // 0x00639BA0
    void FinishShowingRegionName();                          // 0x00639930
};
extern const float g_epsilon_0129ba3c;                       // 0x0129BA3C (1e-4f)
class CTCBaseModel {
public:
    virtual ~CTCBaseModel();
    CThing* ParentThing;                                     // +0x04
    bool    ToBeRemoved;                                     // +0x08
    bool    IsInGlobalUpdateTCs;                             // +0x09
    const CWorld* GetWorld() const;                          // 0x00686A80
    CPlayerManager* GetPlayerManager();                      // 0x00686D40
};
#pragma pack(push, 1)
class CTCDRegionExit : public CTCBaseModel {
public:
    bool  Active;                                            // +0x0c
    bool  InLetterBoxMode;                                   // +0x0d
    bool  WaitingForConfirmation;                            // +0x0e
    bool  WaitingForActivationOrPlayerToLeave;               // +0x0f
    bool  WaitingForPlayerToLeaveMessageArea;                // +0x10
    unsigned char _pad11[3];
    float Radius;                                            // +0x14
    float MessageRadius;                                     // +0x18
    unsigned char _pad1c[4];
    unsigned __int64 EntranceConnectedToUID;                 // +0x20
    bool  ReversedOnMiniMap;                                 // +0x28
    bool  HiddenOnMiniMap;                                   // +0x29
    CIntelligentPointer<CThing> PEntranceConnectedTo;        // +0x2a
    const CCharString& GetDisplayName() const;               // 0x00779260
    void MoveHeroThroughExit();                              // 0x00779080
    void FrameUpdate();
};
#pragma pack(pop)

void CTCDRegionExit::FrameUpdate()
{
    if (!Active)
        return;
    if (!PEntranceConnectedTo.GetPItem())
        return;
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
    if (!hero || hero->Dead)
        return;
    bool moveHero = false;
    const C3DVector& exitPos = ParentThing->GetPosition();
    const C3DVector& heroPos = hero->GetPosition();
    float dx = heroPos.X - exitPos.X;
    float dy = heroPos.Y - exitPos.Y;
    float dz = heroPos.Z - exitPos.Z;
    float distSq = dz * dz + dy * dy + dx * dx;
    bool inside = distSq < Radius * Radius;
    if (!WaitingForActivationOrPlayerToLeave && inside) {
        moveHero = true;
    } else if (MessageRadius > g_epsilon_0129ba3c && g_epsilon_0129ba3c + Radius < MessageRadius
               && distSq < MessageRadius * MessageRadius) {
        if (!WaitingForPlayerToLeaveMessageArea) {
            if (!InLetterBoxMode) {
                g_pMainGameComponent->DisplayEngine->StartLetterBoxMode(0.5f, 0.5f, 1.7777778f);
                if (GetWorld()->ShowRegionNames) {
                    CTCRegionDisplay* display;
                    if (hero->HasTC(0x8e))
                        display = hero->GetTC<CTCRegionDisplay>(0x8e);
                    display->SetCurrentAreaName(GetDisplayName(), 0.0f, false, false, true);
                }
            }
            InLetterBoxMode = true;
        }
    } else if (InLetterBoxMode && !g_pPlayerGui->Busy) {
        g_pMainGameComponent->DisplayEngine->EndLetterBox();
        if (GetWorld()->ShowRegionNames) {
            CTCRegionDisplay* display;
            if (hero->HasTC(0x8e))
                display = hero->GetTC<CTCRegionDisplay>(0x8e);
            display->FinishShowingRegionName();
        }
        InLetterBoxMode = false;
    }
    if (!inside)
        WaitingForActivationOrPlayerToLeave = false;
    if (distSq > MessageRadius * MessageRadius)
        WaitingForPlayerToLeaveMessageArea = false;
    if (moveHero && hero->Health > g_epsilon_0129ba3c) {
        MoveHeroThroughExit();
        WaitingForActivationOrPlayerToLeave = true;
    }
}