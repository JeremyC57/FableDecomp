# parfam.py <addr>...: respell the CStringParser / CPersistContext declarations of landed rows to the Ego_r / FableWin spellings
import sys, re, glob, os, shutil
S = os.environ["S"]; O = S + "/wave34/orig/"
for a in sys.argv[1:]:
    p = glob.glob(f"rebuild/src/compiled/{a[:2]}/{a[2:4]}/*_{a}.cpp")[0]; s0 = s = open(p, encoding="utf-8").read()
    s = re.sub(r"CPersistContext\(CStringParser\* (\w+), void \(\*(\w+)\)\(CCharString\*\), bool (\w+),(\s*\n\s*)CDefinitionManager\* (\w+), CSymbolMap\* (\w+), CCharString\* (\w+)\)",
               r"CPersistContext(CStringParser& \1, void (__fastcall* \2)(const CCharString&), bool \3,\4const CDefinitionManager* \5, const CSymbolMap* \6, CCharString* \7)", s)
    s = re.sub(r"CPersistContext (\w+)\(&(\w+), ", r"CPersistContext \1(\2, ", s)
    s = re.sub(r"typedef void \((?:__cdecl)?\s*\*\s*(\w+)\)\((?:CCharString\*(?: \w+)?|const char\*)\);", r"typedef void (__fastcall* \1)(const CCharString&);", s)
    s = s.replace("void Init(char* text", "void Init(const char* text").replace("CStringParser(char* text", "CStringParser(const char* text")
    s = s.replace("void Init(CCharString& data, CDefinitionManager* defs);", "void Init(CCharString& data, const CDefinitionManager* defs);")
    s = re.sub(r"void Init\(CStringParser\* parser, (\w+) callback, bool loading, CDefinitionManager\* defs,(\s*\n\s*)CSymbolMap\* symbols = 0, CCharString\* name = 0\);",
               r"void Init(CStringParser& parser, \1 callback, bool loading, const CDefinitionManager* defs,\2const CSymbolMap* symbols = 0, CCharString* name = 0);", s)
    s = s.replace("context.Init(&parser, ", "context.Init(parser, ")
    if s != s0:
        if not os.path.exists(O + p): os.makedirs(os.path.dirname(O + p), exist_ok=True); shutil.copy(p, O + p)
        open(p, "w", encoding="utf-8").write(s); print("edited", a)
    else: print("UNCHANGED", a)
