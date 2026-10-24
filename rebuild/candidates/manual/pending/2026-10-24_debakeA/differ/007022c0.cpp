// Grade: DIFFER(22v22) -- exit order only: retail lays the shared `mov al,1` exit right after the two tests (`jg true ; ... jne false ; true: mov al,1 ; ret ; false: xor al,al`), every form here puts the `xor al,al` exit first (`jg true ; ... je true ; xor al,al ; ret ; true: mov al,1`). Tried: !IsCore() (sete), nested ifs with swapped returns, early `return true` (sete), `a || !b`, `IsCore() ? false : true` (6 grades). Next: `if (!(a && b)) return true; return false;` or an inline bool IsCoreFlag() accessor on the def.
// CTCQuestCard::IsOptional() const -- a quest card is optional unless it is a core quest
// Retail 0x007022C0, 22 bytes. PDB declaration order (Ego_r.pdb) is IsCore, IsExclusive,
// IsOptional, IsVignette, IsRouteToAppearOnMinimap; the retail bodies sit in that order at
// 0x00702280 (reads def +0x58 IsCore), 0x007022A0 (def +0x5A IsExclusive), 0x007022C0 (this).
// IsOptional re-reads the same IsCore flag as 0x00702280 with the senses inverted, i.e. it is
// the negation of the inlined IsCore():
//   FeatIndex (+0x14) <= -1 and the card def's IsCore set -> not optional, else optional.
// Shape: the shared `mov al,1` exit sits right after the fall-through as in IsCore, the
// `xor al,al` arm last; the bool narrows to `mov al` (no `xor eax,eax`).
// CQuestCardDef sits 4 bytes below its Ego_r.pdb offsets in retail (PDB IsCore +0x5C); the def
// is reached through the card's CDefPointer object word (+0x0C) with a local retail view.
// Leaf: IsOptional

#include "engine/CTCQuestCard.h"

struct CQuestCardDefView                // retail CQuestCardDef (PDB offset - 4)
{
    unsigned char _base[0x58];          // +0x00..+0x57 CDefClassBase / earlier members
    bool IsCore;                        // +0x58
    bool IsVignette;                    // +0x59
    bool IsExclusive;                   // +0x5A
};

class CTCQuestCardQueries : public CTCQuestCard
{
public:
    const CQuestCardDefView* GetDef() const { return (const CQuestCardDefView*)PDef_Object; }
    bool IsCore() const
    {
        if (FeatIndex <= -1)
        {
            if (GetDef()->IsCore)
                return true;
        }
        return false;
    }
    bool IsOptional() const;
};

bool CTCQuestCardQueries::IsOptional() const
{
    if (FeatIndex <= -1)
    {
        if (GetDef()->IsCore)
            return false;
    }
    return true;
}
