# h27_pb.py: naming wave 27, family pb (DECISIONS_W26_RESOLVED D7) -- own row 0x004128A0 re-authored as the STLport 4.6 member
#  vector<CWideString, allocator<CWideString> >::push_back(const CWideString&), the spelling reloc_bindings binds there (0x004917B5 and
#  nine more callers; Ego_r.pdb / FableWin.pdb ?push_back@?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@QAEXABVCWideString@@@Z),
#  emitted by an explicit instantiation as the own row 0x00412330 emits its _M_insert_overflow. Was a Leaf-less free __fastcall
#  Vector_PushBack over a VectorPushBackOverlay::InsertRange view. The allocator-less ?push_back@?$vector@VCWideString@@@std@@ bound by
#  0x005955AB retires with it (BINDINGS).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
he.put('004128a0', 'src', '''// std::vector<CWideString>::push_back(const CWideString& x) -- STLport 4.6: copy-construct x at the finish when there is room,
//   else grow through _M_insert_overflow (one copy, at the end)
// Leaf: ?push_back@?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@QAEXABVCWideString@@@Z
// Naming wave 27 (family pb, DECISIONS_W26 D7): the STLport member, the spelling reloc_bindings binds at 0x004128A0 (0x004917B5 and
//   the vector<CWideString> callers; Ego_r.pdb / FableWin.pdb spell it the same), emitted by an explicit instantiation as the own
//   row 0x00412330 emits its _M_insert_overflow. Was a Leaf-less free __fastcall Vector_PushBack over a VectorPushBackOverlay view.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004128A0, 57 bytes, thiscall `ret 4`: CWideString(const CWideString&) 0x0099B720 under the
//   placement-new null test, `add [esi+4],4`; else _M_insert_overflow 0x00412330 (finish, x, the __false_type temporary homed in
//   x's argument slot, 1, true).
inline void* operator new(unsigned int, void* where) { return where; }

class CWideString {
public:
    CWideString(const CWideString& other);                // 0x0099B720
    ~CWideString();                                       // 0x0099B510
private:
    void* PStringData;                                    // +0x00
};

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport 4.6 vector (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void push_back(const _Tp& __x);
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                // 0x00412330
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage) {
        _Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, __false_type(), 1, true);
}

template void vector<CWideString>::push_back(const CWideString&);
}
''')

# 0x005955AB NUISystem::CFrontEndManager::GetProfileNames: the vector takes STLport's allocator argument, so its push_back names the
#  0x004128A0 spelling above and its erase the bound STLport erase 0x00412450; the Leaf is the Ego_r / FableWin protected spelling
#  ?GetProfileNames@CFrontEndManager@NUISystem@@IAEXAAV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@@Z.
a = '005955ab'
he.edit(a, 'src', '''namespace std {
template <class T> class vector {
public:''', '''namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport 4.6: the allocator argument
public:''')
he.edit(a, 'src', '''class CFrontEndManager {
public:
    void GetProfileNames(std::vector<CWideString>& names);''', '''class CFrontEndManager {
protected:                                      // Ego_r.pdb / FableWin.pdb IAE
    void GetProfileNames(std::vector<CWideString>& names);''')
p, rel, t = he._rw(a, 'src')
if 'family pb' not in t:
    L = t.split('\\n') if False else t.split('\n')
    i = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')][0]
    L[i] = '// Leaf: ?GetProfileNames@CFrontEndManager@NUISystem@@IAEXAAV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@@Z'
    L.insert(i + 1, '// Naming wave 27 (family pb, DECISIONS_W26 D7): std::vector takes STLport\'s allocator argument, so push_back names\n'
             '//   0x004128A0\'s STLport spelling and erase the bound STLport erase 0x00412450; GetProfileNames is protected (Ego_r.pdb /\n'
             '//   FableWin.pdb IAE). Was the allocator-less vector<CWideString> (?push_back@?$vector@VCWideString@@@std@@..) and public.')
    he._w(p, '\n'.join(L))
