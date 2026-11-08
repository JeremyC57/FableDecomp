// std::vector::resize(n) -- Adjust the element count using default-constructed values.
// Leaf: ?resize@?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QAEXI@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#include "fable_compiler.h"
class CCharString {
public: char* Data; CCharString(); ~CCharString();
};
class CThing;
class CThingAICreature;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
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
    FABLE_FORCEINLINE void resize(unsigned int n,const T& value) {
        if(n<size()) erase(this->_M_start+n,this->_M_finish);
        else _M_fill_insert(this->_M_finish,n-size(),value);
    }
    void resize(unsigned int n) {resize(n,T());}
};
}
template void std::vector<CIntelligentPointer<CThing> >::resize(unsigned int);