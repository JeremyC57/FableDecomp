# u2fix.py: the hidden binders of 0x0041A410 (0x0049D6B0 / 0x0049DDD0 / 0x0049DEC0) and 0x00AE18C0 (0x00AE2230), as u1fix.py
p='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF305/rows/'
o='CWideString __fastcall GetLevelsDirectory_0041a410();       // 0x0041A410 (L"Data\\\\Levels\\\\", hidden result in ecx)'
for a in ('0049d6b0','0049ddd0','0049dec0'):
    t=open(p+a+'.cpp',encoding='utf-8').read(); n=t.count('\n'); assert t.count(o)==1,a
    t=t.replace(o,'namespace NResourceDirectoryNames { CWideString __fastcall GetStaticMapDir(); }   // 0x0041A410 (L"Data\\\\Levels\\\\", hidden result in ecx; Ego_r + FableWin)')
    k=t.count('GetLevelsDirectory_0041a410()'); assert k>=1
    t=t.replace('GetLevelsDirectory_0041a410()','NResourceDirectoryNames::GetStaticMapDir()')
    assert 'GetLevelsDirectory_0041a410' not in '\n'.join(l.split('//')[0] for l in t.splitlines())
    assert t.count('\n')==n; open(p+a+'.cpp','w',encoding='utf-8').write(t); print(a,k)
a='00ae2230'; t=open(p+a+'.cpp',encoding='utf-8').read(); n=t.count('\n')
o='    void WriteToFile(CDataOutputStream& stream);                       // 0x00AE2050\n};'
assert t.count(o)==1
t=t.replace(o,'    void WriteToFile(CDataOutputStream& stream);                       // 0x00AE2050\n    friend class CParticleProgram;   // as engine/CParticleSimulation.h (the real class befriends it)\nprotected: static void __fastcall WriteVectorArray(const C4DVector* vectors, unsigned long count, CDataOutputStream& stream);   // 0x00AE18C0\n};')
o='void __fastcall WriteVectors_00ae18c0(const C4DVector* vectors, unsigned long count, CDataOutputStream& stream);   // 0x00AE18C0'
assert t.count(o)==1
t=t.replace(o,'// WriteVectorArray 0x00AE18C0: the protected static CParticleSimulation member, declared in the class above (Ego_r + FableWin KIX)')
assert t.count('WriteVectors_00ae18c0(NonSimPositions, NonSimCount, stream);')==1
t=t.replace('WriteVectors_00ae18c0(NonSimPositions, NonSimCount, stream);','CParticleSimulation::WriteVectorArray(NonSimPositions, NonSimCount, stream);')
open(p+a+'.cpp','w',encoding='utf-8').write(t); print(a,t.count('\n')-n)
