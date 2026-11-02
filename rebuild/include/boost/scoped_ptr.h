#ifndef FABLE_BOOST_SCOPED_PTR_H
#define FABLE_BOOST_SCOPED_PTR_H
namespace boost {
template<class T> class scoped_ptr {
public: explicit scoped_ptr(T* p=0):ptr(p) {}
 ~scoped_ptr();
private: T* ptr;
};
}
#endif

