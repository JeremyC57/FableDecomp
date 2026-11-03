#ifdef FABLE_RETAIL_UI_FACTORY
#ifdef FABLE_RETAIL_PRIMITIVE_BUMP
#ifndef FABLE_BOOST_SCOPED_PTR_H
#define FABLE_BOOST_SCOPED_PTR_H
// Original boost::scoped_ptr, confirmed PDB private ptr and public operator->.
namespace boost {
template<class T> class scoped_ptr {
private:
    T* ptr;
    scoped_ptr(const scoped_ptr&);
    scoped_ptr& operator=(const scoped_ptr&);
public:
    explicit scoped_ptr(T* p=0):ptr(p){}
    ~scoped_ptr(){delete ptr;}
    T* operator->() const { return ptr; }
};
}
#endif

#else
#ifndef FABLE_BOOST_SCOPED_PTR_H
#define FABLE_BOOST_SCOPED_PTR_H
namespace boost {
template<class T> class scoped_ptr {
public: explicit scoped_ptr(T* p=0):ptr(p) {}
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_CPLAYERGUI_DTOR)
 ~scoped_ptr() { delete ptr; }
 T* operator->() const { return ptr; }
 T* get() const { return ptr; }
 void swap(scoped_ptr& other) { T* old=other.ptr;other.ptr=ptr;ptr=old; }
 void reset(T* p=0) { scoped_ptr(p).swap(*this); }
#else
 ~scoped_ptr();
#endif
#if defined(FABLE_RETAIL_CENTERED_TEXT) || defined(FABLE_RETAIL_UI_FACTORY)
 T& operator*() const { return *ptr; }
#endif
private: T* ptr;
};
}
#endif

#endif

#else
#ifdef FABLE_RETAIL_GFMAIN
#ifdef FABLE_RETAIL_PRIMITIVE_BUMP
#ifndef FABLE_BOOST_SCOPED_PTR_H
#define FABLE_BOOST_SCOPED_PTR_H
// Original boost::scoped_ptr, confirmed PDB private ptr and public operator->.
namespace boost {
template<class T> class scoped_ptr {
private:
    T* ptr;
    scoped_ptr(const scoped_ptr&);
    scoped_ptr& operator=(const scoped_ptr&);
public:
    explicit scoped_ptr(T* p=0):ptr(p){}
    ~scoped_ptr(){delete ptr;}
    T* operator->() const { return ptr; }
};
}
#endif

#else
#ifndef FABLE_BOOST_SCOPED_PTR_H
#define FABLE_BOOST_SCOPED_PTR_H
namespace boost {
template<class T> class scoped_ptr {
public: explicit scoped_ptr(T* p=0):ptr(p) {}
#ifdef FABLE_RETAIL_SCOREBOARD
 ~scoped_ptr() { delete ptr; }
 T* operator->() const { return ptr; }
 T* get() const { return ptr; }
 void swap(scoped_ptr& other) { T* old=other.ptr;other.ptr=ptr;ptr=old; }
 void reset(T* p=0) { scoped_ptr(p).swap(*this); }
#else
 ~scoped_ptr();
#endif
#if defined(FABLE_RETAIL_CENTERED_TEXT) || defined(FABLE_RETAIL_WILL_BUTTON_TIMER) || defined(FABLE_RETAIL_GFMAIN)
 T& operator*() const { return *ptr; }
#ifdef FABLE_RETAIL_GFMAIN
 T*get()const{return ptr;}
#endif
#endif
private: T* ptr;
};
}
#endif

#endif

#else
#ifdef FABLE_RETAIL_PRIMITIVE_BUMP
#ifndef FABLE_BOOST_SCOPED_PTR_H
#define FABLE_BOOST_SCOPED_PTR_H
// Original boost::scoped_ptr, confirmed PDB private ptr and public operator->.
namespace boost {
template<class T> class scoped_ptr {
private:
    T* ptr;
    scoped_ptr(const scoped_ptr&);
    scoped_ptr& operator=(const scoped_ptr&);
public:
    explicit scoped_ptr(T* p=0):ptr(p){}
    ~scoped_ptr(){delete ptr;}
    T* operator->() const { return ptr; }
};
}
#endif

#else
#ifndef FABLE_BOOST_SCOPED_PTR_H
#define FABLE_BOOST_SCOPED_PTR_H
namespace boost {
template<class T> class scoped_ptr {
public: explicit scoped_ptr(T* p=0):ptr(p) {}
#ifdef FABLE_RETAIL_SCOREBOARD
 ~scoped_ptr() { delete ptr; }
 T* operator->() const { return ptr; }
 T* get() const { return ptr; }
 void swap(scoped_ptr& other) { T* old=other.ptr;other.ptr=ptr;ptr=old; }
 void reset(T* p=0) { scoped_ptr(p).swap(*this); }
#else
 ~scoped_ptr();
#endif
#if defined(FABLE_RETAIL_CENTERED_TEXT) || defined(FABLE_RETAIL_WILL_BUTTON_TIMER)
 T& operator*() const { return *ptr; }
#endif
private: T* ptr;
};
}
#endif

#endif

#endif

#endif
