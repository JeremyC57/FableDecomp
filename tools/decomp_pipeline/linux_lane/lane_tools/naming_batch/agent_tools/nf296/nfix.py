# nfix.py: NF296 respells -- word respell on the given lines of each binder's CODE part (comments untouched except the declaration line,
# whose comment gets a short "NF296: PDB name, was <old>" note); optional `const` on the declaration line; every listed line must change.
import re,sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF296/rows/'
def resp(a,old,new,lines,decl=None,const_line=None):
    if old.startswith('int '): pass
    p=A+a+'.cpp'; L=open(p,encoding='utf-8').read().split('\n')
    for n in lines:
        l=L[n-1]; k=l.find('//'); code,com=(l,'') if k<0 else (l[:k],l[k:])
        c2=re.sub(r'\b%s\b'%re.escape(old),new,code); assert c2!=code,(a,n,old,l[:100])
        if n==const_line:
            c3=re.sub(r'(\b%s\s*\([^)]*\))\s*;'%re.escape(new),r'\1 const;',c2,count=1); assert c3!=c2,(a,n,'const'); c2=c3
        if n==decl:
            com=(com+'; ' if com else '// ')+'NF296: PDB name, was %s'%old
        L[n-1]=c2+com
    open(p,'w',encoding='utf-8').write('\n'.join(L)); print('ok',a,old,'->',new,lines)
U=sys.argv[1]
if U=='u1':
    resp('00d9ee20','IsSpawnPointToSpawn','ShouldSpawnPointSpawn',[915,938],decl=915)
    resp('00892f10','DeactivateQuestLater','AddPendingQuestToDeactivate',[20,32],decl=20)
    resp('004fc190','ContainsMap','IsMapIncluded',[25,38],decl=25)
    resp('0057b0f7','SetSunTan','SetTan',[32,51],decl=32)
    resp('007f14f0','CreateHeadEffect','DoDieFX',[103,124],decl=103)
    resp('0057d8be','AddRenownEntry','AddRenownChange',[59,107],decl=59)
    resp('0066bbf0','UpdateSounds','PlayAllSoundEvents',[17,24],decl=17)
    resp('0069f8b0','CancelAbilities','CancelMove',[50,69],decl=50)
    resp('00e82650','InitialiseVariablesCheck','SetCompletedThisClub',[93,122],decl=93)
    resp('00e9fcd0','ResetMissionState','InitialiseVariables',[79,86],decl=79)
    resp('0054ea00','InitialiseFromDef','GetDataFromDef',[55,62],decl=55)
    resp('00a67ea0','InitialiseFromVolume','InitialiseFromVolumeTextureMipmap',[31,34],decl=34)
    resp('009beeb0','OnPresent','OnPostSwapScreens',[26,60],decl=26)
    resp('0088e400','SetEnabled','SetMiniMapEnabled',[2,7])
if U=='u2':
    for a,l in (('004afa60',[40,65]),('004afb00',[39,64]),('004b26b0',[121,136])):
        resp(a,'BindsEntityScript','IsEntityScriptBound',l,decl=l[0],const_line=l[0])
    resp('0080fd30','GetMaxCreatureCount','GetNumberCanSummon',[10,54],decl=10,const_line=10)
    resp('0096e540','GetMaxCreatureCount','GetNumberCanSummon',[141,154],decl=141,const_line=141)
    resp('0065935e','ResetBrain','ForceAIReset',[52,93],decl=52)
    resp('006cd970','FindPosZ','GetHeightZBelowAt',[6,10],const_line=6)
    resp('006cd970','int flag','bool flag',[6])   # R1: PDB ?GetHeightZBelowAt@CThingCreatureBase@@QBEMABVC3DVector@@_N1@Z takes (const C3DVector&, bool, bool)
    resp('006cd990','FindPosZAt','GetHeightZBelowAt',[4,7],const_line=4)
    resp('0077b1c0','GetThingByUID','GetPThingWithUID',[21,49],decl=21)
    resp('00730070','FindThingByUID','GetPThingWithUID',[112,142,150,158],decl=112)
    for a,l in (('00729bc0',[33,60]),('00779340',[27,71]),('00800460',[25,46])):
        resp(a,'GetThingByUID','GetPThingWithUID',l,decl=l[0])
    resp('0098ec40','RemovePendingRequests','CloseAllStreamsForFile',[15,38],decl=15)
    resp('0098ec70','RemovePending','CloseAllStreamsForFile',[8,27],decl=8)
    resp('00991450','RemovePendingRequests','CloseAllStreamsForFile',[20,47],decl=20)
    for a,l in (('00751c70',[50,88]),('00751e80',[65,107]),('00752930',[63,110])):
        resp(a,'AngleBetweenPositions','GFGetAngleXYTo',l,decl=l[0])
    resp('00752930','AngleYZBetweenPositions','GFGetAngleYZTo',[64,111],decl=64)
    resp('007a9960','GetAnaloguePosition','GetJoystickPosition',[51,88],decl=51)
    resp('0069f670','GetPerformedAction','HasPerformedAction',[66,77],decl=66)
    resp('0069fc40','GetPerformedAction','HasPerformedAction',[68,81],decl=68,const_line=68)
    resp('00848ae0','GetCanItemBeQuickAssigned','HasPerformedAction',[50,63],decl=50,const_line=50)
def wrap(a,n,acc):
    # put one declaration line under `<acc>:` and reopen `public:` on the same line (line count kept; PDB access code I/A)
    p=A+a+'.cpp'; L=open(p,encoding='utf-8').read().split('\n'); l=L[n-1]; k=l.find('//'); code,com=(l,'') if k<0 else (l[:k],l[k:])
    m=re.match(r'^(\s*)(.*?;)(\s*)$',code); assert m,(a,n,l)
    L[n-1]=m.group(1)+'%s: %s public:'%(acc,m.group(2))+m.group(3)+com; open(p,'w',encoding='utf-8').write('\n'.join(L)); print('wrap',a,n,acc)
if U=='u3':
    resp('0097e890','SetNavigationIgnoreThing','SetThingToNotAvoid',[196,207],decl=196); wrap('0097e890',196,'protected')
    resp('0097e890','SetNavigationSpeedScale','SetProximityForDirectApproach',[197,208],decl=197); wrap('0097e890',197,'protected')
    resp('00947280','SelectPrettyThing','FindPrettyThingInShop',[65,82],decl=65); wrap('00947280',65,'private')
    resp('009474f0','SelectPrettyThing','FindPrettyThingInShop',[105,165],decl=105); wrap('009474f0',105,'private')
    resp('00be32a0','Initialise','BuildSharedIndexBuffers',[18,23],decl=18); wrap('00be32a0',18,'private')
    resp('00b8d780','InitializeOcclusionBuffers','BuildUnitBox',[120,133],decl=120); wrap('00b8d780',120,'private')
    resp('00b99b50','FrameStartCommon','CalcFrameConstantSIMDVars',[20,37],decl=20); wrap('00b99b50',20,'private')
    resp('0069b790','UpdateEarthQuakes','UpdateEarthquake',[41,49],decl=41); wrap('0069b790',41,'private')
    resp('00417001','GetTimeSinceLastRenderFrameStart','GetPredictedTimeSinceLastRenderFrame',[154,184,202],decl=154,const_line=154); wrap('00417001',154,'protected')
    resp('00a62860','DeleteShaderEntries','CloseShaders',[49,52],decl=49); wrap('00a62860',49,'private')
    resp('00670a10','UpdateGravity','UpdateHeightAboveGround',[83,94],decl=83); wrap('00670a10',83,'private')
    resp('0077cdd0','Update','DoStopRunning',[11,18],decl=11); wrap('0077cdd0',11,'protected')
    resp('0049a0b0','LongToWideString','GFIntToWideString',[63,118,135],decl=63)
if U=='u4':
    resp('004b4490','UpdateScript','RunSubThreads',[55,135],decl=55)
    resp('00665260','GetHeadAngleOffset','GetLookAngleRelToBodyXY',[37,96],decl=37,const_line=37)
    resp('0072ac90','IsMoveActive','IsShieldActive',[20,35],decl=20)
    resp('007363a0','GetCombatTypeIndex','GetCombatType',[135,200],decl=135)
    resp('008c55b0','IsCollidingWithLandscape','IsAbortingCurrentAction',[25,79,83],decl=25)
    for a,o,l in (('0093d180','IsInCombatMode',[153,170]),('0093e240','UsesCreatureModes',[34,94]),('0093e5e0','IsInCombatMode',[40,136]),
                  ('0096edb0','IsInCombatMode',[144,190]),('0096ee90','IsInCombatMode',[24,127]),('00972110','UsesCreatureModes',[33,93])):
        resp(a,o,'IsAbleToStrafe',l,decl=l[0],const_line=l[0])
    resp('00490927','IsInSpecialMode','IsPerformingRegionLoad',[82,128],decl=82,const_line=82)
    resp('0079eaa0','ClearMeshEffect','ClearHighlighted',[48,81],decl=48)
    resp('007d8e20','ClearMeshEffect_004bffc0','ClearHighlighted',[51,153],decl=51)
    resp('008f17d0','ClearMeshEffect_004bffc0','ClearHighlighted',[34,96],decl=34)
    for a,o,l in (('0069f4f0','RestartCurrentMode',[44,81]),('0069f670','UpdateFrontMode',[46,86]),('0069fc40','OnUnsheatheFinished',[43,90]),
                  ('0078c860','RestartCurrentMode',[15,45]),('0078c8b0','RestartCurrentMode',[15,33]),('0078e9c0','RestartCurrentMode',[107,156]),
                  ('00793ec0','RestartCurrentMode',[107,156]),('008ba3c0','UpdateFrontMode',[113,170]),('008ed850','RestartCurrentMode',[35,58])):
        resp(a,o,'RefreshCurrentMode',l,decl=l[0])
    resp('008ba3c0','IsPrimarySlotEmpty','IsPrimarySlotFree',[99,143,154],decl=99,const_line=99)
    resp('007f1410','HasMode','IsModeActive',[54,106],decl=54,const_line=54)
    resp('008c2490','HasMode','IsModeActive',[26,58],decl=26,const_line=26)
    resp('00684210','IsCrimeHandled','HasCrimeBeenReported',[57,124],decl=57)
    resp('00958cb0','IsDeedHandled','HasCrimeBeenReported',[71,120],decl=71)
    resp('0065c36e','ProcessCrimeDeeds','ClearCrimes',[84,110],decl=84)
    resp('00838bc0','HasDamage','IsMeleeWeapon',[113,293],decl=113)
    resp('004e1537','Reset','Clear',[41,47],decl=41)
