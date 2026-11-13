#ifndef FABLE_ENGINE_CVECTORMAP_H
#define FABLE_ENGINE_CVECTORMAP_H
#include "fable_compiler.h"
// CVectorMap<K,V,CompareType> (Codex pass 11 consolidation, one model): Ego_r TPI CVectorMap derives CArray<pair<K,V> >
// (a public STLport vector); retail LowerBound reads start +0 / finish +4, Compare is at +12 and Dirty at +13 (sizeof 0x10,
// the "CVectorMap 0x10" container size of the generated headers). LowerBound is out of line in retail (every caller calls it).
#include "engine/CArray.h"
template<class K,class V> class CKeyPairCompareLess {
public:
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
    const_iterator LowerBound(const K&) const;
#ifdef FABLE_RETAIL_CVECTORMAP_UPPERBOUND
    // Chunk 754 (row 0x009AD5F0 CDefinitionManager::DecompressDefinitionData): the const UpperBound, out of line in retail
    // (Ego_r.pdb ?UpperBound@?$CVectorMap@K...@@QBE?AV?$_Vector_const_iterator@...@std@@ABK@Z family; CompressedOffsets
    // instance 0x009B1FF0). A declaration only.
    const_iterator UpperBound(const K&) const;
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
#ifdef FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
    // Retail opt-in (2026-11-03, chunk 720, the size-optimised CTCInventoryWeapons TU): retail expands Find inside the
    // forced-inline CThing::GetTC there (`call LowerBound; mov esi,[esi+4]; cmp eax,esi; je; cmp [eax],ebx; jle`); under
    // optimize("s") a plain inline Find is called out of line instead (0x005C552C, 0x005C36C2).
    FABLE_FORCEINLINE
#endif
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
template<class K,class V,class CompareType>
#ifdef FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
inline
#else
__declspec(noinline)
#endif
typename CVectorMap<K,V,CompareType>::const_iterator CVectorMap<K,V,CompareType>::LowerBound(const K& key) const
{
#ifdef FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
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
#endif
