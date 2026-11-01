// CCombatActionBase::SetupCombatAnimation() -- let the creature pick an entry from CombatAnimations and play it from -1 to 1
// Naming wave 28 (family d4): 0x00666B50 is ?GetAppropriateCombatAnimationFromList@CThingCreatureBase@@QBEJABV?$vector@PBVCAnimationEntry@@..@Z
//   (Ego_r.pdb / FableWin.pdb const member, DECISIONS_W27 D4) over the STLport vector<const CAnimationEntry*>. Was CThingCreatureBase_Calls::..(const CAnimationEntryVector&) (QAE).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00856AC0, length 52 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CThingCreatureBase::
// GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>&) @ 0x00666B50 (returns the
// index), CCreatureActionBase::SetAnimation(CAnimationEntry*, float, float) @ 0x00693030 (-1.0f, 1.0f).
// CombatAnimations is the vector whose first-pointer sits at +0xf0 (Ego_r.pdb name; +0x104 in the PDB build;
// retail's CCombatActionBase is 0x114 bytes, modelled locally).
// Shape: the vector address is pushed (edi) before the creature is fetched, the index call's result indexes
// the vector's first pointer (`mov ecx,[edi] ; mov edx,[ecx+eax*4]`), then the two float immediates and the
// entry are pushed right to left.
class CAnimationEntry;

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport 4.6 vector, 12 bytes
public:
    _Tp* _M_start;                              // +0x00
    _Tp* _M_finish;                             // +0x04
    _Tp* _M_end_of_storage;                     // +0x08
    const _Tp& operator[](unsigned int __n) const { return *(_M_start + __n); }
};
}

class CThingCreatureBase { public:                    // engine/CThingCreatureBase.h is a data layout without members: only this call is used
    long GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>& animations) const;   // 0x00666B50
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;               // +0x08
    unsigned char _pad10[0xf0 - 0x10];          // +0x10
    std::vector<const CAnimationEntry*> CombatAnimations;   // +0xf0
    unsigned char _padfc[0x114 - 0xfc];         // +0xfc
    void SetAnimation(const CAnimationEntry* entry, float start, float end);   // 0x00693030 (CCreatureActionBase)
    void SetupCombatAnimation();
};

void CCombatActionBase::SetupCombatAnimation()
{
    SetAnimation(CombatAnimations[PCreature.GetPItem()->GetAppropriateCombatAnimationFromList(CombatAnimations)], -1.0f, 1.0f);
}