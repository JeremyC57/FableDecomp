// Behaviour test for 0x00756540 GFSerialiseMapBinaryOut<pair<long,long>, CCharString, less<CCharString> >
#include <cstdio>
#include <cstring>

static int g_ctor = 0, g_dtor = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString() : PStringData("") { ++g_ctor; }
    CCharString(const char* s) : PStringData(s) {}
    ~CCharString() { ++g_dtor; }
};

struct CDataOutputStreamWriter {
    long  Longs[32];
    const char* Strings[32];
    char  Order[64];
    int   NumLongs, NumStrings, NumOps;
    CDataOutputStreamWriter() : NumLongs(0), NumStrings(0), NumOps(0) {}
    void WriteSLONG(long value) { Longs[NumLongs++] = value; Order[NumOps++] = 'L'; }
    void WriteNullTerminatedString(const CCharString& text) { Strings[NumStrings++] = text.PStringData; Order[NumOps++] = 'S'; }
};

template <class T1, class T2> struct pair { T1 first; T2 second; pair(const T1& a, const T2& b) : first(a), second(b) {} };
template <class T> struct less { };
struct _Rb_tree_node_base { bool _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right; };
template <class V> struct _Rb_tree_node : _Rb_tree_node_base { V _M_value_field; _Rb_tree_node(const V& v) : _M_value_field(v) {} };
template <class B> struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* x)
    {
        if (x->_M_right != 0) {
            x = x->_M_right;
            while (x->_M_left != 0)
                x = x->_M_left;
        }
        else {
            _Rb_tree_node_base* y = x->_M_parent;
            while (x == y->_M_right) {
                x = y;
                y = y->_M_parent;
            }
            if (x->_M_right != y)
                x = y;
        }
        return x;
    }
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* n) : _M_node(n) { }
    const V* operator->() const { return &((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& o) const { return _M_node != o._M_node; }
};
template <class K, class V, class C> class map {
public:
    typedef pair<const K, V> value_type;
    typedef _Rb_tree_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
    unsigned int size() const { return _M_node_count; }
};
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CCharString> {
    CCharString DefaultVal;
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const CCharString& value) const { stream.WriteNullTerminatedString(value); }
};
template <> struct CPersistTraits<pair<long, long> > {
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const pair<long, long>& value) const
    {
        stream.WriteSLONG(value.first);
        stream.WriteSLONG(value.second);
    }
};
template <class V, class K, class C>
void GFSerialiseMapBinaryOut(CDataOutputStreamWriter& stream, const map<K, V, C>& value)
{
    CPersistTraits<K> keyTraits;
    CPersistTraits<V> valueTraits;
    stream.WriteSLONG(value.size());
    for (typename map<K, V, C>::const_iterator it = value.begin(); it != value.end(); ++it) {
        keyTraits.TransferBinaryOut(stream, it->first);
        valueTraits.TransferBinaryOut(stream, it->second);
    }
}

typedef pair<const CCharString, pair<long, long> > TValue;
typedef _Rb_tree_node<TValue> TNode;

int main()
{
    // tree: "Bower" root, "Albion" left, "Oakvale" right (in-order A, B, O)
    TNode a(TValue(CCharString("Albion"), pair<long, long>(10, 100)));
    TNode b(TValue(CCharString("Bower"), pair<long, long>(20, 200)));
    TNode o(TValue(CCharString("Oakvale"), pair<long, long>(30, 300)));
    _Rb_tree_node_base header;
    header._M_parent = &b; header._M_left = &a; header._M_right = &o;
    b._M_parent = &header; b._M_left = &a; b._M_right = &o;
    a._M_parent = &b; a._M_left = a._M_right = 0;
    o._M_parent = &b; o._M_left = o._M_right = 0;
    map<CCharString, pair<long, long>, less<CCharString> > m;
    m._M_header = &header; m._M_node_count = 3;
    CDataOutputStreamWriter w;
    int c0 = g_ctor, d0 = g_dtor;
    GFSerialiseMapBinaryOut(w, m);
    int ok = 1;
    if (g_ctor - c0 != 1 || g_dtor - d0 != 1) ok = 0;
    if (w.NumOps != 10 || std::strncmp(w.Order, "LSLLSLLSLL", 10) != 0) ok = 0;
    long want[7] = { 3, 10, 100, 20, 200, 30, 300 };
    for (int i = 0; i < 7; ++i) if (w.Longs[i] != want[i]) ok = 0;
    if (std::strcmp(w.Strings[0], "Albion") || std::strcmp(w.Strings[1], "Bower") || std::strcmp(w.Strings[2], "Oakvale")) ok = 0;
    // empty map: only the count
    _Rb_tree_node_base h2; h2._M_parent = 0; h2._M_left = &h2; h2._M_right = &h2;
    m._M_header = &h2; m._M_node_count = 0;
    CDataOutputStreamWriter w2;
    GFSerialiseMapBinaryOut(w2, m);
    if (w2.NumOps != 1 || w2.Longs[0] != 0) ok = 0;
    if (!ok) { std::printf("00756540_TEST FAIL\n"); return 1; }
    std::printf("00756540_TEST PASS\n");
    return 0;
}