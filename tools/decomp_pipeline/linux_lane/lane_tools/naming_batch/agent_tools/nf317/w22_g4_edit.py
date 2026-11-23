import sys, shutil, os, re
sys.path.insert(0, 'tools'); import nf
E = {e[0]: e for e in nf.entries()}
WRAP = re.compile(r'\n[ \t]*(protected|private):\n([ \t]*)([^\n]*;)\n[ \t]*public:\n[ \t]+(//[^\n]*)\n')
def tidy(t):   # the script's access wrap left the declaration's trailing comment on its own line: rejoin it
    return WRAP.sub(lambda m: '\n%s:\n%s%s   %s\npublic:\n' % (m.group(1), m.group(2), m.group(3), m.group(4)), t)
def edit(a, reps, base='att', test_reps=None):
    s = str(nf.src_of(a)); shutil.copy(s, 'orig/%s.cpp' % a)
    t = open('work/att/%s.cpp' % a if base == 'att' else s, encoding='utf-8').read()
    t = tidy(t)
    for o, n in reps:
        if t.count(o) != 1: print('FAIL', a, t.count(o), repr(o[:80])); return
        t = t.replace(o, n)
    if base != 'att' and E[a][2] != 'binders1':
        P = E[a][3]
        if re.search(r'^// Leaf:.*$', t, re.M): t = re.sub(r'^// Leaf:.*$', '// Leaf: ' + P, t, count=1, flags=re.M)
        else: t = '// Leaf: ' + P + '\n' + t
    open('rows/%s.cpp' % a, 'w', encoding='utf-8').write(t); print('OK', a)
CV = "    const T& operator[](%s n) const { return *(%s + n); }   // STLport's const overload (the PDB const self reads through it)\n"
edit('00439ab0', [("    T& operator[](long n) { return *(_M_start + n); }\n", "    T& operator[](long n) { return *(_M_start + n); }\n" + CV % ('long', '_M_start'))])
edit('004b1110', [("    T& operator[](unsigned long n) { return *(begin() + n); }\n", "    T& operator[](unsigned long n) { return *(begin() + n); }\n    const T* begin() const { return _M_start; }\n" + CV % ('unsigned long', 'begin()'))])
edit('009f4d40', [("    T& operator[](long n) { return *(begin() + n); }\n", "    T& operator[](long n) { return *(begin() + n); }\n    const T* begin() const { return m_first; }\n" + CV % ('long', 'begin()'))])
LC = "    const long& operator[](unsigned long n) const { return %s; }   // STLport's const overload (the PDB const self reads through it)\n"
for a in ('00581cfd', '00581d8e', '00581fa4'):
    edit(a, [("    long& operator[](unsigned long n) { return *(begin() + n); }\n", "    long& operator[](unsigned long n) { return *(begin() + n); }\n" + LC % '*(begin() + n)')])
edit('00581d4e', [("    long& operator[](unsigned long n) { return first[n]; }\n", "    long& operator[](unsigned long n) { return first[n]; }\n" + LC % 'first[n]')])
# B: the callee takes its PDB const (binder respell onto the PDB spelling; no landed self row defines either form)
edit('005dc070', [("    void TransferBinaryOut(CDataOutputStream& stream);   // 0x005DB630\n", "    void TransferBinaryOut(CDataOutputStream& stream) const;   // 0x005DB630 ?TransferBinaryOut@CAnimationEntry@@QBEXAAVCDataOutputStream@@@Z (both PDBs)\n")])
edit('00958e20', [("    bool IsGuardOnLevel();                      // 0x00958C00\n", "private:\n    bool IsGuardOnLevel() const;                // 0x00958C00 ?IsGuardOnLevel@CAIStateGroup_MinionReactToCrime@@ABE_NXZ (both PDBs)\npublic:\n")])
edit('0096d380', [("    bool IsStaffed();                           // 0x0096D2D0\n", "protected:\n    bool IsStaffed() const;                     // 0x0096D2D0 ?IsStaffed@CAIStateGroup_EatAtTavern@@IBE_NXZ (both PDBs)\npublic:\n")])
# C: R1 const slots (virtual calls: no symbol)
edit('005faa60', [("    virtual unsigned char IsBlocked();        // slot 3 (+0x0C)\n", "    virtual unsigned char IsBlocked() const;  // slot 3 (+0x0C); const: the PDB-const self calls it (ruling R1)\n")])
edit('0096d190', [("    virtual bool IsStillValid();                // slot 14 (+0x38)\n    virtual bool IsStateGroupStillValid();      // slot 15 (+0x3c)\n",
                   "    virtual bool IsStillValid() const;          // slot 14 (+0x38); PDB MBE overrides\n    virtual bool IsStateGroupStillValid() const;   // slot 15 (+0x3c); PDB MBE (other state groups); const: the PDB-const self calls it (ruling R1)\n")])
# D: a local pointer to a member in a const member is a pointer to const
edit('009d4a50', [("    CMemoryDataOutputStream* data = &StringStream;\n", "    const CMemoryDataOutputStream* data = &StringStream;\n")])
edit('008d0ba0', [("    CIntelligentPointer<CThing>* p = &PThing;\n", "    const CIntelligentPointer<CThing>* p = &PThing;\n")])
# E: the force-inline lookup templates read only through const pointers: const-qualified (no symbol)
edit('004c8c90', [("    template<class T> FABLE_FORCEINLINE T* GetTC(long wanted)        //", "    template<class T> FABLE_FORCEINLINE T* GetTC(long wanted) const  //")])
edit('00665e90', [("    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)\n", "    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType) const\n")])
# F: binder friend (retail access forces it; NF305 Q5)
edit('008c63c0', [("    void WhackGround();                     // 0x008C61C0\n", "private:\n    void WhackGround();                     // 0x008C61C0 ?WhackGround@CCreatureAction_TrollWhackGroundBase@@AAEXXZ (both PDBs: private)\n    friend class CCreatureAction_TrollTunnel;   // retail's private access forces it (ruling NF305 Q5)\npublic:\n")], base='head')
# G: the sequence base polymorphic (its OnCreate is the PDB UAE virtual)
edit('00908b50', [("    void* nativeVptr; // data-only vptr; no sequence vtable is emitted by this caller\n", "    virtual void _v00(); // slot 0 (not modelled); no sequence vtable is emitted by this caller\n"),
                  ("    void OnCreate(); // non-virtual direct-call adapter for the real base slot at 0x008B0650\n", "    virtual void OnCreate(); // 0x008B0650 ?OnCreate@CCombatSequenceBase@@UAEXXZ (both PDBs; called qualified below)\n"),
                  ("    void OnCreate(); // selected body; native PDB identifies this as a virtual override\n", "    virtual void OnCreate(); // rtti-vtables CCombatSequence_VillagerComboUnblockable slot 1; PDB UAE\n")], base='head')
# H: retry with the pad's element type kept
edit('00499c50', [("    unsigned char _pad00[0x50];                            // +0x00 textures, text tags, wide text, current tag\n", "    unsigned char _pad04[0x4c];                            // +0x04 textures, text tags, wide text, current tag (the vptr takes +0x00)\n"),
                  ("    void DisplayProgress(const CCharString& textTag, float progress, bool a, bool b);\n", "    virtual void DisplayProgress(const CCharString& textTag, float progress, bool a, bool b);   // PDB UAE (labels_rtti_port)\n")], base='head')
