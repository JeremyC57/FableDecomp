// GFSerialiseMapBinaryOut(CDataOutputStream& out, const std::map<CCharString, long>& m) -- write the count, then every (key, value)
// Leaf: ??$GFSerialiseMapBinaryOut@JVCCharString@@U?$less@VCCharString@@@std@@@@YIXAAVCDataOutputStream@@ABV?$map@VCCharString@@JU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@J@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004314C8, 85 bytes, `__fastcall` (stream in ecx, container in edx), frame-pointer size module.
// Identity (proven): the binary-save arm of CPersistContext::TransferMap<long, CCharString> 0x00475026, which CPlayerGuiDef::Transfer 0x004736C4 applies to ScriptSprites (+0x934, "ScriptSprites") and MiniMapGraphics (+0x94C) -- Ego_r.pdb map<CCharString,long>; also called by the bound CPersistContext::Transfer(map<CCharString,long>&) 0x00431493; Ego_r.pdb ??$GFSerialiseMapBinaryOut@JVCCharString@@U?$less@VCCharString@@@std@@@@YIXAAVCDataOutputStream@@ABV?$map@VCCharString@@J....
// Callees: CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0 (the key traits' DefaultVal), CDataOutputStream::WriteSLONG
//   0x00993EB0, CDataOutputStream::WriteNullTerminatedString 0x00995AE0, std::_Rb_global<bool>::_M_increment 0x004292C0 (cdecl).
// Shape (family M024, representative 0x004314C8): `CPersistTraits<K> key_traits; CPersistTraits<V> value_traits;
//   out.WriteSLONG(m.size()); for (it = m.begin(); it != m.end(); ++it) { key_traits.TransferBinaryOut(out, it->first);
//   value_traits.TransferBinaryOut(out, it->second); }` -- `optimize("y", off)` + `optimize("s", on)`; the value traits'
//   DefaultVal store is dead; STLport tree header at +0, node count +4, node key +0x10, value +0x14.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString {
public:
    CCharString();                                        // 0x0099E4B0
    ~CCharString();                                       // 0x0099EAE0
    void* PStringData;
};
class CDataOutputStream {
public:
    void WriteSLONG(long value);                          // 0x00993EB0
    void WriteNullTerminatedString(const CCharString& s); // 0x00995AE0
};
template <class T> class CPersistTraits;
template <> class CPersistTraits<CCharString> {
public:
    void TransferBinaryOut(CDataOutputStream& out, const CCharString& value) const { out.WriteNullTerminatedString(value); }
    CCharString DefaultVal;
};
template <> class CPersistTraits<long> {
public:
    CPersistTraits() : DefaultVal(0) { }
    void TransferBinaryOut(CDataOutputStream& out, const long& value) const { out.WriteSLONG(value); }
    long DefaultVal;
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* __x);   // 0x004292C0
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    const _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    const _Value* operator->() const { return &operator*(); }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
    _Rb_tree_node_base* _M_node;
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    typedef _Rb_tree_iterator<pair<const _Key, _Tp> > const_iterator;
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
    unsigned int size() const { return _M_node_count; }
    _Rb_tree_node_base* _M_header;                        // +0x00
    unsigned int _M_node_count;                           // +0x04
    _Compare _M_key_compare;                              // +0x08
};
}
template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryOut(CDataOutputStream& out, const std::map<K, V, C>& m)
{
    CPersistTraits<K> key_traits;
    CPersistTraits<V> value_traits;
    out.WriteSLONG(m.size());
    for (typename std::map<K, V, C>::const_iterator it = m.begin(); it != m.end(); ++it) {
        key_traits.TransferBinaryOut(out, it->first);
        value_traits.TransferBinaryOut(out, it->second);
    }
}
template void __fastcall GFSerialiseMapBinaryOut<long, CCharString, std::less<CCharString> >(CDataOutputStream& out, const std::map<CCharString, long, std::less<CCharString> >& m);