// NCreatureMode::CModeScorpionKingExhausted::FrameUpdate() -- base update; count the recovery frames down, and once they are spent recover when the animation list is empty or its current animation may be replaced
// Leaf: ?FrameUpdate@CModeScorpionKingExhausted
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E10E0, 110 bytes (+ int3 padding), `ret`; vtable 0x012858A0 slot 3.
// Callees: CModeBase::FrameUpdate @ 0x0073AC50 (qualified), CVectorMap::LowerBound @ 0x0040F020 (inlined HasTC + GetTC, type
//   0x5a = bit 26 of the +0x28 dword = CTCAnimationComplex), CTCAnimationComplex::GetAnimation @ 0x0070B460,
//   CModeScorpionKingExhausted::Recover @ 0x008DF640 (landed).
// Name: FableWin.pdb `?FrameUpdate@CModeScorpionKingExhausted@NCreatureMode@@UAEXXZ` (slot 3).
// Layout: retail CModeScorpionKingExhausted (the landed Recover / ctor 0x008D82B0): RecoveryCount +0x68 (Ego_r.pdb +0x68).
// Shape: `T* p = 0; if (HasTC(k)) p = GetTC<T>(k);` (the `xor ecx,ecx` preload); a non-zero count is decremented and the
//   function returns (if/else), so the recover test sits in the else arm with its own epilogue.
struct CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
    }
};

struct CAnimateThing {
    long          UpdateMode;              // +0x00
    void*         AnimsBegin;              // +0x04
    void*         AnimsEnd;                // +0x08
    unsigned char _pad0c[0x44];
    unsigned char CurrentAnimType;         // +0x50
    unsigned char CurrentAnimFlags;        // +0x51
};
struct CTCAnimationComplex {               // interface type 0x5a
    CAnimateThing& GetAnimation();         // 0x0070B460
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
    void FrameUpdate();                    // 0x0073AC50
};
struct CModeStandBase : CModeBase {        // retail sizeof 0x68
    unsigned char _stand[0x4c];
};

namespace NCreatureMode {
struct CModeScorpionKingExhausted : CModeStandBase {   // vtable 0x012858A0
    long          RecoveryCount;           // +0x68
    unsigned char Filter[0x0c];            // +0x6c CAnimFilterOfGroup
    void*         PCreature;               // +0x78
    bool          bRecovering;             // +0x7c
    void Recover();                        // 0x008DF640
    void FrameUpdate();
};
}

void NCreatureMode::CModeScorpionKingExhausted::FrameUpdate()
{
    CModeBase::FrameUpdate();
    CTCAnimationComplex* tc = 0;
    if (ParentThing->HasTC(0x5a))
        tc = ParentThing->GetTC<CTCAnimationComplex>(0x5a);
    CAnimateThing& animate = tc->GetAnimation();
    if (RecoveryCount)
        --RecoveryCount;
    else if (animate.AnimsBegin == animate.AnimsEnd || (animate.CurrentAnimFlags & 2))
        Recover();
}