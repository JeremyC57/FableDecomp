#include <cstdio>
#include <cstring>

struct _RTL_CRITICAL_SECTION { int Depth; int Entered; };   // stand-in: counts entries

class CCriticalSectionLock {
public:
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryLock) : CriticalSection(&section), Locked(true)
    { (void)tryLock; ++section.Depth; ++section.Entered; }
    ~CCriticalSectionLock() { Release(); }
    void Release() { if (Locked) { --CriticalSection->Depth; Locked = false; } }
    _RTL_CRITICAL_SECTION* CriticalSection;
    bool Locked;
};

class CCharString {
public:
    CCharString(const char* t) : Text(t) {}
    const char* Text;
};

namespace _STL {
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
class set {                                            // stand-in: a sorted array of names behind the header word
public:
    typedef _Rb_tree_const_iterator<_Key> const_iterator;
    const_iterator end() const { return const_iterator(_M_header); }
    const_iterator find(const _Key& k) const { return const_iterator(_M_find(k)); }
    _Rb_tree_node_base* _M_find(const _Key& k) const
    {
        for (int i = 0; i < Count; ++i)
            if (std::strcmp(Names[i], k.Text) == 0) return &Nodes[i];
        return _M_header;
    }
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    _Compare            _M_key_compare;
    const char*         Names[4];
    mutable _Rb_tree_node_base Nodes[4];
    int                 Count;
};
}

class CFileInstaller {
public:
    bool IsFileCompleted(const CCharString& name) const
    {
        CCriticalSectionLock lock(CriticalSection, false);
        return CompletedFiles.find(name) != CompletedFiles.end();
    }
    void*                          _padding_;
    mutable _RTL_CRITICAL_SECTION  CriticalSection;
    _STL::set<CCharString>         CompletedFiles;
};

int main()
{
    CFileInstaller fi;
    std::memset(&fi.CriticalSection, 0, sizeof(fi.CriticalSection));
    _STL::_Rb_tree_node_base header;
    fi.CompletedFiles._M_header = &header;
    fi.CompletedFiles._M_node_count = 2;
    fi.CompletedFiles.Names[0] = "data/levels/a.lev";
    fi.CompletedFiles.Names[1] = "data/levels/b.lev";
    fi.CompletedFiles.Count = 2;
    if (!fi.IsFileCompleted(CCharString("data/levels/a.lev")) || !fi.IsFileCompleted(CCharString("data/levels/b.lev")) ||
        fi.IsFileCompleted(CCharString("data/levels/c.lev")) || fi.CriticalSection.Entered != 3 || fi.CriticalSection.Depth != 0) {
        std::printf("009c43e0_TEST FAIL\n"); return 1;
    }
    std::printf("009c43e0_TEST PASS\n");
    return 0;
}