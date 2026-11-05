// CTCQuestCard::IsOptional() const -- a quest card is optional unless it is core: a feat card (FeatIndex > -1) never is,
// otherwise the card def's IsCore flag decides.
// Leaf: ?IsOptional@CTCQuestCard@@QBE_NXZ
// Identity: Ego_r.pdb 0x00723DE0 + FableWin.pdb 0x02554960 (?IsOptional@CTCQuestCard@@QBE_NXZ); Ego_r order IsCore / IsExclusive /
//   IsOptional = retail 0x00702280 / 0x007022A0 / 0x007022C0.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007022C0, length 22 bytes, thiscall `ret`.
// Layout: CTCQuestCard (engine/CTCQuestCard.h, flattened default) PDef +0x0C, FeatIndex +0x14; the def's IsCore is retail
//   +0x58 (Ego_r CQuestCardDef +0x5C: the RewardObjects vector ahead of it is 12 bytes in retail STLport, 16 in Ego_r).
// Shape: IsOptional is `!IsCore()` over the inline IsCore body (the landed out-of-line IsCore 0x00702280 has the same
//   shape: `if (FeatIndex <= -1) { if (IsCore) return true; } return false;`). Written as `if (!IsCore()) return true;
//   return false;`, VC7.1 inverts the inlined branches and keeps retail's block order (`jg true ; jne false ; true: mov al,1 ;
//   ret ; false: xor al,al ; ret`). Every direct spelling of the condition (|| / && / nested ifs / ternary / locals, 48 tried)
//   puts the false tail first. Plain C++ (asm cleanup round 2b; replaces the oracle byte bake).
#define CTCQuestCard CTCQuestCardDonor
#include "engine/CTCQuestCard.h"
#undef CTCQuestCard

class CQuestCardDef {                                     // retail view of the card def (no engine header)
public:
    unsigned char _pad_0x00[0x58];
    bool          IsCore;                                 // +0x58
    bool          IsVignette;                             // +0x59
    bool          IsExclusive;                            // +0x5a
    bool          MakeVignetteRouteAppearOnMinimap;       // +0x5b
};

class CTCQuestCard : public CTCQuestCardDonor {
public:
    bool IsCore() const                                   // 0x00702280 out of line; expanded here
    {
        if (FeatIndex <= -1) {
            if (PeekDef()->IsCore)
                return true;
        }
        return false;
    }
    bool IsOptional() const;
private:
    const CQuestCardDef* PeekDef() const { return reinterpret_cast<const CQuestCardDef*>(PDef_Object); }  // CDefPointer::operator->
};

bool CTCQuestCard::IsOptional() const
{
    if (!IsCore())
        return true;
    return false;
}
