// CTCQuestCard::IsOptional() const -- a quest card is optional unless it is a core quest
// Grade: DIFFER(22v22) -- exit-block order: retail `test ; jne F` with the `mov al,1` exit falling through first; VC7.1 lays out `je T ; xor al,al ; ret ; T: mov al,1`. sete form 21v22, `||` form and empty-then inert. 3 grades.
// Leaf: ?IsOptional@CTCQuestCard@@QBE_NXZ
#define CTCQuestCard CTCQuestCardPdb
#include "engine/CTCQuestCard.h"
#undef CTCQuestCard

class CQuestCardDef
{
public:
    unsigned char _base[0x58];
    bool IsCore;                        // +0x58
};

class CTCQuestCard : public CTCQuestCardPdb
{
public:
    const CQuestCardDef* GetDef() const { return (const CQuestCardDef*)PDef_Object; }
    bool IsOptional() const;
};

bool CTCQuestCard::IsOptional() const
{
    if (FeatIndex > -1 || !GetDef()->IsCore)
        return true;
    return false;
}
