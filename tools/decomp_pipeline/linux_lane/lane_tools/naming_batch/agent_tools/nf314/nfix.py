# nfix.py: NF314 respells -- word respell on the given lines of each binder's CODE part (comments untouched except the declaration line,
# whose comment gets a short "NF314: PDB name, was <old>" note); optional `const` on the declaration line; every listed line must change.
import re,sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF314/rows/'
def resp(a,old,new,lines,decl=None,const_line=None):
    if old.startswith('int '): pass
    p=A+a+'.cpp'; L=open(p,encoding='utf-8').read().split('\n')
    for n in lines:
        l=L[n-1]; k=l.find('//'); code,com=(l,'') if k<0 else (l[:k],l[k:])
        c2=re.sub(r'\b%s\b'%re.escape(old),new,code); assert c2!=code,(a,n,old,l[:100])
        if n==const_line:
            c3=re.sub(r'(\b%s\s*\([^)]*\))\s*;'%re.escape(new),r'\1 const;',c2,count=1); assert c3!=c2,(a,n,'const'); c2=c3
        if n==decl:
            com=(com+'; ' if com else '// ')+'NF314: PDB name, was %s'%old
        L[n-1]=c2+com
    open(p,'w',encoding='utf-8').write('\n'.join(L)); print('ok',a,old,'->',new,lines)
def wrap(a,n,acc):
    # put one declaration line under `<acc>:` and reopen `public:` on the same line (line count kept; PDB access code I/A)
    p=A+a+'.cpp'; L=open(p,encoding='utf-8').read().split('\n'); l=L[n-1]; k=l.find('//'); code,com=(l,'') if k<0 else (l[:k],l[k:])
    m=re.match(r'^(\s*)(.*?;)(\s*)$',code); assert m,(a,n,l)
    L[n-1]=m.group(1)+'%s: %s public:'%(acc,m.group(2))+m.group(3)+com; open(p,'w',encoding='utf-8').write('\n'.join(L)); print('wrap',a,n,acc)
U=sys.argv[1]
# NF314 units (line numbers from today's HEAD copies in rows/)
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
# ---- NF314 units ----
if U=='u1':
    for a,l in (('008c4f50',[38,51]),('008e46d0',[38,51]),('0095cb80',[40,53]),('00976fc0',[38,51]),('00977060',[38,51]),
                ('0097c760',[38,51]),('0097c830',[38,51]),('0097c920',[38,51])):
        resp(a,'SetName','SetCombatAnimation',l,decl=l[0])        # entry 1: Ego_r + FableWin ?SetCombatAnimation@CCombatActionBase@@QAEXABVCCharString@@@Z
    resp('007a1870','DestroyObject','DeleteData',[77,78],decl=77)   # entry 2: ?DeleteData@?$CCountedPointer@VCLightningBeam@CTCWillLightning@@@@SIXPAX@Z
    for a,l in (('00413120',[58,70]),('0048a210',[93,157])):
        resp(a,'Init','Initialise',l,decl=l[0]); wrap(a,l[0],'protected')   # entry 4: ?Initialise@CInitBaseClass@@IAE_NXZ
    resp('00a6c170','Init','Initialise',[42],decl=42); wrap('00a6c170',42,'protected')
    resp('00a6c170','Init','CInitBaseClass::Initialise',[54])        # qualified: CSystemFont declares its own Initialise (name hiding)
    resp('00d87cb0','PlayCutScene','PlayCutscene',[78,101,117],decl=78)   # entry 7: ?PlayCutscene@CQ_HeroSoulsNostroScript@NScript@@QAEXVCCharString@@@Z
if U=='u2':
    # access-only: the PDB virtual is protected (MAE); the local base models declare it public virtual (UAE)
    for a,n in (('008cf7d0',91),('00924880',27)): wrap(a,n,'protected')      # ?FinishAction@CCombatActionBase@@MAEXXZ (entry 14)
    wrap('00923cd0',10,'protected')                                          # ?OnAnimationLoaded@CCombatActionBase@@MAEXXZ (entry 15)
if U=='u3':
    # prefix item: 0x008E9710 ClearStrafingCombinationAnims is private in both PDBs (?..@CModeStrafe@NCreatureMode@@AAEX_N@Z)
    wrap('008e9710',47,'private'); wrap('008e9850',111,'private')
    p=A+'008e9710.cpp'; t=open(p,encoding='utf-8').read()
    o='// Leaf: ClearStrafingCombinationAnims\n'; assert t.count(o)==1
    open(p,'w',encoding='utf-8').write(t.replace(o,'// Leaf: ?ClearStrafingCombinationAnims@CModeStrafe@NCreatureMode@@AAEX_N@Z\n')); print('leaf 008e9710')
if U=='u5':
    # __partial_sort's tail is STLport 4.6 sort_heap (stlport/fable_containers.h line 2316; self rows 0x0081C7D0 / 0x0086C890 / 0x009B5AF0)
    for a in ('0081d580','0086d0c0','009b6ad0'): resp(a,'__sort_heap','sort_heap',[31,32],decl=31)
if U=='u6':
    # GetSubDef<T>: both PDBs name the member template instance ??$GetSubDef@V<T>@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBV<T>@@@@@Z
    # (const); the binders declared one non-template overload per T. The declaration becomes the member template (T deduced at the call).
    for a,n in (('00731e20',33),('00747820',18),('007aefd0',88),('007b0800',42),('007dfb20',43),('007e5f00',21),('007e7640',31),
                ('007ea510',22),('0080fd90',27),('0096f6f0',42),('0096f730',42)):
        p=A+a+'.cpp'; L=open(p,encoding='utf-8').read().split('\n'); l=L[n-1]; k=l.find('//'); code,com=(l,'') if k<0 else (l[:k],l[k:])
        m=re.match(r'^(\s*)bool GetSubDef\(CDefPointer<const (\w+)>\* out\)(\s*const)?;(\s*)$',code); assert m,(a,n,l)
        new=m.group(1)+'template <class T> bool GetSubDef(CDefPointer<const T>* out) const;'+m.group(4)
        com=(com+'; ' if com else '// ')+'NF314: PDB member template GetSubDef<%s>%s'%(m.group(2),'' if m.group(3) else ', const')
        L[n-1]=new+com; open(p,'w',encoding='utf-8').write('\n'.join(L)); print('tmpl',a,m.group(2),'const-added' if not m.group(3) else '')
if U=='u7':
    for a,n in (('006f5b50',20),('00867950',23),('008b8560',8),('008b9110',14)): wrap(a,n,'protected')   # ??0CCombatActionBase@@IAE@J@Z (entry 94)
    for a,n in (('009d2160',33),('009d2660',32)): wrap(a,n,'protected')   # ?Clear@?$CSmallVector@VCCharString@@$07@@IAEXXZ (entry 364)
    resp('006650e0','__stdcall AngleDifference','__fastcall GFGetSignedAngleDifference',[44],decl=44)   # ?GFGetSignedAngleDifference@@YIMMM@Z (entry 801)
    resp('006650e0','AngleDifference','GFGetSignedAngleDifference',[76])
if U=='u7s':
    # self: 0x009D2110 CSmallVector<CCharString,8>::Clear is protected in FableWin + Ego_r (?Clear@?$CSmallVector@VCCharString@@$07@@IAEXXZ)
    wrap('009d2110',29,'protected')
    p=A+'009d2110.cpp'; t=open(p,encoding='utf-8').read(); o='// Leaf: ?Clear@\n'; assert t.count(o)==1
    open(p,'w',encoding='utf-8').write(t.replace(o,'// Leaf: ?Clear@?$CSmallVector@VCCharString@@$07@@IAEXXZ\n')); print('leaf 009d2110')
if U=='u7b':
    for a,n in (('008c91b0',20),('008ce3c0',20),('008cf8f0',10)): wrap(a,n,'protected')   # ??0CCombatActionBase@@IAE@J@Z (entry 94, late census)
if U=='u9':
    # entry 801: Ego_r + FableWin ?GFGetSignedAngleDifference@@YIMMM@Z (__fastcall); the binders declared __stdcall AngleDifference (two float
    # stack arguments, callee pops 8 either way: no register argument for floats under __fastcall, so the call bytes are the same)
    for a,l in (('00725060',[20,55]),('00751820',[17,30,38]),('00751970',[15,26,39]),('00751c70',[54,90,91,95,96]),('00752300',[22,113]),
                ('00752930',[65,110,111,113,114]),('007faf60',[71,123]),('007fb270',[15,100]),('00856b20',[18,104]),('008c7e50',[50,74])):
        resp(a,'__stdcall AngleDifference','__fastcall GFGetSignedAngleDifference',[l[0]],decl=l[0]); resp(a,'AngleDifference','GFGetSignedAngleDifference',l[1:])
if U=='u10':
    # entries 607 / 608: GetPFirstThing is a member template in both PDBs (??$GetPFirstThing@<Filter>@VCThing@@@CThingSearchTools@@QBE_N...)
    for a,n in (('00921630',79),('00910100',92)):
        p=A+a+'.cpp'; L=open(p,encoding='utf-8').read().split('\n'); l=L[n-1]; k=l.find('//'); code,com=(l,'') if k<0 else (l[:k],l[k:])
        m=re.match(r'^(\s*)bool GetPFirstThing\(const (\w+)& filter, CThing\*\* (\w+)\) const;(\s*)$',code); assert m,(a,n,l)
        L[n-1]=m.group(1)+'template <class F, class T> bool GetPFirstThing(const F& filter, T** %s) const;'%m.group(3)+m.group(4)+com+'; NF314: PDB member template GetPFirstThing<%s,CThing>'%m.group(2)
        open(p,'w',encoding='utf-8').write('\n'.join(L)); print('tmpl',a)
    # entry 82: ??9CCharString@@QBE_NPBD@Z (operator!=, Ego_r + FableWin); the stand-in row spelled it Compare
    resp('006bbf20','bool Compare(const char\\* s) const','bool operator!=(const char* s) const',[5]) if False else None
    p=A+'006bbf20.cpp'; t=open(p,encoding='utf-8').read()
    o1='    bool Compare(const char* s) const;\n'; o2='    if (pB->Compare(kEmptyDisplayNameTag))\n'; assert t.count(o1)==1 and t.count(o2)==1
    t=t.replace(o1,'    bool operator!=(const char* s) const;   // 0x0099E960; NF314: PDB name, was Compare\n').replace(o2,'    if (*pB != kEmptyDisplayNameTag)\n')
    open(p,'w',encoding='utf-8').write(t); print('op!= 006bbf20')
