# b2fix.py: NF296 batch B2 -- word respells on given lines of each binder's code part (comments untouched), optional `const` added to a
# declaration line (PDB QBE where the binder declared QAE). Every listed line must change; line counts kept.
import re
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF296/rows/'
def resp(a,old,new,lines,const_line=None):
    p=A+a+'.cpp'; L=open(p,encoding='utf-8').read().split('\n')
    for n in lines:
        l=L[n-1]; k=l.find('//'); code,com=(l,'') if k<0 else (l[:k],l[k:])
        c2=re.sub(r'\b%s\b'%re.escape(old),new,code); assert c2!=code,(a,n,old,l[:100])
        if n==const_line:
            c3=re.sub(r'(\b%s\s*\([^)]*\))\s*;'%re.escape(new),r'\1 const;',c2,count=1); assert c3!=c2,(a,n,'const'); c2=c3
        L[n-1]=c2+com
    open(p,'w',encoding='utf-8').write('\n'.join(L)); print('ok',a,old,'->',new,lines)
resp('00b2a010','Announce','SetupEnableConsoleVariable',[50,104])
resp('0054e0b0','Initialize','GetDataFromDef',[94,100])
resp('00811760','HasDamageAttribute4','IsShotStrike',[65,90])
resp('00690030','GetEventScrollAmount','GetMouseWheelMovement',[136,160],const_line=136)
resp('00bf7f70','CalcCameraRangeState','GetDesiredLODBand',[91,109])
resp('009d66a0','RemoveBank','UnregisterBank',[22,37]); resp('009d66a0','AddBank','RegisterBank',[23,39])
resp('0063a760','IsPassableAtBlock','GetNavPreferredAt',[37,138])
resp('00adfdc0','Reset','Clear',[37,80,90])
resp('007dd270','GetProgressRatio','GetPercentageComplete',[27,71],const_line=27)
# (reverted, port: binds on the derived CCombatActionBase) resp('008b8cb0','GetAnimationFraction','GetPercentageComplete',[36,45],const_line=36)
# (reverted, port: binds on the derived CCombatActionBase) resp('008c9f40','GetProgress','GetPercentageComplete',[38,51],const_line=38)
# (reverted, port: binds on the derived CCombatActionBase) resp('008ec3f0','GetProgressRatio','GetPercentageComplete',[35,53],const_line=35)
for a,ls in (('009b0200',[18,87]),('00a2eaa0',[20,101,102,103,104,105]),('00a4beb0',[17,64,72,75,82]),('00a89450',[18,85,86]),('00a8e770',[19,48]),('00abbf30',[48,106,107,108]),('00bfd670',[22,63,64])):
    resp(a,'WriteUShort','WriteUWORD',ls)
resp('00abaee0','WriteUSHORT','WriteUWORD',[27,96])
resp('00bb6e00','RegisterEnable','AddEnableConsoleVariable',[64,90]); resp('00bba480','RegisterEnable','AddEnableConsoleVariable',[62,88]); resp('00bc7e90','RegisterEnable','AddEnableConsoleVariable',[21,29])
resp('00be1e70','SetCameraDesc','SetCameraState',[65,153,174])
resp('0088ba40','IsEnabledAndRunnable','IsRunnableMain',[30]); resp('0088b6f0','IsEnabledAndRunnable','IsRunnableMain',[29])
# then (as run, by sed): rows/00690030.cpp line 136 comment '(descriptive)' -> '(Ego_r + FableWin QBE; NF296, was GetEventScrollAmount)'
