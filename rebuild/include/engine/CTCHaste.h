#ifndef FABLE_ENGINE_CTCHASTE_H
#define FABLE_ENGINE_CTCHASTE_H
#include "engine/CTCBase.h"
// TPI0x2f115 original fields; native00667ff5 writes PauseFrames+14.
#ifdef FABLE_RETAIL_CTCHASTE
// Retail opt-in (chunk 755, row 0x006A02B0 CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction): Ego_r.pdb
// CTCHaste::Multipliers is std::map<void*,float> (+0x18, the STLport three-word tree); SetActionSpeedMultiplier(void*, float)
// (Ego_r 0x0051BF50 / FableWin ?SetActionSpeedMultiplier@CTCHaste@@QAEXPAXM@Z) is expanded in that row: map::insert (the
// out-of-line _Rb_tree::insert_unique 0x00580D21) of one value_type temporary, then the private
// ?UpdateActionSpeedMultiplier@CTCHaste@@AAEXXZ 0x00800020. Plain inline: an out-of-line copy exists in Ego_r.
#include "stlport/fable_map.h"
#ifdef FABLE_RETAIL_CTCBERSERK_INITABILITY_PASS29
// The native void*/float map owner uses size optimization. The retail placement copy and predecessor are expanded in these members.
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
namespace std {
typedef pair<void* const,float> BerserkMapValue;
typedef _Select1st<BerserkMapValue> BerserkSelect;
typedef _Rb_tree<void*,BerserkMapValue,BerserkSelect,less<void*>,allocator<BerserkMapValue> > BerserkTree;
template<> inline _Rb_tree_node<BerserkMapValue>* BerserkTree::_M_create_node(const BerserkMapValue& x){_Link_type p=this->_M_header.allocate(1);std::_Construct(&p->_M_value_field,x);return p;}
template<> inline _Rb_tree_iterator<BerserkMapValue,_Nonconst_traits<BerserkMapValue> > BerserkTree::_M_insert(_Rb_tree_node_base* raw_x,_Rb_tree_node_base* raw_y,const BerserkMapValue& value,_Rb_tree_node_base* raw_w) {
    _Link_type w=(_Link_type)raw_w;
    _Link_type x=(_Link_type)raw_x;
    _Link_type y=(_Link_type)raw_y;
    _Link_type z;
    if(y==this->_M_header._M_data || (w==0 && (x!=0 || _M_key_compare(BerserkSelect()(value),_S_key(y))))) {
        z=_M_create_node(value);
        _S_left(y)=z;
        if(y==this->_M_header._M_data) {_M_root()=z;_M_rightmost()=z;}
        else if(y==_M_leftmost()) _M_leftmost()=z;
    } else {
        z=_M_create_node(value);
        _S_right(y)=z;
        if(y==_M_rightmost()) _M_rightmost()=z;
    }
    _S_parent(z)=y;
    _S_left(z)=0;
    _S_right(z)=0;
    _Rb_global_inst::_Rebalance(z,this->_M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(z);
}
template<> inline pair<_Rb_tree_iterator<BerserkMapValue,_Nonconst_traits<BerserkMapValue> >,bool> BerserkTree::insert_unique(const BerserkMapValue& v) {
    _Link_type y=this->_M_header._M_data;
    _Link_type x=_M_root();
    bool comp=true;
    while(x!=0) {
        y=x;
        comp=_M_key_compare(BerserkSelect()(v),_S_key(x));
        x=comp?_S_left(x):_S_right(x);
    }
    iterator j=iterator(y);
    if(comp)
        if(j==begin()) return pair<iterator,bool>(_M_insert(y,y,v),true);
        else j._M_node=_Rb_global_inst::_M_decrement(j._M_node);
    if(_M_key_compare(_S_key(j._M_node),BerserkSelect()(v)))
        return pair<iterator,bool>(_M_insert(x,y,v),true);
    return pair<iterator,bool>(j,false);
}
}
#endif

#ifdef FABLE_RETAIL_CTCHASTE_SETACTIONSPEEDMULTIPLIER_FORCEINLINE
#include "fable_compiler.h"   // FABLE_FORCEINLINE (NF61 opt-in below)
#endif
class CTCHaste : public CTCBase {
private:
    float Multiplier;bool ToTerminate;long PauseFrames;
    std::map<void*,float> Multipliers;                                   // +0x18
    void UpdateActionSpeedMultiplier();                                  // 0x00800020
#ifdef FABLE_RETAIL_NPLAYERGUI_CDRAWHEALTH_PASS28
public:
    float GetActionSpeedMultiplier(); // Ego_r and FableWin, retail 0x007FFFB0
private:
#endif
#ifdef FABLE_RETAIL_CTCHASTE_UNSET
public:
    // Chunk 764 (row 0x00859660 CCombatActionBase::ResetMeleeCombatAbilityEffects): Ego_r.pdb ?UnsetActionSpeedMultiplier@CTCHaste@@QAEXPAX@Z,
    // out of line in retail at 0x0058148D (the bound RemoveMultiplier(CTCHeroStats*) spelling is wrong: NAME_FIXES).
    void UnsetActionSpeedMultiplier(void* key);                          // 0x0058148D
private:
#endif
#ifdef FABLE_RETAIL_CTCHASTE_UNSET_INLINE
public:
    // NF61 opt-in (round 7, row 0x00694020 CCreatureActionBase::FrameUpdate): the same Ego_r.pdb member
    //   ?UnsetActionSpeedMultiplier@CTCHaste@@QAEXPAX@Z with its body visible (the out-of-line copy 0x0058148D is exactly
    //   `Multipliers.erase(key); UpdateActionSpeedMultiplier();`), which retail expands there (erase(const key_type&) 0x0058119B, then
    //   0x00800020). Needs FABLE_RETAIL_STLP_TREE_ERASE_KEY (map::erase(const K&)); do not define with FABLE_RETAIL_CTCHASTE_UNSET.
    void UnsetActionSpeedMultiplier(void* key) { Multipliers.erase(key); UpdateActionSpeedMultiplier(); }
private:
#endif
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HASTE; }
    void SetOnHitPause(long frames) { PauseFrames=frames; }
#ifdef FABLE_RETAIL_CTCHASTE_SETACTIONSPEEDMULTIPLIER_OUTOFLINE
    // NF61 opt-in (round 7, row 0x005812AE): the same member declared only, for the row that defines retail's out-of-line copy
    // (0x005812AE, unreferenced COMDAT; the body below is its text).
    void SetActionSpeedMultiplier(void* key, float multiplier);
#else
#ifdef FABLE_RETAIL_CTCHASTE_SETACTIONSPEEDMULTIPLIER_FORCEINLINE
    // NF61 opt-in (round 7, row 0x0078B410 CCreatureAction_HeroLoadRangedWeapon::Initialise). Inline-budget: retail expands this body
    //   there (0x0078B5xx: the pair temporary built in the frame, `lea ecx,[esi+18h] ; call 0x00580D21`, then 0x00800020); VC7.1 calls
    //   the member out of line in that large caller, also under /d2inlS20 (469 v 493 bytes). Same body.
    FABLE_FORCEINLINE
#endif
    void SetActionSpeedMultiplier(void* key, float multiplier)
    {
#ifdef FABLE_RETAIL_CTCHASTE_MAKE_PAIR
        // Original debug SetActionSpeedMultiplier calls std::make_pair before insert.
        Multipliers.insert(std::make_pair(key, multiplier));
#else
        Multipliers.insert(std::map<void*,float>::value_type(key, multiplier));
#endif
        UpdateActionSpeedMultiplier();
    }
#endif
#ifdef FABLE_RETAIL_CTCHASTE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x004E0CA4
public:
#endif
};
#else
class CTCHaste : public CTCBase {
private:
    float Multiplier;bool ToTerminate;long PauseFrames;
    unsigned char Multipliers[12]; // std::map<void*,float>, retail STLport.
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HASTE; }
    void SetOnHitPause(long frames) { PauseFrames=frames; }
};
#endif
FABLE_STATIC_ASSERT_AT(56, sizeof(CTCHaste)==0x24);

#endif
