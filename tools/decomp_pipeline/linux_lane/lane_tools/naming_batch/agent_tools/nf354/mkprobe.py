#!/usr/bin/env python3
"""mkprobe.py <src> <out> : header-route probe for a CCutsceneCastMap binder (agent NF354). Removes the row-local CCutsceneCastMap /
CCutsceneArgMap / CCutsceneFlagMap models and the stand-in RunCutsceneMacro declaration, puts stlport/fable_map.h in their place under
FABLE_RETAIL_STLP_MAP (+ MAP_INDEX_DECL_ONLY, MAP_CLEAR_DECL_ONLY) with the PDB RunCutsceneMacro declaration, and retypes the uses to
std::map. Measurement only (work/probes)."""
import re, sys
t = open(sys.argv[1], encoding='utf-8').read()
def cut_struct(t, name):
    m = re.search(r'^struct %s\b[^;{]*\{' % name, t, re.M)
    if not m: 
        m2 = re.search(r'^struct %s;[^\n]*\n' % name, t, re.M)
        return (t[:m2.start()] + t[m2.end():], m2.start()) if m2 else (t, None)
    i = m.end(); d = 1
    while d:
        d += {'{': 1, '}': -1}.get(t[i], 0); i += 1
    j = t.index('\n', i) + 1
    return t[:m.start()] + t[j:], m.start()
pos = []
for n in ('CCutsceneCastMap', 'CCutsceneArgMap', 'CCutsceneFlagMap'):
    t, p = cut_struct(t, n)
    if p is not None: pos.append(p)
m = re.search(r'^void __fastcall RunCutsceneMacro\([^;]*;[^\n]*\n', t, re.M)
t = t[:m.start()] + t[m.end():]; pos.append(m.start())
ins = ('// NF354 probe: the header route.\n#define FABLE_RETAIL_STLP_MAP\n#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY\n'
       '#define FABLE_RETAIL_STLP_MAP_CLEAR_DECL_ONLY\n#include "stlport/fable_map.h"\n'
       'void __fastcall RunCutsceneMacro(const CCharString& name, std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>& cast,\n'
       '                                 std::map<CCharString, bool>* flags, std::map<CCharString, CCharString>* args, bool a5, bool a6);   // 0x00CBFB7D\n')
p = min(pos); t = t[:p] + ins + t[p:]
t = re.sub(r'\bCCutsceneCastMap\b', 'std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>', t)
t = re.sub(r'\bCCutsceneArgMap\b', 'std::map<CCharString, CCharString>', t)
t = re.sub(r'\bCCutsceneFlagMap\b', 'std::map<CCharString, bool>', t)
t = re.sub(r'^int __cdecl rand\(\);', 'extern "C" int __cdecl rand();', t, flags=re.M)
open(sys.argv[2], 'w', encoding='utf-8').write(t)
