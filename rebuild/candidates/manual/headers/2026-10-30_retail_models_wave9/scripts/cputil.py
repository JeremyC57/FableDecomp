# wave 9: helpers for the CCountedPointer migration (comment-aware depth, member parsing)
import re
def strip_comments(t):
    """same length; comments and string/char literal contents blanked (newlines kept)"""
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
                if t[j] == "\\": j += 1
                j += 1
            for k in range(i + 1, min(j, n)): out[k] = " "
            i = j + 1; continue
        i += 1
    return "".join(out)
def depth_at(clean, pos):
    """brace depth at pos (clean text); also returns the stack of openers' preceding text heads"""
    d = 0; stack = []
    for i in range(pos):
        c = clean[i]
        if c == "{":
            s = clean.rfind(";", 0, i); s2 = clean.rfind("}", 0, i); s3 = clean.rfind("{", 0, i)
            head = clean[max(s, s2, s3) + 1:i].strip()
            stack.append(head); d += 1
        elif c == "}":
            d -= 1
            if stack: stack.pop()
    return d, stack
def pp_depth(clean, pos):
    d = 0
    for l in clean[:pos].splitlines():
        s = l.strip()
        if re.match(r"#\s*if", s): d += 1
        elif re.match(r"#\s*endif", s): d -= 1
    return d
def items(body):
    """split a class body (clean text, without the outer braces) into top-level member items"""
    res = []; i = 0; n = len(body); start = 0; d = 0; par = 0
    while i < n:
        c = body[i]
        if c == "(": par += 1
        elif c == ")": par -= 1
        elif c == "{":
            if d == 0 and par == 0:
                # function body (or nested aggregate): find matching brace
                j = i + 1; dd = 1
                while j < n and dd:
                    if body[j] == "{": dd += 1
                    elif body[j] == "}": dd -= 1
                    j += 1
                head = body[start:i]
                # a nested class/struct/enum ends with `;`
                if re.search(r"\b(class|struct|union|enum)\b[^()]*$", head):
                    k = body.find(";", j); k = n if k < 0 else k + 1
                    res.append(("nested", body[start:k])); start = k; i = k; continue
                res.append(("func", head)); start = j; i = j; continue
        elif c == ";" and par == 0:
            res.append(("decl", body[start:i])); start = i + 1
        i += 1
    if body[start:].strip(): res.append(("tail", body[start:]))
    out = []
    for kind, h in res:
        h = re.sub(r"\b(public|private|protected)\s*:", " ", h).strip()
        if not h: continue
        out.append((kind, h))
    return out
def member_name(head):
    h = re.sub(r"template\s*<[^>]*>", " ", head)
    h = re.sub(r"__declspec\s*\([^)]*\)", " ", h)
    m = re.search(r"(operator\s*(?:->|\*|=|==|!=|\(\)|\[\]|!|bool|[A-Za-z_]\w*\s*\*?)|~?\s*[A-Za-z_]\w*)\s*\(", h)
    if "(" in h and m: return re.sub(r"\s+", "", m.group(1)), True
    m = re.findall(r"[A-Za-z_]\w*", h)
    return (m[-1] if m else ""), False
