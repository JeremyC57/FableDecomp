#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
// Model of NCameraModes::CGetAllThingsOfType::operator()(const CThing*) @ 0x00877110: append the thing to the functor's
// list (STLport push_back: malloc'd 12-byte node linked before the sentinel) when its def name equals DefName.
class CDefString { public:
    const char* Text;
    bool operator==(const char* text) const { return std::strcmp(Text, text) == 0; }   // 0x005B3440
};
class CThing {
public:
    const char* DefName;
    CDefString GetDefName() const { CDefString s; s.Text = DefName; return s; }      // 0x004C7CC0
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : public _List_node_base { T _M_data; };
template <class T> struct _List_iterator {
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    _List_node_base* _M_node;
};
template <class T> class list {
public:
    typedef _List_iterator<T> iterator;
    list()
    {
        _M_node = (_List_node_base*)malloc(sizeof(_List_node<T>));
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    iterator end() { return iterator(_M_node); }
    void insert(iterator position, const T& x)
    {
        _List_node<T>* node = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        ::new (&node->_M_data) T(x);
        _List_node_base* prev = position._M_node->_M_prev;
        node->_M_next = position._M_node;
        node->_M_prev = prev;
        prev->_M_next = node;
        position._M_node->_M_prev = node;
    }
    void push_back(const T& x) { insert(end(), x); }
    _List_node_base* _M_node;
};
class CGetAllThingsOfType {
public:
    char*                    DefName;
    list<const CThing*>*     ListToChange;
    void operator()(const CThing* thing);
};
void CGetAllThingsOfType::operator()(const CThing* thing)
{
    if (thing->GetDefName() == DefName)
        ListToChange->push_back(thing);
}
int main()
{
    bool ok = true;
    char wanted[] = "OBJECT_CHEST";
    CThing things[5] = { { "OBJECT_CHEST" }, { "CREATURE_BANDIT" }, { "OBJECT_CHEST" }, { "OBJECT_CHEST_LID" },
                         { "OBJECT_CHEST" } };
    list<const CThing*> found;
    CGetAllThingsOfType collect;
    collect.DefName = wanted;
    collect.ListToChange = &found;
    for (int i = 0; i < 5; ++i)
        collect(&things[i]);
    const CThing* expect[3] = { &things[0], &things[2], &things[4] };
    int n = 0;
    for (_List_node_base* p = found._M_node->_M_next; p != found._M_node; p = p->_M_next, ++n) {
        if (n >= 3 || ((_List_node<const CThing*>*)p)->_M_data != expect[n]) ok = false;
        if (p->_M_next->_M_prev != p) ok = false;
    }
    if (n != 3 || found._M_node->_M_prev->_M_next != found._M_node) ok = false;
    if (!ok) { std::printf("00877110_TEST FAIL\n"); return 1; }
    std::printf("00877110_TEST PASS\n");
    return 0;
}