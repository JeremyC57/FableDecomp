#ifdef FABLE_RETAIL_SCOPED_PTR
#ifndef FABLE_BOOST_SCOPED_PTR_H
#define FABLE_BOOST_SCOPED_PTR_H
// boost::scoped_ptr (Boost 1.30 boost/scoped_ptr.hpp): one T* member; the accessors are inline (retail reads the
// pointer straight through them, e.g. CMainGameComponent::PeekWorld). Union of the members used by the rows that need it.
namespace boost {
template<class T> class scoped_ptr {
public:
    explicit scoped_ptr(T* p=0):ptr(p) {}
    ~scoped_ptr();
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }
private:
    T* ptr;
};
}
#endif // FABLE_BOOST_SCOPED_PTR_H
#else
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

#endif // FABLE_RETAIL_SCOPED_PTR
