#ifndef FABLE_STLPORT_PHOTO_ITERATORS_H
#define FABLE_STLPORT_PHOTO_ITERATORS_H
// Original STLport4.6 _iterator_base.h and _iterator.h; real reverse adapter and empty iterator base.
namespace std {
#ifndef FABLE_STLPORT_HTTP
struct input_iterator_tag {};struct forward_iterator_tag:input_iterator_tag {};struct bidirectional_iterator_tag:forward_iterator_tag {};
#endif
template<class C,class V,class D=ptrdiff_t,class P=V*,class R=V&>struct iterator{typedef C iterator_category;typedef V value_type;typedef D difference_type;typedef P pointer;typedef R reference;};
template<class I>struct iterator_traits{typedef typename I::iterator_category iterator_category;typedef typename I::value_type value_type;typedef typename I::difference_type difference_type;typedef typename I::pointer pointer;typedef typename I::reference reference;};
template<class I>class reverse_iterator:public iterator<typename iterator_traits<I>::iterator_category,typename iterator_traits<I>::value_type,typename iterator_traits<I>::difference_type,typename iterator_traits<I>::pointer,typename iterator_traits<I>::reference>{
protected:I current;
public:typedef typename iterator_traits<I>::reference reference;typedef typename iterator_traits<I>::pointer pointer;typedef I iterator_type;
reverse_iterator(){}explicit reverse_iterator(I x):current(x){}reverse_iterator(const reverse_iterator& x):current(x.current){}
I base()const{return current;}reference operator*()const{I tmp=current;return *--tmp;}pointer operator->()const{return &(operator*());}reverse_iterator& operator++(){--current;return *this;}
};
template<class I>bool operator==(const reverse_iterator<I>& a,const reverse_iterator<I>& b){return a.base()==b.base();}
template<class I>bool operator!=(const reverse_iterator<I>& a,const reverse_iterator<I>& b){return !(a==b);}
}
#endif
