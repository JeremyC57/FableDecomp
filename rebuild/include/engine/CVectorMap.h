#ifndef FABLE_ENGINE_CVECTORMAP_H
#define FABLE_ENGINE_CVECTORMAP_H
#include "fable_compiler.h"
// CVectorMap<K,V,CompareType> (Codex pass 11 consolidation, one model): Ego_r TPI CVectorMap derives CArray<pair<K,V> >
// (a public STLport vector); retail LowerBound reads start +0 / finish +4, Compare is at +12 and Dirty at +13 (sizeof 0x10,
// the "CVectorMap 0x10" container size of the generated headers). LowerBound is out of line in retail (every caller calls it).
#include "engine/CArray.h"
template<class K,class V> class CKeyPairCompareLess {
public:
#ifdef FABLE_RETAIL_CKEYPAIRCOMPARELESS_FORCEINLINE
    // Per-TU inline opt-in (NF80, q10923: the /Os STLport heap / insertion-sort TUs of the CVectorMap<CCharString, ..> sorts, rows
    // 0x004DFDBB / 0x004E1C8A): retail expands the pair compare in place there (0x004DFDD6..0x004DFDEA: the two key pointers,
    // the CCharString pointer tests, then `call 0x00429950`), where under optimize("s") the plain inline body is called out of line.
    FABLE_FORCEINLINE
#endif
    bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; }
#if defined(FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND) || defined(FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND)
    // chunk 849 (row 0x00501450): the key/key overload, the second of the two the Codex pass 13 TPI read gives CKeyPairCompareLess; the
    // inline LowerBound below compares through it.
    bool operator()(const K& a,const K& b) const { return a<b; }
#endif
};
template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >
class CVectorMap : public CArray<std::pair<K,V> > {
public:
    typedef std::pair<K,V> value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
#ifdef FABLE_RETAIL_CVECTORMAP_CTOR
    // Opt-in (chunk 766, row 0x004304E0): FableWin.pdb ??0?$CVectorMap@...@@QAE@XZ (82 instances; Ego_r inlines it). Retail's
    // body clears the Dirty byte after the vector's three zero words (CParentDefClassBase ctor 0x00430370 `mov [esi+36h],al`).
    CVectorMap() : Dirty(false) { }
#endif
    iterator LowerBound(const K&);
#ifdef FABLE_RETAIL_CVECTORMAP_PODIUM_UPPERBOUND
    // Both original non-const UpperBound methods; native008146A0 and original module309.
    iterator UpperBound(const K&);
#endif
    const_iterator LowerBound(const K&) const;
#ifdef FABLE_RETAIL_CVECTORMAP_UPPERBOUND
    // Chunk 754 (row 0x009AD5F0 CDefinitionManager::DecompressDefinitionData): the const UpperBound, out of line in retail
    // (Ego_r.pdb ?UpperBound@?$CVectorMap@K...@@QBE?AV?$_Vector_const_iterator@...@std@@ABK@Z family; CompressedOffsets
    // instance 0x009B1FF0). A declaration only.
    const_iterator UpperBound(const K&) const;
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_COUNT
    // Opt-in (NF62, row 0x0073A8E0 CTCCreatureModeManager::AllocateMode): Ego_r.pdb ?Count@?$CVectorMap@KVCRegisteredModeInfo@
    // CTCCreatureModeManager@@V?$CKeyPairCompareLess@..@@@@QBEJABK@Z (const, no retail out-of-line copy): inline there, the const
    // UpperBound (0x0073D250) is called first, then the const LowerBound (0x0073D210), and the difference taken (`sub ; sar 3`).
    // Needs FABLE_RETAIL_CVECTORMAP_UPPERBOUND.
    long Count(const K& key) const { const_iterator upper=UpperBound(key); return upper-LowerBound(key); }
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_GETKEYLIST
    // Retail opt-in (chunk Q15, row 0x0086B9D0): Ego_r.pdb ?GetKeyList@?$CVectorMap@J..@@QBEXAAV?$vector@J..@std@@@Z, out of line in
    // retail (0x0086D240 for <long, CRandomAppearanceMorph::CSkeletalMorph>). Declaration only.
    void GetKeyList(std::vector<K>& keys) const;
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_VALFIND
    // Retail opt-in (wf15, row 0x005F6BD6 CTCInventoryAbilities::SetAbilityAsAvailable): FableWin.pdb
    // ??$ValFind@W4EHeroAbility@@@?$CVectorMap@JW4EHeroAbility@@..@@QAE?AV?$_Vector_iterator.. (0x018580E0): the linear search by
    // value, expanded inline in retail's size TU (cursor loaded before last, `jmp cond`, early return, then last).
    template<class V2> FABLE_FORCEINLINE iterator ValFind(const V2& value) {
        iterator it=this->begin();
        iterator last=this->end();
        for(;it!=last;++it)
            if(it->second==value)return it;
        return last;
    }
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_VALFIND_CONST
    // Retail opt-in (agentC32, row 0x006A1A00 CTCCarrying::GetCarriedThingInterpolatedPosAndOrientation): FableWin.pdb 0x0221C0F0
    // ??$ValFind@PBVCThing@@@?$CVectorMap@JV?$CIntelligentPointer@VCThing@@@@..@@QBE?AV?$_Vector_const_iterator.. -- the const linear
    // search by value, expanded in retail (0x006A1A07..0x006A1A33: end() re-read after every compare call and once more for the
    // not-found return; FABLE_FORCEINLINE like the VALFIND opt-in: a plain inline is called out of line). The value is compared through Get() (CBaseIntelligentPointer::GetPItem 0x00A01B50).
    template<class V2> FABLE_FORCEINLINE const_iterator ValFind(const V2& value) const {
        for(const_iterator it=this->begin();it!=this->end();++it)
            if(it->second.Get()==value)return it;
        return this->end();
    }
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
    // Retail opt-in (2026-11-03, chunk 720, the size-optimised CTCInventoryWeapons TU): retail expands Find inside the
    // forced-inline CThing::GetTC there (`call LowerBound; mov esi,[esi+4]; cmp eax,esi; je; cmp [eax],ebx; jle`); under
    // optimize("s") a plain inline Find is called out of line instead (0x005C552C, 0x005C36C2).
    FABLE_FORCEINLINE
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_FIND_NAMES_PASS29
    // Ego_d names the input val and the iterator found. The FableWin body tests end
    // before calling Compare(val,found->first) and returns end on either failure.
    iterator Find(const K& val) {
        iterator found=LowerBound(val);
        if(found!=this->end() && !Compare(val,found->first))
            return found;
        return this->end();
    }
#else
    iterator Find(const K& key) {
        iterator i=LowerBound(key);
#ifdef FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
        // Retail opt-in (Codex pass18): original01896640 uses the original comparison member and positive return arm.
        if(i!=this->end()&&!Compare(key,i->first))return i;
        return this->end();
#else
        return i==this->end() || key<i->first ? this->end() : i;
#endif
    }
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
    FABLE_FORCEINLINE
#endif
    const_iterator Find(const K& key) const {
        const_iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
#ifdef FABLE_RETAIL_CVECTORMAP_ASSIGN
    // Retail opt-in (chunk 751, rows 0x0045CFAD CSoundMap::operator= / 0x0046313E CSimVoice::operator=, size-optimised TUs): the
    // member-wise assignment is expanded at both call sites (the vector operator= called out of line, Compare copies nothing, the
    // Dirty byte copied through al); under optimize("s") the implicit operator= is called out of line instead (a 33-byte
    // ??4CVectorMap COMDAT), so the expanded form is __forceinline (no out-of-line ??4CVectorMap in Fable.exe).
    FABLE_FORCEINLINE CVectorMap& operator=(const CVectorMap& other) {
        CArray<std::pair<K,V> >::operator=(other);
        Compare=other.Compare;
        Dirty=other.Dirty;
        return *this;
    }
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_ASSIGN_OUTOFLINE
    // Retail opt-in (chunk 900, row 0x00F2A000 CCutsceneDef::operator=): in the cutscene-def TU the implicit CVectorMap assignment
    // is called out of line, not expanded (`lea eax,[edi+29h] ; push eax ; lea ecx,[esi+29h] ; call 0x006313A0`), while the rest of
    // the parent-def chain (FABLE_RETAIL_PDCB_FORCEINLINE_ASSIGN) is expanded around it. 0x006313A0 is that COMDAT (27 bytes: the
    // vector operator= 0x00431F90, then the Dirty byte +0x0D; FableWin.pdb
    // ??4?$CVectorMap@KVCSubDefInfo@CDefClassBase@@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@@QAEAAV0@ABV0@@Z), so
    // Fable.exe does hold an out-of-line ??4CVectorMap (the ASSIGN note below says otherwise). A declaration only, as
    // FABLE_RETAIL_STLP_VECTOR_SWAP_OUTOFLINE: the row calls it.
    CVectorMap& operator=(const CVectorMap& other);
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_INSERT
    // Retail opt-in (chunk 796, row 0x008E18D0 CModeThunderLightningStorm::UpdateLightningBeams; the landed 0x00767DA0
    // UpdateParticleAttachment local model): Ego_r.pdb CVectorMap::Insert (push_back, then mark Dirty) and Update (sort with the
    // stored Compare when Dirty, then clear it), both inline in retail (the push_backs and the guarded std::sort calls 0x0076C000 /
    // 0x0076C1A0 sit in the callers).
#if defined(FABLE_RETAIL_CVECTORMAP_INSERT_FORCEINLINE) && defined(_MSC_VER)
    // (Also w22 agent2042 row 0x007F2DC0 CTCLightningNode::FrameUpdate, the same device: retail
    // expands both inserts of FrameUpdate, 0x007F3312..0x007F336D, where VC7.1 scores the plain inline Insert 191 and calls it.)
    // Q19 nested opt-in (row 0x008E16A0 CModeScreamerSuperAttack::FrameUpdate): the same Insert forced inline, as the
    // INSERT_KEYVALUE overload below (Ego_r.pdb holds no out-of-line CVectorMap::Insert). In that TU a plain inline Insert is called
    // out of line, while retail expands both push_backs in place (the first straight into _M_insert_overflow 0x0076ADA0 on the
    // still-empty vector, 0x008E1832; then `mov byte ptr [Dirty],1`).
    __forceinline
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_INSERT_VECTOR_PUSHBACK
    void Insert(const value_type& value) { std::vector<value_type>::push_back(value); Dirty=true; }
#else
    void Insert(const value_type& value) { this->push_back(value); Dirty=true; }
#endif
    void Update() { if(Dirty) { std::sort(this->begin(),this->end(),Compare); Dirty=false; } }
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
    // Nested opt-in (chunk 873, row 0x005DD670 CAnimationEntry::StatRegisterClass): Insert as the PDBs declare it, (key, value) by const
    // reference (FableWin.pdb ?Insert@?$CVectorMap@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@..@@QAEXABW4EAnimComponent@@
    // ABV?$CIVCountedPointer@VCAnimComponentBase@@@@@Z, and nine Ego_r-era instantiations, all `ABK ABV`). Forced inline: Ego_r.pdb holds
    // no out-of-line CVectorMap::Insert (FableWin.pdb, unoptimised, 61) and retail expands all 26 calls of that row, while a plain inline
    // Insert is called out of line there (3919 of 5317 bytes). The pair comes from make_pair inside its own statement, so it is
    // destroyed before Dirty is set (0x005DD7B0 `dec [esi+4]` ... 0x005DD7BC `mov byte ptr [Dirty],1`), and its push_back is a plain
    // inline that VC7.1's caller budget expands at the first 17 of the 26 sites and calls (0x005E3110) at the rest, as in retail.
#if defined(_MSC_VER)
    __forceinline
#else
    inline
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
    // Nested opt-in (row 0x004EE23F CTCBase::RegisterTCs): the pair is built as a value_type temporary, not through make_pair.
    // Retail pushes the pair constructor's own return value (0x004EE2C6 `call pair::pair ; push eax ; mov ecx,esi ; call push_back`,
    // all 340 sites), which only a direct temporary gives; an inlined make_pair re-materialises the temporary's address
    // (`lea eax,[pair]`). FableWin.pdb (unoptimised, 61 out-of-line Insert instances) holds no make_pair for any CVectorMap key/value
    // pair (not <CCharString, CTCBase::CRegisteredTCInfo>, not <EAnimComponent, CIVCountedPointer<CAnimComponentBase> >), which
    // also points to a direct construction. Row 0x005DD670 keeps the make_pair body below (its 17/26 push_back split depends on it).
    void Insert(const K& key,const V& value) { this->push_back(value_type(key,value)); Dirty=true; }
#else
    void Insert(const K& key,const V& value) { this->push_back(std::make_pair(key,value)); Dirty=true; }
#endif
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_SORT_INLINE
    // Retail opt-in (chunk 2005, row 0x00A12EE0): Ego_r.pdb ?Sort@?$CVectorMap@..@@QAEXXZ as the inline body (std::sort with the
    // stored Compare, then Dirty cleared), expanded at 0x00A132B9..0x00A13334 (no Dirty test ahead of it).
#ifdef FABLE_RETAIL_CVECTORMAP_SORT_INLINE_FORCE
    FABLE_FORCEINLINE
#endif
    void Sort() { std::sort(this->begin(),this->end(),Compare); Dirty=false; }
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_SORT
    // Retail opt-in (row 0x004EE23F CTCBase::RegisterTCs): Ego_r.pdb ?Sort@?$CVectorMap@..@@QAEXXZ (the CCharString /
    // CTCBase::CRegisteredTCInfo instance at Ego_r 0x00507625; 58 FableWin.pdb instances) and FableWin.pdb ?Update@?$CVectorMap@..@@QAEXXZ.
    // Update is expanded in the caller (`cmp byte ptr [Dirty],0 ; je ; mov ecx,esi ; call Sort`, 0x004F912E). Sort is an inline body
    // (std::sort over [begin, end) with the stored Compare, then Dirty cleared) that RegisterTCs, past VC7.1's inline budget, calls out of
    // line (0x004EBACE, `push Compare ; call std::sort ; mov byte ptr [esi+0Dh],0`); row 0x005DD670 has the same pair expanded in place
    // (FABLE_RETAIL_CVECTORMAP_INSERT's Update). Needs FABLE_RETAIL_STLP_SORT_FASTCALL (std::sort is __fastcall in retail).
    void Sort() { std::sort(this->begin(),this->end(),Compare); Dirty=false; }   // 0x004EBACE (CCharString / CRegisteredTCInfo instance)
#ifndef FABLE_RETAIL_CVECTORMAP_INSERT   // that opt-in's Update is the same body with Sort written out
    void Update() { if(Dirty) Sort(); }
#endif
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_SORT_OUTOFLINE
    // NF55 opt-in (2026-11-17, row 0x005F6423 CTCInventoryAbilities::SetAbilityToButton): Ego_r.pdb ?Sort@?$CVectorMap@..@@QAEXXZ called
    // out of line (the <long, EHeroAbility> instance 0x005FA47D) from the inline FableWin.pdb ?Update@?$CVectorMap@..@@QAEXXZ
    // (`cmp byte ptr [Dirty],0 ; je ; call Sort`). Declaration of Sort only; Update is the inline body.
    void Sort();
    void Update() { if(Dirty) Sort(); }
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_DEALLOCATEMEMORY
    // NF87 opt-in (row 0x004B41F0 CRuntimeQuestPersistence::Clear): Ego_r.pdb / FableWin.pdb ?DeallocateMemory@?$CVectorMap@VCCharString@@
    // VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@..@@QAEXXZ (Ego_d.pdb locals this, temp), out of line in retail (0x004BB6B0, landed).
    // Declaration only.
    void DeallocateMemory();
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_ERASEFROMKEY
    // NF85 opt-in (row 0x004B39B0 CQuestManager::ClearQuestPersistScript): FableWin.pdb ?EraseFromKey@?$CVectorMap@VCCharString@@VCPersistInfo@
    // CRuntimeQuestPersistence@NQuestManager@@..@@QAEXABVCCharString@@@Z (Ego_d.pdb locals: key, `lower`, then `upper` in an inner block),
    // inline in retail: LowerBound, the end() test, UpperBound, then the vector's range erase. Needs FABLE_RETAIL_CVECTORMAP_PODIUM_UPPERBOUND.
    void EraseFromKey(const K& key) {
        iterator lower=LowerBound(key);
        if(lower!=this->end()) {
            iterator upper=UpperBound(key);
            this->erase(lower,upper);
        }
    }
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_SETDIRTY
    // Opt-in (chunk Q7, row 0x0086BFB0 CRandomAppearanceMorph::TransferBinaryIn): Ego_r.pdb ?SetDirty@?$CVectorMap@..@@QAEX_N@Z
    // (CSkeletalMorph, CSkeletalMorphs::CEntry, CAnimComponentBase instances), inline in retail (`mov byte ptr [map+0Dh],0`).
    void SetDirty(bool dirty) { Dirty=dirty; }
#endif
private:
    CompareType Compare;
    bool Dirty;
};
// LowerBound: out of line in retail (every caller calls it). A same-TU noinline body (EXTRA_RULES chunk 422): VC7.1 then knows
// the callee does not capture the caller's key temporary, so Find compares the folded constant key (00626E40 `cmp [eax],8`).
#ifdef FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
// Per-TU opt-in (chunk 849, row 0x00501450 CWorldMap::GenerateRegionConnectivityGraph; the Codex pass 13 macros
// FABLE_RETAIL_CVECTORMAP_SIZE_LOWERBOUND / _ORDINARY_LOWERBOUND): the non-const LowerBound as an ordinary inline body in the debug
// donor's spelling (`long count=size(); iterator left=begin();`, count/2, the stored Compare on the keys, `left=++mid`), which that
// TU expands in place (0x00501645..0x00501687: finish then start loaded, `(finish-start)>>3`, `cdq ; sub ; sar` halving, `cmp
// [esi+eax*8],0C2h ; jge`), while the const overload stays the out-of-line noinline body below (0x004365B0, called at 0x00501597).
template<class K,class V,class CompareType>
inline typename CVectorMap<K,V,CompareType>::iterator CVectorMap<K,V,CompareType>::LowerBound(const K& key)
{
    long count=this->size();
    iterator left=this->begin();
    while(count>0) {
        long count2=count/2;
        iterator mid=left+count2;
        if(Compare(mid->first,key)) { left=++mid; count-=count2+1; }
        else count=count2;
    }
    return left;
}
#else
template<class K,class V,class CompareType>
__declspec(noinline) typename CVectorMap<K,V,CompareType>::iterator CVectorMap<K,V,CompareType>::LowerBound(const K& key)
{
#ifdef FABLE_RETAIL_CVECTORMAP_LOWERBOUND_COUNT_FIRST
    // NF85 opt-in (row 0x004B9750, CVectorMap<CCharString, CRuntimeQuestPersistence::CPersistInfo>::LowerBound): the count taken before
    // `first`, the non-const twin of NF82's FABLE_RETAIL_CVECTORMAP_CONST_LOWERBOUND_COUNT_FIRST (retail 0x004B9750 and the const
    // 0x004B97D0 are byte-identical apart from the operator< call displacement).
    long count=this->size();
    iterator first=this->begin();
#else
    iterator first=this->begin();
    long count=this->end()-first;
#endif
    while(count>0) {
        long half=count/2;
        iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_COMPONENT_LOWERBOUND_OWNER_PASS26
// Fidelity scaffolding: explicit instance retains the genuine generic body and caller visibility.
// The caller includes this definition before its size-optimised row body.
#include "engine/ETCInterfaceType.h"
class CTCBase;
template<> inline __declspec(noinline)
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::iterator
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key)
{
 iterator first=this->begin();
 long count=this->end()-first;
 while(count>0) {
  long half=count/2;
  iterator middle=first+half;
  if(middle->first<key) { first=middle+1; count-=half+1; }
  else count=half;
 }
 return first;
}
#endif
#ifdef FABLE_RETAIL_CVECTORMAP_PODIUM_UPPERBOUND
template<class K,class V,class CompareType>
#ifdef FABLE_RETAIL_CVECTORMAP_UPPERBOUND_COUNT_FIRST
// NF85 opt-in (rows 0x004B9850 / 0x004B39B0, the CVectorMap<CCharString, CRuntimeQuestPersistence::CPersistInfo> instance): out of line
// (every retail caller calls it) with the count taken before `first`, as the LowerBound twin 0x004B9750 computes it.
__declspec(noinline)
#endif
typename CVectorMap<K,V,CompareType>::iterator CVectorMap<K,V,CompareType>::UpperBound(const K& key)
{
#ifdef FABLE_RETAIL_CVECTORMAP_UPPERBOUND_COUNT_FIRST
    long count=this->size();
    iterator first=this->begin();
#else
    iterator first=this->begin();
    long count=this->end()-first;
#endif
    while(count>0){
        long half=count/2;
        iterator middle=first+half;
        if(!(key<middle->first)){first=++middle;count-=half+1;}
        else count=half;
    }
    return first;
}
#endif
template<class K,class V,class CompareType>
#ifdef FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
inline
#else
__declspec(noinline)
#endif
typename CVectorMap<K,V,CompareType>::const_iterator CVectorMap<K,V,CompareType>::LowerBound(const K& key) const
{
#if defined(FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND) || defined(FABLE_RETAIL_CVECTORMAP_CONST_LOWERBOUND_COUNT_FIRST)
    // (NF82 opt-in FABLE_RETAIL_CVECTORMAP_CONST_LOWERBOUND_COUNT_FIRST, row 0x004B97D0: the out-of-line const body with the count taken
    // before `first`, as retail's CVectorMap<CCharString, CPersistInfo> instance computes it; the inline opt-in's text.)
    long count=this->size();
    const_iterator first=this->begin();
#else
    const_iterator first=this->begin();
    long count=this->end()-first;
#endif
    while(count>0) {
        long half=count/2;
        const_iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}
#if defined(FABLE_RETAIL_UPDATE_CURFEW_PASS29) && defined(_MSC_VER)
// Restore the non-const LowerBound owners at 0040f020 and 006804c0 to their default
// /O2 /Oy flags around its original search, then reset with optimize("", on)
// before any caller's function-level optimization.
#include "engine/ETCInterfaceType.h"
class CTCBase;
#pragma optimize("ty", on)
template<> inline __declspec(noinline)
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::iterator
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key)
{
    iterator first=this->begin();
    long count=this->end()-first;
    while(count>0) {
        long half=count/2;
        iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}
#pragma optimize("", on)
#endif
#if defined(FABLE_RETAIL_CVECTORMAP_CONST_LOWERBOUND_SPEED_OWNER_PASS27) && defined(_MSC_VER)
// Fidelity: scaffolding, not retail. Restore the 0x004365B0 owner's default
// /O2 /Oy flags around its original search, then reset with optimize("", on)
// before any caller's function-level optimization.
#include "engine/ETCInterfaceType.h"
class CTCBase;
#pragma optimize("ty", on)
template<> inline __declspec(noinline)
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::const_iterator
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key) const
{
    const_iterator first=this->begin();
    long count=this->end()-first;
    while(count>0) {
        long half=count/2;
        const_iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}
#pragma optimize("", on)
#endif
#if defined(FABLE_RETAIL_CVECTORMAP_UPPERBOUND) && defined(FABLE_RETAIL_CVECTORMAP_UPPERBOUND_BODY)
// Per-TU opt-in (egod e02, row 0x0086B9D0 CRandomAppearanceMorph::GetRandomSkeletalMorph): the body of the const UpperBound declared by
// FABLE_RETAIL_CVECTORMAP_UPPERBOUND: an ordinary out-of-class template member, visible in the TU (VC7.1 keeps it out of line by itself;
// graded without and with noinline: same bytes) (Ego_d random_appearance_morph.obj
// instantiates LowerBound / UpperBound<long, CSkeletalMorph> in the row's TU: retail 0x0086A930 / 0x0086A980; the landed 0x0086A980 text:
// `!(key < middle->first)` moves first past middle). With it VC7.1 knows the calls write no memory and does not reload *seed after
// them (0x0086BAD4: `imul ebp,ebp,24A1h` on the value stored before the UpperBound / LowerBound calls).
template<class K,class V,class CompareType>
typename CVectorMap<K,V,CompareType>::const_iterator CVectorMap<K,V,CompareType>::UpperBound(const K& key) const
{
    const_iterator first=this->begin();
    long count=this->end()-first;
    while(count>0) {
        long half=count/2;
        const_iterator middle=first+half;
        if(!(key<middle->first)) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}
#endif
#if defined(FABLE_RETAIL_CVECTORMAP_GETKEYLIST) && defined(FABLE_RETAIL_CVECTORMAP_GETKEYLIST_BODY)
// Per-TU opt-in (egod e02, row 0x0086B9D0): the body of GetKeyList (declared by FABLE_RETAIL_CVECTORMAP_GETKEYLIST), an ordinary
// out-of-class template member visible in the TU (VC7.1 keeps it out of line by itself, as in the retail TU; Ego_d random_appearance_morph.obj instantiates GetKeyList<long, CSkeletalMorph> = retail 0x0086D240). Ego_d
// locals key_list, no_key, last_key, it; retail 0x0086D240: no_key = true, last_key = 0, each new key pushed (the inline vector<long>
// push_back), then last_key = it->first and no_key = false.
template<class K,class V,class CompareType>
void CVectorMap<K,V,CompareType>::GetKeyList(std::vector<K>& key_list) const
{
    bool no_key=true;
    K last_key=0;
    for(const_iterator it=this->begin();it!=this->end();++it) {
        if(no_key||it->first!=last_key) {
            key_list.push_back(it->first);
            last_key=it->first;
            no_key=false;
        }
    }
}
#endif
#endif
