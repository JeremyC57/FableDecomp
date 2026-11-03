# add_inst.py <src>...: after `template class _Rb_tree<ARGS>;` add the explicit instantiation of the member template
# `_M_find<K>` (class instantiation does not instantiate member templates), and spell the _M_find return type as
# STLport's `_Rb_tree_node<_Value>*` where a row wrote `_Rb_tree_node_base*`.
import re, sys
def split_args(s):
    out, d, cur = [], 0, ""
    for ch in s:
        if ch == "<": d += 1
        elif ch == ">": d -= 1
        if ch == "," and d == 0: out.append(cur.strip()); cur = ""
        else: cur += ch
    out.append(cur.strip()); return out
for path in sys.argv[1:]:
    s = open(path, encoding="utf-8").read(); o = s
    s = re.sub(r"_Rb_tree_node_base\*(\s+)(_M_find\(const _KT&)", r"_Rb_tree_node<_Value>*\1\2", s)
    s = re.sub(r"(template <class _KT>\n)_Rb_tree_node_base\*(\s+_Rb_tree<[^>]*>::_M_find)", r"\1_Rb_tree_node<_Value>*\2", s)
    m = re.search(r"^template\s+class\s+_Rb_tree\s*<", s, re.M)
    if not m or "::_M_find<" in s: print("SKIP", path); continue
    i = m.end(); d = 1
    while d: 
        d += {"<": 1, ">": -1}.get(s[i], 0); i += 1
    args = s[m.end():i - 1]; j = s.index(";", i) + 1
    a = split_args(args); K, V = a[0], a[1]
    inst = f"\ntemplate _Rb_tree_node<{V} >* _Rb_tree<{args}>::_M_find<{K} >(const {K}&) const;"
    s = s[:j] + inst + s[j:]
    open(path, "w", encoding="utf-8").write(s); print("added", path)
