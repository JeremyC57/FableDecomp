// NUISystem::CComponent::FindAllChildrenOfType(long type, std::list<CComponent*>* found) -- append this component when its type byte matches, otherwise ask every child (virtual) to collect its matches
// Retail: Fable.exe (TLC, VC7.1) @ 0x00531000, 139 bytes (144 with padding).
// Callees: malloc 0x00BFEA0E (the STLport node allocation), FindAllChildrenOfType on each child
//          through vtable slot 6 (`call [edx+18h]`).
//
// Shape: the Type byte is at +0x12d in retail (Ego_r +0x155, `movzx`); a match is
// `found->push_back(this)` inlined: end() (the list's sentinel) is fetched before the 12-byte node is
// malloc'd, the value is placement-constructed (null test on `&node->value`), and the node is linked
// before the sentinel (retail std::list = the sentinel pointer only, no size word). A match does not
// descend into the children.
// Children is the retail 12-byte std::vector<CCountedPointer<CComponent>> at +0xb0 (Ego_r.pdb +0xb8:
// the States map before it is smaller in the shipped binary), 8-byte elements {object, count block};
// as in the landed CComponent rows (0x00530460, 0x005309E0) the element is reached through the
// retail iterator object (`*(begin() + i)`), so `first` is re-read after every virtual call and the
// size is recomputed (`(last - first) >> 3`) for each loop test; the index is unsigned (`jb`).
extern "C" void* __cdecl malloc(unsigned int size);            // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

template<class T> struct CRetailList {                          // retail (STLport) std::list: the sentinel pointer only
    struct node { node* next; node* prev; T value; };
    struct iterator {
        node* p;
        explicit iterator(node* q) : p(q) { }
        iterator(const iterator& o) : p(o.p) { }
    };
    node* head;
    iterator end() { return iterator(head); }
    node* create_node(const T& value)
    {
        node* n = (node*)malloc(sizeof(node));
        new (&n->value) T(value);
        return n;
    }
    iterator insert(iterator where, const T& value)
    {
        node* n = create_node(value);
        node* next = where.p;
        node* prev = next->prev;
        n->next = next;
        n->prev = prev;
        prev->next = n;
        next->prev = n;
        return iterator(n);
    }
    void push_back(const T& value) { insert(end(), value); }
};
template<class T> struct CRetailVector {                        // retail std::vector: {first, last, end} with an iterator OBJECT
    T* first; T* last; T* end_of_storage;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) { }
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }    // STLport operator[] = *(begin() + n)
};

struct CComponent;

struct CCountedComponentPointer {                               // CCountedPointer<NUISystem::CComponent>
    CComponent* m_pObject;                                      // +0
    void*       m_pCount;                                       // +4
};

struct CComponent {                                             // retail NUISystem::CComponent view
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void FindAllChildrenOfType(long type, CRetailList<CComponent*>* found);   // slot 6 (+0x18)
    unsigned char _body[0xb0 - 0x04];                           // +0x04..+0xaf
    CRetailVector<CCountedComponentPointer> Children;           // +0xb0
    unsigned char _padbc[0x12d - 0xbc];
    unsigned char Type;                                         // +0x12d
};

void CComponent::FindAllChildrenOfType(long type, CRetailList<CComponent*>* found)
{
    if (Type == type) {
        found->push_back(this);
    } else {
        for (unsigned long i = 0; i < Children.size(); ++i)
            Children[i].m_pObject->FindAllChildrenOfType(type, found);
    }
}