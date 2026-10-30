# shared helpers for the wave 1 migration scripts
import re
def find_block(text, cls, start=0):
    """(start, end) of the first column-0-ish `class|struct cls ... { ... };` definition (end after `;` and newline)."""
    rx = re.compile(r'^[ \t]*(?:template\s*<[^>{;]*>\s*)?(?:class|struct)\s+' + re.escape(cls) + r'\b[^;{]*\{', re.M)
    m = rx.search(text, start)
    if not m: return None
    i = m.end(); d = 1
    while d and i < len(text):
        c = text[i]
        if c == '{': d += 1
        elif c == '}': d -= 1
        i += 1
    j = text.find(';', i)
    if j < 0: return None
    k = j + 1
    # swallow a trailing comment on the same line and the newline
    nl = text.find('\n', k)
    if nl == -1: nl = len(text)
    tail = text[k:nl]
    if tail.strip() == '' or tail.strip().startswith('//'): k = nl + 1 if nl < len(text) else nl
    return m.start(), k
def norm(block):
    n = re.sub(r'//[^\n]*', '', block); return re.sub(r'\s+', ' ', n).strip()
