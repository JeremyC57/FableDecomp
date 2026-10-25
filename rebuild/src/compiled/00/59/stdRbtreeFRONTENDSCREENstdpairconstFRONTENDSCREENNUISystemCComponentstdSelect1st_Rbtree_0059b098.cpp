// std::_Rb_tree<FRONTEND_SCREEN, pair<const FRONTEND_SCREEN, NUISystem::CComponent*>, _Select1st<...>, less<FRONTEND_SCREEN>, allocator<...> >::~_Rb_tree() -- STLport tree destructor: clear() the nodes, then free the header node
// Leaf: ??1?$_Rb_tree@W4FRONTEND_SCREEN@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059B098, 23 bytes: `mov esi,ecx ; call clear (0x0059AF2A) ; if (header) free(header)`
//   (size-optimised TU: pop ecx).
// Identity: the out-of-line clear it calls, 0x0059AF2A, is the m_mapScreens tree's _Rb_tree::clear (rename1: count test,
//   _M_erase(root), header reset; the landed NUISystem::CFrontEndManager::ClearComponents 0x0059525D tail-jumps to it), so
//   this is that tree's destructor (`clear()` then _Rb_tree_base's header deallocate). Callers: the CFrontEndManager
//   destructor 0x005954AD (its vtable store 0x012521A8 has RTTI .?AVCFrontEndManager@NUISystem@@; the manifest's
//   CEngineScreenEffectDisplacementRenderer label there is propagated) and the `jmp` thunk 0x0059B343 (map::~map) after
//   the landed map() 0x0059B32A. struct_layouts_egor: CFrontEndManager::m_mapScreens is map<FRONTEND_SCREEN,
//   NUISystem::CComponent*>. The earlier std::list<T_0059b098>::~list placeholder (collision pass) mistook it for a list.
#pragma optimize("s", on)
extern "C" void __cdecl free(void*);
enum FRONTEND_SCREEN { FRONTEND_SCREEN_NONE };
namespace NUISystem { class CComponent; }

namespace std {
template <class _Tp> class allocator;
template <class _T1, class _T2> struct pair;
template <class _Pair> struct _Select1st;
template <class _Tp> struct less;
struct _Rb_tree_node_base;

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    _Rb_tree_node_base* _M_header;              // +0x00 (the header node)
    unsigned int        _M_node_count;          // +0x04
    void clear();                               // 0x0059AF2A for this tree
    ~_Rb_tree()
    {
        clear();
        if (_M_header)
            free(_M_header);
    }
};

typedef pair<const FRONTEND_SCREEN, NUISystem::CComponent*> TScreenValue;
template class _Rb_tree<FRONTEND_SCREEN, TScreenValue, _Select1st<TScreenValue>, less<FRONTEND_SCREEN>,
                        allocator<TScreenValue> >;
}
