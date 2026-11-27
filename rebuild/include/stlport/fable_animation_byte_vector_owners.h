#ifndef FABLE_STLP_ANIMATION_BYTE_VECTOR_OWNERS_H
#define FABLE_STLP_ANIMATION_BYTE_VECTOR_OWNERS_H
#ifdef FABLE_RETAIL_STLP_ANIMATION_BYTE_VECTOR_OWNERS
// Inline-budget: the speed-built enclosing leaf expands both resize overloads.
// The out-of-line copies at 00486189 and 004861DF use size code; the latter has a frame.
#if defined(_MSC_VER)
#pragma optimize("s",on)
#endif
#if defined(_MSC_VER)
#pragma optimize("y",off)
#endif
// Inline-budget: 00485B6C expands the unsigned-byte fill_n and directly calls fill.
template<> FABLE_FORCEINLINE unsigned char* std::fill_n(unsigned char* first,unsigned int n,const unsigned char& value) {
    fill(first,first+n,value);
    return first+n;
}
// Inline-budget: the retail caller invokes the size-built byte specialization at 00485B6C.
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
void std::vector<unsigned char>::_M_insert_overflow(iterator position,const unsigned char& x,const std::__true_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    unsigned char* newStart=this->_M_end_of_storage.allocate(len);
#if defined(FABLE_RETAIL_STLP_DEFINITION_STREAM_BYTE_COPIES) && defined(_MSC_VER)
// Inline-budget: the retail byte-vector owner calls the shared pointer-copy routine here.
#pragma inline_depth(0)
#endif
    unsigned char* newFinish=(unsigned char*)__copy_trivial(this->_M_start,position,newStart);
#if defined(FABLE_RETAIL_STLP_DEFINITION_STREAM_BYTE_COPIES) && defined(_MSC_VER)
#pragma inline_depth()
#endif
    newFinish=fill_n(newFinish,fillLength,x);
#if defined(FABLE_RETAIL_STLP_DEFINITION_STREAM_BYTE_COPIES) && defined(_MSC_VER)
// Inline-budget: the retail byte-vector owner calls the shared pointer-copy routine here.
#pragma inline_depth(0)
#endif
    if(!atEnd) newFinish=(unsigned char*)__copy_trivial(position,this->_M_finish,newFinish);
#if defined(FABLE_RETAIL_STLP_DEFINITION_STREAM_BYTE_COPIES) && defined(_MSC_VER)
#pragma inline_depth()
#endif
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
template<> inline void std::vector<unsigned char>::_M_fill_insert(
    iterator position, size_type n, const unsigned char& x) {
    if(n!=0) {
        if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n) {
            value_type x_copy=x;
            const size_type elems_after=this->_M_finish-position;
            value_type* old_finish=this->_M_finish;
            if(elems_after>n) {
#if defined(FABLE_RETAIL_STLP_DEFINITION_STREAM_BYTE_COPIES) && defined(_MSC_VER)
// Inline-budget: the retail byte-vector owner calls the shared pointer-copy routine here.
#pragma inline_depth(0)
#endif
                __copy_trivial(this->_M_finish-n,this->_M_finish,this->_M_finish);
#if defined(FABLE_RETAIL_STLP_DEFINITION_STREAM_BYTE_COPIES) && defined(_MSC_VER)
#pragma inline_depth()
#endif
                this->_M_finish+=n;
                __copy_trivial_backward(position,old_finish-n,old_finish);
                fill(position,position+n,x_copy);
            } else {
                fill(this->_M_finish,this->_M_finish+(n-elems_after),x_copy);
                this->_M_finish+=n-elems_after;
#if defined(FABLE_RETAIL_STLP_DEFINITION_STREAM_BYTE_COPIES) && defined(_MSC_VER)
// Inline-budget: the retail byte-vector owner calls the shared pointer-copy routine here.
#pragma inline_depth(0)
#endif
                __copy_trivial(position,old_finish,this->_M_finish);
#if defined(FABLE_RETAIL_STLP_DEFINITION_STREAM_BYTE_COPIES) && defined(_MSC_VER)
#pragma inline_depth()
#endif
                this->_M_finish+=elems_after;
                fill(position,old_finish,x_copy);
            }
        } else _M_insert_overflow(position,x,_IsPODType(),n,false);
    }
}
#if defined(_MSC_VER)
#pragma optimize("",on)
#pragma optimize("s",on)
#endif
template<> inline
#if defined(FABLE_RETAIL_STLP_DEFINITION_STREAM_BYTE_COPIES) && defined(_MSC_VER)
// Inline-budget: the framed one-argument resize calls this retail specialization at 00486189.
__declspec(noinline)
#endif
void std::vector<unsigned char>::resize(size_type n,const unsigned char& x) { if (n<size()) erase(begin()+n,end()); else insert(end(),n-size(),x); }
#if defined(_MSC_VER)
#pragma optimize("y",off)
#endif
template<> inline void std::vector<unsigned char>::resize(size_type n) { resize(n,value_type()); }
#if defined(_MSC_VER)
#pragma optimize("",on)
#endif

#endif
#endif
