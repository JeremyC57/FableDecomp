# r1fix.py: NF299 in-place respells of 0x00644424 (GetMultiplayerColour 0x00449B60 + GetPlayer 0x004498C0 onto the CPlayerManager.h
# members), 0x00B92070 (ClipBoundingBox 0x00B4CED0 into the local CShadowClipVolume) and 0x009F35D0 (GetWidth 0x00415BD0 into the
# local CSurface). Every edit keeps the line count and the line of every later statement.
p='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF299/rows/'
def rep(t,o,n,c=1):
    assert t.count(o)==c,(o,t.count(o)); return t.replace(o,n)
# --- 0x00644424
f=p+'00644424.cpp'; t=open(f,encoding='utf-8').read(); n0=t.count('\n')
t=rep(t,'// Calls4498C0 GetPlayer(long),44A2A0 GetSpiritScoreText(long,CWideString&)const\n// (PDB donor445C20, wrong native propagated DrawGetEnvironmentInfo name),\n',
        '// Calls4498C0 GetPlayer(long),44A2A0 GetSpiritScoreText(long,CWideString&)const (PDB donor445C20, wrong native propagated DrawGetEnvironmentInfo name),\n')
t=rep(t,'#pragma optimize("s",on)\n','#pragma optimize("s",on)\n#define FABLE_RETAIL_CPLAYERMANAGER_MULTIPLAYERCOLOUR   // NF299: GetMultiplayerColour 0x00449B60 from CPlayerManager.h (Ego_r + FableWin QBE)\n')
t=rep(t,'struct PlayerManagerMethods:CPlayerManager {CPlayer&GetPlayer(long);bool GetSpiritScoreText(long,CWideString&)const;CRGBColour GetMultiplayerColour(long)const;};',
        'struct PlayerManagerMethods:CPlayerManager {bool GetSpiritScoreText(long,CWideString&)const;};   // GetPlayer 0x004498C0 / GetMultiplayerColour 0x00449B60: the CPlayerManager.h members (NF299)')
assert t.count('\n')==n0; open(f,'w',encoding='utf-8').write(t)
# --- 0x00B92070
f=p+'00b92070.cpp'; t=open(f,encoding='utf-8').read(); n0=t.count('\n')
t=rep(t,'class CShadowClipVolume { public:\n','class C3DBoundingBox; class CShadowClipVolume { public: bool ClipBoundingBox(const C3DBoundingBox& box, bool& inside) const;   // 0x00B4CED0\n')
t=rep(t,'struct CShadowClipVolumeMethods : CShadowClipVolume {\n    bool ClipBoundingBox(const C3DBoundingBox& box, bool& inside) const;   // 0x00B4CED0\n};\n',
        '// NF299: ClipBoundingBox 0x00B4CED0 is declared in CShadowClipVolume above (Ego_r + FableWin\n// ?ClipBoundingBox@CShadowClipVolume@@QBE_NABVC3DBoundingBox@@AA_N@Z); it was called through a\n// CShadowClipVolumeMethods stand-in.\n')
t=rep(t,'static_cast<const CShadowClipVolumeMethods&>(volume).ClipBoundingBox(bounds, inside)','volume.ClipBoundingBox(bounds, inside)')
assert 'CShadowClipVolumeMethods' not in t.replace('// CShadowClipVolumeMethods stand-in.','')
assert t.count('\n')==n0; open(f,'w',encoding='utf-8').write(t)
# --- 0x009F35D0
f=p+'009f35d0.cpp'; t=open(f,encoding='utf-8').read(); n0=t.count('\n')
t=rep(t,'    unsigned long GetHeight() const;   // 0x00415BF0\n','    unsigned long GetWidth() const; unsigned long GetHeight() const;   // 0x00415BD0 / 0x00415BF0\n')
t=rep(t,'    unsigned long GetWidth() const;                // 0x00415BD0\n','    // GetWidth 0x00415BD0 is the CSurface member above (NF299; it was declared on this stand-in)\n')
assert t.count('\n')==n0; open(f,'w',encoding='utf-8').write(t)
print('ok')
