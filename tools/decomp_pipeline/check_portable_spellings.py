"""check_portable_spellings.py -- MSVC-only spellings that must not appear bare in the reconstruction.

Owner, 2026-11-08: the reconstruction must stay portable C++ (it byte-matches retail under VC7.1 AND compiles elsewhere). VC7.1's
`__forceinline` keyword is spelled through FABLE_FORCEINLINE (rebuild/include/fable_compiler.h): `__forceinline` under MSVC, so the
retail bytes are unchanged, plain `inline` elsewhere. A bare `__forceinline` code token is rejected; one inside an
`#ifdef _MSC_VER` / `#if defined(_MSC_VER)` true branch, or the name of a `#ifndef _MSC_VER` shim (`#define __forceinline inline`),
is tolerated. `#pragma` lines need nothing: the standard requires an implementation to ignore a pragma it does not recognise.

usage (repo root): python3 tools/decomp_pipeline/check_portable_spellings.py [paths...]   (default: rebuild/include rebuild/src rebuild/tests)
exit 1 and one `path:line: bare __forceinline (use FABLE_FORCEINLINE, fable_compiler.h)` line per hit; exit 0 when clean.
verify_and_land.py calls bare_forceinline_lines() on every payload source and test (NOT_PORTABLE).
"""
import os, re, sys

TOK = re.compile(r'\b__forceinline\b')
EXTS = ('.h', '.hpp', '.cpp', '.inl', '.c')


def strip_comments_and_strings(text):
    """The text with comments and string / character literal contents blanked, same length and line breaks."""
    out = []; i = 0; n = len(text)
    while i < n:
        c = text[i]
        if text.startswith('//', i):
            j = text.find('\n', i); j = n if j < 0 else j
            out.append(' ' * (j - i)); i = j
        elif text.startswith('/*', i):
            j = text.find('*/', i + 2); j = n if j < 0 else j + 2
            out.append(re.sub(r'[^\n]', ' ', text[i:j])); i = j
        elif c in '"\'':
            j = i + 1
            while j < n and text[j] != c and text[j] != '\n':
                if text[j] == '\\': j += 1
                j += 1
            out.append(c + ' ' * (j - i - 1) + (c if j < n and text[j] == c else '')); i = j + 1 if j < n and text[j] == c else j
        else:
            out.append(c); i += 1
    return ''.join(out)


def bare_forceinline_lines(text):
    """1-based line numbers of bare `__forceinline` code tokens in a C++ source."""
    hits = []; stack = []
    for no, line in enumerate(strip_comments_and_strings(text).split('\n'), 1):
        s = line.strip()
        m = re.match(r'#\s*(if|ifdef|ifndef|elif|else|endif)\b(.*)', s)
        if m:
            k, arg = m.groups()
            if k in ('if', 'ifdef', 'ifndef'): stack.append('_MSC_VER' in arg and k != 'ifndef' and '!' not in arg)
            elif k in ('else', 'elif') and stack: stack[-1] = False
            elif k == 'endif' and stack: stack.pop()
            continue
        if not TOK.search(line) or any(stack): continue
        if re.match(r'#\s*(define|undef)\s+__forceinline\b', s): continue
        hits.append(no)
    return hits


def main(argv):
    roots = argv or ['rebuild/include', 'rebuild/src', 'rebuild/tests']
    bad = 0
    for root in roots:
        files = [root] if os.path.isfile(root) else [os.path.join(d, f) for d, _, fs in os.walk(root) for f in fs]
        for f in sorted(files):
            if not f.endswith(EXTS): continue
            try: text = open(f, encoding='latin-1').read()
            except OSError: continue
            for no in bare_forceinline_lines(text):
                print(f'{f}:{no}: bare __forceinline (use FABLE_FORCEINLINE, fable_compiler.h)'); bad += 1
    print(f'check_portable_spellings: {bad} bare __forceinline', file=sys.stderr)
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
