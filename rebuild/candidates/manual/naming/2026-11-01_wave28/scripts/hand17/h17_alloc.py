# h17_alloc.py: naming wave 17, family stl -- the STLport 4.6 class keys of the vector<bool> rows (c675): std::allocator is a
# class (mangled V), not a struct (U): 0x00451583 vector<bool>(n, value, a), 0x00450CCC _Vector_base<bool>(n, a), their landed
# caller 0x00459AF3 (CPlayerInventoryDef ctor, same-TU stand-in of 0x00451583), and 0x00D34F00 (vector<bool> copy ctor, whose
# _Vector_base / vector models lacked the _Alloc parameter). Name-only: the recheck proves the bytes.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
NOTE = '// Naming wave 17: STLport 4.6 declares allocator as a class (mangled V?$allocator), as c675 proved for 0x00451583 / 0x00450CCC.\n'
edit('00451583', 'src', '''template <class T> struct allocator {
    allocator() { }
};''', '''template <class T> class allocator {
public:
    allocator() { }
};''')
edit('00451583', 'src', '// Leaf: ??0?$vector\n', '// Leaf: ??0?$vector@_NV?$allocator@_N@std@@@std@@QAE@IAB_NABV?$allocator@_N@1@@Z\n' + NOTE)
edit('00450ccc', 'src', '''template <class T> struct allocator {           // STLport allocator<T>: empty
    __forceinline allocator() { }''', '''template <class T> class allocator {            // STLport allocator<T>: empty
public:
    __forceinline allocator() { }''')
edit('00450ccc', 'src', '// Leaf: ??0?$_Vector_base\n', '// Leaf: ??0?$_Vector_base@_NV?$allocator@_N@std@@@std@@QAE@IABV?$allocator@_N@1@@Z\n' + NOTE)
edit('00459af3', 'src', '''template <class T> struct allocator {
    allocator() { }
};''', '''template <class T> class allocator {             // STLport 4.6: a class (V?$allocator; naming wave 17)
public:
    allocator() { }
};''')
edit('00d34f00', 'src', '''template <class T> class _Vector_base {
public:
    T* _M_start;                                        // +0
    T* _M_finish;                                       // +4
    T* _M_end_of_storage;                               // +8 (alloc proxy over the empty allocator)
    _Vector_base(unsigned int n, const allocator<T>& a);    // 0x00450CCC
};''', '''template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                        // +0
    T* _M_finish;                                       // +4
    T* _M_end_of_storage;                               // +8 (alloc proxy over the empty allocator)
    _Vector_base(unsigned int n, const A& a);           // 0x00450CCC
};''')
edit('00d34f00', 'src', '''template <class T> class vector : public _Vector_base<T> {''', '''template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {''')
edit('00d34f00', 'src', '''    vector(const vector& x) : _Vector_base<T>(x.size(), x.get_allocator())''', '''    vector(const vector& x) : _Vector_base<T, A>(x.size(), x.get_allocator())''')
edit('00d34f00', 'src', '// Leaf: ??0?$vector@\n', '// Leaf: ??0?$vector@_NV?$allocator@_N@std@@@std@@QAE@ABV01@@Z\n'
     '// Naming wave 17: the _Vector_base / vector models take STLport\'s _Alloc parameter (the callee is c675\'s\n//   ??0?$_Vector_base@_NV?$allocator@_N@std@@@std@@QAE@IABV?$allocator@_N@1@@Z).\n')
