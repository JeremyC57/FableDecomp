// NUISystem::CComponent::FindAllChildren(CCharString name, list<CComponent*>& found) -- append this component when its Name equals name, otherwise ask every child (virtual) to collect its matches
// Leaf: ?FindAllChildren@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00530F30, 198 bytes (208 with padding), `ret 8` (thiscall: name by value, &found);
//   Ego_r.pdb `?FindAllChildren@CComponent@NUISystem@@UAEXVCCharString@@AAV?$list@PAVCComponent@NUISystem@@...@Z`.
// Callees: Compare(const char*, const char*) 0x00411570 (__fastcall, the inline CCharString equality), malloc 0x00BFEA0E
//   (the 12-byte list node), CCharString copy constructor 0x0099EC30 (the by-value argument built in the argument
//   area), ~CCharString 0x0099EAE0 (the callee-destroyed name, at both exits), FindAllChildren on each child through
//   vtable slot 5 (`call [ebp+14h]`).
// Callers: CComponent vtable 0x0124608C slot 5 and 42 more NUISystem vtables (first CSprite 0x0122F5D4; w23/xref.py).
// Label correction: catalogued as "ParticleMorphs_Find"; it is CComponent::FindAllChildren, the by-name sibling of the
//   landed FindAllChildrenOfType 0x00531000 (slot 6).
// Layout: retail CComponent Name +0x20 (Ego_r.pdb +0x24), Children (12-byte STLport vector of 8-byte
//   CCountedPointer<CComponent>) +0xb0; CCharString data block { chars, length }. Model of the landed 0x00531000 and
//   CThingSwitch::OnThingWithScriptNameApplied 0x0083CC10 (CCharString operator==).
// Shape: `if (Name == name) found.push_back(this); else for (i < Children.size()) Children[i]->FindAllChildren(name,
//   found);` -- the list's end() is read before the node malloc, the node value is a null-tested placement copy; the
//   child and its vtable are fetched before the by-value name is copy-constructed for the call.
extern "C" void* __cdecl malloc(unsigned int size);            // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CCharStringData { public:
    const char*   m_pData;                                      // +0x00
    unsigned long m_length;                                     // +0x04
};
long __fastcall Compare(const char* a, const char* b);          // 0x00411570
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                      // 0x0099EC30
    ~CCharString();                                             // 0x0099EAE0
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

template<class T> struct CRetailList {                          // STLport list: the sentinel pointer only
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
template<class T> struct CRetailVector {                        // STLport vector with an iterator object
    T* first; T* last; T* end_of_storage;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) { }
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

namespace NUISystem {
class CComponent;
}
struct CCountedComponentPointer {                               // CCountedPointer<NUISystem::CComponent>
    NUISystem::CComponent* m_pObject;                           // +0
    void*                  m_pCount;                            // +4
};

namespace NUISystem {
class CComponent {
public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void FindAllChildren(CCharString name, CRetailList<CComponent*>& found);   // slot 5 (+0x14)
    unsigned char _body04[0x20 - 0x04];
    CCharString Name;                                           // +0x20
    unsigned char _body24[0xb0 - 0x24];
    CRetailVector<CCountedComponentPointer> Children;           // +0xb0
};

void CComponent::FindAllChildren(CCharString name, CRetailList<CComponent*>& found)
{
    if (Name == name) {
        found.push_back(this);
    } else {
        for (unsigned long i = 0; i < Children.size(); ++i)
            Children[i].m_pObject->FindAllChildren(name, found);
    }
}
}