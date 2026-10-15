// CTCBoastingArea::OnKill() -- when the hero was inside the area (state 2) and is still alive, run the leaving-area handling for the hero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00782A90, length 58 bytes (+ int3 padding).
// Callees: the player-manager accessor @ 0x00686D40 (__cdecl, no arguments: the game singleton
// @ 0x013B8A1C, +0x30; manifest "CViewBase::GetDisplayEngine" is propagated), CPlayerManager::
// GetMainPlayer @ 0x00449970, CPlayer::GetHeroCreature @ 0x00487DC0 (landed names),
// CTCBoastingArea::OnHeroLeavingArea(CThing*) @ 0x00782860.
// Identity: slot 14 of vtable 0x0126B314 = CTCBoastingArea (rtti-vtables.tsv).
// Layout = Ego_r.pdb CTCBoastingArea: HeroWithinRadiusState +0xc (EHeroWithinRadiusState; 2 = inside);
// the hero's dead flag is bit 0 of +0x91 (as the landed 0x006E73B0 / 0x008A0C60).
// Shape: the state is tested twice (zero first, then == 2 after the hero checks), re-read from memory.
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_dead : 1;                               // +0x91 bit 0
};

struct CPlayer {
    CThing* GetHeroCreature();                              // 0x00487DC0
};

struct CPlayerManager {
    CPlayer* GetMainPlayer();                               // 0x00449970
};

CPlayerManager* GetPlayerManager();                         // 0x00686D40

enum EHeroWithinRadiusState { HERO_OUTSIDE = 0, HERO_WITHIN_ANTICIPATION = 1, HERO_WITHIN_RADIUS = 2 };

struct CTCBoastingArea {
    void*                  __vftable;                       // +0x00
    CThing*                ParentThing;                     // +0x04 (CTCBase)
    unsigned char          _pad08[0x4];
    EHeroWithinRadiusState HeroWithinRadiusState;           // +0x0c
    float                  Radius;                          // +0x10
    float                  AnticipationRadius;              // +0x14
    void OnHeroLeavingArea(CThing* hero);                   // 0x00782860
    void OnKill();
};

void CTCBoastingArea::OnKill()
{
    if (HeroWithinRadiusState != HERO_OUTSIDE) {
        CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
        if (hero && !hero->m_dead && HeroWithinRadiusState == HERO_WITHIN_RADIUS)
            OnHeroLeavingArea(hero);
    }
}