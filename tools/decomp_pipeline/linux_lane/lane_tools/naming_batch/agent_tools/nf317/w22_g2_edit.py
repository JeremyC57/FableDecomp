import json, re, sys
sys.path.insert(0, 'tools'); import nf, shutil, os
E = {e[0]: e for e in nf.entries()}
def slots(a, b, pre='_v'):
    xs = ['virtual void %s%02d();' % (pre, i) for i in range(a, b)]
    return '\n'.join('    ' + ' '.join(xs[k:k + 6]) for k in range(0, len(xs), 6))
def leaf(t, a):
    P = E[a][3]
    if re.search(r'^// Leaf:.*$', t, re.M): return re.sub(r'^// Leaf:.*$', '// Leaf: ' + P, t, count=1, flags=re.M)
    return '// Leaf: ' + P + '\n' + t
S = {}
S['00530a40'] = S['00530ce0'] = [(
"    void MakeUnavailable();\n    void MakeAvailable();\n",
slots(83, 92) + "\n    virtual void MakeUnavailable();          // slot 92 (+0x170): 0x00530A40, rtti-vtables NUISystem::CSprite slot 92; PDB UAE\n"
"    virtual void _v93();\n    virtual void MakeAvailable();            // slot 94 (+0x178): 0x00530CE0, rtti-vtables NUISystem::CSprite slot 94; PDB UAE\n", 1)]
S['00993bc0'] = [("public:\n    void SetPosition(unsigned long pos);\n\n    unsigned long m_unused0;   // +0x00\n",
"public:\n    virtual void _v00();                         // slot 0 (not modelled)\n    virtual void SetPosition(unsigned long pos); // slot 1: rtti-vtables CMemoryDataInputStream slot 1; PDB UAE\n\n    // +0x00: the vptr (was m_unused0)\n", 1)]
S['00994300'] = [("    void Close();\n", "    virtual void Close();             // slot 3 (vtbl+0x0C): rtti-vtables CFileDataInputStream slot 3; PDB UAE\n", 1)]
S['00996cc0'] = [("    void Write(const void* data, long size);\n",
"    virtual void v01(); virtual void v02(); virtual void v03();\n    virtual void Write(const void* data, long size);   // slot 4: rtti-vtables CMemoryDataOutputStream slot 4; PDB UAE\n", 1)]
S['00555220'] = [("    bool          ShowDownArrow;         // +0x179\n    void Update(float dt);\n",
"    bool          ShowDownArrow;         // +0x179\n    virtual void Update(float dt);       // primary vftable slot 1: rtti-vtables NUISystem::CScrollableList slot 1; PDB UAE\n", 1)]
S['00645ce9'] = [("    void DrawStatBox(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos, long graphic,\n        long value, long effect, const CWideString& text, unsigned char alpha, bool withText, bool flag);\n",
"    virtual void v01();\nprotected:\n    virtual void DrawStatBox(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos, long graphic,\n        long value, long effect, const CWideString& text, unsigned char alpha, bool withText, bool flag);   // slot 2: rtti-vtables NPlayerGui::CStatBar slot 2; PDB MAE\npublic:\n", 1)]
S['005be028'] = [("    void* __vftable;\n    CThing* ParentThing;                             // +0x04\n    void OnItemAdded(long itemDefIndex, bool unused, bool suppressTutorials);\n",
slots(0, 84) + "\n    CThing* ParentThing;                             // +0x04\nprotected:\n    virtual void OnItemAdded(long itemDefIndex, bool unused, bool suppressTutorials);   // slot 84: rtti-vtables CTCInventoryExperience slot 84; PDB MAE\npublic:\n", 1)]
S['005ea400'] = [("    void DrawBetAndMoney(const CEnginePrimitivePrepareInfo& info,", "    virtual void DrawBetAndMoney(const CEnginePrimitivePrepareInfo& info,", 1)]
S['004c9690'] = [("virtual void v08(); virtual void v09();\n", "virtual void v08();\n    virtual void InitialActivate();              // slot 9 (+0x24): rtti-vtables CThing slot 9; PDB UAE\n", 1),
                 ("    void InitialActivate();\n", "", 1)]
S['00663f50'] = [("class CThing {\npublic:\n    void* __vftable;\n", "class CThing {\npublic:\n" + slots(0, 11) + "\nprotected:\n    virtual void OnDie();                               // slot 11: ?OnDie@CThing@@MAEXXZ (both PDBs)\npublic:\n", 1),
                 ("    void OnDie();                                       // 0x006A4CF0\n", "protected:\n    virtual void OnDie();                               // 0x006A4CF0, slot 11: ?OnDie@CThingPhysical@@MAEXXZ (both PDBs)\npublic:\n", 1),
                 ("    bool LeaveExperienceOrbsOverride : 1;               // bit 5\n    void OnDie();\n", "    bool LeaveExperienceOrbsOverride : 1;               // bit 5\n    virtual void OnDie();                               // slot 11: rtti-vtables CThingCreatureBase slot 11; PDB UAE\n", 1)]
S['006a4d90'] = [("    unsigned char   _pad00[0x60];\n", slots(0, 55) + "\n    unsigned char   _pad04[0x5c];                        // +0x04 (the vptr takes +0x00)\n", 1),
                 ("    bool CanBlockNavigatingCreature();\n", "    virtual bool CanBlockNavigatingCreature() const;     // slot 55: rtti-vtables CThingPhysical slot 55; PDB UBE\n", 1),
                 ("bool CThingPhysical::CanBlockNavigatingCreature()\n", "bool CThingPhysical::CanBlockNavigatingCreature() const\n", 1)]
for a, reps in S.items():
    src = str(nf.src_of(a)); o = 'orig/%s.cpp' % a; r = 'rows/%s.cpp' % a
    if not os.path.exists(o): shutil.copy(src, o)
    t = open(src, encoding='utf-8').read(); ok = True
    for old, new, cnt in reps:
        if t.count(old) != cnt: print('FAIL', a, t.count(old), repr(old[:60])); ok = False; break
        t = t.replace(old, new)
    if ok: open(r, 'w', encoding='utf-8').write(leaf(t, a)); print('OK', a)
