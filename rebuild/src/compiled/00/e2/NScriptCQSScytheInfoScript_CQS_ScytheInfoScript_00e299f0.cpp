// NScript::CQS_ScytheInfoScript::CQS_ScytheInfoScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- base record set, the two arguments stored, then two empty STLport maps (header node allocated and self-linked)
// Leaf: ??0CQS_ScytheInfoScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E299F0, length 113 bytes (padded to 128).
// Label correction: the catalog "CParticleMorphs_HeroMorphDef::CHeroMorphDef" is propagated and wrong. The base ctor 0x00CB8110
// stores vtable 0x012C1648 (its destructor is 0x00CBD510 in this chunk), this ctor stores 0x012E22C8, and the only caller is the
// `__fastcall` factory 0x00E2AB00 (`new` of 0x64 bytes, its ecx/edx arguments forwarded as (a, b)). No RTTI row for either
// vtable, so both classes are named by address.
// Callees: the base ctor @ 0x00CB8110, malloc @ 0x00BFEA0E (each map's 0x18 / 0x24-byte header node).
// Shape: the two argument words stored after the base call and BEFORE the derived vtable store belong to an inline
// `__declspec(novtable)` intermediate base's initialiser list (as the derived class's own initialisers they land after the
// vtable store); each map is STLport's _Rb_tree: the base part nulls and allocates the header, the derived part zeroes the
// count, then _M_empty_initialize colours it red, nulls the root and self-links leftmost/rightmost (re-reading the header).
// Naming wave 35: identity level-script ctor: called by the Alloc 00e2ab00 on the new block, stores script vtable 012e22c8; PDB ??0CQS_ScytheInfoScript@NScript@@QAE@PAVCScriptDataBase@@...; outer CQS_ScytheInfoScript proven by its Main 00e29b20 entity registrations; was CScriptRecordSet_00e299f0::CScriptRecordSet_00e299f0 (wave3/labels_alloc.tsv); PDB ??0CQS_ScytheInfoScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
//   Was `??0CScriptRecordSet_00e299f0@@QAE@JJ@Z`; the class is renamed into NScript and takes the PDB parameter types, handed to the row's
//   argument-carrying base model unchanged (data = the first argument, stored at +0x44; the interface at +0x40); same bytes.
class CScriptDataBase;
class CGameScriptInterfaceBase;
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
class CScriptBase { public:              // vtable 0x012C1648, 0x40 bytes (naming wave 35: the stand-in CScriptBase is CScriptBase, ctor 0x00CB8110)
    virtual ~CScriptBase();
    unsigned char _body[0x3c];
    CScriptBase();                  // 0x00CB8110
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
struct __declspec(novtable) CScriptRecordSetBase_00e299f0 : CScriptBase {   // inline intermediate base
    long             m_40;                  // +0x40
    long             m_44;                  // +0x44
    CScriptRecordSetBase_00e299f0(long a, long b) : m_40(b), m_44(a) {}
};
namespace NScript {
class CQS_ScytheInfoScript : public CScriptRecordSetBase_00e299f0 {
public:   // vtable 0x012E22C8, 0x64 bytes
    long             m_48;                  // +0x48
    CRetailMap<0x18> m_map4c;               // +0x4c (8-byte values)
    CRetailMap<0x24> m_map58;               // +0x58 (20-byte values)
    CQS_ScytheInfoScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQS_ScytheInfoScript();    // ??1CQS_ScytheInfoScript@NScript@@UAE@XZ
};
}
using NScript::CQS_ScytheInfoScript;
NScript::CQS_ScytheInfoScript::CQS_ScytheInfoScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptRecordSetBase_00e299f0((long)data, (long)&game)
{
}