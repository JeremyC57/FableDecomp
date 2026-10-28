#include <cstdio>
#include <cstdlib>
#include <cstring>

// C3DMeshStats::PackNames (0x00A2EEF0) through a copy of the model: a 16-bit offset of the dummy names, then the
// non-empty helper names and an empty terminator, then the dummy names and their terminator, copied into a char array.
class CCharString { public:
    const char* PData;                                  // null = empty
    int GetLength() const { return PData == 0 ? 0 : (int)strlen(PData); }
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct SNameNode : _Rb_tree_node_base { CCharString _M_value_field; };
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
struct CNameSet {                                       // set<CCharString>: header + count
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    bool empty() const { return _M_node_count == 0; }
};

class CMemoryDataOutputStream { public:
    char Buffer[256];
    unsigned int Length;
    unsigned int Position;
    CMemoryDataOutputStream() : Length(0), Position(0) { }
    void Write(const void* data, unsigned int size)
    {
        memcpy(Buffer + Position, data, size);
        Position += size;
        if (Position > Length) Length = Position;
    }
    void WriteUWORD(unsigned short value) { unsigned char b[2] = { (unsigned char)value, (unsigned char)(value >> 8) }; Write(b, 2); }
    void WriteNullTerminatedString(const CCharString& s) { if (s.PData) Write(s.PData, (unsigned int)strlen(s.PData)); Write("", 1); }
    void SetPosition(unsigned int p) { Position = p; }
    unsigned int GetPosition() const { return Position; }
    unsigned int GetLength() const { return Length; }
    const void* PeekData() const { return Buffer; }
};

struct CCharArray {                                     // CArray<char> (STLport vector<char>)
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void resize(unsigned int n)
    {
        if (n < size())
            _M_finish = _M_start + n;                  // erase(begin() + n, end())
        else {
            unsigned int add = n - size();             // _M_fill_insert(end(), add, char())
            if ((unsigned int)(_M_end_of_storage - _M_finish) < add) {
                unsigned int len = size() + (size() > add ? size() : add);
                char* s = (char*)malloc(len);
                memcpy(s, _M_start, size());
                _M_finish = s + size();
                free(_M_start);
                _M_start = s;
                _M_end_of_storage = s + len;
            }
            memset(_M_finish, 0, add);
            _M_finish += add;
        }
    }
    char* GetData() { return size() > 0 ? _M_start : (char*)this; }
};

static void PackNames(CCharArray& packed_names, const CNameSet& helper_point_names, const CNameSet& dummy_object_names)
{
    if (helper_point_names.empty() && dummy_object_names.empty())
        return;
    CMemoryDataOutputStream stream;
    stream.WriteUWORD(0);
    for (_Rb_tree_node_base* it = helper_point_names._M_header->_M_left; it != helper_point_names._M_header; it = _M_increment(it))
        if (((SNameNode*)it)->_M_value_field.GetLength() != 0)
            stream.WriteNullTerminatedString(((SNameNode*)it)->_M_value_field);
    CCharString empty = { "" };
    stream.WriteNullTerminatedString(empty);
    unsigned int dummy_offset = stream.GetPosition();
    for (_Rb_tree_node_base* it2 = dummy_object_names._M_header->_M_left; it2 != dummy_object_names._M_header; it2 = _M_increment(it2))
        if (((SNameNode*)it2)->_M_value_field.GetLength() != 0)
            stream.WriteNullTerminatedString(((SNameNode*)it2)->_M_value_field);
    stream.WriteNullTerminatedString(empty);
    stream.SetPosition(0);
    stream.WriteUWORD((unsigned short)dummy_offset);
    unsigned int length = stream.GetLength();
    packed_names.resize(length);
    memcpy(packed_names.GetData(), stream.PeekData(), length);
}

static int Fail()
{
    std::printf("00a2eef0_TEST FAIL\n");
    return 1;
}

int main()
{
    // helper set { "", "hand", "head" } as root "hand" with left "" and right "head"
    _Rb_tree_node_base header1 = { false, 0, 0, 0 };
    SNameNode n_empty, n_hand, n_head;
    n_empty._M_value_field.PData = 0;
    n_hand._M_value_field.PData = "hand";
    n_head._M_value_field.PData = "head";
    n_hand._M_parent = &header1; n_hand._M_left = &n_empty; n_hand._M_right = &n_head;
    n_empty._M_parent = &n_hand; n_empty._M_left = 0; n_empty._M_right = 0;
    n_head._M_parent = &n_hand; n_head._M_left = 0; n_head._M_right = 0;
    header1._M_parent = &n_hand; header1._M_left = &n_empty; header1._M_right = &n_head;
    CNameSet helpers = { &header1, 3 };
    // dummy set { "gen" }
    _Rb_tree_node_base header2 = { false, 0, 0, 0 };
    SNameNode n_gen;
    n_gen._M_value_field.PData = "gen";
    n_gen._M_parent = &header2; n_gen._M_left = 0; n_gen._M_right = 0;
    header2._M_parent = &n_gen; header2._M_left = &n_gen; header2._M_right = &n_gen;
    CNameSet dummies = { &header2, 1 };
    _Rb_tree_node_base header3 = { false, 0, 0, 0 };
    header3._M_left = &header3; header3._M_right = &header3;
    CNameSet none = { &header3, 0 };

    CCharArray out = { 0, 0, 0 };
    PackNames(out, none, none);                                  // both empty: untouched
    if (out._M_start != 0) return Fail();
    PackNames(out, helpers, dummies);
    const char expect[] = { 13, 0, 'h', 'a', 'n', 'd', 0, 'h', 'e', 'a', 'd', 0, 0, 'g', 'e', 'n', 0, 0 };
    if (out.size() != sizeof(expect) || memcmp(out._M_start, expect, sizeof(expect)) != 0) return Fail();
    PackNames(out, none, dummies);                               // shrink: erase path
    const char expect2[] = { 3, 0, 0, 'g', 'e', 'n', 0, 0 };
    if (out.size() != sizeof(expect2) || memcmp(out._M_start, expect2, sizeof(expect2)) != 0) return Fail();
    free(out._M_start);
    std::printf("00a2eef0_TEST PASS\n");
    return 0;
}