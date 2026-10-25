// CFileInstaller::IsFileCompleted(const CCharString& name) const -- under the installer lock, is name in the CompletedFiles set
// Leaf: ?IsFileCompleted@CFileInstaller@@QBE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C43E0, 61 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: FableWin.pdb ?IsFileCompleted@CFileInstaller@@QBE_NABVCCharString@@@Z; catalogued as `CodeSectionMap_Contains`.
// Callees: CCriticalSectionLock ctor 0x00A62920 (the section, false) / dtor 0x00A629A0, STLport
//   _Rb_tree<CCharString,...>::_M_find(const CCharString&) const @ 0x004B9370 (node in eax, `ret 4`; the landed
//   CFaction::IsAlliedWithFaction 0x006B9460 calls it for its set<CCharString> too).
// Layout: retail CFileInstaller CriticalSection +0x04 (locked from a const member, so mutable), CompletedFiles
//   set<CCharString> +0x44 (STLport 12 bytes: _M_header +0, _M_node_count +4, empty less<> +8).
// Shape: `find(name) != end()` -- end() is evaluated first (the header word is read before the _M_find call), the
//   compare lands after the lock's destructor (`mov edi,eax ; call ~ ; cmp edi,esi ; setne al`).
class CCharString {
public:
    ~CCharString();
private:
    void* PStringData;
};

struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };   // Win32 critical section, 24 bytes

class CCriticalSectionLock {                    // 8 bytes on the stack (Ego_r.pdb)
public:
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryLock);   // 0x00A62920
    ~CCriticalSectionLock();                                              // 0x00A629A0
    _RTL_CRITICAL_SECTION* CriticalSection;     // +0x00
    bool Locked;                                // +0x04
};

namespace std {
template <class _Tp> struct less {};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class _Value> struct _Rb_tree_const_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_const_iterator(_Rb_tree_node_base* x) : _M_node(x) {}
    bool operator!=(const _Rb_tree_const_iterator& y) const { return _M_node != y._M_node; }
};

template <class _Key, class _Compare = less<_Key> >
class set {
public:
    typedef _Rb_tree_const_iterator<_Key> const_iterator;
    const_iterator end() const { return const_iterator(_M_header); }
    const_iterator find(const _Key& k) const { return const_iterator(_M_find(k)); }
    _Rb_tree_node_base* _M_find(const _Key& k) const;   // 0x004B9370
private:
    _Rb_tree_node_base* _M_header;              // +0x00
    unsigned int        _M_node_count;          // +0x04
    _Compare            _M_key_compare;         // +0x08
};
}

class CFileInstaller {
public:
    bool IsFileCompleted(const CCharString& name) const;
private:
    void*                          _padding_;          // +0x00
    mutable _RTL_CRITICAL_SECTION  CriticalSection;    // +0x04
    unsigned char                  _pad1c[0x28];       // +0x1C BufferChunks .. ActiveFiles
    std::set<CCharString>         CompletedFiles;     // +0x44
};

bool CFileInstaller::IsFileCompleted(const CCharString& name) const
{
    CCriticalSectionLock lock(CriticalSection, false);
    return CompletedFiles.find(name) != CompletedFiles.end();
}