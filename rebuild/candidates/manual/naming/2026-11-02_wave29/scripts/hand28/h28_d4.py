# h28_d4.py: naming wave 28, family d4 (DECISIONS_W27_RESOLVED D4) -- the 0x007345D0 / CTCCombat parameter-type family:
#  0x00666B50 ?GetAppropriateCombatAnimationFromList@CThingCreatureBase@@QBEJABV?$vector@PBVCAnimationEntry@@V?$allocator@PBVCAnimationEntry@@@std@@@std@@@Z
#  0x007345D0 ?GetAppropriateCombatAnimationFromList@CTCCombat@@QBEJABV?$vector@PBVCAnimationEntry@@V?$allocator@PBVCAnimationEntry@@@std@@@std@@@Z
#  (Ego_r.pdb 0x00540D20 / 0x0074C070, FableWin.pdb 0x01D9BE30 / 0x024FE600), the callee 0x00669CB0
#  ??$GetComponent@VCAnimComponentCombatHandedness@@@CAnimationEntry@@QBEXPAPBVCAnimComponentCombatHandedness@@@Z (its own row's Leaf),
#  CThing::PeekWorld 0x004C7AB0 / CTCBase::PeekWorld 0x00686A80 (IBE, both PDBs) and CWorld::GetWorldSeed 0x0049D810 (QBE, bound).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
F = 'd4'
L66 = '?GetAppropriateCombatAnimationFromList@CThingCreatureBase@@QBEJABV?$vector@PBVCAnimationEntry@@V?$allocator@PBVCAnimationEntry@@@std@@@std@@@Z'
L73 = '?GetAppropriateCombatAnimationFromList@CTCCombat@@QBEJABV?$vector@PBVCAnimationEntry@@V?$allocator@PBVCAnimationEntry@@@std@@@std@@@Z'

# ---- 0x00666B50 own row
a = '00666b50'
he.edit(a, 'src', '// CThingCreatureBase::GetAppropriateCombatAnimationFromList(const vector<CAnimationDef*>& anims) -- with',
        '// CThingCreatureBase::GetAppropriateCombatAnimationFromList(const vector<const CAnimationEntry*>& anims) const -- with')
he.edit(a, 'src', '''    bool empty() const { return first == last; }
    _Tp size() const { return last - first; }''', '''    bool empty() const { return first == last; }
    unsigned int size() const { return last - first; }''')
he.edit(a, 'src', '''struct SCombatAnimInfo {
    unsigned char _pad00[8];
    long Stance;                                // +0x08
};
struct CAnimationDef {
    void GetCombatInfo(const SCombatAnimInfo** out) const;   // 0x00669CB0
};
struct CAnimationDefVector {
    CAnimationDef** first;
    CAnimationDef** last;
    CAnimationDef** end;
    bool empty() const { return first == last; }
    long size() const { return last - first; }
};
''', '''class CAnimComponentCombatHandedness { public:
    void* m_pVtable;                            // +0x00
    unsigned long IVRefCount;                   // +0x04
    long StartHandedness;                       // +0x08 ECombatAnimationHandedness
};
class CAnimationEntry { public:
    template <class T> void GetComponent(const T** out) const;   // 0x00669CB0 (T = CAnimComponentCombatHandedness)
};
''')
he.edit(a, 'src', '''struct CTCCombatAnimations {
    long SelectAnimation(const CAnimationDefVector& anims);   // 0x007345D0
};
class CWorld { public:
    unsigned long* GetPRandomSeed();            // 0x0049D810
};''', '''class CTCCombat { public:                       // the combat component (interface 0x30)
    long GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>& entries) const;   // 0x007345D0
};
class CWorld { public:
    unsigned long* GetWorldSeed() const;        // 0x0049D810
};''')
he.edit(a, 'src', '''    CInterfaceMap Components;                   // +0x44
    CWorld* GetWorld();                         // 0x004C7AB0''', '''    CInterfaceMap Components;                   // +0x44
protected:
    const CWorld& PeekWorld() const;            // 0x004C7AB0 (Ego_r.pdb / FableWin.pdb IBE)
public:''')
he.edit(a, 'src', '''    long CombatStance;                          // +0x138
    long GetAppropriateCombatAnimationFromList(const CAnimationDefVector& anims);
};

long CThingCreatureBase::GetAppropriateCombatAnimationFromList(const CAnimationDefVector& anims)
{
    {
        CTCCombatAnimations* combat;
        if (GetInterface(&combat, 0x30))
            return combat->SelectAnimation(anims);
    }''', '''    long PreviousActionHandedness;              // +0x138 ECombatAnimationHandedness (retail)
    long GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>& anims) const;
};

long CThingCreatureBase::GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>& anims) const
{
    {
        CTCCombat* combat;
        if (GetInterface(&combat, 0x30))
            return combat->GetAppropriateCombatAnimationFromList(anims);
    }''')
he.edit(a, 'src', '''    for (CAnimationDef** it = anims.first; it != anims.last; ++it, ++index) {
        const SCombatAnimInfo* info;
        (*it)->GetCombatInfo(&info);
        if (info->Stance == CombatStance || CombatStance == 2)''', '''    for (const CAnimationEntry* const* it = anims.first; it != anims.last; ++it, ++index) {
        const CAnimComponentCombatHandedness* info;
        (*it)->GetComponent(&info);
        if (info->StartHandedness == PreviousActionHandedness || PreviousActionHandedness == 2)''')
he.edit_all(a, 'src', '''        unsigned long* seed = GetWorld()->GetPRandomSeed();''', '''        unsigned long* seed = PeekWorld().GetWorldSeed();''', 2)
he.edit(a, 'src', '''// the animation's info getter @ 0x00669CB0 (out pointer), vector<long> grow @ 0x00440990, CThing::GetWorld @ 0x004C7AB0,
// CWorld::GetPRandomSeed @ 0x0049D810,''', '''// CAnimationEntry::GetComponent<CAnimComponentCombatHandedness> @ 0x00669CB0 (out pointer), vector<long> grow @ 0x00440990,
// CThing::PeekWorld @ 0x004C7AB0, CWorld::GetWorldSeed @ 0x0049D810,''')
note(a, F, 'own Leaf is the Ego_r.pdb / FableWin.pdb const member over STLport vector<const CAnimationEntry*>\n'
     '//   (DECISIONS_W27 D4); the combat component (interface 0x30) is CTCCombat and its selector 0x007345D0 is\n'
     '//   CTCCombat::GetAppropriateCombatAnimationFromList; 0x00669CB0 is GetComponent<CAnimComponentCombatHandedness> (own row\'s\n'
     '//   Leaf); 0x004C7AB0 is the protected CThing::PeekWorld (IBE). Was QAE over CAnimationDefVector with CTCCombatAnimations::\n'
     '//   SelectAnimation, CAnimationDef::GetCombatInfo and CThing::GetWorld stand-ins.', leaf=L66)

# ---- 0x007345D0 own row: a CTCCombat member (was CTCCombat_Methods : CTCCombat). engine/CTCCombat.h flattens the CTCBase
#  base (vptr + ParentThing as data words), so the protected CTCBase::PeekWorld cannot be reached from it: the row models
#  CTCBase / CTCCombat locally at the header's offsets (the c696 0x0070B5B0 / 0x00740540 precedent for a flattened vptr).
a = '007345d0'
he.edit(a, 'src', '''//   CTCBase::GetWorld 0x00686A80, CWorld::GetPRandomSeed 0x0049D810, GFROR13 0x00497890, free 0x00BFEA14.''',
        '''//   CTCBase::PeekWorld 0x00686A80, CWorld::GetWorldSeed 0x0049D810, GFROR13 0x00497890, free 0x00BFEA14.''')
he.edit(a, 'src', '''// Layout: CTCCombat (engine/CTCCombat.h): ComboChainPos +0x0C (-777 = none), ComboChainID +0x10, ParentCreature
//   +0x24; retail CThingCreatureBase PreviousActionHandedness +0x138; components: ComboStage +8 / ComboID +0x0C,
//   StartHandedness +8, MeleeFlourish +8 (bool).
#include "engine/CTCCombat.h"
''', '''// Layout: CTCCombat at engine/CTCCombat.h's offsets: ComboChainPos +0x0C (-777 = none), ComboChainID +0x10, ParentCreature
//   +0x24; retail CThingCreatureBase PreviousActionHandedness +0x138; components: ComboStage +8 / ComboID +0x0C,
//   StartHandedness +8, MeleeFlourish +8 (bool). engine/CTCCombat.h / engine/CTCBase.h flatten the CTCBase vptr into a data
//   word, so CTCBase (protected PeekWorld) and CTCCombat are modelled locally, polymorphic, at the headers' offsets.
''')
he.edit(a, 'src', '''class CThingCreatureBase { public:
    unsigned char _pad00[0x138];''', '''class CThing;
class CWorld;
class CCombatManager;
class CTCBase {                                         // vtable 0x0125BE0C (RTTI .?AVCTCBase@@), sizeof 0xc
public:
    virtual ~CTCBase();                                 // 0x00686830
protected:
    const CWorld& PeekWorld() const;                    // 0x00686A80 (Ego_r.pdb / FableWin.pdb IBE)
public:
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
};
class CThingCreatureBase;
class CAnimationEntry;
namespace std { template <class _Tp> class allocator; template <class _Tp, class _Alloc> class vector; }
class CTCCombat : public CTCBase {
public:
    long ComboChainPos;                                 // +0x0c
    long ComboChainID;                                  // +0x10
    long ComboChainGracePeriodCountdown;                // +0x14
    long HandednessGracePeriodCountdown;                // +0x18
    long UnarmedCombatIdleCountdown;                    // +0x1c
    CCombatManager* CombatManager;                      // +0x20
    CThingCreatureBase* ParentCreature;                 // +0x24
    long GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*, std::allocator<const CAnimationEntry*> >& entries) const;
};

class CThingCreatureBase { public:
    unsigned char _pad00[0x138];''')
he.edit(a, 'src', '''class CWorld {
public:
    unsigned long* GetPRandomSeed();                    // 0x0049D810
};''', '''class CWorld {
public:
    unsigned long* GetWorldSeed() const;                // 0x0049D810
};''')
he.edit(a, 'src', '''struct CTCCombat_Methods : CTCCombat {
    CWorld* GetWorld() const;                           // 0x00686A80 (CTCBase)
    __forceinline unsigned long GetRandomIndex(unsigned long range) const
    {
        unsigned long r = GFRandom(GetWorld()->GetPRandomSeed());
        if (range == 0)
            return 0;
        return r % range;
    }
    long GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>& entries) const;
};

long CTCCombat_Methods::GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>& entries) const
{''', '''__forceinline unsigned long GetRandomIndex(const CWorld& world, unsigned long range)
{
    unsigned long r = GFRandom(world.GetWorldSeed());
    if (range == 0)
        return 0;
    return r % range;
}

long CTCCombat::GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>& entries) const
{''')
he.edit_all(a, 'src', 'GetRandomIndex(preferred.size())', 'GetRandomIndex(PeekWorld(), preferred.size())', 1)
he.edit_all(a, 'src', 'GetRandomIndex(handednessMatches.size())', 'GetRandomIndex(PeekWorld(), handednessMatches.size())', 1)
he.edit_all(a, 'src', 'GetRandomIndex(comboMatches.size())', 'GetRandomIndex(PeekWorld(), comboMatches.size())', 1)
note(a, F, 'own row defines the Ego_r.pdb / FableWin.pdb CTCCombat member (DECISIONS_W27 D4; was\n'
     '//   CTCCombat_Methods : CTCCombat); the world comes from the protected CTCBase::PeekWorld 0x00686A80 (DECISIONS_W24 D7;\n'
     '//   was CTCCombat_Methods::GetWorld) and its seed from the bound CWorld::GetWorldSeed 0x0049D810 (QBE).', leaf=L73)

# ---- callers of 0x00666B50: the candidate list is the STLport vector<const CAnimationEntry*> (CCombatActionBase::CombatAnimations,
#  Ego_r.pdb) and the call is the const CThingCreatureBase member.
VEC = '''namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport 4.6 vector, 12 bytes
public:
    _Tp* _M_start;                              // +0x00
    _Tp* _M_finish;                             // +0x04
    _Tp* _M_end_of_storage;                     // +0x08
%s};
}
'''
CALLNOTE = ('0x00666B50 is ?GetAppropriateCombatAnimationFromList@CThingCreatureBase@@QBEJABV?$vector@PBVCAnimationEntry@@..@Z\n'
            '//   (Ego_r.pdb / FableWin.pdb const member, DECISIONS_W27 D4) over the STLport vector<const CAnimationEntry*>. Was %s.')
a = '00856ac0'
he.edit(a, 'src', '''struct CAnimationEntryVector {                  // std::vector<const CAnimationEntry*>
    const CAnimationEntry** m_pFirst;           // +0x00
    const CAnimationEntry** m_pLast;            // +0x04
    const CAnimationEntry** m_pEnd;             // +0x08
    const CAnimationEntry* operator[](long index) const { return m_pFirst[index]; }
};

class CThingCreatureBase_Calls { public:              // engine/CThingCreatureBase.h exists but is not used: only its GetAppropriateCombatAnimationFromList call is used
    long GetAppropriateCombatAnimationFromList(const CAnimationEntryVector& animations);   // 0x00666B50
};
''', VEC % '''    const _Tp& operator[](unsigned int __n) const { return *(_M_start + __n); }
''' + '''
class CThingCreatureBase { public:                    // engine/CThingCreatureBase.h is a data layout without members: only this call is used
    long GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>& animations) const;   // 0x00666B50
};
''')
he.edit(a, 'src', '    CIntelligentPointer<CThingCreatureBase_Calls> PCreature;          // +0x08',
        '    CIntelligentPointer<CThingCreatureBase> PCreature;               // +0x08')
he.edit(a, 'src', '    CAnimationEntryVector CombatAnimations;     // +0xf0', '    std::vector<const CAnimationEntry*> CombatAnimations;   // +0xf0')
note(a, F, CALLNOTE % 'CThingCreatureBase_Calls::..(const CAnimationEntryVector&) (QAE)')

a = '00857520'
he.edit(a, 'src', '''struct CAnimationEntryVector {              // STLport vector<const CAnimationEntry*>
    const CAnimationEntry** m_pFirst;
    const CAnimationEntry** m_pLast;
    const CAnimationEntry** m_pEnd;
    const CAnimationEntry** begin() const { return m_pFirst; }
    const CAnimationEntry* const& operator[](long n) const { return *(begin() + n); }
};
''', VEC % '''    _Tp* begin() const { return _M_start; }
    const _Tp& operator[](unsigned int __n) const { return *(begin() + __n); }
''')
he.edit(a, 'src', '''    long GetAppropriateCombatAnimationFromList(const CAnimationEntryVector& animations);   // 0x00666B50 (CThingCreatureBase)
''', '')
he.edit(a, 'src', '''class CBaseObject;
class CBaseObjectPointer;''', '''class CThingCreatureBase : public CThing { public:
    long GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>& animations) const;   // 0x00666B50
};
class CBaseObject;
class CBaseObjectPointer;''')
he.edit(a, 'src', '    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>',
        '    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08')
he.edit(a, 'src', '    CAnimationEntryVector CombatAnimations; // +0xf0', '    std::vector<const CAnimationEntry*> CombatAnimations; // +0xf0')
note(a, F, CALLNOTE % 'CThing::..(const CAnimationEntryVector&) (QAE); PCreature is the Ego_r\n//   CIntelligentPointer<CThingCreatureBase>')

a = '00667b30'
he.edit(a, 'src', '''    long GetEntryAddress() const { return (long)Entry; }''', '''    const CAnimationEntry* GetEntryAddress() const { return (const CAnimationEntry*)Entry; }''')
he.edit(a, 'src', '''struct SScalarTag { SScalarTag() { } };
struct CRetailVector_dword {                    // STLport vector<const CAnimationEntry*> seen as dwords
    long* first;
    long* last;
    long* end;
    CRetailVector_dword() : first(0), last(0), end(0) { }
    ~CRetailVector_dword() { if (first) free(first); }
    void InsertGrow_005e1420(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x005E1420
    void push_back(const long& value)
    {
        if (last != end) {
            new (last) long(value);
            ++last;
        }
        else
            InsertGrow_005e1420(last, value, SScalarTag(), 1, true);
    }
    const long& operator[](long n) const { return *(first + n); }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (InsertGrow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
__declspec(noinline) void CRetailVector_dword::InsertGrow_005e1420(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    long* p = (long*)GrowVectorStorage_s7(this, count);
    if (p)
        *p = value;
    
}
#pragma auto_inline(on)''', '''namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __true_type { __true_type() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport 4.6 vector<const CAnimationEntry*>, 12 bytes
public:
    _Tp* first;
    _Tp* last;
    _Tp* end;
    vector() : first(0), last(0), end(0) { }
    ~vector() { if (first) free(first); }
protected:
    void _M_insert_overflow(_Tp* where, const _Tp& value, const __true_type& tag, unsigned int count, bool noTail);   // 0x005E1420
public:
    void push_back(const _Tp& value)
    {
        if (last != end) {
            new (last) _Tp(value);
            ++last;
        }
        else
            _M_insert_overflow(last, value, __true_type(), 1, true);
    }
    const _Tp& operator[](unsigned int n) const { return *(first + n); }
};
}
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (_M_insert_overflow, instantiated in retail's TU): reads the value through its
// const reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* where, const _Tp& value, const std::__true_type& tag, unsigned int count, bool noTail)
{
    _Tp* p = (_Tp*)GrowVectorStorage_s7(this, count);
    if (p)
        *p = value;
    
}
#pragma auto_inline(on)''')
he.edit(a, 'src', '''    long GetAppropriateCombatAnimationFromList(const CRetailVector_dword& animations);   // 0x00666B50''',
        '''    long GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>& animations) const;   // 0x00666B50''')
he.edit(a, 'src', '''    CRetailVector_dword candidates;''', '''    std::vector<const CAnimationEntry*> candidates;''')
he.edit(a, 'src', '''    return (const CAnimationEntry*)candidates[index];''', '''    return candidates[index];''')
note(a, F, CALLNOTE % 'CThingCreatureBase::..(const CRetailVector_dword&) (QAE); the candidate\n//   list\'s grow-insert is the own row 0x005E1420\'s ?_M_insert_overflow@?$vector@PBVCAnimationEntry@@V?$allocator@..@std@@@std@@IAEX..@Z\n//   (same-TU body kept; was CRetailVector_dword::InsertGrow_005e1420)')
