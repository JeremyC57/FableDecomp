// GetArmourWeightsFromThing(const CThing& thing, multimap<CDefPointer<const CArmourDef>, float>& weights, ...) -- collect the armour definitions a thing wears with their protection weights (file-static helper)
// Leaf: ?GetArmourWeightsFromThing@?A0x
// Retail: 00868e50, 456 bytes, fastcall.
// Flags: /Gr
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#define FABLE_RETAIL_STLP_LIST
#include "fable_compiler.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingObjectDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CArmourDef.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
#include "engine/CCompareDefPointer.h"
#include "stlport/fable_map.h"

template<class T> bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* out) const
{
    if(index>0) {
        CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index);
        if(!def.IsNull()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}
template<> bool CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<const CAppearanceModifierDef>* out) const
{
    const CSubDefInfo* info;
    {
        CCharString name("CAppearanceModifierDef");
        info=GetPSubDefInfo(name);
    }
    if(info) {
        CDefPointer<const CDefClassBase> def=PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

namespace std {
template<class K,class V,class KeyOfValue,class Compare,class A>
typename _Rb_tree<K,V,KeyOfValue,Compare,A>::iterator
_Rb_tree<K,V,KeyOfValue,Compare,A>::insert_equal(const V& value)
{
    _Rb_tree_node_base* parent=this->_M_header._M_data;
    _Rb_tree_node_base* node=_M_root();
    while(node!=0) {
        parent=node;
        node=_M_key_compare(KeyOfValue()(value),_S_key(node)) ? _S_left(node) : _S_right(node);
    }
    return _M_insert(node,parent,value);
}
}
namespace std {

template<class K,class V,class KeyOfValue,class Compare,class A>
typename _Rb_tree<K,V,KeyOfValue,Compare,A>::iterator
_Rb_tree<K,V,KeyOfValue,Compare,A>::_M_insert(_Rb_tree_node_base* x,_Rb_tree_node_base* y,const V& value,_Rb_tree_node_base* w)
{
    _Link_type node;
    if(y==this->_M_header._M_data || (w==0 && (x!=0 || _M_key_compare(KeyOfValue()(value),_S_key(y))))) {
        node=_M_create_node(value);
        _S_left(y)=node;
        if(y==this->_M_header._M_data) {
            _M_root()=node;
            _M_rightmost()=node;
        } else if(y==_M_leftmost()) _M_leftmost()=node;
    } else {
        node=_M_create_node(value);
        _S_right(y)=node;
        if(y==_M_rightmost()) _M_rightmost()=node;
    }
    node->_M_parent=y;
    node->_M_left=0;
    node->_M_right=0;
    _Rb_global_inst::_Rebalance(node,this->_M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(node);
}

}
namespace {
void FABLE_FASTCALL GetArmourWeightsFromThing(const CThing& thing,
    std::multimap<CDefPointer<const CArmourDef>,float,CCompareDefPointer<const CArmourDef,float> >& armour_weights,
    float& total_armour_weight)
{
    const CGameDefinitionManager& def_man=thing.PeekDefinitionManager();
    const CTCHeroAttachableAppearanceModifiers* ptc_armour_clothes;
    if(thing.PeekTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS,&ptc_armour_clothes)) {
        std::list<long> clothes_list;
        ptc_armour_clothes->GetAppearanceModifierList(clothes_list,APPEARANCE_CLOTHING);
        for(std::list<long>::iterator it=clothes_list.begin();it!=clothes_list.end();++it) {
            CDefPointer<const CThingObjectDef> pdef;
            long clothing_item_def_index=*it;
            def_man.GetDef(clothing_item_def_index,&pdef);
            CDefPointer<const CAppearanceModifierDef> pclothing_def;
            pdef->GetSubDef(&pclothing_def);
            if(pclothing_def->ArmourDefIndex>=0) {
                CDefPointer<const CArmourDef> parmour_def;
                def_man.GetDef(pclothing_def->ArmourDefIndex,&parmour_def);
                armour_weights.insert(std::make_pair(parmour_def,pclothing_def->OveralProtectionWeighting));
                total_armour_weight+=pclothing_def->OveralProtectionWeighting;
            }
        }
    }
}
}