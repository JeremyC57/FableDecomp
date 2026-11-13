// CTCTavernGameCoinGolf::FadeOutTableOccupants(bool restore) -- while the coin-golf table is in use, fade every live human near
//   the camera (2-D distance, 0.4..0.6) and every live coin except the held one (3-D distance to the coin top clamped to camera
//   height, 0.2..0.3) towards transparent so they do not block the shot; restore == true sets them all back to alpha 0xFF.
// Leaf: ?FadeOutTableOccupants@CTCTavernGameCoinGolf@@AAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D46D0, 780 bytes, thiscall, `ret 4`.
// Callees: CTCTavernGameCoinGolf::GetCamera 0x007D3560, malloc 0x00BFEA0E / free 0x00BFEA14 (inline std::list ctor/dtor),
//   CTCBase::PeekThingSearchTools 0x00686D90, CThingSearchTools::GetAllThings<CThingCreatureBase,list,CThingFilter_IsHuman>
//   0x007D4CE0 (same TU, out of line; its body is visible so the list head stays in a register: FABLE_RETAIL_CTHINGSEARCHTOOLS_
//   ALLTHINGSBODY + _ALLTHINGSBODY_TYPEMASK copy the landed 0x007D4CE0 row -- PeekClassList, the scan test, the CThingCreatureBase
//   THING_TYPE mask test 0xF, the filter, push_back), CThing::GetPos 0x004C73D0, CBaseIntelligentPointer::GetPItem 0x00A01B50, _ftol2 0x00BFEA70,
//   CVectorMap::LowerBound 0x0040F020 (inline GetTC), NGraphicAppearance::CTCGraphicAppearance::SetAlpha 0x004BFAC0.
// Shape: lookAt lives in its own block (its slot is reused for the coin position copy); the coin top is written back into
//   pos.Z through std::min (by-reference select), then the 3-D distance; `0.6f < dist` / `dist < 0.4f` comparison forms.
//   Classes (Ego_r.pdb chain CTCTavernGameCoinGolf : CTCTavernGameCoinBase : CTCTavernGame): the shared CTCTavernGame header
//   (retail sizeof 0xF0); CTCTavernGameCoinBase has no shared header, so a local class with its PDB members Arrow/Arrow1/Arrow2
//   (+0xF0/F4/F8), Coins (+0xFC), Held (+0x108), BoardPos (+0x10C) -- Ego_r offsets less 0x18 (CTCTavernGame 264 -> 0xF0), and
//   less 0x1C after Coins (Ego_r's vector is 16 bytes, retail STLport's 12); the PDB
//   Coin record (60 bytes, Thing at +0x25, pack(1)).
#include <math.h>
#include "fable_compiler.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_TYPEMASK
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETALPHA
#include "engine/CThingCreatureBase.h"
#include "engine/CTCBase.h"
#include "engine/CTCTavernGame.h"
#include "engine/CVectorMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "stlport/fable_containers.h"

// Both PDBs: ??RCThingFilter_IsHuman@@QBE_NABVCThingCreatureBase@@@Z (Ego_r 0x005A3150, FableWin 0x01F95C80).
class CThingFilter_IsHuman { public: bool operator()(const CThingCreatureBase& creature) const; };   // 0x006A7B90

class CTCTavernGameCoinBase : public CTCTavernGame {      // no shared header: PDB members up to BoardPos, retail offsets
public:                                                  // (Ego_r offsets minus 0x18: CTCTavernGame 264 -> 0xF0; minus 0x1C after the vector)
#pragma pack(push, 1)
    struct Coin {
        unsigned char _pad00[0x24];
        bool Owned;                                 // +0x24
        CIntelligentPointer<CThing> Thing;          // +0x25
        unsigned char _pad2d[0x3c - 0x2d];
    };
#pragma pack(pop)
    CThingObject* Arrow;                            // +0xF0  (Ego_r +264)
    CThingObject* Arrow1;                           // +0xF4  (Ego_r +268)
    CThingObject* Arrow2;                           // +0xF8  (Ego_r +272)
    std::vector<Coin> Coins;                        // +0xFC  (Ego_r +276)
    Coin* Held;                                     // +0x108 (Ego_r +292; vector 16 -> 12 bytes)
    C3DVector BoardPos;                             // +0x10C (Ego_r +296)
};

class CTCTavernGameCoinGolf : public CTCTavernGameCoinBase {
public:
    void GetCamera(C3DVector& camPos, C3DVector& lookAt);   // 0x007D3560
private:
    void FadeOutTableOccupants(bool restore);
};

void CTCTavernGameCoinGolf::FadeOutTableOccupants(bool restore)
{
    C3DVector camPos;
    {
        C3DVector lookAt;
        GetCamera(camPos, lookAt);
    }
    if (!ParentThing->HasTC(TCI_TAVERN_TABLE))
        return;
    std::list<CThingCreatureBase*> creatures;
    CThingFilter_IsHuman filter;
    PeekThingSearchTools().GetAllThings<CThingCreatureBase>(filter, creatures);
    for (std::list<CThingCreatureBase*>::iterator it = creatures.begin(); it != creatures.end(); ++it) {
        CThingCreatureBase* creature = *it;
        if (creature && !creature->IsDead()) {
            const C3DVector& pos = creature->GetPos();
            float dx = pos.X - camPos.X;
            float dy = pos.Y - camPos.Y;
            float dist = (float)sqrt(dx * dx + dy * dy);
            unsigned char alpha;
            if (restore || 0.6f < dist)
                alpha = 0xff;
            else if (dist < 0.4f)
                alpha = 0;
            else
                alpha = (unsigned char)((dist - 0.4f) * 1274.9998779296875f);
            NGraphicAppearance::CTCGraphicAppearance* tc;
            if (creature->GetTC(TCI_GRAPHIC_APPEARANCE_NEW, &tc))
                tc->SetAlpha(alpha);
        }
    }
    for (unsigned i = 0; i < Coins.size(); ++i) {
        CThing* thing = Coins[i].Thing.Get();
        if (thing && !thing->IsDead() && &Coins[i] != Held) {
            C3DVector pos = thing->GetPos();
            pos.Z = std::min(pos.Z + 0.4f, camPos.Z);
            float dx = pos.X - camPos.X;
            float dy = pos.Y - camPos.Y;
            float dz = pos.Z - camPos.Z;
            float dist = (float)sqrt(dx * dx + dy * dy + dz * dz);
            unsigned char alpha;
            if (restore || 0.3f < dist)
                alpha = 0xff;
            else if (dist < 0.2f)
                alpha = 0;
            else
                alpha = (unsigned char)((dist - 0.2f) * 2549.999755859375f);
            NGraphicAppearance::CTCGraphicAppearance* tc;
            if (thing->GetTC(TCI_GRAPHIC_APPEARANCE_NEW, &tc))
                tc->SetAlpha(alpha);
        }
    }
}