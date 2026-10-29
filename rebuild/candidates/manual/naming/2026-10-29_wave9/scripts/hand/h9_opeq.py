# wave 9 sub-wave 0x0099EFB0: CCharString::operator= is ??4CCharString@@QAEABV0@ABV0@@Z (Ego_r 0x00AFA810, FableWin 0x02F61830;
# const& return; opeq pass 2026-10-28; NAME_FIXES chunks 529/531/538/571/574). Rows whose local CCharString sat inside a
# row-local namespace (or std) emitted ??4CCharString@<ns>@@QAEAAU01@ABU01@@Z: the class is hoisted to global scope as
# `class CCharString` with the const& return (declaration only; codegen unchanged).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, hoist, opeq_const
H = lambda a, pat=r'struct CCharString\s*\{', **kw: hoist(a, 'src', pat, fix=opeq_const, **kw)
H('00cb93e0')
H('009b77b0')
H('00cb68c0', qual=['StringData'])
H('00cbac70', extra_before='struct CCharStringData;')
H('00cbb4d0', qual=['StringData', 'StringInstances'])
H('00e54990', extra_before='struct CCharStringData;')
H('006eac50', qual=['CBasicStringData', 'EmptyString'])
H('007565e0', qual=['CCharStringData', 'ReleaseStringData'])
H('007adbf0')
H('004d0f80')
H('004e1c8a', qual=['CBasicStringChar'])
H('004fe630', qual=['CCharStringData', 'CAFile', 'CCharStringInstances'])
edit('004fe630', 'src', 'void CCharString::LoadFromFile(CAFile*file,void*context,int length){UnassignString();if(length==-1)length=file->GetFileSize()-file->GetFilePosition();p=new CCharStringData(length,\' \');if(p&&length>0)file->ReadRaw(p->Data.PData,length,context);}',
     '} void CCharString::LoadFromFile(X_004fe630::CAFile*file,void*context,int length){UnassignString();if(length==-1)length=file->GetFileSize()-file->GetFilePosition();p=new X_004fe630::CCharStringData(length,\' \');if(p&&length>0)file->ReadRaw(p->Data.PData,length,context);} namespace X_004fe630 {')
H('00af4ee0', pat=r'class CCharString\s*\{')
# found by the catalog-wide sweep (unbound namespaced spellings ??4CCharString@<ns>@@QAEAAU01@ABU01@@Z, same call target)
H('0052b9d0', extra_before='struct CCharStringData;\n')
H('0052bff0', pat=r'struct CCharString\s*\{void\*p;')
H('00569550', extra_before='struct CCharStringData;\n')
# long default-length ctor (??0CCharString@@QAE@PBDJ@Z, 0x0099EBF0) in the two hoisted rows that spelled it int
edit('006eac50', 'src', 'CCharString(const char*,int=-1);', 'CCharString(const char*,long=-1);')
edit('004fe630', 'src', 'CCharString(const char*s,int n=-1)', 'CCharString(const char*s,long n=-1)')
# stand-in names for 0x0099EFB0 (retail call target; PDB CCharString::operator=(const CCharString&) const& return)
edit('0047c600', 'src', 'struct MemberA {                 // sizeof == 4\n    unsigned long _p;\n    MemberA& operator=(const MemberA& _rhs);   // out-of-line -> __fastcall call A',
     'class CCharString { public:      // sizeof == 4\n    unsigned long _p;\n    const CCharString& operator=(const CCharString& _rhs);   // 0x0099EFB0')
edit('0047c600', 'src', '    MemberA       _b;            // +0x04', '    CCharString   _b;            // +0x04')
edit('009e6fa0', 'src', 'struct SymbolString:CCharString{SymbolString(const SymbolString&);SymbolString& operator=(const SymbolString&);bool',
     'struct SymbolString:CCharString{SymbolString(const SymbolString&);bool')
edit('007fefc0', 'src', '    void Assign(const CCharString& src);', '    const CCharString& operator=(const CCharString& src);   // 0x0099EFB0')
edit('007fefc0', 'src', '    out->Assign(this->arr[index]);', '    *out = this->arr[index];')
edit('00890690', 'src', 'struct CScriptAttachmentComponent {\n    void AttachToScript(int scriptId);\n};',
     'class CCharString { public:\n    void* Data;\n    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0\n};')
edit('00890690', 'src', '    ((CScriptAttachmentComponent*)((char*)target + 0x58))->AttachToScript(scriptId);',
     '    *(CCharString*)((char*)target + 0x58) = *(const CCharString*)scriptId;')
edit('009ea620', 'src', 'struct ConBuffer {\n    void*         data;    // +0x00', 'class CCharString { public:\n    void* Data;\n    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0\n};\n\nstruct ConBuffer {\n    CCharString   data;    // +0x00')
edit('009ea620', 'src', '    // member at 0x99efb0: copies contents of another buffer\n    void CopyContents(ConBuffer* src);\n', '')
edit('009ea620', 'src', '    this->front.CopyContents(&this->back);', '    this->front.data = this->back.data;')
edit('0057a5a5', 'src', 'struct Sub { void M(int* p); };', 'class CCharString { public: void* Data; const CCharString& operator=(const CCharString& other); };   // 0x0099EFB0')
edit('0057a5a5', 'src', '((Sub*)((char*)this+0x170))->M(&g_glob);', '*(CCharString*)((char*)this+0x170) = *(const CCharString*)&g_glob;')
edit('005db4a0', 'src', 'struct SubObj {\n    void copyFrom(SubObj* src);   // __fastcall -> harness makes __fastcall (ecx=this)\n};',
     'class CCharString { public:\n    void* Data;\n    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0\n};')
edit('005db4a0', 'src', '    SubObj sub;         // at +8', '    CCharString sub;    // at +8')
edit('005db4a0', 'src', '    n->sub.copyFrom(&((CAnimComponentBase*)this)->sub);', '    n->sub = ((CAnimComponentBase*)this)->sub;')
edit('00890970', 'src', 'struct StrDest {\n    void ctor(const int* e);\n};', 'class CCharString { public: void* Data; const CCharString& operator=(const CCharString& other); };   // 0x0099EFB0\nstruct StrDest;')
edit('00890970', 'src', '    dest->ctor(&g_attTable[attitude]);', '    *(CCharString*)dest = *(const CCharString*)&g_attTable[attitude];')
# 00404720 (CStringParser ctor, non-const declaration) left: its test FAILs at baseline (DEFERRED.tsv)
edit('007fefc0', 'tst', '    void Assign(const CCharString& src);', '    const CCharString& operator=(const CCharString& src);')
edit('007fefc0', 'tst', 'void CCharString::Assign(const CCharString& src){ captured_self=this; captured_src=&src; }', 'const CCharString& CCharString::operator=(const CCharString& src){ captured_self=this; captured_src=&src; return *this; }')
