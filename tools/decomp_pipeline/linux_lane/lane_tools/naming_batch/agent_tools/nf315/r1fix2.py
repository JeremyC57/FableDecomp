# r1fix2.py: 0x00644424 second step -- CPlayerManager.h's MULTIPLAYERCOLOUR opt-in names `class ::CRGBColour`, so declare the class first
# (the row defines its local CRGBColour later); two comment lines merged to keep the line count.
f='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF315/rows/00644424.cpp'
t=open(f,encoding='utf-8').read(); n0=t.count('\n')
o='// 449B60 GetMultiplayerColour(long)const,438010 Draw2DText (thunk to64B6DA).\n// Draw2DText signature PDB44AAD0: const vector/string/font/colour&,float,EEngine2DLayer,bool.\n'
assert t.count(o)==1; t=t.replace(o,'// 449B60 GetMultiplayerColour(long)const,438010 Draw2DText (thunk to64B6DA). Draw2DText signature PDB44AAD0: const vector/string/font/colour&,float,EEngine2DLayer,bool.\n')
o='#define FABLE_RETAIL_CPLAYERMANAGER_MULTIPLAYERCOLOUR   // NF315: GetMultiplayerColour 0x00449B60 from CPlayerManager.h (Ego_r + FableWin QBE)\n'
assert t.count(o)==1; t=t.replace(o,o+'class CRGBColour;   // NF315: the opt-in declares `class ::CRGBColour GetMultiplayerColour(long) const`; the class is defined below\n')
assert t.count('\n')==n0; open(f,'w',encoding='utf-8').write(t); print('ok')
