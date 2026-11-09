#!/usr/bin/env python3
"""genfx.py: one-lever source variants of s8/a3.cpp around the 8 fixed-register (esi/edi) sites -> s8/fx/<tag>.cpp."""
import sys
from pathlib import Path
A = Path(__file__).resolve().parents[2]
base = (A / 's8/a3.cpp').read_text(encoding='utf-8')
V = {}
def var(tag, pairs):
    s = base
    for old, new in pairs:
        assert s.count(old) == 1, (tag, old[:60], s.count(old))
        s = s.replace(old, new)
    V[tag] = s
SH_OLD = """                            if (weaponState == "MELEE" || weaponState == "RANGED")
                                weaponState = "NONE";
                            else
                                mode = "NONE";"""
var('sh_elseif', [(SH_OLD, """                            if (weaponState == "MELEE")
                                weaponState = "NONE";
                            else if (weaponState == "RANGED")
                                weaponState = "NONE";
                            else
                                mode = "NONE";""")])
var('sh_neg', [(SH_OLD, """                            if (!(weaponState == "MELEE") && !(weaponState == "RANGED"))
                                mode = "NONE";
                            else
                                weaponState = "NONE";""")])
var('sh_scope', [(SH_OLD, """                            {
                                if (weaponState == "MELEE" || weaponState == "RANGED")
                                    weaponState = "NONE";
                                else
                                    mode = "NONE";
                            }""")])
MV_OLD = """                            EScriptEntityMoveType moveType;
                            if (params[3] == "SNEAK")
                                moveType = (EScriptEntityMoveType)2;
                            else if (params[3] == "RUN")
                                moveType = (EScriptEntityMoveType)1;
                            else
                                moveType = (EScriptEntityMoveType)0;"""
var('mv_tern', [(MV_OLD, """                            EScriptEntityMoveType moveType = (params[3] == "SNEAK") ? (EScriptEntityMoveType)2
                                : (params[3] == "RUN") ? (EScriptEntityMoveType)1 : (EScriptEntityMoveType)0;""")])
var('mv_init0', [(MV_OLD, """                            EScriptEntityMoveType moveType = (EScriptEntityMoveType)0;
                            if (params[3] == "SNEAK")
                                moveType = (EScriptEntityMoveType)2;
                            else if (params[3] == "RUN")
                                moveType = (EScriptEntityMoveType)1;""")])
var('mv_ifs', [(MV_OLD, """                            EScriptEntityMoveType moveType = (EScriptEntityMoveType)0;
                            if (params[3] == "SNEAK")
                                moveType = (EScriptEntityMoveType)2;
                            if (params[3] == "RUN")
                                moveType = (EScriptEntityMoveType)1;""")])
END_OLD = """        if (weaponState == "MELEE") {
            CScriptThing hero = pDSTGame->GetHero();
            pDSTGame->EntityUnsheatheMeleeWeapon(hero, false);
        }
        if (weaponState == "RANGED") {"""
var('end_elseif', [(END_OLD, END_OLD.replace('        if (weaponState == "RANGED") {', '        else if (weaponState == "RANGED") {').replace('        }\n        else if', '        } else if'))])
LK_OLD = """                            CScriptThing lookAtThing = (CCharString("HERO") == params[2]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[2]);
                            lookAt = lookAtThing.GetPos();"""
assert base.count(LK_OLD) == 1
var('lk_noname', [(LK_OLD, """                            lookAt = ((CCharString("HERO") == params[2]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[2])).GetPos();""")])
LK_THEN = """                        if (target && target->IsValid()) {
                            lookAt = target->GetScriptThing().GetPos();
                        } else {
                            CScriptThing lookAtThing"""
var('lk_late', [("""                        C3DVector lookAt;
                        if (target && target->IsValid()) {""", """                        C3DVector lookAt;
                        if (target != 0 && target->IsValid()) {""")])
tags = sys.argv[1:] or list(V)
for t in tags:
    (A / f's8/fx/{t}.cpp').write_text(V[t], encoding='utf-8')
    print(t)
