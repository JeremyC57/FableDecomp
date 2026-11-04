# postfix.py <famdir>...: cheap callee/global respellings to established spellings, applied to generated sources in place; prints changed rows
import sys,re,json,os
NOTE={}
def fix(a,s):
    notes=[]
    if re.search(r'\bg_bScriptAnimationFlag\b|\bg_PlayAnimationDefaultFlag\b',s):
        s2=re.sub(r'(?m)^([ \t]*)extern\s+bool\s+(g_bScriptAnimationFlag|g_PlayAnimationDefaultFlag)\s*;[^\n]*$',
                  r'\1namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748 (established spelling; naming wave 35: was \2)',s)
        s2=re.sub(r'(?<![\w:])(g_bScriptAnimationFlag|g_PlayAnimationDefaultFlag)\b(?!\))','NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations',s2)
        # restore the comment mention
        s2=s2.replace('naming wave 35: was NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations','naming wave 35: was the stand-in global')
        if s2!=s: s=s2; notes.append('global 0x01375748 respelled NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations')
    if a=='00e299f0' and 'CRecordSet_00cbd510' in s:
        s=s.replace('struct CRecordSet_00cbd510 {                // vtable 0x012C1648, 0x40 bytes','class CScriptBase { public:              // vtable 0x012C1648, 0x40 bytes (naming wave 35: was the stand-in CRecordSet_00cbd510; ctor 0x00CB8110)')
        s=re.sub(r'\bCRecordSet_00cbd510\b','CScriptBase',s); notes.append('stand-in CRecordSet_00cbd510 respelled CScriptBase (0x00CB8110 = CScriptBase::CScriptBase)')
    return s,notes
for fam in sys.argv[1:]:
    g=[l.rstrip('\n').split('\t') for l in open(fam+'/gen.tsv')]
    for r in g:
        if r[1]!='OK': continue
        d=json.loads(r[2]); s=open(d['path']).read()
        s2,notes=fix(r[0],s)
        if notes:
            # record in the naming-wave note line
            s2=s2.replace('; same bytes.','; same bytes. Also: %s.'%'; '.join(notes),1) if '; same bytes.' in s2 else s2
            open(d['path'],'w').write(s2); d['postfix']=notes; r[2]=json.dumps(d); print(r[0],notes)
    open(fam+'/gen.tsv','w').write(''.join('\t'.join(x)+'\n' for x in g))
