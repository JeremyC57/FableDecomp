# nfix.py: NF299 respells -- word respell on the given lines of each binder's CODE part (comments untouched except the declaration line,
# whose comment gets a short "NF299: PDB name, was <old>" note); optional `const` on the declaration line; every listed line must change.
import re,sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF299/rows/'
def resp(a,old,new,lines,decl=None,const_line=None):
    if old.startswith('int '): pass
    p=A+a+'.cpp'; L=open(p,encoding='utf-8').read().split('\n')
    for n in lines:
        l=L[n-1]; k=l.find('//'); code,com=(l,'') if k<0 else (l[:k],l[k:])
        c2=re.sub(r'\b%s\b'%re.escape(old),new,code); assert c2!=code,(a,n,old,l[:100])
        if n==const_line:
            c3=re.sub(r'(\b%s\s*\([^)]*\))\s*;'%re.escape(new),r'\1 const;',c2,count=1); assert c3!=c2,(a,n,'const'); c2=c3
        if n==decl:
            com=(com+'; ' if com else '// ')+'NF299: PDB name, was %s'%old
        L[n-1]=c2+com
    open(p,'w',encoding='utf-8').write('\n'.join(L)); print('ok',a,old,'->',new,lines)
def wrap(a,n,acc):
    # put one declaration line under `<acc>:` and reopen `public:` on the same line (line count kept; PDB access code I/A)
    p=A+a+'.cpp'; L=open(p,encoding='utf-8').read().split('\n'); l=L[n-1]; k=l.find('//'); code,com=(l,'') if k<0 else (l[:k],l[k:])
    m=re.match(r'^(\s*)(.*?;)(\s*)$',code); assert m,(a,n,l)
    L[n-1]=m.group(1)+'%s: %s public:'%(acc,m.group(2))+m.group(3)+com; open(p,'w',encoding='utf-8').write('\n'.join(L)); print('wrap',a,n,acc)
U=sys.argv[1]
# NF299 units (line numbers from today's HEAD copies in rows/)
if U=='a1':
    for a,l in (('00996230',[34,37]),('0052ada0',[30,38]),('009967e0',[24,29]),('00996a40',[23,26]),('009c4330',[41,43])):
        resp(a,'Init','Initialise',l,decl=l[0]); wrap(a,l[0],'private')          # Ego_r + FableWin ?Initialise@CCompressorZlib@@AAEXXZ
    for a,l in (('0072ba40',[53,54,71,72]),('0072b420',[67,68,98,99])):
        resp(a,'CancelAbilities','CancelMove',l,decl=l[0])                         # CTCDivineWrath (entry 442) and CTCUnholyPower (NF296 entry 571 follow-up)
    resp('00771620','PushSourceDefIndex','PushNewSourceDef',[29,37],decl=29)        # overload of the row's own PushNewSourceDef(const CCharString&)
    resp('007e61b0','PlaySound','PlaySoundW',[55,67],decl=55)                       # PDB spelling (windows.h maps PlaySound to PlaySoundW in the original TU)
    resp('0093ae80','GetVillage','GetPVillage',[84,128],decl=84,const_line=84)
    resp('0098f3a0','ThreadProc','BeginThread',[97,119],decl=97); wrap('0098f3a0',97,'private')
    resp('006a1e00','IsMeleeWeapon','IsProjectile',[48,93],decl=48,const_line=48)
if U=='a2':
    for a,l in (('004b64e0',[34,60]),('00a2eaa0',[25,106,107,108]),('00a75400',[13,25]),('00a89450',[19,88,89,90,91]),('00a8fd40',[32,129,135])):
        resp(a,'WriteBuffer','WriteCompressed',l,decl=l[0])                       # Ego_r + FableWin ?WriteCompressed@CDataOutputStream@@QAEXPBXJ@Z
    resp('00ac3b00','WriteData','WriteCompressed',[14,49],decl=14)
    resp('00ac3b00','unsigned long size','long size',[14])                          # R1: PDB parameter type J (was K)
    resp('009fccf0','ReleaseResources','Uninitialise',[11,16],decl=11)
    resp('009fccf0','int flag','bool flag',[11])                                    # R1: PDB ?Uninitialise@CVolume@@QAEX_N@Z (was H)
    resp('009fcd10','ReleaseVolume','Uninitialise',[9,12],decl=9)
    resp('009fcd10','long flags','bool flags',[9])                                  # R1: PDB _N (was J)
    for a,l in (('009fcd40',[23,25]),('00a68130',[44,43]),('00a69550',[23,25]),('00a67ea0',[35,33])):
        resp(a,'ReleaseVolume','Uninitialise',l,decl=l[0])
if U=='a3':
    resp('009cd850','DeallocateMemory','Clear',[80,161,162,163,164],decl=80)        # CPackedUIntArray members only (lines 153-155 are CArray::DeallocateMemory)
    resp('00666ea0','PeekWorldMap','PeekWorldMap',[],decl=None) if False else None
    resp('0099b560','UnassignString','UnassignString',[],decl=None) if False else None
    wrap('0099b560',54,'protected')
    resp('0099bdb0','UnassignString_0099b4d0','UnassignString',[22,68],decl=22)     # already in a protected section
    resp('0099b7d0','Release','UnassignString',[62,75],decl=62)
    for a in ('008d48f0','008d49b0'):
        resp(a,'GetPWeapon','GetPWeaponUsed',[39,62 if a=='008d48f0' else 63],decl=39)
if U=='t':
    for a,l in (('008d70a0',[52,74,76]),('008d88a0',[50,89]),('008e97b0',[42,63,66,69,72])):
        resp(a,'FindCombinationAnim','GetFirstCombinationAnimFromType',l,decl=l[0])   # Ego_r + FableWin ?GetFirstCombinationAnimFromType@CAnimateThing@@QAEPAVCAnimSet@@J@Z
if U=='b':
    resp('007c6da0','StopSample','StopAttachedSound',[51,84],decl=51)              # Ego_r + FableWin ?StopAttachedSound@CTCSoundPlayer@@QAEXK@Z
if U=='c':
    resp('00a67ff0','ReleaseVolume','Uninitialise',[40,39],decl=40)                # CVolume only (line 55 is CVolumeTexture::Uninitialise, another function)
