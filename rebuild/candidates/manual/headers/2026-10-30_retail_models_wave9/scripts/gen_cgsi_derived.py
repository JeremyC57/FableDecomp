"""Idempotently add the FABLE_RETAIL_NSCRIPT branch to engine/CGameScriptInterface.h: class CGameScriptInterface :
public CGameScriptInterfaceBase with the 916 overrides (declarations copied from CGameScriptInterfaceBase.h minus `= 0`),
slot 917 SetTrapAsActive, and the retail members +0x04..+0x4c. The flattened generated model stays the #else branch."""
import re, sys, os
root = os.environ.get("FABLE_ROOT", ".")
inc = os.path.join(root, "rebuild/include/engine")
base = open(os.path.join(inc, "CGameScriptInterfaceBase.h"), encoding="utf-8").read()
path = os.path.join(inc, "CGameScriptInterface.h")
cur = open(path, encoding="utf-8").read()
MARK = "// Retail opt-in (FABLE_RETAIL_NSCRIPT)"
if MARK in cur:
    print("already has retail branch"); sys.exit(0)
body = base[base.index("class CGameScriptInterfaceBase"):]
body = body[body.index("{")+1: body.index("\n};")]
out = []
for ln in body.split("\n"):
    s = ln.strip()
    if not s: continue
    if s.startswith("virtual ~CGameScriptInterfaceBase"): continue
    if s in ("public:", "protected:", "private:"):
        out.append(s); continue
    if not re.match(r"(protected: )?virtual ", s): continue
    m = re.match(r"((?:protected: )?virtual .*?) = 0;(\s*public:)?\s*(//.*)?$", s)
    if not m: raise SystemExit("unparsed: " + s)
    decl = m.group(1) + ";" + (" public:" if m.group(2) else "")
    out.append("    " + decl.ljust(100) + " " + (m.group(3) or ""))
decls = "\n".join(out)
branch = '''%s: retail model (hand-written 2026-10-29, wave 1 class-model migration,
// scripts/gen_cgsi_derived.py). RTTI .?AVCGameScriptInterface@@ : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass
// (ghidra_out/rtti_bases.tsv); vtable 0x01260F0C = 918 slots: 0 vector deleting dtor 0x006E7D50, 1..916 the overrides of the
// base's pure slots (same order and names, ghidra_out/cgsi_slots_aligned.tsv), 917 SetTrapAsActive 0x00895130 (FableWin.pdb
// ?SetTrapAsActive@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z). Ctor 0x006E7740 (Ego_r.pdb mangled ctor bound in
// rebuild/oracles/reloc_bindings.tsv); the ~CGameScriptInterface body address is not proven (only the deleting dtor is).
// Size 0x50: members are the generated #else model's (Ego_r names, retail STLport sizes); HeroSThing / ScriptTimers stay
// opaque (CScriptThing needs the includer's CCountedPointer; std::map<long,long> is STLport 0xc). Slot 308's nested-enum
// stand-in is inherited from CGameScriptInterfaceBase.h. Non-virtual members of the PDB class are not declared here yet.
#ifdef FABLE_RETAIL_NSCRIPT
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CGameScriptInterfaceBase.h"
class CDisplayEngine;
class CGameDefinitionManager;
class CMainGameComponent;
class CPlayerManager;
class CThingSearchTools;
class CWorld;

class CGameScriptInterface : public CGameScriptInterfaceBase {
public:
    CGameScriptInterface(CMainGameComponent&, const CGameDefinitionManager&, CWorld&, CPlayerManager&, CDisplayEngine&); // 0x006E7740
    virtual ~CGameScriptInterface();                                     // slot 0  0x006E7D50 (vector deleting)
%s
public:
    virtual void SetTrapAsActive(const CScriptThing&, bool) const;                                       // 917 0x00895130
    CWorld*                 World;                           // +0x04
    CMainGameComponent*     Component;                       // +0x08
    CDisplayEngine*         DisplayEngine;                   // +0x0c
    CGameDefinitionManager* DefinitionManager;               // +0x10
    CPlayerManager*         PlayerManager;                   // +0x14
    CThingSearchTools*      ThingSearchTools;                // +0x18
    long                    CurrentPlayer;                   // +0x1c
    long                    CurrentLevelID;                  // +0x20
    long                    CurrentScriptLevelID;            // +0x24
    long                    CurrentScriptID;                 // +0x28
    bool                    InMovieSequence;                 // +0x2c
    bool                    AllowScreenFadingIfAlreadyFaded; // +0x2d
    unsigned char           _pad_0x2e[0x2];                  // +0x2e
    unsigned char           HeroSThing[0xc];                 // +0x30 CScriptThing
    unsigned char           ScriptTimers[0xc];               // +0x3c std::map<long,long> (STLport)
    long                    CameraResetToViewBehindHeroCount; // +0x48
    long                    CreateCreatureDelayFrames;       // +0x4c
};
FABLE_STATIC_ASSERT(sizeof(CGameScriptInterface) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CGameScriptInterface, World) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CGameScriptInterface, HeroSThing) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CGameScriptInterface, CreateCreatureDelayFrames) == 0x4c);
#else
''' % (MARK, decls)
i = cur.index("#include <stddef.h>")
new = cur[:i] + branch + cur[i:]
j = new.rindex("#endif // FABLE_ENGINE_CGAMESCRIPTINTERFACE_H")
new = new[:j] + "#endif // FABLE_RETAIL_NSCRIPT\n" + new[j:]
open(path, "w", encoding="utf-8", newline="\n").write(new)
print("written", len(out), "decl lines")
