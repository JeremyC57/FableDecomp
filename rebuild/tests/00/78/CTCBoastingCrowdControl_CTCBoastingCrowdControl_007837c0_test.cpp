#include <cstdio>
#include <cstdlib>

// Model: CTCBoastingCrowdControl's constructor -- the STLport map constructor (header node allocated and
// self-linked), an inline clear() and the two flags.
class CThing { public: int Id; };

struct CBaseIntelligentPointer {
    void* m_pItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: };

struct _Rb_tree_node_base {
    unsigned char       _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class K, class V> struct pair {
    K first;
    V second;
};

static int g_eraseCalls = 0;

struct CHeroCrowdMap {
    typedef pair<unsigned int, CIntelligentPointer<CThing> > value_type;
    struct _Node : _Rb_tree_node_base {
        value_type _M_value_field;
    };
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    unsigned int        _M_key_compare;

    CHeroCrowdMap() : _M_header(0)
    {
        _M_header = (_Node*)std::malloc(sizeof(_Node));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
    void _M_erase(_Rb_tree_node_base*) { ++g_eraseCalls; }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
};

class CTCBase {
public:
    virtual ~CTCBase() { }
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase(CThing& thing) : ParentThing(&thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
};

class CTCBoastingCrowdControl : public CTCBase {
public:
    bool          Active;
    CHeroCrowdMap HeroCrowdThings;
    bool          CrowdSizesSuppressed;

    CTCBoastingCrowdControl(CThing& thing) : CTCBase(thing)
    {
        HeroCrowdThings.clear();
        Active = false;
        CrowdSizesSuppressed = false;
    }
};

int main()
{
    CThing thing;
    thing.Id = 7;
    CTCBoastingCrowdControl* c = new CTCBoastingCrowdControl(thing);
    _Rb_tree_node_base* h = c->HeroCrowdThings._M_header;
    if (c->ParentThing != &thing || c->Active || c->CrowdSizesSuppressed || h == 0 || h->_M_color != 0
        || h->_M_parent != 0 || h->_M_left != h || h->_M_right != h || c->HeroCrowdThings._M_node_count != 0
        || g_eraseCalls != 0) {
        std::printf("007837c0_TEST FAIL\n");
        return 1;
    }
    std::free(h);
    delete c;
    std::printf("007837c0_TEST PASS\n");
    return 0;
}