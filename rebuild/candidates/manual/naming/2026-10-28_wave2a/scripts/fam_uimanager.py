#!/usr/bin/env python3
"""Respell the UI manager singleton getter 0x0041E5F2 to its PDB form
`?GetInstance@CManager@NUISystem@@SIPAV12@XZ` (static NUISystem::CManager* __fastcall
CManager::GetInstance(void); Ego_r.pdb + FableWin.pdb; its ctor stores the CManager vtable, the
CFrontEndManager getter is the 0xE0-byte twin at 0x00595582 -- wave-1 CONFLICTS.md).

Shape handled: a `class CManager` model inside `namespace NUISystem` declaring
`static CManager* GetInstance();` (cdecl spelling SA) -> `static CManager* __fastcall GetInstance();`
(no arguments: the call is the same bytes). Models of other classes (CFrontEndManager, *_Calls,
...) are not handled here (their other members would move with a class rename).
Idempotent.  usage: fam_uimanager.py [--check] <file>...
"""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import run
from fam_getter_common import make_transform

CFGS = [dict(cls='CManager', ns=['NUISystem'], names=['GetInstance'], new='GetInstance', ret='CManager*')]
transform = make_transform(CFGS)

if __name__ == '__main__':
    run(transform, sys.argv[1:])
