# h19_own.py: naming wave 19, families c685 / gif -- the chunk-685 / chunk-684 queue targets whose OWN landed row carries a spelling the
# catalog proves wrong (name-only; the bodies are unchanged):
#   00ab3640  CTCMapwho_OnDie (a generic `OnDie` delete stub) -> ?DeleteData@?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@SIXPAX@Z
#             (Ego_r; chunk 685: the deleter BuildBoneInfoTree's count block stores) -- the callee 0x00AB34C0 is ??1CBoneInfo@C3DAnimationBuilder@@QAE@XZ
#             (FableWin.pdb; the row destroying a CBoneInfo's vector of counted pointers through __destroy_aux 0x00AB2BD0).
#   0099f560  global_Destval (Dest_val) -> ?RemoveWhiteSpaceBeginEnd@CCharString@@QAEXXZ (Ego_r / FableWin; chunk 685) calling
#             ?RemoveWhiteSpaceBegin@CCharString@@QAEXXZ 0x0099F450 and ?RemoveWhiteSpaceEnd@CCharString@@QAEXXZ 0x0099F4D0 (FableWin.pdb; the
#             landed-names label of 0x0099F560 already named the second callee RemoveWhiteSpaceEnd).
#   00a89320 / 00a89350  C3DMesh2::GetVertexCount / GetTriangleCount are const in Ego_r (QBEKXZ; chunk 685 EF): `const` added.
#   009ff6b0  ?InternalGetFrameForcedLoading@CGraphicInfoFrame@@ABE_NK@Z (Ego_r; chunk 684): private const, unsigned long flags (was public QAE_NJ).
#   00ab1480  global_MoveCountedPointer (Move_CountedPointer_00ab1480) -> the STLport 4.6 _algobase.h
#             std::__copy<CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, same, long>(first, last, result, const random_access_iterator_tag&, long*)
#             (Ego_r; chunk 685): the same count-down loop under the template spelling.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_own.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
# OWN_FAM=gif runs only the chunk-684 row 009ff6b0 (family gif); OWN_FAM=c685 (default) the chunk-685 rows
_F = os.environ.get('OWN_FAM', 'c685')
if _F == 'gif': _X |= {'00ab3640', '0099f560', '00a89320', '00a89350', '00ab1480'}
else: _X |= {'009ff6b0'}
def put(a, text):
    p, rel, t = he._rw(a, 'src')
    if t == text: print('already', a); return
    he._w(p, text); print('ok', a)
if '00ab3640' not in _X:
    put('00ab3640', '''// CCountedPointer<C3DAnimationBuilder::CBoneInfo>::DeleteData(void* object) -- the per-type deleter the count block stores: destroy the bone info and free it (null-guarded `delete`).
// Leaf: ?DeleteData@?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB3640; static __fastcall, object in ecx. Ego_r.pdb name (chunk 685: the deleter
//   C3DAnimationBuilder::BuildBoneInfoTree 0x00AB1F30 stores in the count block, see the landed Reset 0x00AB36B0); the catalog's
//   CTCMapwho::OnDie / DeleteObject_00ab3640 were propagated labels of the shared `delete p` shape (naming wave 19).
// Callees: ??1CBoneInfo@C3DAnimationBuilder@@QAE@XZ 0x00AB34C0 (FableWin.pdb), operator delete 0x00BFE9BC.
class C3DAnimationBuilder { public:
    class CBoneInfo { public:
        ~CBoneInfo();   // 0x00AB34C0
    };
};
void operator delete(void*);   // 0x00bfe9bc
template <class T> class CCountedPointer { public:
    static void __fastcall DeleteData(void* object);
};
template <class T> void __fastcall CCountedPointer<T>::DeleteData(void* object)
{
    delete (T*)object;
}
template class CCountedPointer<C3DAnimationBuilder::CBoneInfo>;
''')
if '0099f560' not in _X:
    put('0099f560', '''// CCharString::RemoveWhiteSpaceBeginEnd() -- strip the leading, then the trailing white space.
// Leaf: ?RemoveWhiteSpaceBeginEnd@CCharString@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099F560. Ego_r / FableWin.pdb name (chunk 685); the catalog's CCharString::Trim / Dest_val
//   were guesses (naming wave 19). Callees: ?RemoveWhiteSpaceBegin@CCharString@@QAEXXZ 0x0099F450, ?RemoveWhiteSpaceEnd@CCharString@@QAEXXZ
//   0x0099F4D0 (FableWin.pdb; the chunk 314 landed-names label of this row already named the second).
class CCharString { public:
    void RemoveWhiteSpaceBegin();      // 0x0099F450
    void RemoveWhiteSpaceEnd();        // 0x0099F4D0
    void RemoveWhiteSpaceBeginEnd();   // 0x0099F560
};
void CCharString::RemoveWhiteSpaceBeginEnd()
{
    RemoveWhiteSpaceBegin();
    RemoveWhiteSpaceEnd();
}
''')
for a, fn in (('00a89320', 'GetVertexCount'), ('00a89350', 'GetTriangleCount')):
    if a in _X: continue
    he.resub(a, 'src', r'(?m)^    unsigned long %s\(\);' % fn, '    unsigned long %s() const;   // ?%s@C3DMesh2@@QBEKXZ (Ego_r.pdb, chunk 685: const; naming wave 19)' % (fn, fn))
    he.resub(a, 'src', r'(?m)^unsigned long C3DMesh2::%s\(\)(?! const)' % fn, 'unsigned long C3DMesh2::%s() const' % fn)   # already-const guard
a = '009ff6b0'
if a not in _X:
    he.edit(a, 'src', '    bool InternalGetFrameForcedLoading(long flags);\n};', 'private:\n    bool InternalGetFrameForcedLoading(unsigned long flags) const;   // ?InternalGetFrameForcedLoading@CGraphicInfoFrame@@ABE_NK@Z (Ego_r.pdb, chunk 684: private const, unsigned long; naming wave 19)\n};')
    he.edit(a, 'src', 'bool CGraphicInfoFrame::InternalGetFrameForcedLoading(long flags)\n', 'bool CGraphicInfoFrame::InternalGetFrameForcedLoading(unsigned long flags) const\n')
    he.resub(a, 'src', r'(?m)^// CGraphicInfoFrame::InternalGetFrameForcedLoading\(long flags\)', '// CGraphicInfoFrame::InternalGetFrameForcedLoading(unsigned long flags) const')
a = '00ab1480'
if a not in _X:
    p, rel, t = he._rw(a, 'src')
    if '__copy' not in t:
        t = t.replace('// _Move<CCountedPointer*> -- copy-assign [first, last) onto dest (count-block release/adopt inlined), return the advanced dest\n',
                      '// std::__copy<CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, long>(first, last, result, const random_access_iterator_tag&, long*)\n// -- STLport 4.6 _algobase.h random-access copy back end: copy-assign [first, last) onto result (count-block release/adopt inlined), return the advanced result\n// Leaf: ??$__copy@PAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@PAV1@J@std@@YIPAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAJ@Z\n// Name: chunk 685 NAME_FIXES (the STLport 4.6 instantiation; in neither PDB -- its hand-mangled spelling ...PAV1@0ABU... drops one parameter, VC7.1 emits ...PAV1@00ABU... for the five-parameter text);\n//   the row was landed as the descriptive Move_CountedPointer_00ab1480 (naming wave 19: the same body under the template spelling).\n')
        t = t.replace('struct CCountedPointer {\n    void* m_pObject;                       // +0\n', 'class C3DAnimationBuilder { public: class CBoneInfo; };\ntemplate <class T> class CCountedPointer { public:\n    T* m_pObject;                          // +0\n')
        t = t.replace('        void* object = other.m_pObject;\n', '        T* object = other.m_pObject;\n')
        k = t.index('struct SIteratorTag { SIteratorTag() { } };')
        t = t[:k] + ('namespace std {\nstruct random_access_iterator_tag { };\ntemplate <class _RandomAccessIter, class _OutputIter, class _Distance>\n'
            '_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*)\n{\n'
            '    for (_Distance __n = __last - __first; 0 < __n; --__n, ++__first, ++__result)\n        *__result = *__first;\n    return __result;\n}\n'
            'template CCountedPointer<C3DAnimationBuilder::CBoneInfo>* __fastcall __copy<CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, long>'
            '(CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, const random_access_iterator_tag&, long*);\n}\n')
        he._w(p, t); print('ok', a)
    else: print('already', a)
