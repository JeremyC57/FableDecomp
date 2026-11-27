// 007808b0
// Leaf: ?InitFirePoints@CTCFlammable@@AAEXXZ
// Flags: /Gr
// Inline-budget: The size-optimized CDefString overflow expands allocation, copy and fill. Its separately emitted native bodies are verified with the row.
#define FABLE_RETAIL_STLP_RB_GLOBAL_INCREMENT_SIZE_BODY_PASS27
#define FABLE_RETAIL_CDEFSTRING_EQ_CHARSTRING
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_LESS
#define FABLE_RETAIL_STLP_LESS_CCHARSTRING_SIZE_BODY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_DIRECT
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_NOINLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_FORWARDER
#define FABLE_RETAIL_FLAMMABLE_INIT_FIRE_POINTS_PASS29
#define FABLE_RETAIL_CTCFLAMMABLE_FRAMEUPDATE
#define FABLE_RETAIL_CTCFLAMMABLE_FIREPOINTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CDEFSTRING_COMPARE_FRIEND
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPERNAMES
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CDefString.h"
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#include "engine/NFire_CFirePoint.h"
#include "engine/CFlammableDef.h"
#include "engine/CTCFlammable.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

// The retail specialization is size-optimized. Its original body remains visible to the speed-built caller.
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
namespace std {
typedef pair<const CCharString,long> EffectStringMapValue;
typedef _Rb_tree<CCharString,EffectStringMapValue,_Select1st<EffectStringMapValue>,
                 less<CCharString>,allocator<EffectStringMapValue> > EffectStringMapTree;
template<> inline void EffectStringMapTree::clear() {
      if(_M_node_count!=0) {
        _M_erase(static_cast<_Link_type>(this->_M_header._M_data->_M_parent));
        this->_M_header._M_data->_M_left=this->_M_header._M_data;
        this->_M_header._M_data->_M_parent=0;
        this->_M_header._M_data->_M_right=this->_M_header._M_data;
        _M_node_count=0;
      }
    }
    template<> inline EffectStringMapTree::~_Rb_tree() { clear(); }
    template<> template<> inline _Rb_tree_node<EffectStringMapValue>*
EffectStringMapTree::_M_find<CCharString>(const CCharString& key) const
{
    _Link_type y=this->_M_header._M_data;
    _Link_type x=_M_root();
    while (x!=0) {
        if (!_M_key_compare(_S_key(x),key)) { y=x; x=_S_left(x); }
        else x=_S_right(x);
    }
    if (y==this->_M_header._M_data || _M_key_compare(key,_S_key(y)))
        y=this->_M_header._M_data;
    return y;
}
}
#ifdef _MSC_VER
#pragma optimize("", on)
#endif

#ifdef _MSC_VER
#pragma optimize("y", off)
#endif
// The retail specialization is size-optimized. Its original body remains visible to the speed-built caller.
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
namespace std {
// Inline budget at 00465a72: the native size-owned overflow inlines allocation, copy and fill.
template<> FABLE_FORCEINLINE CDefString* FABLE_FASTCALL __uninitialized_fill_n(CDefString* first,unsigned int n,const CDefString& x,const __false_type&) { CDefString* cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur; }
template<> FABLE_FORCEINLINE CDefString* FABLE_FASTCALL __uninitialized_copy(CDefString* first,CDefString* last,CDefString* result,const __false_type&) { CDefString* cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur; }
template<> FABLE_FORCEINLINE CDefString* allocator<CDefString>::allocate(size_t n,const void*) { return n ? static_cast<CDefString*>(malloc(n*sizeof(CDefString))) : 0; }
template<> inline void vector<CDefString>::_M_insert_overflow(iterator position,const CDefString& x,const __false_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    CDefString* newStart=this->_M_end_of_storage.allocate(len);CDefString* newFinish=newStart;
    newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
    if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
    else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
    if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
}
#ifdef _MSC_VER
#pragma optimize("", on)
#endif

inline void CFlammableDef::GetAllFireHelpers(std::vector<CDefString>& helper_points) const
{
    if(false) {} else for (CFireEffectCreationSet::TContainmentMap::const_iterator it=EffectCreationSet.ContainmentMap.begin(); it!=EffectCreationSet.ContainmentMap.end(); ++it)
        helper_points.push_back((*it).first);
}

inline NFire::CFirePoint::CFirePoint(CThing& parent_thing, const CCharString& helper_name, long helper_index, long effect, long fire_resistance)
    : HelperName(helper_name), HelperIndex(helper_index), EffectIndex(effect), Resistance(fire_resistance),
      Burning(false), FireDamageAfterFrame(0), PParentThing(&parent_thing)
{
}

inline bool CFlammableDef::GetEffectAtHelperPoint(const CCharString& name,long& effect) const
{
    CFireEffectCreationSet::TContainmentMap::const_iterator it;
    if(false) {} else for(it=EffectCreationSet.ContainmentMap.begin();it!=EffectCreationSet.ContainmentMap.end();++it) {
        if(it->first == name) {
            effect=it->second;
            return true;
        }
    }
    return false;
}

inline bool CFlammableDef::GetEffectAtHelperPoint(CDefString name, long& effect) const
{
    CFireEffectCreationSet::TContainmentMap::const_iterator it = EffectCreationSet.ContainmentMap.find(name);
    if (it != EffectCreationSet.ContainmentMap.end()) {
        effect = it->second;
        return true;
    }
    return false;
}

void CTCFlammable::InitFirePoints()
{
    FirePoints.clear();
    if (!PDef.Get()) { FABLE_RETAIL_ASSERT_ARM(false); }
    CArray<CDefString> fire_names;
    PDef->GetAllFireHelpers(fire_names);
    std::map<CCharString, long> fire_pt_enumerator;
    if (false) {} else for (CArray<CDefString>::iterator it=fire_names.begin(); it!=fire_names.end(); ++it) {
        CCharString name=*it;
        fire_pt_enumerator.insert(std::make_pair(name, 0));
    }
    std::vector<CCharString> all_names;
    GetParentThing().GetTCGraphicAppearance()->GetAllHelperPointNames(all_names);
    if (all_names.empty()) { FABLE_RETAIL_ASSERT_ARM(false); }
    HasFirePoint=false;
    if (false) {} else for (std::vector<CCharString>::iterator it_all=all_names.begin();it_all!=all_names.end();++it_all) {
        const CCharString& name=*it_all;
        std::map<CCharString,long>::iterator it_look_up=fire_pt_enumerator.find(name);
        if (it_look_up!=fire_pt_enumerator.end()) {
            long effect;
            if (PDef->GetEffectAtHelperPoint(name,effect)) {
                FirePoints.push_back(NFire::CFirePoint(GetParentThing(),name,it_look_up->second++,effect,PDef->FireResistance));
                HasFirePoint=true;
            }
        }
    }
    if (!HasFirePoint && !all_names.empty() && !fire_names.empty()) {
        long effect;
        if (PDef->GetEffectAtHelperPoint(fire_names.front(),effect)) {
            FirePoints.push_back(NFire::CFirePoint(GetParentThing(),all_names.front(),0,effect,PDef->FireResistance));
            HasFirePoint=true;
        }
    }
}