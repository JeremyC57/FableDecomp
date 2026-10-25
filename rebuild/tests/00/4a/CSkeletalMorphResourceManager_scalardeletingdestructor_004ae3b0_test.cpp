// Behaviour test for CSkeletalMorphResourceManager's scalar deleting destructor (0x004AE3B0), driven through the source
// object's Create/Destroy helpers: deleting the manager runs the CachedMorphs tree destructor exactly once and frees
// the object.
#include <cstdio>

static int g_treeCtor = 0, g_treeDtor = 0;
class CCharString;
class C3DSkeletalMorphTarget;
template <class T> class CCountedPointer { public: T* Data; void* Info; };
namespace std {
template <class _Tp> struct less { };
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair { };
template <class _Pair> struct _Select1st { };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    void* _M_header; unsigned int _M_node_count; _Compare _M_key_compare;
    _Rb_tree();
    ~_Rb_tree();
};
typedef pair<const CCharString, CCountedPointer<C3DSkeletalMorphTarget> > TValue;
typedef _Rb_tree<CCharString, TValue, _Select1st<TValue>, less<CCharString>, allocator<TValue> > TTree;
template <> TTree::_Rb_tree() : _M_header(0), _M_node_count(0) { ++g_treeCtor; }
template <> TTree::~_Rb_tree() { ++g_treeDtor; }
}

class CSkeletalMorphResourceManager;
CSkeletalMorphResourceManager* CreateSkeletalMorphResourceManager();
void DestroySkeletalMorphResourceManager(CSkeletalMorphResourceManager* manager);

int main()
{
    CSkeletalMorphResourceManager* m = CreateSkeletalMorphResourceManager();
    if (m == 0 || g_treeCtor != 1 || g_treeDtor != 0) { std::printf("004ae3b0_TEST FAIL create\n"); return 1; }
    DestroySkeletalMorphResourceManager(m);
    if (g_treeDtor != 1) { std::printf("004ae3b0_TEST FAIL destroy\n"); return 1; }
    std::printf("004ae3b0_TEST PASS\n");
    return 0;
}
