// CCombatActionBase::SetCombatAnimation(const CCharString& name) -- empties CombatAnimations and asks the creature for every combat animation possible under that name
// Leaf: SetCombatAnimation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008574D0, length 76 bytes (+ int3 padding).
// Callees: memmove 0x00BFEAE6 (__cdecl), CBaseIntelligentPointer::GetPItem() 0x00A01B50 (on PCreature +8),
//          CThingCreatureBase::GetPossibleCombatAnimations(const CCharString&, vector<const CAnimationEntry*>&) const
//          0x006668A0 (manifest "Animation_FilterAndCompile"; Ego_r PDB ?GetPossibleCombatAnimations@CThingCreatureBase@@QBE_NABVCCharString@@AAV?$vector@PBVCAnimationEntry@@...).
// Catalog label "CTCInGameMenu::SetPDef" is propagated: the body sits right before
// ?Initialise@CCombatActionBase (0x00857520), is called 340 times from CCreatureAction_* Initialise
// bodies with a CCharString temporary, and matches Ego_r ?SetCombatAnimation@CCombatActionBase@@QAEXABVCCharString@@@Z.
// Layout: PCreature (CIntelligentPointer<CThingCreatureBase>, 8 bytes) +0x08, CombatAnimations (the
// retail 12-byte vector<const CAnimationEntry*>) +0xf0 (PDB +0x104; the base class containers are
// each 4 bytes smaller in retail).
// Shape: VC6-era `clear()` = `erase(begin(), end())` = `last = CopyDown(end(), end(), begin())`, the
// copy-down helper written as an early-out (`if (src == srcEnd) return dest;`, GOTCHAS wave 17): both
// operands are the same `last` load, giving retail's `cmp eax,eax ; jne`; the never-taken path
// memmoves the byte difference and adds it to memmove's result. The vector's address is kept in edi
// (the inlined members run on it); the call's arguments are pushed before the GetPItem object fetch.
#include <string.h>

struct CCharString;
struct CAnimationEntry;
struct CAnimationEntryVector;

struct CThingCreatureBase {
    bool GetPossibleCombatAnimations(const CCharString& name, CAnimationEntryVector& anims) const;   // 0x006668A0
};

struct CCreaturePointer {                       // CIntelligentPointer<CThingCreatureBase>
    void*               __vftable;
    void*               m_pItem;
    CThingCreatureBase* GetPItem() const;       // CBaseIntelligentPointer::GetPItem 0x00A01B50
    CThingCreatureBase* operator->() const { return GetPItem(); }
};

struct CAnimationEntryVector {                  // retail vector<const CAnimationEntry*>: three words
    const CAnimationEntry** m_pFirst;
    const CAnimationEntry** m_pLast;
    const CAnimationEntry** m_pEnd;

    static const CAnimationEntry** CopyDown(const CAnimationEntry** src, const CAnimationEntry** srcEnd, const CAnimationEntry** dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned long bytes = (char*)srcEnd - (char*)src;
        return (const CAnimationEntry**)((char*)memmove(dest, src, bytes) + bytes);
    }
    void Erase(const CAnimationEntry** eraseFirst, const CAnimationEntry** eraseLast)
    {
        m_pLast = CopyDown(eraseLast, m_pLast, eraseFirst);
    }
    void clear() { Erase(m_pFirst, m_pLast); }
};

struct CCombatActionBase {
    unsigned char         _pad_0x00[0x08];
    CCreaturePointer      PCreature;            // +0x08
    unsigned char         _pad_0x10[0xe0];
    CAnimationEntryVector CombatAnimations;     // +0xf0 (PDB +0x104)
    void SetCombatAnimation(const CCharString& name);
};

void CCombatActionBase::SetCombatAnimation(const CCharString& name)
{
    CombatAnimations.clear();
    PCreature->GetPossibleCombatAnimations(name, CombatAnimations);
}
