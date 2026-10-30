#include <stddef.h>

typedef unsigned char uint8_t;

class CCharString;

class CThingPlayerCreature
{
public:
    bool LearnExpression(const CCharString& expression, long value, bool immediate);
};

struct CThingPlayerCreature_GiveHeroExpression_Overlay
{
    unsigned char m_Pad00[0x91];
    uint8_t m_Flags91; // 0x91
};

// engine callees (addresses reloc-masked by the parity checker)
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */

class CGameScriptInterface
{
public:
    void GiveHeroExpression(const CCharString& expression, long value, bool immediate) const;

private:
    unsigned char m_Pad00[0x14];
    void* m_Field14; // 0x14
};

void CGameScriptInterface::GiveHeroExpression(
    const CCharString& expression,
    long value,
    bool immediate) const
{
    CThingPlayerCreature* const hero =
        (&((CPlayerManager*)m_Field14)->GetMainPlayer())->GetPControlledCreature();

    if ((hero != 0) &&
        ((reinterpret_cast<const CThingPlayerCreature_GiveHeroExpression_Overlay*>(hero)->m_Flags91 & 0x01) == 0))
    {
        hero->LearnExpression(expression, value, immediate);
    }
}