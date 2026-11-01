# family newline (hand): 0x013B8BA8 is `anonymous namespace'::NEWLINE, a const CWideString(L"\n") of the CTCInventoryWeapons TU
# (ESTABLISHED_SPELLINGS 2026-11-01: ?NEWLINE@?A0x<TU hash>@@3VCWideString@@B; the initializer row 0x0121AB3F already defines it).
# The same-TU caller 0x005C3444 named it by the descriptive g_NewLineWideString: it now declares the file-static.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
edit('005c3444', 'src', 'extern const CWideString g_NewLineWideString;                     // 0x013B8BA8 (a static separator string; name descriptive)',
     "namespace { extern const CWideString NEWLINE; }                  // 0x013B8BA8 (`anonymous namespace'::NEWLINE of this TU, L\"\\n\"; ESTABLISHED_SPELLINGS)")
edit_all('005c3444', 'src', 'g_NewLineWideString', 'NEWLINE')
