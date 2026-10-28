#include <stdio.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
// std::vector::resize(n) -- Adjust the element count using default-constructed values.
// Leaf: ?resize@?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QAEXI@Z
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
template void std::vector<CIntelligentPointer<CThing> >::resize(unsigned int);

static int ctors,dtors,erases,inserts,bad;
static void* expected_pos;static void* expected_last;static unsigned int expected_count;
CCharString::CCharString():Data(0){++ctors;} CCharString::~CCharString(){++dtors;}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){++ctors;} CBaseIntelligentPointer::~CBaseIntelligentPointer(){++dtors;}
namespace std {
template<class T,class A> T* vector<T,A>::erase(T* first,T* last) {
    ++erases;if(first!=expected_pos || last!=expected_last)bad=1;this->_M_finish=first;return first;
}
template<class T,class A> void vector<T,A>::_M_fill_insert(T* pos,unsigned int n,const T& value) {
    ++inserts;if(pos!=expected_pos || n!=expected_count || !(value.PData==0))bad=1;this->_M_finish+=n;
}
}
int main() {
    typedef CIntelligentPointer<CThing> Item;Item entries[5];std::vector<Item> v;
    v._M_start=entries;v._M_finish=entries+3;v._M_end_of_storage._M_data=entries+5;
    ctors=dtors=0;expected_pos=entries+1;expected_last=entries+3;v.resize(1);
    int ok=!bad && erases==1 && inserts==0 && ctors==1 && dtors==1 && v._M_finish==entries+1;
    expected_pos=entries+1;expected_count=3;v.resize(4);
    ok=ok && !bad && erases==1 && inserts==1 && ctors==2 && dtors==2 && v._M_finish==entries+4;
    expected_pos=entries+4;expected_count=0;v.resize(4);
    ok=ok && !bad && inserts==2 && ctors==3 && dtors==3 && v._M_finish==entries+4;
    puts(ok ? "00739b40_TEST PASS" : "00739b40_TEST FAIL");return ok ? 0 : 1;
}