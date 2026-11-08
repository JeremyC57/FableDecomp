#!/usr/bin/env python3
"""sites.py <calls_out.txt>: one line per non-register call hunk (session 4 inline-site tracker)."""
import sys, re
t = open(sys.argv[1]).read()
SHORT = [('GetLength', 'GetLength'), ('??8_Rb_tree_base_iterator', '=='), ('??9_Rb_tree_base_iterator', '!='),
         ('PlayCutsceneSound', 'PlaySound'), ('??1CScriptThing', '~CScriptThing'), ('??1?$vector@VCScriptThing', '~vec<CScriptThing>'),
         ('??1CScriptGameResourceObjectScriptedThingBase', '~SGROSTB'), ('??1CCharString', '~CCharString'),
         ('?CSExtractParameterList', 'CSExtract'), ('?clear@?$vector@VCCharString', 'vec<CCharString>::clear'),
         ('?find@?$map@VCCharString@@_N', 'map<CS,bool>::find'), ('?find@?$set@VCCharString', 'set<CS>::find'),
         ('??A?$vector@VCCharString', 'vec<CS>[]'), ('??0?$vector', 'vector ctor')]
KNOWN = {'0x40f110': 'vec<CS>[] (retail calls)', '0x414de0': 'vec<CS> ctor', '0xcd2d28': 'vec<Bypass> ctor', '0x8ac5a0': 'vec<CScriptThing> ctor',
         '0x41c820': '== (retail calls)', '0x7adb30': 'map<CS,bool>::find', '0x4baa30': 'set<CS>::find', '0x485fe8': 'vec<CS>::clear', '0xcbfaca': 'CSExtract',
         '0x4aa840': '~CScriptThing', '0x403a00': 'GetLength'}
def sh(s):
    s = s.strip()
    for k, v in SHORT:
        if k in s: return v
    return KNOWN.get(s, s[:40])
for h in t.split('\n@@')[1:]:
    lines = h.split('\n')
    m = re.match(r' (\w+) retail\[\d+:\d+\] ann (\S+) (0x[0-9a-f]+) \| ours\[\d+:\d+\] \+(0x[0-9a-f]+)', lines[0])
    if not m: continue
    tag, ann, ra, oa = m.groups()
    minus = [re.sub(r'^\s*- [0-9a-f]+ ', '', l).split('  (ann')[0] for l in lines[1:] if re.match(r'^\s*- ', l)]
    plus = [re.sub(r'^\s*\+ [0-9a-f]+ ', '', l) for l in lines[1:] if re.match(r'^\s*\+ ', l)]
    if all('[' in x for x in minus + plus): continue
    print(f'{ra} ann{ann:>6} {tag:7} R: {", ".join(sh(x) for x in minus):40} O: {", ".join(sh(x) for x in plus)}')
