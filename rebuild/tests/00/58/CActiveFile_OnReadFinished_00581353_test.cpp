#include <stdio.h>
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#define FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE
#include "stlport/fable_containers.h"
class CTattooDef { public: mutable unsigned references; CTattooDef() : references(0) {} };
template<class T> class CDefPointer {
public:
    T* Object;
    explicit CDefPointer(T* p=0) : Object(p) { if (Object) ++Object->references; }
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->references; }
    ~CDefPointer() { if (Object) --Object->references; }
};
namespace std {
template<> inline vector<CDefPointer<const CTattooDef> >::~vector() {
    _Destroy(this->_M_start,this->_M_finish);
}
}
int main() {
    CTattooDef tattoo;
    {
        std::vector<CDefPointer<const CTattooDef> > empty;
        if(!empty.empty()) return 1;
    }
    {
        CDefPointer<const CTattooDef> handle(&tattoo);
        {
            std::vector<CDefPointer<const CTattooDef> > copies(3, handle);
            if(copies.size()!=3 || tattoo.references!=4) return 1;
        }
        if(tattoo.references!=1) return 1;
        CDefPointer<const CTattooDef> absent;
        std::vector<CDefPointer<const CTattooDef> > nulls(2, absent);
        if(nulls.size()!=2 || nulls[0].Object!=0) return 1;
    }
    if(tattoo.references!=0) return 1;
    puts("00581353_TEST PASS");
    return 0;
}
