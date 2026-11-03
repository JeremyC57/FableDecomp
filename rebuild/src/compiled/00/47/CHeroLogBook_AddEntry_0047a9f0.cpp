// CHeroLogBook::AddEntry(CWideString const &,CWideString const &,CWideString const &,CHeroLogBook::CHeroLogBookEntry::ECategory,bool) -- add a unique hero-log entry and notify sound/map interfaces.
// Leaf: ?AddEntry@CHeroLogBook@@QAEXABVCWideString@@00W4ECategory@CHeroLogBookEntry@1@_N@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "engine/CHeroLogBook.h"
#include "fable_game.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCInventoryMap.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"

#pragma auto_inline(off)
namespace std {
// Original STLport4.6.2 _tree.c345..421; native47c500/47c720.
template<class K,class V,class KeyOfValue,class Compare,class A>
typename _Rb_tree<K,V,KeyOfValue,Compare,A>::iterator
_Rb_tree<K,V,KeyOfValue,Compare,A>::_M_insert(_Rb_tree_node_base* x0,_Rb_tree_node_base* y0,const V& value,_Rb_tree_node_base* w0) {
    _Link_type w=(_Link_type)w0,x=(_Link_type)x0,y=(_Link_type)y0,z;
    if(y==this->_M_header._M_data || (w==0 && (x!=0 || _M_key_compare(KeyOfValue()(value),_S_key(y))))) {
        z=_M_create_node(value);_S_left(y)=z;
        if(y==this->_M_header._M_data){_M_root()=z;_M_rightmost()=z;}
        else if(y==_M_leftmost())_M_leftmost()=z;
    } else {
        z=_M_create_node(value);_S_right(y)=z;
        if(y==_M_rightmost())_M_rightmost()=z;
    }
    _S_parent(z)=y;_S_left(z)=0;_S_right(z)=0;
    _Rb_global_inst::_Rebalance(z,this->_M_header._M_data->_M_parent);++_M_node_count;
    return iterator(z);
}
template<class K,class V,class KeyOfValue,class Compare,class A>
pair<_Rb_tree_iterator<V,_Nonconst_traits<V> >,bool>
_Rb_tree<K,V,KeyOfValue,Compare,A>::insert_unique(const V& value) {
    _Link_type y=this->_M_header._M_data,x=_M_root();bool comp=true;
    while(x!=0){y=x;comp=_M_key_compare(KeyOfValue()(value),_S_key(x));x=comp?_S_left(x):_S_right(x);}
    iterator j=iterator(y);
    if(comp) {if(j==begin())return pair<iterator,bool>(_M_insert(y,y,value),true);else --j;}
    if(_M_key_compare(_S_key(j._M_node),KeyOfValue()(value)))return pair<iterator,bool>(_M_insert(x,y,value),true);
    return pair<iterator,bool>(j,false);
}
}
// Original UnassignString body is inlined by native99b510; NoStrings decrements at99b54c.
CWideString::~CWideString(){UnassignString();--NoStrings;}
template<class K,class V,class C,class A> V& std::map<K,V,C,A>::operator[](const K& key) {
    iterator i=lower_bound(key);
    if(i==end() || key_comp()(key,i->first))i=insert(i,value_type(key,V()));
    return i->second;
}
template<> __declspec(noinline) void std::__destroy_aux<CHeroLogBook::CHeroLogBookEntry*>(CHeroLogBook::CHeroLogBookEntry* first,CHeroLogBook::CHeroLogBookEntry* last,const std::__false_type&) {
    for(;first!=last;++first)std::_Destroy(&*first);
}
template<class T> bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* result)const {
    if(index<=0)return false;
    CDefPointer<const CDefClassBase> definition=GetPDefFromGlobalIndex(index);
    if(!definition.IsNull()) { result->SetDynamicCast(definition);return true; }
    return false;
}
template<> std::vector<CHeroLogBook::CHeroLogBookEntry>::vector(const std::vector<CHeroLogBook::CHeroLogBookEntry>& other)
 : std::_Vector_base<CHeroLogBook::CHeroLogBookEntry,std::allocator<CHeroLogBook::CHeroLogBookEntry> >(other.size(),other.get_allocator()) {
    this->_M_finish=std::__uninitialized_copy(other.begin(),other.end(),this->_M_start,std::__false_type());
}
template<class C,class I> int __cdecl std::__char_traits_base<C,I>::compare(const C* first,const C* second,size_t count) {
    for(size_t i=0;i<count;++i)if(first[i]!=second[i])return first[i]<second[i]?-1:1;
    return 0;
}
#pragma auto_inline(on)

void CHeroLogBook::AddEntry(const CWideString& name_tag,const CWideString& abbreviated_name,const CWideString& text_tag,CHeroLogBookEntry::ECategory log_category,bool add_to_map)
{
    typedef std::vector<CHeroLogBookEntry> EntryVector;
    typedef std::map<CHeroLogBookEntry::ECategory,EntryVector> EntryMap;
    for(EntryMap::iterator category=LogEntries.begin();category!=LogEntries.end();++category)
        for(EntryVector::iterator entry=category->second.begin();entry!=category->second.end();++entry)
            if(entry->GetName()==name_tag)return;
    CThingPlayerCreature* creature=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if(creature && !creature->IsDead() && creature->HasTC(TCI_SOUND_PLAYER)) {
        CTCSoundPlayer& sound_player=static_cast<CTCSoundPlayer&>(creature->GetTC(TCI_SOUND_PLAYER));
        sound_player.PlayAttachedCriteria("LOG_ENTRY",false,true,false,-1);
    }
    if(add_to_map && creature->HasTC(TCI_INVENTORY_MAP)) {
        CTCInventoryMap* inventory_map=static_cast<CTCInventoryMap*>(&creature->GetTC(TCI_INVENTORY_MAP));
        if(inventory_map)inventory_map->AddLogBookSaveInfo(name_tag,abbreviated_name,text_tag,log_category);
    }
    CDefPointer<const NUISystem::CUIMiscThingsDef> misc_def;
    long index=CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF",1);
    CGameDefinitionManager::Get().CDefinitionManager::GetDef(index,&misc_def);
    CHeroLogBookEntry::ECategory category_from_tag=log_category==CHeroLogBookEntry::CATEGORY_TUTORIAL?CHeroLogBookEntry::CATEGORY_BASICS:log_category;
    if(text_tag.Contains(misc_def->LogBookBasicsCategoryString))category_from_tag=CHeroLogBookEntry::CATEGORY_BASICS;
    if(text_tag.Contains(misc_def->LogBookObjectsCategoryString))category_from_tag=CHeroLogBookEntry::CATEGORY_OBJECTS;
    if(text_tag.Contains(misc_def->LogBookTownsCategoryString))category_from_tag=CHeroLogBookEntry::CATEGORY_TOWNS;
    if(text_tag.Contains(misc_def->LogBookHeroCategoryString))category_from_tag=CHeroLogBookEntry::CATEGORY_HERO;
    if(text_tag.Contains(misc_def->LogBookCombatCategoryString))category_from_tag=CHeroLogBookEntry::CATEGORY_COMBAT;
    if(text_tag.Contains(misc_def->LogBookQuestCategoryString))category_from_tag=CHeroLogBookEntry::CATEGORY_QUEST;
    if(text_tag.Contains(misc_def->LogBookStoryCategoryString))category_from_tag=CHeroLogBookEntry::CATEGORY_STORY;
    CHeroLogBookEntry new_entry(name_tag,abbreviated_name,text_tag);
    EntryMap::iterator existing=LogEntries.find(category_from_tag);
    if(existing!=LogEntries.end())existing->second.push_back(new_entry);
    else {
        EntryVector new_vector;
        EntryMap::value_type new_value(category_from_tag,new_vector);
        LogEntries.insert(new_value);
        LogEntries[category_from_tag].push_back(new_entry);
    }
}