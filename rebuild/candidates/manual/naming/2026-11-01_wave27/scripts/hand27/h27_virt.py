# h27_virt.py: naming wave 27, family virt (DECISIONS_W26_RESOLVED D5, the two virtual rows) -- own Leafs on their PDB spellings
#  (Ego_r.pdb + FableWin.pdb identical; neither row has a landed binder):
#  0x00A86E20 ?GetPossibleCollisionSet@CPhysicsFacesSegmentLeaf@@MBEXMABVC3DVector@@0AAV?$vector@JV?$allocator@J@std@@@std@@@Z
#    (protected virtual const: slot 4 of vtable 0x0129DEA4), and
#  0x004FF080 ?LoadTopologyFromFile@CWorldMap@@UAEXJV?$CCountedPointer@VCAFile@@@@AAVCASuspendableProcess@@@Z (virtual: slot 6 of
#    vtable 0x01244AEC). The class's vptr is the compiler's (the `void* _vptr` member goes); no vtable is emitted (no ctor here).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
NOTE = '// Naming wave 27 (family virt, DECISIONS_W26 D5): %s'
def note(a, txt, leaf=None):
    p, rel, t = he._rw(a, 'src')
    if 'family virt' in t: return
    L = t.split('\n'); i = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
    if leaf:
        if i: L[i[0]] = '// Leaf: ' + leaf
        else: L.insert(1, '// Leaf: ' + leaf); i = [1]
    L.insert((i[0] + 1) if i else 1, NOTE % txt); he._w(p, '\n'.join(L))
a = '00a86e20'
he.edit(a, 'src', '''    long GetNoFaces() const { return TriangleListIndexLast - TriangleListIndexFirst; }
    void GetPossibleCollisionSet(float radius, const C3DVector& from, const C3DVector& to, std::vector<long>& faces);
};

void CPhysicsFacesSegmentLeaf::GetPossibleCollisionSet(float radius, const C3DVector& from, const C3DVector& to, std::vector<long>& faces)''',
'''    long GetNoFaces() const { return TriangleListIndexLast - TriangleListIndexFirst; }
protected:
    virtual void GetPossibleCollisionSet(float radius, const C3DVector& from, const C3DVector& to, std::vector<long>& faces) const;   // slot 4
};

void CPhysicsFacesSegmentLeaf::GetPossibleCollisionSet(float radius, const C3DVector& from, const C3DVector& to, std::vector<long>& faces) const''')
note(a, 'GetPossibleCollisionSet is a protected virtual const in both PDBs (MBE; slot 4 of\n//   vtable 0x0129DEA4). Was a public non-virtual (QAE).',
     '?GetPossibleCollisionSet@CPhysicsFacesSegmentLeaf@@MBEXMABVC3DVector@@0AAV?$vector@JV?$allocator@J@std@@@std@@@Z')
a = '004ff080'
he.edit(a, 'src', '''class CWorldMap { public:
    void*            _vptr;                                  // +0x00
    long             f4;                                     // +0x04''', '''class CWorldMap { public:                                   // vptr +0x00 (vtable 0x01244AEC)
    long             f4;                                     // +0x04''')
he.edit(a, 'src', '''    void LoadTopologyFromFile(long map, CCountedPointer<CAFile> file, CASuspendableProcess& process);
};''', '''    virtual void LoadTopologyFromFile(long map, CCountedPointer<CAFile> file, CASuspendableProcess& process);   // slot 6
};''')
note(a, 'LoadTopologyFromFile is virtual in both PDBs (UAE; slot 6 of vtable 0x01244AEC), so\n//   the vptr is the compiler\'s. Was a non-virtual (QAE) over a `void* _vptr` member.',
     '?LoadTopologyFromFile@CWorldMap@@UAEXJV?$CCountedPointer@VCAFile@@@@AAVCASuspendableProcess@@@Z')
