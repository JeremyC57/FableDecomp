# family anon (hand part): GetDataFromEventString 0x00870C90 is the anonymous-namespace file-static
# ?GetDataFromEventString@?A0xaf422eae@@YI_NABVCCharString@@JAAV2@@Z (Ego_r / FableWin; owner policy 2026-11-01: the source
# spells it inside `namespace { }`, the bindings table carries the PDB hash, the tools match ignoring the hash). Its two
# global-namespace pointer-out callers are respelled (a reference is passed as the same pointer). HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
D0 = 'bool __fastcall GetDataFromEventString(const CCharString& data, long index, CCharString* out);   // 0x00870C90'
D1 = ('namespace { bool __fastcall GetDataFromEventString(const CCharString& data, long index, CCharString& out); }   // 0x00870C90\n'
      '//   (Ego_r / FableWin ?GetDataFromEventString@?A0xaf422eae@@YI_NABVCCharString@@JAAV2@@Z: the file-static of its own TU)')
for A in ('00870d70', '00870de0'):
    edit(A, 'src', D0, D1)
edit('00870d70', 'src', '// Callees: GetDataFromEventString(const CCharString&, long index, CCharString* out) @ 0x00870C90 (__fastcall:\n// string in ecx, index in edx, out pointer on the stack)',
     '// Callees: GetDataFromEventString(const CCharString&, long index, CCharString& out) @ 0x00870C90 (__fastcall:\n// string in ecx, index in edx, out reference on the stack)')
edit('00870d70', 'src', '    if (!GetDataFromEventString(data, 0, &Object))', '    if (!GetDataFromEventString(data, 0, Object))')
edit('00870d70', 'src', '    if (!GetDataFromEventString(data, 1, &Hand))', '    if (!GetDataFromEventString(data, 1, Hand))')
edit('00870d70', 'src', '    if (GetDataFromEventString(data, 2, &destroyOnExit))', '    if (GetDataFromEventString(data, 2, destroyOnExit))')
edit('00870de0', 'src', '    return GetDataFromEventString(data, 0, &Hand) ? true : false;', '    return GetDataFromEventString(data, 0, Hand) ? true : false;')
