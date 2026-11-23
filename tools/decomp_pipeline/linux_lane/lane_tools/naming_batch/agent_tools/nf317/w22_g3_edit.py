import sys, shutil, os, re
sys.path.insert(0, 'tools'); import nf
E = {e[0]: e for e in nf.entries()}
def apply(path_in, path_out, reps, leaf=None):
    t = open(path_in, encoding='utf-8').read()
    for o, n in reps:
        if t.count(o) != 1: print('FAIL', path_out, t.count(o), repr(o[:70])); return
        t = t.replace(o, n)
    if leaf:
        if re.search(r'^// Leaf:.*$', t, re.M): t = re.sub(r'^// Leaf:.*$', '// Leaf: ' + leaf, t, count=1, flags=re.M)
        else: t = '// Leaf: ' + leaf + '\n' + t
    open(path_out, 'w', encoding='utf-8').write(t); print('OK', path_out.split('/')[-1])
def row(a, reps, test_reps):
    s, t = str(nf.src_of(a)), str(nf.test_of(a))
    shutil.copy(s, 'orig/%s.cpp' % a)
    if reps is not None: apply(s, 'rows/%s.cpp' % a, reps, E[a][3] if E[a][2] != 'binders1' else None)
    else: shutil.copy('work/att/%s.cpp' % a, 'rows/%s.cpp' % a); print('ATT', a)
    if test_reps:
        shutil.copy(t, 'orig/%s_test.cpp' % a); apply(t, 'rows/%s_test.cpp' % a, test_reps)
# 23: IBE (protected const)
row('00687c90', None, [("    bool IsOtherQuickAccessItemButtonHeld(long param);\n",
   "protected:\n    bool IsOtherQuickAccessItemButtonHeld(long param) const;   // the row's PDB IBE spelling\n    friend int main();\n")])
# 38: QBE
row('00662860', None, [("    long GetDefaultWeaponDef();\n", "    long GetDefaultWeaponDef() const;   // the row's PDB QBE spelling\n")])
# 66: virtual, vptr in the pad
row('006f56e0', None, [("    char pad[0x11c];\n    C3DVector hitPos;\n\n    void SetHitPosition(C3DVector pos);\n",
   "    char pad[0x118];                    // +0x04 (the vptr takes +0x00)\n    C3DVector hitPos;\n\n    virtual void SetHitPosition(C3DVector pos);   // the row's PDB UAE spelling\n")])
# 99
row('00bc64d0', None, [("    char pad_0[0x50];\n", "    char pad_0[0x4c];                   // +0x04 (the vptr takes +0x00)\n"),
   ("    void Update(const CEnginePrimitive& prim, unsigned long a3);\n", "    virtual void Update(const CEnginePrimitive& prim, unsigned long a3);   // the row's PDB UAE spelling\n")])
# 85: virtual const, vptr in m_Pad0000 (row and test)
r85 = [("// CGameScriptInterface: +0x00 vftable slot modelled as raw padding, +0x04 CWorld*.\nclass CGameScriptInterface\n{ public:\n    unsigned char                    m_Pad0000[0x04];\n    CGSIAddLineToConversationWorld*  m_pWorld;            // +0x04\n\n    void AddLineToConversation(",
        "// CGameScriptInterface: +0x00 the vptr (AddLineToConversation is virtual: rtti-vtables CGameScriptInterface slot 366, PDB UBE), +0x04 CWorld*.\nclass CGameScriptInterface\n{ public:\n    CGSIAddLineToConversationWorld*  m_pWorld;            // +0x04\n\n    virtual void AddLineToConversation(")]
row('00890750', r85, [("class CGameScriptInterface\n{ public:\n    unsigned char                    m_Pad0000[0x04];\n    CGSIAddLineToConversationWorld*  m_pWorld;\n\n    void AddLineToConversation(",
        "class CGameScriptInterface\n{ public:\n    CGSIAddLineToConversationWorld*  m_pWorld;            // +0x04 (the vptr takes +0x00)\n\n    virtual void AddLineToConversation(")])
# 109 binder: the caller's CQuestManager model takes the PDB const; the test's stub follows
row('00892f60', None, [("    bool IsQuestCompleted(const CCharString& questName);\n", "    bool IsQuestCompleted(const CCharString& questName) const;\n"),
   ("bool CQuestManager::IsQuestCompleted(const CCharString& questName)\n", "bool CQuestManager::IsQuestCompleted(const CCharString& questName) const\n")])
