// CThingAICreature::DrawGetThingDisplayInformation(CThingDrawInformation* info) const -- the creature's display bar: the owning player's def colour (opaque) when the creature belongs to a valid player, else opaque black; the bar is the creature's health percentage clamped to 0..1; relative size 0.7; bar on; reports false
// Grade: RELOCATION_MATCH -- the colour written byte by byte (R, G, B, then the opaque alpha) in both arms keeps it a frame record, so the epilogue pops interleave with the bar clamp as retail
// Leaf: ?DrawGetThingDisplayInformation@CThingAICreature
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830CB0, 199 bytes (+9 int3). `ret 4`, bool in al. CThingAICreature vtable 0x0127293C
//         slot 34 (+0x88); CThing's own slot 0x004CD220 is `xor al,al ; ret 4`.
// Identity: Ego_r.pdb ?DrawGetThingDisplayInformation@CThingAICreature@@UBE_NPAVCThingDrawInformation@@@Z -- the only
//         bool(pointer) const virtual the PDB gives CThingAICreature, CThingBuilding and CThingPlayerCreature alike (the same
//         slot-34 bodies 0x00830CB0 / 0x0082D1D0 / 0x006A9D10), and the 20-byte CThingDrawInformation (Ego_r: DrawString +0,
//         RelativeSize +4, BarPercentage +8, Colour +0xC, BarOn +0x10) is exactly the record the body fills. The catalog's
//         "vfunc_34" / landed twin's "GetHighlightInfo_slot34" are descriptive.
// Callees: CThing::PeekPlayerManager() const 0x004C79F0 (Ego_r ?PeekPlayerManager@CThing@@IBEABVCPlayerManager@@XZ; a const
//         member calls the const getters), CPlayerManager::GetPlayer(long) const 0x00449910 (Ego_r QBEABV; the non-const
//         QAEAAV twin is 0x004498C0), CInitBaseClass::IsValid() const 0x0099A330 (`mov al,[ecx+4] ; ret`: Ego_r CInitBaseClass
//         {vptr, bool Valid +4}, ?IsValid@CInitBaseClass@@QBE_NXZ), CThingPhysical::GetHealthPercentage() const 0x006A4E40
//         (`MaxHealth > 0 ? Health / MaxHealth`: Ego_r MaxHealth +0xC8 / Health +0xCC less the 0x18 CThing shrink = +0xB0 /
//         +0xB4), the def's slot 1 (Delete) on the last release. Floats: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// Layout: CThing PlayerNumber +0x90 (signed char); CPlayer::PlayerDef +0x21C (Ego_r +0x224); CPlayerDef::Colour +0x40 (Ego_r
//         +0x44, def classes pack to 1); CRGBColour bytes B, G, R, A.
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"

class CRGBColour { public:                                   // Ego_r.pdb, 4 bytes (B, G, R, A)
    unsigned char B, G, R, A;
};
class CPlayerDef : public CDefClassBase { public:
    unsigned char _pad[0x40 - sizeof(CDefClassBase)];
    CRGBColour    Colour;                                    // +0x40 [Ego_r +0x44]
};
class CInitBaseClass { public:                               // Ego_r.pdb, 8 bytes
    virtual ~CInitBaseClass();
    bool IsValid() const;                                    // 0x0099A330
    bool Valid;                                              // +0x04
};
class CPlayer : public CInitBaseClass { public:
    unsigned char                  _pad08[0x21c - 0x08];
    CDefPointer<const CPlayerDef>  PlayerDef;                // +0x21c [Ego_r +0x224]
};
class CPlayerManager { public:
    const CPlayer& GetPlayer(long number) const;             // 0x00449910
};
class CThingDrawInformation { public:                        // Ego_r.pdb, 20 bytes
    CCharString DrawString;                                  // +0x00
    float       RelativeSize;                                // +0x04
    float       BarPercentage;                               // +0x08
    CRGBColour  Colour;                                      // +0x0c
    bool        BarOn;                                       // +0x10
    bool        StringOn;                                    // +0x11
};
class CThing { public:
    virtual ~CThing();
    unsigned char _pad04[0x90 - 0x04];
    signed char   PlayerNumber;                              // +0x90
protected:
    const CPlayerManager& PeekPlayerManager() const;         // 0x004C79F0
};
class CThingPhysical : public CThing { public:
    float GetHealthPercentage() const;                       // 0x006A4E40
};
class CThingAICreature : public CThingPhysical { public:
    virtual bool DrawGetThingDisplayInformation(CThingDrawInformation* info) const;
};

bool CThingAICreature::DrawGetThingDisplayInformation(CThingDrawInformation* info) const
{
    CRGBColour colour;
    long playerNumber = PlayerNumber;
    const CPlayer* player;
    if (playerNumber != -1 && (player = &PeekPlayerManager().GetPlayer(playerNumber))->IsValid() == true) {
        CDefPointer<const CPlayerDef> def(player->PlayerDef);
        colour.R = def->Colour.R;
        colour.G = def->Colour.G;
        colour.B = def->Colour.B;
        colour.A = 255;
    } else {
        colour.R = 0;
        colour.G = 0;
        colour.B = 0;
        colour.A = 255;
    }
    float health = GetHealthPercentage();
    info->BarPercentage = health;
    if (health < 0.0f)
        info->BarPercentage = 0.0f;
    else if (health > 1.0f)
        info->BarPercentage = 1.0f;
    info->Colour = colour;
    info->BarOn = true;
    info->RelativeSize = 0.7f;
    return false;
}