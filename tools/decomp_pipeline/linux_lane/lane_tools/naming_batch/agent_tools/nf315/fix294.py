# fix294.py: NF315 in-place edits (zero line delta), orig/<addr>.cpp -> rows/<addr>.cpp; each pair must match exactly once.
import sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF315'
E={
'00717bc0':[
 ('    CActionTalkBase(CThing* target, bool a, bool b, bool c);   // 0x00717950\n',
  'protected: CActionTalkBase(CThing& target, bool a, bool b, bool c); public:   // 0x00717950; NF315: Ego_r + FableWin ??0CActionTalkBase@NTCScriptedControl@@IAE@AAVCThing@@_N11@Z\n'),
 ('    : CActionTalkBase(target, false, false, false)\n','    : CActionTalkBase(*target, false, false, false)\n'),
],
'00997800':[
 ('#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE\n#include "engine/CAFile.h"                         // CAFile::SplitPath(const CWideString&, ...) 0x00997620 (Ego_r.pdb); fable_string.h CWideString\nCWideString __fastcall MakePath(const CWideString& drive, const CWideString& directory, const CWideString& name, const CWideString& extension);   // 0x00997780\n',
  '#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE\n#define FABLE_RETAIL_CAFILE_MAKEPATH   // NF315 add-only opt-in: CAFile::MakePath 0x00997780 (Ego_r + FableWin ?MakePath@CAFile@@SI?AVCWideString@@ABV2@000@Z; was the free stand-in)\n#include "engine/CAFile.h"                         // CAFile::SplitPath(const CWideString&, ...) 0x00997620 (Ego_r.pdb); fable_string.h CWideString\n'),
 ('    return MakePath(drive, directory, name, extension);','    return CAFile::MakePath(drive, directory, name, extension);'),
],
'00d80e40':[
 ('    struct CBriarRoseImposter { static','    struct CBRIAR_ROSE_IMPOSTER { static'),
 ('&CBriarRoseImposter::Alloc','&CBRIAR_ROSE_IMPOSTER::Alloc'),
],
'00a5eea0':[
 ('#include "engine/CBankFile.h"\n','#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY   // NF315: CBankFile::EntryExists 0x009CC190 (Ego_r + FableWin ?EntryExists@CBankFile@@QBE_NK@Z) from the header\n#include "engine/CBankFile.h"\n'),
 ('\nstruct CStreamingFontDataBank_Methods : CBankFile {\n    bool EntryExists(unsigned long index);                    // 0x009CC190 (CBankFile)\n',
  '\nstruct CStreamingFontDataBank_Methods : CBankFile {\n'),
],
'009ce8d0':[
 ('    void WriteZeros(unsigned long count);          // 0x009CCB20\n',
  'private: void WritePadding(unsigned long size); public:   // 0x009CCB20; NF315: Ego_r / FableWin ?WritePadding@CBankFile@@AAEXK@Z (private), was WriteZeros\n'),
 ('        WriteZeros(Align(length) - length);','        WritePadding(Align(length) - length);'),
],
}
for a in (sys.argv[1:] or E):
    t=open(A+'/orig/%s.cpp'%a,encoding='utf-8').read(); n0=t.count('\n')
    for o,nw in E[a]:
        c=t.count(o)
        if c!=1: sys.exit('%s: pattern count %d: %r'%(a,c,o[:60]))
        t=t.replace(o,nw)
    open(A+'/rows/%s.cpp'%a,'w',encoding='utf-8').write(t)
    print(a,'lines',n0,'->',t.count('\n'))
