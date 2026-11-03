p='hdr/stlport/fable_containers.h'; s=open(p).read()
old=open('work/fable_containers.h.old').read()
blk=old[old.index('#ifdef FABLE_RETAIL_STLP_VECTOR\n    // Retail branch (chunk 728, row 0x006D2BD0'):]
blk=blk[:blk.index('#endif\n')+len('#endif\n')]
# insert at same anchor line as old: find the line preceding blk in old
pre=old[:old.index(blk)].splitlines(True)[-1]
assert s.count(pre)>=1, pre
i=s.index(pre)+len(pre)
# ensure we insert at the right occurrence: use the occurrence whose context matches
cands=[k for k in range(len(s)) if s.startswith(pre,k)]
ctx=old[:old.index(blk)][-400:]
best=None
for k in cands:
    if s[:k+len(pre)].endswith(ctx[-200:]): best=k
assert best is not None
i=best+len(pre)
s=s[:i]+blk+s[i:]
open(p,'w').write(s)
p='hdr/stlport/fable_map.h'; s=open(p).read()
old="    static void __cdecl _Rebalance(_Rb_tree_node_base*,_Rb_tree_node_base*&);\n"
assert s.count(old)==1
s=s.replace(old,old+"    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base*,_Rb_tree_node_base*&,_Rb_tree_node_base*&,_Rb_tree_node_base*&);   // 0x0042A34E (chunk 728 round 2)\n")
old="    iterator end(){return iterator(this->_M_header._M_data);}\nprivate:\n    void _M_erase(_Rb_tree_node<V>*);\n"
assert s.count(old)==1
s=s.replace(old,"""    iterator end(){return iterator(this->_M_header._M_data);}
#ifdef FABLE_RETAIL_STLP_MAP
    // Retail branch (chunk 728 round 2, row 0x006D1E30 CTCWife::FrameUpdate): STLport 4.6 _tree.h erase(iterator), inline: retail
    // calls _Rb_global<bool>::_Rebalance_for_erase 0x0042A34E with the header's parent / left / right, frees the node and
    // decrements _M_node_count in place (0x006D2B80 .. 0x006D2BA5).
    void erase(iterator position) {
        _Rb_tree_node<V>* y=static_cast<_Rb_tree_node<V>*>(_Rb_global_inst::_Rebalance_for_erase(position._M_node,
            this->_M_header._M_data->_M_parent,this->_M_header._M_data->_M_left,this->_M_header._M_data->_M_right));
        this->_M_header.deallocate(y,1);
        --_M_node_count;
    }
#endif
private:
    void _M_erase(_Rb_tree_node<V>*);
""")
old="    iterator end(){return _M_t.end();}\n#ifdef FABLE_RETAIL_STLP_MAP\n"
assert s.count(old)==1, s.count(old)
s=s.replace(old,"    iterator end(){return _M_t.end();}\n#ifdef FABLE_RETAIL_STLP_MAP\n    void erase(iterator position){_M_t.erase(position);}          // chunk 728 round 2: STLport 4.6 _map.h erase(iterator)\n",1)
if 'FABLE_RETAIL_STLP_SET\n    // Retail branch (chunk 728, row 0x006D2BD0' not in s:
    oldm=open('work/fable_map.h.old').read()
    blk=oldm[oldm.index('#ifdef FABLE_RETAIL_STLP_SET\n    // Retail branch (chunk 728, row 0x006D2BD0'):]
    blk=blk[:blk.index('#endif\n')+len('#endif\n')]
    a="    size_t erase(const K& value){return _M_t.erase(value);}\n"
    assert s.count(a)==1
    s=s.replace(a,a+blk)
open(p,'w').write(s)
print('ok')
