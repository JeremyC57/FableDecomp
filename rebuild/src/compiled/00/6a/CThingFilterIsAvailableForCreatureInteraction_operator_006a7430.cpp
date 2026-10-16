// CThingFilter_IsAvailableForCreatureInteraction::operator()(CThing* thing) -- true when thing and SourceThing are both children or both adults, are different things, and thing is available for the interaction Type
// Leaf: ??RCThingFilter_IsAvailableForCreatureInteraction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7430, length 102 bytes.
// Callees: CThingCreatureBase::IsChild 0x00661F70 (thiscall, landed), CThingAICreature::IsAvailableForInteractionType 0x00830320.
// Layout: PDB CThingFilter_IsAvailableForCreatureInteraction { CThingAICreature* SourceThing (+0); ECreatureInteractionType Type (+4); }.
// CThing's 64-bit UID sits at +0x18 (compared low dword then high dword).
// Shape: the child test is `(a && b) || (!a && !b)` with each IsChild call repeated (not CSE'd: the calls are
// opaque); one `&&` return expression materialises every exit as an int; the 64-bit `!=` gives `cmp ; jne ; cmp ; je`; the bool is materialised as an int (`mov eax,1`).
enum ECreatureInteractionType { ECIT_DUMMY = 0 };

struct CThing {
    unsigned char    _pad00[0x18];
    unsigned __int64 UID;                           // +0x18
};

struct CThingCreatureBase : CThing {
    bool IsChild() const;                           // 0x00661F70
};

struct CThingAICreature : CThingCreatureBase {
    bool IsAvailableForInteractionType(ECreatureInteractionType type);   // 0x00830320
};

struct CThingFilter_IsAvailableForCreatureInteraction {
    CThingAICreature*        SourceThing;           // +0x00
    ECreatureInteractionType Type;                  // +0x04
    bool operator()(CThingAICreature* thing) const;
};

bool CThingFilter_IsAvailableForCreatureInteraction::operator()(CThingAICreature* thing) const
{
    return ((thing->IsChild() && SourceThing->IsChild()) || (!thing->IsChild() && !SourceThing->IsChild()))
        && thing->UID != SourceThing->UID
        && thing->IsAvailableForInteractionType(Type);
}