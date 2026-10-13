#!/usr/bin/env python3
"""Overlay <cstdio>, <cstring>, ... wrappers that import the C functions into namespace std.

The DDK 3790.1830 CRT headers ship the 1994-style C++ wrappers (`#include <stdio.h>` only), so
`std::printf` fails to resolve; retail VC7.1's wrappers add `namespace std { using ::printf; ... }`.
Only the wrappers are replaced; the C headers themselves are the DDK's. Usage: write_std_shims.py <include-dir>"""
import os, sys
inc = sys.argv[1]
SHIMS = {
 'cstdio': ('stdio.h', 'size_t FILE fpos_t clearerr fclose feof ferror fflush fgetc fgetpos fgets fopen fprintf fputc fputs fread freopen fscanf fseek fsetpos ftell fwrite getc getchar gets perror printf putc putchar puts remove rename rewind scanf setbuf setvbuf sprintf sscanf tmpfile tmpnam ungetc vfprintf vprintf vsprintf'),
 'cstdlib': ('stdlib.h', 'size_t div_t ldiv_t abort abs atexit atof atoi atol bsearch calloc div exit free getenv labs ldiv malloc mblen mbstowcs mbtowc qsort rand realloc srand strtod strtol strtoul system wcstombs wctomb'),
 'cstring': ('string.h', 'size_t memchr memcmp memcpy memmove memset strcat strchr strcmp strcoll strcpy strcspn strerror strlen strncat strncmp strncpy strpbrk strrchr strspn strstr strtok strxfrm'),
 'cmath': ('math.h', 'acos asin atan atan2 ceil cos cosh exp fabs floor fmod frexp ldexp log log10 modf pow sin sinh sqrt tan tanh'),
 'cwchar': ('wchar.h', 'size_t mbstate_t wint_t wchar_t fwprintf swprintf wcscat wcschr wcscmp wcscpy wcslen wcsncmp wcsncpy wcsstr wcstol wprintf'),
 'cstddef': ('stddef.h', 'size_t ptrdiff_t'),
 'cassert': ('assert.h', ''),
 'cctype': ('ctype.h', 'isalnum isalpha iscntrl isdigit isgraph islower isprint ispunct isspace isupper isxdigit tolower toupper'),
 'cfloat': ('float.h', ''), 'climits': ('limits.h', ''), 'cerrno': ('errno.h', ''),
 'ctime': ('time.h', 'size_t clock_t time_t tm asctime clock ctime difftime gmtime localtime mktime strftime time'),
 'cstdarg': ('stdarg.h', 'va_list'),
}
for name, (header, names) in SHIMS.items():
    path = os.path.join(inc, name)
    if os.path.islink(path):
        os.unlink(path)
    body = (f'// {name}: Linux-lane shim over the DDK 3790.1830 CRT (its own <{name}> predates the std:: using-declarations)\n'
            f'#ifndef _SHIM_{name.upper()}_\n#define _SHIM_{name.upper()}_\n#include <{header}>\n')
    if names:
        body += 'namespace std {\n' + ''.join(f'    using ::{n};\n' for n in names.split()) + '}\n'
    body += '#endif\n'
    open(path, 'w').write(body)
print(f'std shims written into {inc}: {", ".join(SHIMS)}')
