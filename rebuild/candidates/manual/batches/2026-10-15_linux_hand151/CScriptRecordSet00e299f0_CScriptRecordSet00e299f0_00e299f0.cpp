// CScriptRecordSet_00e299f0::CScriptRecordSet_00e299f0(long, long) -- base record set, the two arguments stored, then two empty STLport maps (header node allocated and self-linked)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E299F0, length 113 bytes (padded to 128).
// Leaf: ??0CScriptRecordSet_00e299f0
// Label correction: the catalog "CParticleMorphs_HeroMorphDef::CHeroMorphDef" is propagated and wrong. The base ctor 0x00CB8110
// stores vtable 0x012C1648 (its destructor is 0x00CBD510 in this chunk), this ctor stores 0x012E22C8, and the only caller is the
// `__fastcall` factory 0x00E2AB00 (`new` of 0x64 bytes, its ecx/edx arguments forwarded as (a, b)). No RTTI row for either
// vtable, so both classes are named by address.
// Callees: the base ctor @ 0x00CB8110, malloc @ 0x00BFEA0E (each map's 0x18 / 0x24-byte header node).
// Shape: the two argument words stored after the base call and BEFORE the derived vtable store belong to an inline
// `__declspec(novtable)` intermediate base's initialiser list (as the derived class's own initialisers they land after the
// vtable store); each map is STLport's _Rb_tree: the base part nulls and allocates the header, the derived part zeroes the
// count, then _M_empty_initialize colours it red, nulls the root and self-links leftmost/rightmost (re-reading the header).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CRecordSet_00cbd510 {                // vtable 0x012C1648, 0x40 bytes
    virtual ~CRecordSet_00cbd510();
    unsigned char _body[0x3c];
    CRecordSet_00cbd510();                  // 0x00CB8110
};
struct _Rb_tree_node_base {
    char                _M_color;           // +0x00
    _Rb_tree_node_base* _M_parent;          // +0x04
    _Rb_tree_node_base* _M_left;            // +0x08
    _Rb_tree_node_base* _M_right;           // +0x0c
};
template <unsigned int NodeSize> struct _Rb_tree_base {
    _Rb_tree_node_base* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)malloc(NodeSize); }
};
template <unsigned int NodeSize> struct CRetailMap : _Rb_tree_base<NodeSize> {   // STLport map/_Rb_tree (12 bytes)
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CRetailMap() : _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        this->_M_header->_M_color = 0;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
};
struct __declspec(novtable) CScriptRecordSetBase_00e299f0 : CRecordSet_00cbd510 {   // inline intermediate base
    long             m_40;                  // +0x40
    long             m_44;                  // +0x44
    CScriptRecordSetBase_00e299f0(long a, long b) : m_40(b), m_44(a) {}
};
struct CScriptRecordSet_00e299f0 : CScriptRecordSetBase_00e299f0 {   // vtable 0x012E22C8, 0x64 bytes
    long             m_48;                  // +0x48
    CRetailMap<0x18> m_map4c;               // +0x4c (8-byte values)
    CRetailMap<0x24> m_map58;               // +0x58 (20-byte values)
    CScriptRecordSet_00e299f0(long a, long b);
};
CScriptRecordSet_00e299f0::CScriptRecordSet_00e299f0(long a, long b)
    : CScriptRecordSetBase_00e299f0(a, b)
{
}
