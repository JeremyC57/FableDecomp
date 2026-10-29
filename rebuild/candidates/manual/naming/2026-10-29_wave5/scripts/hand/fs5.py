# wave 5 hand edits that need the rebuild/include/fable_string.h change (naming/w5/fable_string.h.w5, diff in
# fable_string.h.diff): CWideString gains GetAsCharArray (0x0099AF10), FindCharPos (0x0099B070), Sprintf (0x0099BA70),
# SetLength (0x0099BC50); AllocStringData (0x0099B3C0) becomes protected. Exact single replacements; HE_OV=<overlay>.
#   0041a430, 0055cfb0: the `const wchar_t* text = s;` conversion calls 0x0099AF10, which is GetAsCharArray
#     (?GetAsCharArray@CWideString@@QBEPBGXZ, DECISIONS.tsv proven; operator const wchar_t* is 0x0099AF00).
#   0055e6b0: the free FormatWideString stand-in is CWideString::Sprintf (__cdecl member, this first: same bytes), and
#     the CWideStringOperations adapter's FindCharPos / SetLength are CWideString members; source and test (the test
#     defines the stubs and carries a host copy of the header under !_MSC_VER).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
edit('0041a430', 'src', 'GetLength @ 0x0099B220, operator const wchar_t* @ 0x0099AF10,', 'GetLength @ 0x0099B220, GetAsCharArray @ 0x0099AF10,')
edit('0041a430', 'src', '    const wchar_t* text = directory;\n', '    const wchar_t* text = directory.GetAsCharArray();\n')
edit('0055cfb0', 'src', 'CWideString::operator const wchar_t* 0x0099AF10,', 'CWideString::GetAsCharArray 0x0099AF10,')
edit('0055cfb0', 'src', '    const wchar_t* text = description;\n', '    const wchar_t* text = description.GetAsCharArray();\n')
for k in ('src', 'tst'):
    edit('0055e6b0', k, 'void __cdecl FormatWideString(CWideString*, const wchar_t*, ...); //0099BA70\n', '')
    edit('0055e6b0', k, '    FormatWideString(&text,L"%d",value); //0122DE44\n', '    text.Sprintf(L"%d",value); //0099BA70 CWideString::Sprintf, L"%d" 0122DE44\n')
    edit('0055e6b0', k, 'struct CWideStringOperations : CWideString {\n    long FindCharPos(wchar_t) const; //0099B070\n    void SetLength(long); //0099BC50\n};\n', '')
    edit('0055e6b0', k, "        CWideStringOperations& operations=reinterpret_cast<CWideStringOperations&>(value);\n"
                        "        operations.SetLength(operations.FindCharPos(L'.')+1+NumDecPlacesToShow);\n",
                        "        value.SetLength(value.FindCharPos(L'.')+1+NumDecPlacesToShow); //0099BC50, 0099B070\n")
# the test's stubs and its host copy of the header
edit('0055e6b0', 'tst', 'void __cdecl FormatWideString(CWideString* result,const wchar_t* fmt,...) {\n',
     'void __cdecl CWideString::Sprintf(const wchar_t* fmt,...) {\n    CWideString* result=this;\n')
edit('0055e6b0', 'tst', 'long CWideStringOperations::FindCharPos(wchar_t c) const {', 'long CWideString::FindCharPos(wchar_t c) const {')
edit('0055e6b0', 'tst', 'void CWideStringOperations::SetLength(long n) {', 'void CWideString::SetLength(long n) {')
edit('0055e6b0', 'tst', '    operator const wchar_t*() const;\n    long GetLength() const;\n    const CWideString& operator=(const CWideString& other);',
     '    operator const wchar_t*() const;\n    const wchar_t* GetAsCharArray() const;\n    long GetLength() const;\n'
     '    long FindCharPos(wchar_t c) const;\n    void __cdecl Sprintf(const wchar_t* format, ...);\n    void SetLength(long length);\n'
     '    const CWideString& operator=(const CWideString& other);')
for k in ('src', 'tst'):   # keep the header comments truthful
    edit('0055e6b0', k, '// NumDecPlacesToShow+c and LowerIsBetter+d. Shared fable_string.h supplies strings;\n'
                        '// the method-only wide string view adds FindCharPos and SetLength omitted there.\n',
                        '// NumDecPlacesToShow+c and LowerIsBetter+d. Shared fable_string.h supplies the strings\n'
                        '// (CWideString::Sprintf, FindCharPos and SetLength are its members).\n')
    edit('0055e6b0', k, 'append0099B940/0099B8D0; formatter0099BA70;', 'append0099B940/0099B8D0; Sprintf0099BA70;')
# 0055cfb0 also carries the root CBaseClassNonCopyable as a struct with a public ctor/dtor; the PDB pair is protected
# (??0CBaseClassNonCopyable@@IAE@XZ 0x0099A2F0 / ??1CBaseClassNonCopyable@@MAE@XZ 0x0099A300, DECISIONS.tsv proven)
edit('0055cfb0', 'src', 'struct CBaseClassNonCopyable {\n    CBaseClassNonCopyable();                 // 0x0099A2F0\n'
                        '    virtual ~CBaseClassNonCopyable();        // 0x0099A300\n};',
                        'class CBaseClassNonCopyable {\nprotected:\n    CBaseClassNonCopyable();                 // 0x0099A2F0 (protected: IAE)\n'
                        '    virtual ~CBaseClassNonCopyable();        // 0x0099A300 (protected virtual: MAE)\n};')
