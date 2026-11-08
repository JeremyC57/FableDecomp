#!/usr/bin/env python3
"""gen4.py <out.cpp> <device> [<device> ...]: apply session-4 inline devices to src/00cbfb7d.cpp (base) -> out."""
import sys
A = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent885'
base = open(f'{A}/probe/s4/base.cpp').read()
D0 = '#pragma inline_depth(0)'
D = '#pragma inline_depth()'
DEV = {}
def dev(name, old, new):
    DEV.setdefault(name, []).append((old, new))
# --- retail CALLS, ours expands -------------------------------------------------------------------------------------
dev('ctor_values',
    '                        std::vector<CCharString> values;\n',
    f'{D0}   // 0x00CD0DA4: retail calls this vector ctor (0x00414DE0) out of line\n                        std::vector<CCharString> values;\n{D}\n')
dev('ctor_bypass',
    '                        std::vector<CScreenFilterSThingByPass> bypass;\n',
    f'{D0}   // 0x00CD0E20: retail calls this vector ctor (0x00CD2D28) out of line\n                        std::vector<CScreenFilterSThingByPass> bypass;\n{D}\n')
dev('ctor_things',
    '                                    std::vector<CScriptThing> things;\n                                    if (value.StartsWith("ALL:"))',
    f'{D0}   // 0x00CD0F3B: retail calls this vector ctor (0x008AC5A0) out of line\n                                    std::vector<CScriptThing> things;\n{D}\n                                    if (value.StartsWith("ALL:"))')
dev('idx_lights',
    '                                CCharString lightLine(lights[i]);\n',
    f'{D0}   // 0x00CD147E: retail calls vector<CCharString>::operator[] (0x0040F110) out of line\n                                CCharString lightLine(lights[i]);\n{D}\n')
dev('idx_lights',
    '                                    CCharString lightLine(lights[index]);\n',
    f'{D0}   // 0x00CD1621: operator[] out of line\n                                    CCharString lightLine(lights[index]);\n{D}\n')
dev('idx_scene',
    """                            for (long j = 0; j < scenes[scene].GetLength(); ++j) {
                                if (((const char*)scenes[scene])[j] == ',') {
""",
    f"""{D0}   // 0x00CD15D5..0x00CD17B0: retail calls vector<CCharString>::operator[] (0x0040F110) out of line in the scene loop
                            for (long j = 0; j < scenes[scene].GetLength(); ++j) {{
                                if (((const char*)scenes[scene])[j] == ',') {{
{D}
""")
dev('idx_scene',
    """                                    value = "";
                                } else if (((const char*)scenes[scene])[j] != ' ') {
                                    value.Append(((const char*)scenes[scene])[j], value.GetLength());
                                }
                            }
""",
    f"""                                    value = "";
{D0}
                                }} else if (((const char*)scenes[scene])[j] != ' ') {{
                                    value.Append(((const char*)scenes[scene])[j], value.GetLength());
                                }}
                            }}
{D}
""")
dev('eq_gold',
    """                            if (it == givenObjects.end())
                                givenObjects["Gold"] = gold;""",
    f"""                            std::map<CCharString, long>::iterator last = givenObjects.end();
{D0}   // 0x00CC8407: retail calls the iterator == (0x0041C820) out of line here
                            if (it == last)
{D}
                                givenObjects["Gold"] = gold;""")

# --- retail EXPANDS, ours calls (row-local __forceinline wrappers; nested call variant) ------------------------------
HELP_ANCHOR = '// One cutscene frame: run a script frame when enabled'
dev('fi_len_cue', HELP_ANCHOR,
    """// Retail expands CCharString::GetLength / StartsWith's prefix length / the iterator == at a few sites past VC7.1's
// caller-size inline cap (the rest stay out-of-line calls): these wrappers force the expansion there.
#ifdef _MSC_VER
#define CUTSCENE_FORCEINLINE __forceinline
#else
#define CUTSCENE_FORCEINLINE inline
#endif
static CUTSCENE_FORCEINLINE long ExpandedLength(const CCharString& text) { return text.GetLength(); }
static CUTSCENE_FORCEINLINE bool StartsWithExpanded(const CCharString& text, const CCharString& prefix)
{
    return strncmp(text.GetAsCharArray(), prefix.GetAsCharArray(), ExpandedLength(prefix)) == 0;
}
template <class Iterator> static CUTSCENE_FORCEINLINE bool ExpandedEquals(const Iterator& a, const Iterator& b) { return a == b; }

""" + HELP_ANCHOR)
dev('fi_len_cue', '                        if (cue.GetLength() != 0)\n', '                        if (ExpandedLength(cue) != 0)   // 0x00CC0040: expanded\n')
dev('fi_len_dollar', 'if (params[k].GetLength() != 0 && params[k].StartsWith("$")) {', 'if (params[k].GetLength() != 0 && StartsWithExpanded(params[k], "$")) {   // 0x00CC03EA: prefix length expanded')
dev('fi_eq_flags', 'if (localFlags.find(expression) == localFlags.end()) {', 'if (ExpandedEquals(localFlags.find(expression), localFlags.end())) {   // 0x00CC61B8: == expanded')
dev('fi_eq_actors', 'if (registeredActors.find(extraName) == registeredActors.end() && extra.IsAlive()) {', 'if (ExpandedEquals(registeredActors.find(extraName), registeredActors.end()) && extra.IsAlive()) {   // 0x00CC6DA9: == expanded')

# --- header twins (FABLE_RETAIL_CCHARSTRING_GETLENGTH_EXPANDED) + row-local iterator == twin ---------------------------
dev('tw_len', '#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ\n', '#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ\n#define FABLE_RETAIL_CCHARSTRING_GETLENGTH_EXPANDED\n')
dev('tw_len', '                        if (cue.GetLength() != 0)\n', '                        if (cue.GetLengthExpanded() != 0)   // 0x00CC0040: expanded by retail\n')
dev('tw_len', 'if (params[k].GetLength() != 0 && params[k].StartsWith("$")) {', 'if (params[k].GetLength() != 0 && params[k].StartsWithExpanded("$")) {   // 0x00CC03EA: prefix length expanded by retail')
dev('tw_eq', HELP_ANCHOR,
    """// Iterator equality forced in place (inline-budget device, see line 2): the body of _Rb_tree_base_iterator::operator==.
#ifdef _MSC_VER
template <class Iterator> static __forceinline bool SameNode(const Iterator& a, const Iterator& b) { return a._M_node == b._M_node; }
#else
template <class Iterator> static inline bool SameNode(const Iterator& a, const Iterator& b) { return a._M_node == b._M_node; }
#endif

""" + HELP_ANCHOR)
dev('tw_eq', 'if (localFlags.find(expression) == localFlags.end()) {', 'if (SameNode(localFlags.find(expression), localFlags.end())) {   // 0x00CC61B8: == expanded by retail')
dev('tw_eq', 'if (registeredActors.find(extraName) == registeredActors.end() && extra.IsAlive()) {', 'if (SameNode(registeredActors.find(extraName), registeredActors.end()) && extra.IsAlive()) {   // 0x00CC6DA9: == expanded by retail')

# --- vector ctors out of line with the allocator temp outside the depth-0 region (named allocator) -------------------
dev('ctor_values2',
    '                        std::vector<CCharString> values;\n',
    f'                        std::allocator<CCharString> stringAllocator;\n{D0}   // 0x00CD0DA4: retail calls this vector ctor (0x00414DE0) out of line\n                        std::vector<CCharString> values(stringAllocator);\n{D}\n')
dev('ctor_bypass2',
    '                        std::vector<CScreenFilterSThingByPass> bypass;\n',
    f'                        std::allocator<CScreenFilterSThingByPass> bypassAllocator;\n{D0}   // 0x00CD0E20: retail calls this vector ctor (0x00CD2D28) out of line\n                        std::vector<CScreenFilterSThingByPass> bypass(bypassAllocator);\n{D}\n')
dev('ctor_things2',
    '                                    std::vector<CScriptThing> things;\n                                    if (value.StartsWith("ALL:"))',
    f'                                    std::allocator<CScriptThing> thingAllocator;\n{D0}   // 0x00CD0F3B: retail calls this vector ctor (0x008AC5A0) out of line\n                                    std::vector<CScriptThing> things(thingAllocator);\n{D}\n                                    if (value.StartsWith("ALL:"))')
out = base
for name in sys.argv[2:]:
    for old, new in DEV[name]:
        n = out.count(old)
        if n != 1: sys.exit(f'{name}: anchor found {n} times: {old[:60]!r}')
        out = out.replace(old, new)
open(sys.argv[1], 'w').write(out)
print('ok', sys.argv[1], sys.argv[2:])
