// std::vector::resize(size_type n) -- default-construct the fill value and call two-argument resize.
// Leaf: ?resize@?$vector@V?$CArray@G@@V?$allocator@V?$CArray@G@@@std@@@std@@QAEXI@Z
// Identity: probable; callee00F3FAF0 resize(CArray<unsigned short>) proven hand952H027/erase00F3F7C0 and both PDB names; wrapper unreferenced.
// Layout: actual PDB class keys; retail three-pointer vector and CArray. No complete element headers. CIVCountedPointeeBase local virtual count view (Count+4, Delete slot1); generated flattened header lacks usable virtual API.
extern "C" void free(void*);
namespace std {template<class T>class allocator{};template<class V,class Al>class _STLP_alloc_proxy:public Al{public:V _M_data;};template<class T,class Al>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,Al> _M_end_of_storage;_Vector_base():_M_start(0),_M_finish(0){_M_end_of_storage._M_data=0;}};template<class T,class Al=allocator<T> >class vector:public _Vector_base<T,Al>{public:vector(){} ~vector(){for(T* p=this->_M_start;p!=this->_M_finish;++p)p->~T();if(this->_M_start)free(this->_M_start);}void resize(unsigned int,const T&);void resize(unsigned int n){const T value=T();resize(n,value);}};}
template<class T>class CArray:public std::vector<T>{};
typedef CArray<unsigned short> Element;
template void std::vector<Element>::resize(unsigned int);
