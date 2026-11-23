# u1fix.py: in-place respells of 0x0049D550 (GetStaticMapDir) and 0x00AE2050 (WriteVectorArray), line count kept
p='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF305/rows/'
t=open(p+'0049d550.cpp',encoding='utf-8').read(); n=t.count('\n')
o='CWideString __fastcall GetLevelsDirectory_0041a410();       // 0x0041A410 (L"Data\\\\Levels\\\\", hidden result in ecx)'
assert t.count(o)==1,o
t=t.replace(o,'namespace NResourceDirectoryNames { CWideString __fastcall GetStaticMapDir(); }   // 0x0041A410 (L"Data\\\\Levels\\\\", hidden result in ecx; Ego_r + FableWin)')
assert t.count('GetLevelsDirectory_0041a410()')==4
t=t.replace('GetLevelsDirectory_0041a410()','NResourceDirectoryNames::GetStaticMapDir()')
assert t.count('\n')==n; open(p+'0049d550.cpp','w',encoding='utf-8').write(t)
t=open(p+'00ae2050.cpp',encoding='utf-8').read(); n=t.count('\n')
o='void __fastcall WriteVectors_00ae18c0(const C4DVector* vectors, unsigned long count, CDataOutputStream& stream);   // 0x00AE18C0'
assert t.count(o)==1
t=t.replace(o,'// WriteVectorArray 0x00AE18C0: the protected static CParticleSimulation member, declared in the class below (Ego_r + FableWin KIX)')
o='    void WriteToFile(CDataOutputStream& stream);\n};'
assert t.count(o)==1
t=t.replace(o,'    void WriteToFile(CDataOutputStream& stream);\nprotected: static void __fastcall WriteVectorArray(const C4DVector* vectors, unsigned long count, CDataOutputStream& stream);   // 0x00AE18C0\n};')
assert t.count('WriteVectors_00ae18c0(Positions, Size, stream);')==1
t=t.replace('WriteVectors_00ae18c0(Positions, Size, stream);','WriteVectorArray(Positions, Size, stream);')
open(p+'00ae2050.cpp','w',encoding='utf-8').write(t); print('ok', t.count('\n')-n)
