// NUISystem::CComponent::PositionChildDying(CComponent* child) -- drop a dying child from the position-children set
// Leaf: ?PositionChildDying@CComponent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00531B40, 92 bytes (96 with padding), virtual, `ret 4`.
// Vtable 0x0122F5D4 (NUISystem::CSprite, inherited from CComponent) slot 127, after the landed AddPositionChild
// (slot 124, 0x00531BA0) and SetPositionParent (slot 125); Ego_r.pdb declares
// `virtual void CComponent::PositionChildDying(CComponent*)`.
// Callees: the SGI tree-iterator `__find(first, last, value, input_iterator_tag)` 0x00534F20 (`__fastcall`, hidden
//   iterator result in ecx, &value in edx; the landed front end 0x00535320 calls the same body),
//   _Rb_global::_Rebalance_for_erase 0x0042A34E (`__cdecl`, node + the header's parent/left/right links), free 0x00BFEA14.
// Shape: std::find over the set (a LINEAR search, as the original source wrote it) and an inline STLport
//   `erase(iterator)` when found (null-tested node free, `dec dword ptr` count). The empty input_iterator_tag
//   temporary is homed in the live `child` argument slot (`lea edx,[esp+0Ch] ; push edx` before `&child` is formed
//   again for edx): that needs the real same-TU `__find` body under `auto_inline(off)` with the tag unnamed.
// Layout: retail CComponent PositionChildren (STLport set<CComponent*>, 12 bytes: header, count, compare) at +0x11c
//   (Ego_r.pdb +0x140; the retail map/vector/list containers before it are smaller), as the landed AddPositionChild.
extern "C" void __cdecl free(void* block);

namespace NUISystem {
class CComponent;

struct SSetNode {                                           // _Rb_tree_node<CComponent*>
    long        _M_color;                                   // +0x00
    SSetNode*   _M_parent;                                  // +0x04
    SSetNode*   _M_left;                                    // +0x08
    SSetNode*   _M_right;                                   // +0x0c
    CComponent* _M_value_field;                             // +0x10
};
struct SSetIterator {                                       // _Rb_tree_iterator (user ctors: returned by hidden pointer)
    SSetNode* _M_node;
    SSetIterator() { }
    SSetIterator(SSetNode* node) : _M_node(node) { }
};
struct SInputIteratorTag { SInputIteratorTag() { } };       // input_iterator_tag

SSetNode* __cdecl Rebalance_for_erase(SSetNode* z, SSetNode*& root, SSetNode*& leftmost, SSetNode*& rightmost);   // 0x0042A34E

#pragma auto_inline(off)
// The real SGI `__find` body 0x00534F20 (in-order successor walk comparing the node value); with it visible VC7.1
// knows the tag is never read and lets it share the live value slot, as retail.
__declspec(noinline) SSetIterator __fastcall Find(SSetIterator first, SSetIterator last, CComponent* const& value,
                                                  const SInputIteratorTag&)
{
    SSetNode* node = first._M_node;
    while (node != last._M_node && node->_M_value_field != value) {
        if (node->_M_right != 0) {
            node = node->_M_right;
            while (node->_M_left != 0)
                node = node->_M_left;
        } else {
            SSetNode* parent = node->_M_parent;
            while (node == parent->_M_right) {
                node = parent;
                parent = parent->_M_parent;
            }
            if (node->_M_right != parent)
                node = parent;
        }
    }
    return SSetIterator(node);
}
#pragma auto_inline(on)

struct CComponentSet {                                      // std::set<CComponent*> (STLport, 12 bytes)
    SSetNode*     _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    SSetIterator begin() const { return SSetIterator(_M_header->_M_left); }
    SSetIterator end() const { return SSetIterator(_M_header); }
    void erase(SSetIterator position)
    {
        SSetNode* erased = Rebalance_for_erase(position._M_node, _M_header->_M_parent, _M_header->_M_left, _M_header->_M_right);
        if (erased)
            free(erased);
        --_M_node_count;
    }
};

class CComponent {
public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void PositionChildDying(CComponent* child);     // slot 127 (+0x1fc)
    unsigned char _pad_0x04[0x11c - 0x04];
    CComponentSet PositionChildren;                         // +0x11c
};

void CComponent::PositionChildDying(CComponent* child)
{
    SSetIterator it = Find(PositionChildren.begin(), PositionChildren.end(), child, SInputIteratorTag());
    if (it._M_node != PositionChildren._M_header)
        PositionChildren.erase(it);
}
}