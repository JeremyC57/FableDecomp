// CSetAndArray_0051a040::CSetAndArray_0051a040() -- default constructor of an unidentified 0x1C-byte record: an empty STLport set of 4-byte keys (header node malloc'd, red, self-linked) followed by a zeroed array and count
// Leaf: ??0CSetAndArray_0051a040
//
// Retail 0x0051A040, 57 bytes (+ int3 padding to 64), plain `ret` (this in ecx, returned in eax).  Callee: 0x00BFEA0E malloc (the 0x14-byte
//   header node = _Rb_tree_node<4-byte value>).
// Identity: no call site or address reference to 0x0051A040 exists in Fable.exe (xref scan), and no Ego_r.pdb type has
//   a 4-byte-key set at +0 followed by 16 bytes, so the owner stays unnamed; the members after the set are modelled as
//   a three-pointer array plus a count only because they are zeroed in that order (+0xC, +0x10, +0x14, +0x18).
// Shape: STLport `_Rb_tree_base(a) : _M_header(0) { _M_header = allocate(1); }`, then `_Rb_tree() : _M_node_count(0),
//   _M_key_compare(comp) { _M_empty_initialize(); }` (color red = 0, root 0, leftmost = rightmost = header; the header
//   pointer is re-read after every store because the stores may alias it), then the array's inline constructor.
// Label: catalog "StdTree_Construct_0051a040".
extern "C" void* __cdecl malloc(unsigned int size);

struct _Rb_tree_node_base {
    char                _M_color;              // +0x0 (0 = red)
    _Rb_tree_node_base* _M_parent;             // +0x4
    _Rb_tree_node_base* _M_left;               // +0x8
    _Rb_tree_node_base* _M_right;              // +0xC
};
struct _Rb_tree_node_ulong : _Rb_tree_node_base {
    unsigned long _M_value_field;              // +0x10
};

class CULongSet {                              // STLport set<unsigned long>: _Rb_tree_base + _Rb_tree
public:
    _Rb_tree_node_ulong* _M_header;            // +0x0
    unsigned long        _M_node_count;        // +0x4
    struct less { } _M_key_compare;            // +0x8 (empty comparator, padded)
    CULongSet() : _M_header(0)
    {
        _M_header = (_Rb_tree_node_ulong*)malloc(sizeof(_Rb_tree_node_ulong));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};

template <class T> class CArray {              // retail CArray: STLport vector, three pointers
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};

class CSetAndArray_0051a040 {
public:
    CULongSet              Keys;               // +0x00
    CArray<unsigned long>  Items;              // +0x0C
    unsigned long          Count;              // +0x18
    CSetAndArray_0051a040();
};

CSetAndArray_0051a040::CSetAndArray_0051a040() : Count(0)
{
}