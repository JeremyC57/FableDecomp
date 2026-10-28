// std::vector::resize(n) -- Adjust the element count using default-constructed values.
// Leaf: ?resize@?$vector@U?$pair@VCCharString@@J@std@@V?$allocator@U?$pair@VCCharString@@J@std@@@2@@std@@QAEXI@Z
class CCharString {
public: char* Data; CCharString(); ~CCharString();
};
class CThing;
class CThingAICreature;
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
};
namespace std {
template<class A,class B> struct pair { A first; B second; pair():first(),second(){} };
template<class T> class allocator {};
template<class V,class T,class A> class _STLP_alloc_proxy : public A { public: V _M_data; };
template<class T,class A> class _Vector_base { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,A> _M_end_of_storage; };
template<class T,class A=allocator<T> > class vector : public _Vector_base<T,A> {
public:
    T* erase(T*,T*);
    void _M_fill_insert(T*,unsigned int,const T&);
    unsigned int size() const {return (unsigned int)(this->_M_finish-this->_M_start);}
    __forceinline void resize(unsigned int n,const T& value) {
        if(n<size()) erase(this->_M_start+n,this->_M_finish);
        else _M_fill_insert(this->_M_finish,n-size(),value);
    }
    void resize(unsigned int n) {resize(n,T());}
};
}
template void std::vector<std::pair<CCharString,long> >::resize(unsigned int);