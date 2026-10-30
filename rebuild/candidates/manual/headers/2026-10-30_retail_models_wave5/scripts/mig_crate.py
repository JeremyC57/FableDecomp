#!/usr/bin/env python3
"""Wave 1 Part A, the CCrate1/CCrate2 Main shape (0x00EF54A0 / 0x00EF5670): the row-local CScriptThing / NScript predicates /
CGameScriptInterfaceBase / CEntityScriptBase become the FABLE_RETAIL_NSCRIPT headers; the row keeps its CCountedPointer
model and gives the inline bodies retail expands (plain inline ~CScriptThing, inline CIsThingAlive ctor, inline
~CPredicate_And) out of class. usage: mig_crate.py <file>..."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block
def cut(t, cls, repl=''):
    b = find_block(t, cls); assert b, cls
    return t[:b[0]] + repl + t[b[1]:]
NS = ('#define FABLE_RETAIL_NSCRIPT\n'
      '#include "engine/CScriptThing.h"                       // vtable 0x01238C8C (79 slots), copy 0x004ABE90, ~CScriptThing 0x004AA840\n'
      'inline CScriptThing::~CScriptThing() { }                // plain inline: expanded at the first exit, 0x004AA840 at the later ones\n')
for p in sys.argv[1:]:
    t = open(p).read()
    if 'FABLE_RETAIL_NSCRIPT' in t: print('skip', p); continue
    t = cut(t, 'CScriptThing', NS)
    b = t.index('namespace NScript {\nclass CPredicateBase {'); e = t.index('return CPredicate_And<P1, P2>(p1, p2);')
    e = t.index('}\n', e) + 2; e = t.index('}\n', e) + 2
    t = t[:b] + ('#include "engine/CIsThingAlive.h"                      // vtable 0x012C32F0, ~CIsThingAlive 0x00E241B0\n'
                 '#include "engine/CPredicate_And.h"                     // <CIsThingAlive, CIsThingAlive> vtable 0x012E1B70, ctor 0x00E24760\n'
                 'inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }\n'
                 'template <class P1, class P2> inline NScript::CPredicate_And<P1, P2>::~CPredicate_And() { }\n') + t[e:]
    t = cut(t, 'CGameScriptInterfaceBase', '#include "engine/CGameScriptInterfaceBase.h"           // vtable slots: CGameScriptInterface 0x01260F0C\n')
    t = t.replace('class CScriptBase;\nclass CPersistContext;\n', '')
    t = cut(t, 'CEntityScriptBase', '#include "engine/CEntityScriptBase.h"                  // vtable 0x012C3224; SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30\n')
    open(p, 'w').write(t); print('edit', p)
