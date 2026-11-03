# bvar.py <base> <out> <variant> : rewrite the def-arm block of SetDefaultValuesForNewProfile
import sys
s=open(sys.argv[1]).read(); v=sys.argv[3]
a=s.index('    if (def.IsNull()) {\n        ClampVideoOption(EffectsDetailNewProfile')
b=s.index('    ClampAntialiasing(AntialiasingNewProfile')
F=[('EffectsDetail','A','A'),('TextureDetail','D','D'),('MeshDetail','C','C'),('ShadowDetail','B','B')]
def call(n,x,y,fb,mx): return f'ClampVideoOption({n}NewProfile, (float)g_Detail{x}, {fb}, (float)g_MaxDetail{y}, {mx});'
null='\n'.join('        '+call(n,x,y,'-1.0f','-1.0f') for n,x,y in F)+'\n        AntialiasingNewProfile = 0;\n'
full='\n'.join('        '+call(n,x,y,f'def->{n}',f'def->Max{n}') for n,x,y in F)+'\n        AntialiasingNewProfile = def->Antialiasing;\n'
if v=='inv':
    blk='    if (!def.IsNull()) {\n'+full+'    } else {\n'+null+'    }\n'
elif v=='ptr':
    blk='    const CConfigOptionsDefaultsDef* defaults = def.Get();\n    if (defaults == 0) {\n'+null+'    } else {\n'+full.replace('def->','defaults->')+'    }\n'
elif v=='tern':
    blk='\n'.join('    '+call(n,x,y,f'def.IsNull() ? -1.0f : def->{n}',f'def.IsNull() ? -1.0f : def->Max{n}') for n,x,y in F)+'\n    AntialiasingNewProfile = def.IsNull() ? 0 : def->Antialiasing;\n'
elif v=='ternp':
    blk='\n'.join('    '+call(n,x,y,f'!def.IsNull() ? def->{n} : -1.0f',f'!def.IsNull() ? def->Max{n} : -1.0f') for n,x,y in F)+'\n    AntialiasingNewProfile = !def.IsNull() ? def->Antialiasing : 0;\n'
elif v=='fbloc':
    # locals for the def values, set once, then one call sequence
    blk=('    float effects = -1.0f, maxEffects = -1.0f, texture = -1.0f, maxTexture = -1.0f, mesh = -1.0f, maxMesh = -1.0f, shadow = -1.0f, maxShadow = -1.0f;\n'
         '    unsigned long antialiasing = 0;\n    if (!def.IsNull()) {\n        effects = def->EffectsDetail; maxEffects = def->MaxEffectsDetail; texture = def->TextureDetail; maxTexture = def->MaxTextureDetail;\n'
         '        mesh = def->MeshDetail; maxMesh = def->MaxMeshDetail; shadow = def->ShadowDetail; maxShadow = def->MaxShadowDetail; antialiasing = def->Antialiasing;\n    }\n'
         '    '+call('EffectsDetail','A','A','effects','maxEffects')+'\n    '+call('TextureDetail','D','D','texture','maxTexture')+'\n    '+call('MeshDetail','C','C','mesh','maxMesh')+'\n    '+call('ShadowDetail','B','B','shadow','maxShadow')+'\n    AntialiasingNewProfile = antialiasing;\n')
elif v=='aafirst':
    blk='    if (def.IsNull()) {\n'+null+'    } else {\n'+full+'    }\n'
    blk=blk.replace('        AntialiasingNewProfile = 0;\n','').replace('        AntialiasingNewProfile = def->Antialiasing;\n','')+'    AntialiasingNewProfile = def.IsNull() ? 0 : def->Antialiasing;\n'
else: raise SystemExit('variant?')
open(sys.argv[2],'w').write(s[:a]+blk+s[b:])
