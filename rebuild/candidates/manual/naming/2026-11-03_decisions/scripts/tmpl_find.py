# tmpl_find.py <src>...: make STLport _Rb_tree::_M_find / _Rb_tree::find member templates (STLport 4.6.1 _tree.h 457-467:
# `template <class _KT> iterator find(const _KT&)`, `template <class _KT> _Rb_tree_node<_Value>* _M_find(const _KT&) const`).
# Rewrites in-class declarations/definitions and out-of-line definitions; leaves map/set wrappers (`_M_t.find`) alone.
import re, sys
TP = r"(?:const\s+)?(?!_KT\b)(\w+(?:::\w+)?)\s*&"
for path in sys.argv[1:]:
    s = open(path, encoding="utf-8").read(); o = s; n = 0
    # 1. out-of-line definitions:  template <...>\n RET _Rb_tree<...>::_M_find(const K& k) const
    def ool(m):
        global n
        return f"{m.group(1)}template <class _KT>\n{m.group(2)}::{m.group(3)}(const _KT&{m.group(5)}){m.group(6)}"
    rx_ool = re.compile(r"(template\s*<[^<>]*>\s*\n)([^\n;{}]*?_Rb_tree<[^;{}()]*?>)::(_M_find|find)\(\s*" + TP + r"(\s*\w*)\s*\)(\s*const)?")
    s = rx_ool.sub(ool, s)
    # 2. in-class declarations / inline definitions (skip lines already templated, out-of-line `::`, map/set `_M_t.find`)
    out = []; cls = None
    for line in s.split("\n"):
        st = line.lstrip()
        mc = re.match(r"^\s*(?:template\s*<[^;]*>\s*)?(?:class|struct)\s+(\w+)\b[^;]*$", line)
        if mc: cls = mc.group(1)
        code = line.split("//")[0].rstrip()
        m = re.match(r"^(\s*)((?:__forceinline\s+|__declspec\(noinline\)\s+|inline\s+)?)([\w:<>,\s\*&]*?)\b(_M_find|find)\s*\(\s*" + TP + r"(\s*\w*)\s*\)", line)
        if (m and not st.startswith(("template", "//", "return")) and "::_M_find" not in line and "::find" not in line
                and "_M_t.find" not in line and cls == "_Rb_tree" and (m.group(4) == "_M_find" or "_M_find(" in code or code.endswith(";"))
                and m.group(3).strip() and not re.search(r"\.\s*(_M_find|find)\s*\(", line.split("(")[0])):
            line = f"{m.group(1)}template <class _KT> {m.group(2)}{m.group(3)}{m.group(4)}(const _KT&{m.group(6)})" + line[m.end():]
        out.append(line)
    s = "\n".join(out)
    if s != o: open(path, "w", encoding="utf-8").write(s); print("changed", path)
    else: print("UNCHANGED", path)
