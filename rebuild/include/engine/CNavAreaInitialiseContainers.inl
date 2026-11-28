#ifdef FABLE_RETAIL_NAV_AREA_INITIALISE_PASS30
namespace std {
// Retail calls the library owners on these lines. The corresponding declarations retain the original template arguments.
#ifdef FABLE_RETAIL_NAV_AREA_LAYER_PUSH
#ifdef FABLE_RETAIL_NAV_AREA_LAYER_BODY
#ifdef _MSC_VER
// Inline-budget: the layer constructor site is a call in retail at 00A7AA36.
#pragma auto_inline(off)
#endif
template<> inline void vector<CNavQuadTree::CNavigationLayer>::push_back(const value_type& value) {
 if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,value);++this->_M_finish;}
 else _M_insert_overflow(this->_M_finish,value,__false_type(),1,true);
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
#else
template<> void vector<CNavQuadTree::CNavigationLayer>::push_back(const CNavQuadTree::CNavigationLayer&);
#endif
#endif
#ifdef FABLE_RETAIL_NAV_AREA_COPY_PAIR
#ifdef FABLE_RETAIL_NAV_AREA_PAIR_BODY
#ifdef _MSC_VER
// Inline-budget: the two retail constructions call the ordinary pair copy while keeping its destination state live.
#pragma auto_inline(off)
#endif
template<> inline pair<unsigned __int64,list<C2DLineF> >::pair(const pair& p):first(p.first),second(p.second){}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
#else
template<> pair<unsigned __int64,list<C2DLineF> >::pair(const pair<unsigned __int64,list<C2DLineF> >&);
#endif
#endif
#ifdef FABLE_RETAIL_NAV_AREA_ERASE_COPY
#ifdef FABLE_RETAIL_NAV_AREA_POSITION_BODY
#ifdef _MSC_VER
// Inline-budget: the action-point erase calls this STLport overload at 00A7AF03.
#pragma auto_inline(off)
#endif
template<> inline CNavigationPosition* FABLE_FASTCALL __copy<CNavigationPosition*,CNavigationPosition*,int>(
 CNavigationPosition* first,CNavigationPosition* last,CNavigationPosition* result,const random_access_iterator_tag&,int*) {
 for(int n=last-first;n>0;--n){*result=*first;++first;++result;}
 return result;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
#else
template<> CNavigationPosition* FABLE_FASTCALL __copy<CNavigationPosition*,CNavigationPosition*,int>(
 CNavigationPosition*,CNavigationPosition*,CNavigationPosition*,const random_access_iterator_tag&,int*);
#endif
#endif
#ifdef FABLE_RETAIL_NAV_AREA_RAW_PAIR_COPY
template<> pair<unsigned __int64,list<C2DLineF> >* FABLE_FASTCALL
__uninitialized_copy<pair<unsigned __int64,list<C2DLineF> >*,pair<unsigned __int64,list<C2DLineF> >*>(
 pair<unsigned __int64,list<C2DLineF> >*,pair<unsigned __int64,list<C2DLineF> >*,
 pair<unsigned __int64,list<C2DLineF> >*,const __false_type&);
#endif
#ifdef FABLE_RETAIL_NAV_AREA_PAIR_GROW
template<>
#ifdef FABLE_RETAIL_NAV_AREA_PAIR_GROW_INLINE
// Inline-budget: 00A7AB60..00A7AC22 expands this specialization; the plain body also gets a separate call when its pair copy owner is external.
FABLE_FORCEINLINE
#else
inline
#endif
void vector<pair<unsigned __int64,list<C2DLineF> > >::_M_insert_overflow(iterator position,const value_type& x,const __false_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();
    const size_type len=oldSize+(max)(oldSize,fillLength);
    value_type* newStart=this->_M_end_of_storage.allocate(len);
    value_type* newFinish=newStart;
    {
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {
            _Construct(newFinish,x);
            ++newFinish;
        } else
            newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd)
            newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
    }
    _M_clear();
    _M_set(newStart,newFinish,newStart+len);
}
#endif
#ifdef FABLE_RETAIL_NAV_AREA_PAIR_PUSH
template<>
#ifdef FABLE_RETAIL_NAV_AREA_PAIR_PUSH_INLINE
// Inline-budget: at 00A7AB38 the leaf directly tests capacity then constructs or grows the vector.
FABLE_FORCEINLINE
#else
inline
#endif
void vector<pair<unsigned __int64,list<C2DLineF> > >::push_back(const value_type& value) {
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish,value);
        ++this->_M_finish;
    } else {
        _M_insert_overflow(this->_M_finish,value,_IsPODType(),1UL,true);
    }
}
#endif
#ifdef FABLE_RETAIL_NAV_AREA_FILL_NODES
template<> void vector<CANavQuadTreeNode*>::_M_fill_insert(iterator,size_type,const value_type&);
#endif
}
#endif
