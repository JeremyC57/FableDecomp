// Grade: DIFFER(354v353) -- frame packing: retail reuses the dead CDefString slot for the first GetInterface key and the distance, and the stats slot for the second key (absent components read that stale slot); ours keeps separate slots, which also rotates eax/edx/ecx
// NCreatureMode::CModeCreatureIsProjectile::OnDeactivate() -- end of a creature being thrown: for a kicked chicken record the throw distance in the hero's longest-kick stat and the online scoreboard, then put the thrown creature's physics back to normal and leave the stand mode
// Leaf: OnDeactivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D90B0, length 353 bytes (padded to 368), `ret`.
// Callees: CGame::GetDefinitionManager through the ILT thunk 0x00415FBC (COMDAT-folded with CGameDefinitionManager::Get 0x0044C6B0,
//   so `this` = the game @ 0x013B86A0 is loaded but unused), CDefinitionManager::GetDefNameFromGlobalIndex @ 0x009ACCC0 (CDefString
//   by value), CDefString::operator==(const char*) @ 0x005B3440 ("BRAIN_CHICKEN" @ 0x0128597C), CPlayerManager::GetMainPlayer
//   @ 0x00449970, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, types 4
//   and 0x10d), CThing::GetPosition @ 0x004C73D0, CTCHeroOnlineScoreboard::UpdateScoreForActivity(long, float, bool) @ 0x0055F3B0,
//   the physics object's virtual slot 68 (+0x110), CModeStandBase::OnDeactivate @ 0x00861D00.
// Layout: landed CModeCreatureIsProjectile::Recover (0x008E08C0): CModeStandBase 0x68, StartThrow +0x68, PCreature +0x84; the
//   creature's brain def (+0x70 object, global def index +0xe8), physics +0x60; the hero's dead bit +0x91 bit 0; the type-4 stats
//   component's longest chicken kick at +0x110; the game's player manager at +0x1c, the player's hero pointer at +0x34.
// Shape: both components come from the out-parameter GetInterface whose result is not tested (an absent component leaves the
//   pointer unset); the distance is the planar sqrt(dx*dx + dy*dy) from the throw start.
#include <math.h>

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

struct C3DVector {
    float X, Y, Z;
};

struct CDefString {
    long m_index;
    CDefString(const CDefString& other) : m_index(other.m_index) { }
    bool operator==(const char* name) const;   // 0x005B3440
};

struct CDefinitionManager {
    CDefString GetDefNameFromGlobalIndex(long index);   // 0x009ACCC0
};

struct CPhysicsObject {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067();
    virtual void SetPhysicsActive(bool active);   // slot 68 (+0x110)
};

struct CBrain {
    unsigned char _pad00[0xe8];
    long          DefGlobalIndex;           // +0xe8
};

struct CThing {
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];       // +0x20 std::bitset of interface types
    CInterfaceMap   m_interfaces;           // +0x44
    unsigned char   _pad4c[0x14];
    CPhysicsObject* PPhysics;               // +0x60
    unsigned char   _pad64[0x0c];
    CBrain*         Brain;                  // +0x70
    unsigned char   _pad74[0x1d];
    unsigned char   m_dead : 1;             // +0x91 bit 0
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
    const C3DVector& GetPosition() const;   // 0x004C73D0
};

struct CBaseIntelligentPointer {
    void* m_pItem;
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CPlayer {
    unsigned char           _pad00[0x34];
    CBaseIntelligentPointer PHero;          // +0x34
    const CBaseIntelligentPointer& GetPHero() const { return PHero; }
};

struct CPlayerManager {
    CPlayer* GetMainPlayer();               // 0x00449970
};

struct CGame {
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;          // +0x1c
    CDefinitionManager* GetDefinitionManager();   // 0x00415FBC (ILT thunk)
};
extern CGame* g_pGame;                      // 0x013B86A0

struct CTCHeroStats {                       // interface type 4
    unsigned char _pad00[0x110];
    float         LongestChickenKick;       // +0x110
};

struct CTCHeroOnlineScoreboard {            // interface type 0x10d
    void UpdateScoreForActivity(long activity, float score, bool increment);   // 0x0055F3B0
};

struct CModeStandBase {                     // retail sizeof 0x68
    virtual void v00();
    CThing*       ParentThing;              // +0x04
    unsigned char _body[0x60];
    void OnDeactivate();                    // 0x00861D00
};

namespace NCreatureMode {

struct CModeCreatureIsProjectile : CModeStandBase {
    C3DVector     StartThrow;               // +0x68
    unsigned char Filter[0x0c];             // +0x74
    unsigned long OldTargetFlags;           // +0x80
    CThing*       PCreature;                // +0x84
    void OnDeactivate();
};

void CModeCreatureIsProjectile::OnDeactivate()
{
    CDefString brain = g_pGame->GetDefinitionManager()->GetDefNameFromGlobalIndex(ParentThing->Brain->DefGlobalIndex);
    if (brain == "BRAIN_CHICKEN") {
        CThing* hero = g_pGame->PlayerManager->GetMainPlayer()->GetPHero().GetPItem();
        if (hero && !hero->m_dead) {
            CTCHeroStats* stats;
            hero->GetInterface(&stats, 4);
            const C3DVector& pos = ParentThing->GetPosition();
            float dx = pos.X - StartThrow.X;
            float dy = pos.Y - StartThrow.Y;
            float distance = (float)sqrt(dx * dx + dy * dy);
            if (stats->LongestChickenKick < distance)
                stats->LongestChickenKick = distance;
            CTCHeroOnlineScoreboard* scoreboard;
            hero->GetInterface(&scoreboard, 0x10d);
            scoreboard->UpdateScoreForActivity(4, distance, false);
        }
    }
    PCreature->PPhysics->SetPhysicsActive(false);
    PCreature->PPhysics->SetPhysicsActive(false);
    CModeStandBase::OnDeactivate();
}

}
