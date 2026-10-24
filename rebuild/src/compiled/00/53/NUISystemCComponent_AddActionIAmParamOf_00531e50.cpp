// NUISystem::CComponent::AddActionIAmParamOf(CCountedPointer<CAction>& action) -- remember an action that takes this component as a parameter (push_back onto ActionsIAmParamOf)
// Leaf: ?AddActionIAmParamOf@CComponent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00531E50, 63 bytes (64 with padding), virtual, `ret 4`.
// Vtable 0x0122F5D4 (NUISystem::CSprite, inherited from CComponent) slot 96; Ego_r.pdb declares
// `virtual void CComponent::AddActionIAmParamOf(CCountedPointer<NUISystem::CAction>&)`.
// Callees: the CRT malloc import thunk 0x00BFEA0E (the STLport node allocator; `operator new` would bind ??2).
// Shape: an inline STLport `list::push_back(x)` = `insert(end(), x)`: the head node is read before the 16-byte node
//   allocation, the counted pointer is copied into the node's data under the placement null test (object word, then
//   the count block, add-ref when present), then the node is linked before the head with `prev = pos->_M_prev` in a
//   local (the landed STLport list model of 0x00757C90 plus a
//   declared iterator copy ctor and `create_node`, which keep the edi push at entry instead of shrink-wrapped).
// Layout: retail CComponent ActionsIAmParamOf (STLport list, 4 bytes: the head node) at +0xe0 (Ego_r.pdb +0xf8).
extern "C" void* __cdecl malloc(unsigned int size);

namespace NUISystem {
class CAction;

struct SCountBlock { long RefCount; };
template <class T> struct CCountedPointer {
    T*           Data;
    SCountBlock* Info;
    void Construct(const CCountedPointer& other)
    {
        Data = other.Data;
        Info = other.Info;
        if (Info)
            ++Info->RefCount;
    }
};

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CListIterator {                    // STLport _List_iterator
    SListNode<T>* _M_node;
    CListIterator(SListNode<T>* n) : _M_node(n) { }
    CListIterator(const CListIterator& other) : _M_node(other._M_node) { }
};
template <class T> struct CRetailList {                      // STLport list (4 bytes: the head node)
    SListNode<T>* _M_node;
    CListIterator<T> end() { return CListIterator<T>(_M_node); }
    SListNode<T>* create_node(const T& x)
    {
        SListNode<T>* tmp = (SListNode<T>*)malloc(sizeof(SListNode<T>));
        T* data = &tmp->_M_data;
        if (data != 0)
            data->Construct(x);
        return tmp;
    }
    CListIterator<T> insert(CListIterator<T> position, const T& x)
    {
        SListNode<T>* tmp = create_node(x);
        SListNode<T>* n = position._M_node;
        SListNode<T>* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return CListIterator<T>(tmp);
    }
    void push_back(const T& x) { insert(end(), x); }
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
    virtual void AddActionIAmParamOf(CCountedPointer<CAction>& action);   // slot 96 (+0x180)
    unsigned char _pad_0x04[0xe0 - 0x04];
    CRetailList<CCountedPointer<CAction> > ActionsIAmParamOf;          // +0xe0
};

void CComponent::AddActionIAmParamOf(CCountedPointer<CAction>& action)
{
    ActionsIAmParamOf.push_back(action);
}
}