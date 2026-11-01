// Self-contained target body; analysis-only helper body replaced by an observational fixture.
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __cdecl
#endif
#define malloc fixture_malloc
#define free fixture_free
// 00525d90 CThingManager::OnLevelUnload
// Leaf: ?OnLevelUnload@CThingManager@@QAEXJ_N@Z
// Flags:
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
namespace std {
template <class _Tp> class allocator { public:
    allocator() {}
    _Tp* allocate(unsigned int __n, const void* = 0) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value p) : _MaybeReboundAlloc(__a), _M_data(p) {}
};
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __x;
    return __first;
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc&__a):_M_start(0),_M_finish(0),_M_end_of_storage(__a,0){}
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a,0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { if(_M_start) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> { public:
    vector(const _Alloc&__a=_Alloc()):_Vector_base<_Tp,_Alloc>(__a){}
    const _Tp*begin()const{return this->_M_start;}const _Tp*end()const{return this->_M_finish;}
    vector(unsigned int __n, const _Tp& __val, const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__n, __a)
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, __val);
    }
};
}
class CThing;
class CThingManager {
private:void SaveThingsOnLevelReadyForUnloading(long,bool,std::vector<CThing*>&);
public:
    void OnLevelsUnload(const std::vector<long>& levels, bool saveThings);   // 0x00525CD0
    void OnLevelUnload(long level, bool saveThings);
};
// Analysis-only nonescaping callee context: its original level loop reads the vector.
// The callee is not a reconstruction target; tests substitute an observational fixture.

void CThingManager::OnLevelUnload(long level, bool saveThings)
{
    std::vector<long> levels(1, level);
    OnLevelsUnload(levels, saveThings);
}

#undef malloc
#undef free
static unsigned allocations,frees,calls;
static void*block;
static long expectedLevel;
static bool expectedSave;
static CThingManager*expectedManager;
extern "C" void* __cdecl fixture_malloc(unsigned int size){if(size!=sizeof(long)||block)abort();++allocations;block=malloc(size);if(!block)abort();return block;}
extern "C" void __cdecl fixture_free(void*p){if(p!=block||!p||calls!=allocations)abort();++frees;free(p);block=0;}
void CThingManager::OnLevelsUnload(const std::vector<long>&levels,bool saveThings){
 if(this!=expectedManager||saveThings!=expectedSave||levels.begin()!=block||levels.end()!=levels.begin()+1||levels.begin()[0]!=expectedLevel||allocations!=calls+1||frees!=calls)abort();++calls;
}
int main(){
 CThingManager manager;expectedManager=&manager;
 const long levels[]={0,1,-1,65535,-65536,2147483647L,-2147483647L,LONG_MIN,LONG_MAX};
 for(unsigned i=0;i<sizeof levels/sizeof levels[0];++i)for(int flag=0;flag<2;++flag){expectedLevel=levels[i];expectedSave=flag!=0;manager.OnLevelUnload(expectedLevel,expectedSave);if(block||frees!=calls||allocations!=calls)return 1;}
 if(calls!=18)return 2;puts("00525d90_TEST PASS");return 0;
}