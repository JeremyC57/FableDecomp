// IsCreatureButNotHero(CThing*) -- true for a thing of type 2 (creature, byte +0x10) whose def name is not "CREATURE_HERO"
// Leaf: IsCreatureButNotHero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085EE10, length 41 bytes (+7 int3 padding).
// Callees: CThing::GetDefName @ 0x004C7CC0 (CDefString through the hidden pointer, `ret 4`) and
// CDefString::operator!=(const char*) const @ 0x005FA740 (landed); the literal 0x01231C88 is "CREATURE_HERO".
// Label: the manifest "CTCScriptedControl::IsActive" is propagated. The callers are CTCTargetingPlayer::
// PostProcessTargetedThing (tail jump at 0x00803B4B with ecx = the owner thing at +4) and CTCTargetingPlayer::FrameUpdate
// (0x00805877); the neighbours 0x0085EE90.. (IsPlayHitResponse, IsRolling, IsKnockedDown, ...) are `__fastcall` free
// helpers on a CThing*, so this one is too (named by role).
// Shape: the 4-byte CDefString temporary lives in the `push ecx` slot; the literal is pushed before the GetDefName call
// (right-to-left evaluation of `GetDefName() != "..."`); `if (a && b) return true; return false;` gives the two
// `mov al,1` / `xor al,al` exits.
struct CDefString {                                  // 4-byte def-name handle (user constructor: hidden-pointer return)
    long m_tablePos;
    CDefString();
    bool operator!=(const char* text) const;         // 0x005FA740
};

struct CThing {
    unsigned char _pad[0x10];
    unsigned char m_type;                            // +0x10 (2 = creature)
    CDefString GetDefName() const;                   // 0x004C7CC0
};

bool __fastcall IsCreatureButNotHero(CThing* thing)
{
    if (thing->m_type == 2 && thing->GetDefName() != "CREATURE_HERO")
        return true;
    return false;
}