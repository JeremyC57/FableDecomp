// std::_List_base<NPlayerGui::CScriptScreenText>::clear() -- destroy and free every script screen text node, leave the sentinel linked to itself
// Leaf: ?clear@?$_List_base@
// Retail: Fable.exe @ 0x0043FDF0, 70 bytes, plain `ret`; callers 0x0043A60A, 0x004405D3, 0x00440CB3 (next to the
// _List_base<CWideString>::clear call 0x0043A5EE on CPlayerGui teardown). Catalog label CNavigatorQueueManager::FrameUpdate is
// wrong: STLport _List_base::clear over CPlayerGui::ScriptScreenText -- per node ~CCharString on FontName (value +0x10,
// 0x0099EAE0) then ~CWideString on Text (value +0, 0x0099B510), free(node) (0x00BFEA14); then the sentinel's
// _M_next / _M_prev point back at itself.
// Layout: STLport list -- one sentinel-node pointer; node {_M_next +0, _M_prev +4, _M_data +8}; NPlayerGui::CScriptScreenText
// (Ego_r.pdb, 24 bytes: CWideString Text +0, C2DVector VPos +4, CRGBColour Colour +0xc, CCharString FontName +0x10,
// ulong FrameAdded +0x14) with an implicit destructor.
// Shape: /O2 (`nop` loop-head pad, end re-read from the sentinel pointer every pass).
#include <stdlib.h>
class CWideString {
public:
    void* PData;
    ~CWideString();                                      // 0x0099B510
};
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};
namespace NPlayerGui {
class CScriptScreenText {
public:
    CWideString   Text;                                  // +0x00
    float         VPos[2];                               // +0x04 C2DVector
    unsigned long Colour;                                // +0x0c CRGBColour
    CCharString   FontName;                              // +0x10
    unsigned long FrameAdded;                            // +0x14
};
}
namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                            // +0x00
    _List_node_base* _M_prev;                            // +0x04
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                           // +0x08
};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> class _List_base {
public:
    _List_node<T>* _M_node;                              // sentinel
    void clear()
    {
        _List_node<T>* cur = (_List_node<T>*)_M_node->_M_next;
        while (cur != _M_node) {
            _List_node<T>* tmp = cur;
            cur = (_List_node<T>*)cur->_M_next;
            _Destroy(&tmp->_M_data);
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
}
template class std::_List_base<NPlayerGui::CScriptScreenText>;