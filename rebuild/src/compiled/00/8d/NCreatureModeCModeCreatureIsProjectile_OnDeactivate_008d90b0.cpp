// NCreatureMode::CModeCreatureIsProjectile::OnDeactivate() -- end of a creature being thrown: for a kicked chicken record the throw distance in the hero's longest-kick stat and the online scoreboard, then put the thrown creature's physics back to normal and leave the stand mode
// Leaf: OnDeactivate
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): a same-TU noinline CInterfaceMap::LowerBound stand-in (GOTCHAS chunk
//   240), the brain def index read into a named local before GetDefNameFromGlobalIndex, and the kick distance computed
//   in its own block (the position/delta temporaries die before the scoreboard key) give retail's frame packing and temp
//   registers.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D90B0, length 353 bytes (padded to 368), `ret`.
// Callees: CMainGameComponent::PeekDefinitionManager through the ILT thunk 0x00415FBC (COMDAT-folded with CGameDefinitionManager::Get 0x0044C6B0,
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

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
// Same-TU stand-in for CVectorMap::LowerBound (not the retail body; GOTCHAS chunk 240 / 008328d0 idiom).
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) 
{
    CInterfaceMap::SEntry* it = first;
    long count = last - it;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) { it += half + 1; count -= half + 1; }
        else count = half;
    }
    return it;
}
#pragma auto_inline(on)

class C3DVector { public:
    float X, Y, Z;
};

class CDefString {
public:
    long m_index;
    CDefString(const CDefString& other) : m_index(other.m_index) { }
    bool operator==(const char* name) const;   // 0x005B3440
};

class CDefinitionManager {
public:
    CDefString GetDefNameFromGlobalIndex(long index) const;   // 0x009ACCC0 (Ego_r.pdb ?GetDefNameFromGlobalIndex@CDefinitionManager@@QBE?AVCDefString@@J@Z)
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

class CThing { public:
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

class CPlayer { public:
    unsigned char           _pad00[0x34];
    CBaseIntelligentPointer PHero;          // +0x34
    const CBaseIntelligentPointer& GetPHero() const { return PHero; }
};

class CPlayerManager { public:
    CPlayer* GetMainPlayer();               // 0x00449970
};

class CGameDefinitionManager : public CDefinitionManager { };
class CMainGameComponent {
public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;          // +0x1c
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x00415FBC (ILT thunk; Ego_r.pdb ?PeekDefinitionManager@CMainGameComponent@@QBEABVCGameDefinitionManager@@XZ)
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

class CTCHeroStats { public:                       // interface type 4
    unsigned char _pad00[0x110];
    float         LongestChickenKick;       // +0x110
};

class CTCHeroOnlineScoreboard { public:            // interface type 0x10d
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
    long index = ParentThing->Brain->DefGlobalIndex;
    CDefString brain = g_pMainGameComponent->PeekDefinitionManager().GetDefNameFromGlobalIndex(index);
    if (brain == "BRAIN_CHICKEN") {
        CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetPHero().GetPItem();
        if (hero && !hero->m_dead) {
            CTCHeroStats* stats;
            hero->GetInterface(&stats, 4);
            float distance;
            {
                const C3DVector& pos = ParentThing->GetPosition();
                float dx = pos.X - StartThrow.X;
                float dy = pos.Y - StartThrow.Y;
                distance = (float)sqrt(dx * dx + dy * dy);
            }
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