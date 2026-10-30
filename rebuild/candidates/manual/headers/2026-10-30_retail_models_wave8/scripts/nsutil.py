# wave 8: namespace-aware placement helpers
import re
def mask(t):
    """same-length copy of t with comments, string and char literals blanked (newlines kept)"""
    out = list(t); i = 0; n = len(t)
    while i < n:
        c = t[i]
        if t.startswith("//", i):
            j = t.find("\n", i); j = n if j < 0 else j
            for k in range(i, j): out[k] = " "
            i = j; continue
        if t.startswith("/*", i):
            j = t.find("*/", i + 2); j = n if j < 0 else j + 2
            for k in range(i, j):
                if t[k] != "\n": out[k] = " "
            i = j; continue
        if c in "\"'":
            j = i + 1
            while j < n and t[j] != c:
                j += 2 if t[j] == "\\" else 1
            for k in range(i + 1, min(j, n)): out[k] = " "
            i = j + 1; continue
        if c == "#" and (i == 0 or t[t.rfind("\n", 0, i) + 1:i].strip() == ""):
            j = t.find("\n", i); j = n if j < 0 else j
            for k in range(i, j): out[k] = " "
            i = j; continue
        i += 1
    return "".join(out)
def scope_stack(t, pos):
    """list of (kind, start) open scopes at pos: kind 'ns' (namespace, start = line start of the keyword) or 'other'"""
    m = mask(t[:pos]); st = []
    for i, ch in enumerate(m):
        if ch == "{":
            pre = m[max(0, i - 200):i]
            mm = re.search(r"\bnamespace(\s+\w+)?\s*$", pre)
            if mm:
                ks = i - len(pre) + mm.start()
                st.append(("ns", m.rfind("\n", 0, ks) + 1))
            else: st.append(("other", i))
        elif ch == "}":
            if st: st.pop()
    return st
def top_insert_pos(t, pos):
    """position to insert a top-level include for a block at pos: pos itself when at top level, the line start of the
    outermost enclosing namespace when only namespaces enclose it, None when a class / function encloses it"""
    st = scope_stack(t, pos)
    if not st: return pos
    if any(k != "ns" for k, _ in st): return None
    return st[0][1]
