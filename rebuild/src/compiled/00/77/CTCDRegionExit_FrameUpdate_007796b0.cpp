// CTCDRegionExit::FrameUpdate() -- while the exit is active and connected: when the live hero walks inside the exit radius
// move it through the exit; inside the message radius start the letterbox and show the destination region's name, and on
// leaving it end the letterbox and finish the region name; re-arm the waiting flags once the hero is outside the radii
// Leaf: FrameUpdate
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
//
// Retail 0x007796B0, 598 bytes (plain `ret`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCBase::GetPlayerManager @ 0x00686D40, CPlayerManager::GetMainPlayer
// @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, CThing::GetPos @ 0x004C73D0 (twice), CDisplayEngine::StartLetterBoxMode
// (float, float, float) @ 0x00434BA0 / EndLetterBox @ 0x004349D0 (CMainGameComponent::Get() 0x013B86A0 +0x28), CTCBase::GetWorld
// const @ 0x00686A80 (byte +0xdf), CVectorMap::LowerBound @ 0x0040F020 (CTCRegionDisplay 0x8e on the hero),
// CTCDRegionExit::GetDisplayName @ 0x00779260, CTCRegionDisplay::SetCurrentAreaName @ 0x00639BA0 / FinishShowingRegionName
// @ 0x00639930, CTCDRegionExit::MoveHeroThroughExit @ 0x00779080; (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790 (byte +0xf6), epsilon 0x0129BA3C.
// Layout = Ego_r.pdb CTCDRegionExit (packed, as the landed MoveHeroThroughExit 0x00779080); CThing dead bit +0x91, +0xb4 float.
#include "fable_compiler.h"
#include "engine/C3DVector.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Dead : 1;                                  // +0x91 bit 0
    unsigned char _pad92[0xb4 - 0x92];
    float         Health;                                    // +0xb4
    const C3DVector& GetPos() const;                    // 0x004C73D0
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> FABLE_FORCEINLINE T* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(key);
        return (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
    }
};
class CThingPlayerCreature : public CThing { };
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };                  // 0x00487DC0
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
         // 0x00449970
class CCharString { public: void* PStringData; };
class CDisplayEngine { public:
    void StartLetterBoxMode(float a, float b, float aspect); // 0x00434BA0
    void EndLetterBox();                                     // 0x004349D0
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* DisplayEngine;                           // +0x28
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    unsigned char _pad00[0xf6];
    bool          Busy;                                      // +0xf6
};
class CWorld { public:
    unsigned char _pad00[0xdf];
    bool          ShowRegionNames;                           // +0xdf
};
class CTCRegionDisplay { public:
    void SetCurrentAreaName(const CCharString& name, float time, bool a, bool b, bool c);   // 0x00639BA0
    void FinishShowingRegionName();                          // 0x00639930
};
extern const float DELTA_NOTIONAL_ZERO;           // 0x0129BA3C (1e-4f)
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                     // +0x04
    bool    ToBeRemoved;                                     // +0x08
    bool    IsInGlobalUpdateTCs;                             // +0x09
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
    const CWorld& PeekWorld() const;   // 0x00686A80
};
#pragma pack(push, 1)
class CTCDRegionExit : public CTCBase {
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
    CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (!hero || hero->Dead)
        return;
    bool moveHero = false;
    const C3DVector& exitPos = ParentThing->GetPos();
    const C3DVector& heroPos = hero->GetPos();
    float dx = heroPos.X - exitPos.X;
    float dy = heroPos.Y - exitPos.Y;
    float dz = heroPos.Z - exitPos.Z;
    float distSq = dz * dz + dy * dy + dx * dx;
    bool inside = distSq < Radius * Radius;
    if (!WaitingForActivationOrPlayerToLeave && inside) {
        moveHero = true;
    } else if (MessageRadius > DELTA_NOTIONAL_ZERO && DELTA_NOTIONAL_ZERO + Radius < MessageRadius
               && distSq < MessageRadius * MessageRadius) {
        if (!WaitingForPlayerToLeaveMessageArea) {
            if (!InLetterBoxMode) {
                CMainGameComponent::Get()->DisplayEngine->StartLetterBoxMode(0.5f, 0.5f, 1.7777778f);
                if (PeekWorld().ShowRegionNames) {
                    CTCRegionDisplay* display;
                    if (hero->HasTC(0x8e))
                        display = hero->GetTC<CTCRegionDisplay>(0x8e);
                    display->SetCurrentAreaName(GetDisplayName(), 0.0f, false, false, true);
                }
            }
            InLetterBoxMode = true;
        }
    } else if (InLetterBoxMode && !CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().Busy) {
        CMainGameComponent::Get()->DisplayEngine->EndLetterBox();
        if (PeekWorld().ShowRegionNames) {
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
    if (moveHero && hero->Health > DELTA_NOTIONAL_ZERO) {
        MoveHeroThroughExit();
        WaitingForActivationOrPlayerToLeave = true;
    }
}