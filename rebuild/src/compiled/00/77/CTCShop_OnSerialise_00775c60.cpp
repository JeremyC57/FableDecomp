// CTCShop::OnSerialise(CPersistContext&, bool) -- save or restore current stock and shop counters
// Leaf: ?OnSerialise@CTCShop@@UAEXAAVCPersistContext@@_N@Z
// Genuine STLport algorithms restored below; local adaptation uses existing declarations.
/*
 *
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 * Modified CRP 7/10/00 for improved conformance / efficiency on insert_unique /
 * insert_equal with valid hint -- efficiency is improved all around, and it is
 * should now be standard conforming for complexity on insert point immediately
 * after hint (amortized constant time).
 *
 */


#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "fable_string.h"
#include "engine/CTCShop.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefString.h"
#include "engine/CPersistContext.h"
#pragma auto_inline(off)
namespace std {
template<> _Rb_tree<long,pair<const long,CShopItem>,_Select1st<pair<const long,CShopItem> >,less<long>,allocator<pair<const long,CShopItem> > >::iterator _Rb_tree<long,pair<const long,CShopItem>,_Select1st<pair<const long,CShopItem> >,less<long>,allocator<pair<const long,CShopItem> > >::_M_insert(_Rb_tree_node_base* __x_,_Rb_tree_node_base* __y_,const pair<const long,CShopItem>& __v,_Rb_tree_node_base* __w_)
{
  _Link_type __w = (_Link_type) __w_;
  _Link_type __x = (_Link_type) __x_;
  _Link_type __y = (_Link_type) __y_;
  _Link_type __z;

  if ( __y == this->_M_header._M_data ||
       ( __w == 0 && 
         ( __x != 0 ||     
           _M_key_compare( _Select1st<pair<const long,CShopItem> >()(__v), _S_key(__y) ) )
	 )
       ) {
    
    __z = _M_create_node(__v);
    _S_left(__y) = __z;               
                                      
    if (__y == this->_M_header._M_data) {
      _M_root() = __z;
      _M_rightmost() = __z;
    }
    else if (__y == _M_leftmost())
      _M_leftmost() = __z;   
  }
  else {
    __z = _M_create_node(__v);
    _S_right(__y) = __z;
    if (__y == _M_rightmost())
      _M_rightmost() = __z;  
  }
  _S_parent(__z) = __y;
  _S_left(__z) = 0;
  _S_right(__z) = 0;
  _Rb_global_inst::_Rebalance(__z, this->_M_header._M_data->_M_parent);
  ++_M_node_count;
  return iterator(__z);
}
template<> pair<_Rb_tree<long,pair<const long,CShopItem>,_Select1st<pair<const long,CShopItem> >,less<long>,allocator<pair<const long,CShopItem> > >::iterator,bool> _Rb_tree<long,pair<const long,CShopItem>,_Select1st<pair<const long,CShopItem> >,less<long>,allocator<pair<const long,CShopItem> > >::insert_unique(const pair<const long,CShopItem>& __v)
{
  _Link_type __y = this->_M_header._M_data;
  _Link_type __x = _M_root();
  bool __comp = true;
  while (__x != 0) {
    __y = __x;
    __comp = _M_key_compare(_Select1st<pair<const long,CShopItem> >()(__v), _S_key(__x));
    __x = __comp ? _S_left(__x) : _S_right(__x);
  }
  iterator __j = iterator(__y);   
  if (__comp)
    if (__j == begin())     
      return pair<iterator,bool>(_M_insert(/* __x*/ __y, __y, __v), true);
    else
      --__j;
  if (_M_key_compare(_S_key(__j._M_node), _Select1st<pair<const long,CShopItem> >()(__v)))
    return pair<iterator,bool>(_M_insert(__x, __y, __v), true);
  return pair<iterator,bool>(__j, false);
}
template<> _Rb_tree<long,pair<const long,CShopItem>,_Select1st<pair<const long,CShopItem> >,less<long>,allocator<pair<const long,CShopItem> > >::iterator _Rb_tree<long,pair<const long,CShopItem>,_Select1st<pair<const long,CShopItem> >,less<long>,allocator<pair<const long,CShopItem> > >::insert_unique(iterator __position,const pair<const long,CShopItem>& __v)
{
  if (__position._M_node == this->_M_header._M_data->_M_left) { 

    
    if (size() <= 0)
      return insert_unique(__v).first;

    if ( _M_key_compare(_Select1st<pair<const long,CShopItem> >()(__v), _S_key(__position._M_node)))
      return _M_insert(__position._M_node, __position._M_node, __v);
    
    else
      {
	bool __comp_pos_v = _M_key_compare( _S_key(__position._M_node), _Select1st<pair<const long,CShopItem> >()(__v) );
	
	if (__comp_pos_v == false)  
	  return __position;
	

	
	
	iterator __after = __position;
	++__after;

	
	
	if (__after._M_node == this->_M_header._M_data)
	  
	  
	  
	  return _M_insert(0, __position._M_node, __v, __position._M_node);
		
	
	
	
	
	
	if(_M_key_compare( _Select1st<pair<const long,CShopItem> >()(__v), _S_key(__after._M_node) )) {
	  if (_S_right(__position._M_node) == 0)
	    return _M_insert(0, __position._M_node, __v, __position._M_node);
	  else
	    return _M_insert(__after._M_node, __after._M_node, __v);
	} else {
	    return insert_unique(__v).first;
	}
      }

  } else if (__position._M_node == this->_M_header._M_data) { 
    if (_M_key_compare(_S_key(_M_rightmost()), _Select1st<pair<const long,CShopItem> >()(__v)))
      
      
      return _M_insert(0, _M_rightmost(), __v, __position._M_node); 
    else
      return insert_unique(__v).first;
  } else {
    iterator __before = __position;
    --__before;
    
    bool __comp_v_pos = _M_key_compare(_Select1st<pair<const long,CShopItem> >()(__v), _S_key(__position._M_node));

    if (__comp_v_pos
      && _M_key_compare( _S_key(__before._M_node), _Select1st<pair<const long,CShopItem> >()(__v) )) {

      if (_S_right(__before._M_node) == 0)
        return _M_insert(0, __before._M_node, __v, __before._M_node); 
      else
        return _M_insert(__position._M_node, __position._M_node, __v);
    
    } else
      {
	
	iterator __after = __position;
	++__after;
	
	
	bool __comp_pos_v = !__comp_v_pos;  
	
	
	
	
	if (!__comp_v_pos) __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _Select1st<pair<const long,CShopItem> >()(__v));
	
	if ( (!__comp_v_pos) 
	     && __comp_pos_v
	     && (__after._M_node == this->_M_header._M_data ||
	        _M_key_compare( _Select1st<pair<const long,CShopItem> >()(__v), _S_key(__after._M_node) ))) {
	  
	  if (_S_right(__position._M_node) == 0)
	    return _M_insert(0, __position._M_node, __v, __position._M_node);
	  else
	    return _M_insert(__after._M_node, __after._M_node, __v);
	} else {
	  
	  if (__comp_v_pos == __comp_pos_v)
	    return __position;
	  else
	    return insert_unique(__v).first;
	}
      }
  }
}
template<> CShopItem& map<long,CShopItem>::operator[](const long& key){iterator it=lower_bound(key);if(it==end()||key_comp()(key,(*it).first))it=insert(it,value_type(key,CShopItem()));return (*it).second;}
}
#pragma auto_inline(on)
void CTCShop::OnSerialise(CPersistContext& context,bool in_game)
{
    CTCBase::OnSerialise(context,in_game);
    if(context.IsSaving()) {
        if(in_game) {
            std::vector<CCharString> current_stock_item_defs_keys;
            std::vector<float> current_stock_item_defs_quantity;
            std::vector<float> current_stock_item_defs_max_stock;
            std::map<long,CShopItem>::iterator end_it=Stock.end();
            std::map<long,CShopItem>::iterator it=Stock.begin();
            for(;it!=end_it;++it) {
                current_stock_item_defs_keys.push_back(GetDefinitionManager().GetDefNameFromGlobalIndex(it->first));
                current_stock_item_defs_quantity.push_back(it->second.Quantity);
                current_stock_item_defs_max_stock.push_back(it->second.MaxStock);
            }
            context.TransferVector("StockItemDefsKeys",current_stock_item_defs_keys);
            context.TransferVector("StockItemDefsQuantity",current_stock_item_defs_quantity);
            context.TransferVector("StockItemDefsMaxStock",current_stock_item_defs_max_stock);
            context.Transfer("DayLastChecked",DayLastChecked,0L);
            context.Transfer("GoldDonated",GoldDonated,0L);
            context.Transfer("SpecialItem",SpecialItem,-1L);
            context.Transfer("DaysSinceSpecialItemPlaced",DaysSinceSpecialItemPlaced,0L);
        }
    } else {
        std::vector<CCharString> current_stock_item_defs_keys;
        std::vector<float> current_stock_item_defs_quantity;
        std::vector<float> current_stock_item_defs_max_stock;
        context.TransferVector("StockItemDefsKeys",current_stock_item_defs_keys);
        context.TransferVector("StockItemDefsQuantity",current_stock_item_defs_quantity);
        context.TransferVector("StockItemDefsMaxStock",current_stock_item_defs_max_stock);
        context.Transfer("DayLastChecked",DayLastChecked,0L);
        context.Transfer("GoldDonated",GoldDonated,0L);
        context.Transfer("SpecialItem",SpecialItem,-1L);
        context.Transfer("DaysSinceSpecialItemPlaced",DaysSinceSpecialItemPlaced,0L);
        if(current_stock_item_defs_keys.size()>0) {
            Stock.clear();
            for(long n=0;n<current_stock_item_defs_keys.size();++n) {
                long def_index=GetDefinitionManager().GetDefGlobalIndexFromName(current_stock_item_defs_keys[n]);
                if(def_index>0 && FindDefaultStockItem(def_index)) {
                    CShopItem item=*FindDefaultStockItem(def_index);
                    item.Quantity=current_stock_item_defs_quantity[n];
                    item.MaxStock=current_stock_item_defs_max_stock[n];
                    Stock[def_index]=item;
                }
            }
        }
    }
}