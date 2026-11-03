import sys,os
W=sys.argv[1]
FR="VC7.1's per-site frequency rule (docs/pipeline/INLINE_DEPTH_REVISIT.md)"
R={
'00cffb60':[(12,"//   by "+FR+" the second forcefield statement's temporaries and every later exit stay out of line (plain inline).")],
'00d1cac0':[(2,"// Levers: plain-inline ~vector<CScriptThing> over an STLport-style by-value Destroy(first, last) helper; every terminate check is `do { if (Term) return; } while (0);` (one statement-macro shape), so "+FR+" leaves the closing-brace exit out of line and the in-loop return inline")],
'00d2c810':[(13,"//   in the second StartScriptingEntity retry loop (every other exit calls it, by "+FR+"); the closing")],
'00d58080':[(15,"//   plain-inline CCountedPointer / base destructors; it expands only at the first wait loop's return, every later exit (and the"),
            (16,"//   function end) calls 0x007E74D0 by "+FR+".")],
'00da0820':[(12,"//   plain-inline destructor: "+FR+" picks the return sites that call the out-of-line vector destructor.")],
'00da8970':[(9,"// Shape: the RestartMarker vector model (plain-inline destructor, DestroyRange + free); "+FR+" decides the")],
'00da9c70':[(23,"//   and the hero handle's ctor stays out of line by "+FR+".")],
'00dbe4e0':[(14,"//   script idiom; ~CScriptThing is plain inline and "+FR+" expands it only before the first wait, so only")],
'00e06050':[(3,"//   the value alone is 991v1007; the plain-inline destructor's expanded / called exits follow "+FR+".")],
'00e1fcf0':[(1,"// Levers: for(;;) with the terminating return first, then `if (alive) { ... } else { if (term) return; break; }` (the dead branch last, so its exits are laid out after the hit-block call); every terminate check is `do { if (term) return; } while (0);` with a plain-inline ~CScriptThing, so "+FR+" leaves the hit-block exit out of line; the CCharString == is the nested CBasicString<char>::operator== (result byte homed in the frame) with `strcmp(text, \"\")` in the null arm; IsAlive is the `Data ? Data->IsAlive() : false` ternary (test al,al).")],
'00e1fef0':[(1,"// Levers: twin of 0x00E1FCF0 (EXPRESSION_WAIT, ExpressionDetectedWait): for(;;) with the terminating return first, alive/else-dead branches, every terminate check `do { if (term) return; } while (0);` with a plain-inline ~CScriptThing, nested CBasicString<char>::operator==, ternary IsAlive.")],
'00e2e520':[(13,"// Shape: generated script code; `for (;;)` with the terminate return first; ~CScriptThing is plain inline and the two"),
            (14,"//   terminate checks are `do { if (...) return; } while (0);`, so "+FR+" expands exactly retail's two exit copies. Compare has")],
'00e4e0e0':[(8,"//   `dist(Me, hero) > dist(blockWay, hero)`; plain-inline ~CScriptThing, the early returns call it by "+FR+".")],
'00e5f8f0':[(12,"//   (plain-inline dtor, CBaseClass ctor/dtor out of line).")],
'00e6a0b0':[(12,"//   the out-of-line destructor (terminate checks as `do { if (...) return; } while (0);`; "+FR+")."),
            (14,"//   tail-merges the two inline destructor exits the other way round. ~CScriptThing is plain inline.")],
'00e6ea30':[(17,"// Shape: the CMeleeThunder handle model (implicit handle dtor expanded only at the first return, by "+FR+").")],
'00e71980':[(16,"// Shape: the CMeleeThunder handle model (implicit handle dtor expanded only at the first return, by "+FR+").")],
'00e9cb20':[(10,"// Shape: the landed CV_ChapelOfEvilScript::WatchForRegionEntry 0x00E6B750 (string ==); plain-inline dtors; the leave"),
            (11,"//   branch's exits call the two locals' dtors out of line, by "+FR+".")],
'00ec1ef0':[(18,"// CHTG_FreedomFighter::Main 0x00D701C0 walk loop; the village ~CScriptThing exits call it out of line by "+FR+".")],
'00ed7320':[(13,"//   plain-inline ~CScriptThing, every terminate check `do { if (...) return; } while (0);`); handle model from")],
'00ed9790':[(1,"// Levers: plain-inline ~CScriptThing; after the predicate + yield prologue a for(;;) with the terminating return first; the prologue and loop-top terminate checks are `do { if (...) return; } while (0);`, the two other in-loop terminate returns are plain `if`, so by "+FR+" only the prologue return and the loop-top return expand the dtor.")],
'00edbd80':[(12,"// Shape: the landed CV_ChapelOfEvilScript::WatchForRegionEntry 0x00E6B750 (string ==); terminate checks `do { if (...) return; } while (0);`.")],
'00ef5cb0':[(12,"// Shape: plain-inline ~CScriptThing; `for (;;) { if (Term()) return; ... }` outer loop (a `while` swaps the two inline"),
            (14,"//   terminate checks as `do { if (...) return; } while (0);`.")],
'00ef84d0':[(1,"// Levers: inline CScriptThing model (CBaseClass + CCountedPointer, plain-inline dtor); the CAnd filter comes from an inline by-value `And(a, b)` factory (the argument address is re-taken, `lea eax,[esp+48h]`); "+FR+" keeps every ~CScriptThing after the scripted-object declaration out of line (merged with the scripted-object dtor tail), leaving the single inline expansion at the early terminate return.")],
'00f061e0':[(2,"// Levers: plain-inline ~CScriptThing; CCountedPointer dtor zero order `Data = 0; Info = 0;`; for(;;) with every terminating return written `do { if (...) return; } while (0);`; by "+FR+" the third (after the health test) is the one exit retail calls 0x004AA840 at")],
'00f07ed0':[(9,"// Shape: the \"cash poor\" health test destroys its CScriptThing temporary out of line while the \"no hire\" one inlines the"),
            (10,"// plain-inline destructor ("+FR+"); the question's last literal reuses the bandit argument's slot.")],
}
for a,eds in R.items():
    p=os.path.join(W,a+'.cpp'); L=open(p).read().split('\n')
    for n,new in eds:
        assert L[n-1].lstrip().startswith('//'),(a,n,L[n-1])
        L[n-1]=new
    open(p,'w').write('\n'.join(L))
NOTE="// Inline-split: exits wrapped individually; retail's construct is unproven"
for a in ('00e2e520','00e9cb20','00ed9790'):
    p=os.path.join(W,a+'.cpp'); L=open(p).read().split('\n'); L.insert(1,NOTE); open(p,'w').write('\n'.join(L))
