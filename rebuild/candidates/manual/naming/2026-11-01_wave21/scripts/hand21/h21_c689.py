# h21_c689.py: naming wave 21, family c689 -- the chunk 689 NAME_FIXES own rows re-authored on proven text, byte-identical only:
#   00613ae0 / 006196d0: the STLport 4.6 _Rb_tree::_M_erase instantiations of the two CTCInventoryMap maps (Ego_r.pdb: CategoriesList
#     map<CHeroLogBook::CHeroLogBookEntry::ECategory, NUISystem::CComponent*>, MapRegions map<long, CRegion>); ~CTCInventoryMap 0x00606F80
#     (chunk 689, landed) binds exactly these two compiler spellings. Re-authored on the landed STLport _tree text (the 0x00834FD0 model,
#     as wave 20 did for 0x00AD14E0 / 0x0043020A); were the descriptive CTreeOwner_00613ae0 / CTreeOwner_006196d0::FreeTree.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_c689.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
TPL = open('/home/user/FableDecomp/rebuild/src/compiled/00/83/stdRbtreeCUnprovenKey00834fd0CUnprovenValue00834fd0CUnprovenKeyOfValue00834fd0st_Merase_00834fd0.cpp', encoding='utf-8', errors='surrogateescape', newline='').read()
i = TPL.index('extern "C" void* __cdecl malloc'); j = TPL.index('struct CUnprovenKey_00834fd0'); k = TPL.index('namespace std {')
BODY = TPL[k:TPL.index('template void _Rb_tree<CUnprovenKey_00834fd0')]
PRE = TPL[i:j]
MODEL = '''// Model: STLport 4.6 stl/_tree.h + stl/_tree.c text in namespace std (speed TU); explicit instantiation emits the member as
//   its own COMDAT, as retail kept every used inline member (no /OPT:REF).
'''
CAT = '''// std::_Rb_tree<CHeroLogBook::CHeroLogBookEntry::ECategory, std::pair<const CHeroLogBook::CHeroLogBookEntry::ECategory, NUISystem::CComponent*>, std::_Select1st<..>, std::less<CHeroLogBook::CHeroLogBookEntry::ECategory>, std::allocator<..> >::_M_erase(std::_Rb_tree_node<..>*) -- STLport 4.6 _M_erase(_Link_type): erase without rebalancing -- recurse right, free the node, continue left
// Retail: Fable.exe (TLC, VC7.1) @ 0x00613AE0, 51 bytes.
// Types (naming wave 21, chunk 689 NAME_FIXES): Ego_r.pdb CTCInventoryMap::CategoriesList (+0x160 PDB, +0x148 retail) is
//   map<CHeroLogBook::CHeroLogBookEntry::ECategory, NUISystem::CComponent*>; the landed ~CTCInventoryMap 0x00606F80 clears it through this
//   instantiation (the member's inline ~map) and binds this compiler spelling. The value is trivially destroyed, the node (0x18) freed.
//   Was the descriptive CTreeOwner_00613ae0::FreeTree (one of 26 bodies of this shape; the manifest label LTextTreeWalkThrough::BuildTreeArray is propagated).
''' + MODEL + '''// Leaf: ?_M_erase@?$_Rb_tree@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@U?$pair@$$CBW4ECategory@CHeroLogBookEntry@CHeroLogBook@@PAVCComponent@NUISystem@@@std@@U?$_Select1st@U?$pair@$$CBW4ECategory@CHeroLogBookEntry@CHeroLogBook@@PAVCComponent@NUISystem@@@std@@@5@U?$less@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@@5@V?$allocator@U?$pair@$$CBW4ECategory@CHeroLogBookEntry@CHeroLogBook@@PAVCComponent@NUISystem@@@std@@@5@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBW4ECategory@CHeroLogBookEntry@CHeroLogBook@@PAVCComponent@NUISystem@@@std@@@2@@Z
''' + PRE + '''class CHeroLogBook { public:
    class CHeroLogBookEntry { public:
        enum ECategory { };
    };
};
namespace NUISystem { class CComponent; }
''' + BODY + '''typedef pair<const CHeroLogBook::CHeroLogBookEntry::ECategory, NUISystem::CComponent*> _Val;
template void _Rb_tree<CHeroLogBook::CHeroLogBookEntry::ECategory, _Val, _Select1st<_Val>, less<CHeroLogBook::CHeroLogBookEntry::ECategory>, allocator<_Val> >::_M_erase(_Rb_tree_node<_Val>*);
}
'''
REG = '''// std::_Rb_tree<long, std::pair<const long, CRegion>, std::_Select1st<std::pair<const long, CRegion> >, std::less<long>, std::allocator<std::pair<const long, CRegion> > >::_M_erase(std::_Rb_tree_node<std::pair<const long, CRegion> >*) -- STLport 4.6 _M_erase(_Link_type): erase without rebalancing -- recurse right, destroy the value (~CRegion), free the node, continue left
// Retail: Fable.exe (TLC, VC7.1) @ 0x006196D0, 59 bytes.
// Types (naming wave 21, chunk 689 NAME_FIXES): Ego_r.pdb CTCInventoryMap::MapRegions is map<long, CRegion>; the landed ~CTCInventoryMap
//   0x00606F80 clears it through this instantiation and binds this compiler spelling. The node's value destructor runs CRegion's out-of-line
//   ~CRegion 0x0051A110 (??1CRegion@@QAE@XZ, already bound there) on +0x14 (`lea ecx,[esi+14h]`), then the node is freed. Was the descriptive
//   CTreeOwner_006196d0::FreeTree over a CMember_0051a110 stand-in (one of 12 bodies of this shape).
''' + MODEL + '''// Leaf: ?_M_erase@?$_Rb_tree@JU?$pair@$$CBJVCRegion@@@std@@U?$_Select1st@U?$pair@$$CBJVCRegion@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCRegion@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBJVCRegion@@@std@@@2@@Z
''' + PRE + '''class CRegion { public:
    unsigned char _body[0x4];                            // layout not needed here: only the out-of-line destructor runs
    ~CRegion();                                          // 0x0051A110
};
''' + BODY + '''typedef pair<const long, CRegion> _Val;
template void _Rb_tree<long, _Val, _Select1st<_Val>, less<long>, allocator<_Val> >::_M_erase(_Rb_tree_node<_Val>*);
}
'''
for a, txt in (('00613ae0', CAT), ('006196d0', REG)):
    if a in _X: continue
    he.put(a, 'src', txt)
# 00619f50 / 00d3aff0: the scalar deleting destructors of CTCInventoryMap (slot 0 of vtable 0x01257A9C) and NScript::CQ_FireHeartScript (slot 0 of
#   vtable 0x012CCB90), emitted by the compiler with the vtable in a TU that constructs the class (the landed 0x008C37C0 / 0x00BB6520 shape);
#   were __fastcall free-function stand-ins (CTCInventoryMap_scalar_deleting_destructor; the CLandscapeBackgroundPatch `vector_deleting_destructor').
GD = '''// %(cls)s::`scalar deleting destructor'(unsigned int) -- run the out-of-line destructor %(dtor)s, then free the object on flag bit 0
// Leaf: %(leaf)s
// Retail: Fable.exe (TLC, VC7.1) @ 0x%(A)s, thiscall, `ret 4`: push esi ; mov esi,ecx ; call %(dtorsym)s ; test byte ptr [esp+8],1 ; je ; push esi ; call operator delete ; add esp,4 ; mov eax,esi ; pop esi ; ret 4.
// Identity (naming wave 21, chunk 689 NAME_FIXES): slot 0 of vtable 0x%(vt)s (%(vtwhy)s); its body calls %(dtorsym)s %(dtor)s
//   (landed, chunk 689) then operator delete 0x00BFE9BC. Was %(was)s.
// Shape: the class's destructor is user-declared and out of line (one slot-0 entry); the vtable (and so this ??_G) is emitted by the
//   construction helper below, as the landed ??_G rows 0x008C37C0 / 0x00BB6520 are. The helper is not retail code.
void __cdecl operator delete(void* p);                       // 0x00BFE9BC
%(decl)s
void* MakeOne_%(a)s()
{
    return new %(qcls)s;
}
'''
TCI = GD % dict(cls='CTCInventoryMap', dtor='0x00606F80', leaf='??_GCTCInventoryMap@@UAEPAXI@Z', A='00619F50', dtorsym='??1CTCInventoryMap@@UAE@XZ',
    vt='01257A9C', vtwhy='RTTI CTCInventoryMap; stored by the landed ctor 0x00606D20',
    was='the __fastcall CTCInventoryMap_scalar_deleting_destructor stand-in (catalog CTCInventoryMap_scalar_deleting_destructor)', a='00619f50', qcls='CTCInventoryMap',
    decl='''class CTCInventoryMap {                                  // FableWin.pdb / Ego_r.pdb; only the slot-0 destructor matters here
public:
    virtual ~CTCInventoryMap();                          // 0x00606F80 (??1CTCInventoryMap@@UAE@XZ)
};''')
FH = GD % dict(cls='NScript::CQ_FireHeartScript', dtor='0x00D3B010', leaf='??_GCQ_FireHeartScript@NScript@@UAEPAXI@Z', A='00D3AFF0', dtorsym='??1CQ_FireHeartScript@NScript@@UAE@XZ',
    vt='012CCB90', vtwhy='??_7CQ_FireHeartScript@NScript@@6B@, stored only by the landed ctor 0x00D34470',
    was='a CLandscapeBackgroundPatch `vector_deleting_destructor\' __fastcall stand-in', a='00d3aff0', qcls='NScript::CQ_FireHeartScript',
    decl='''namespace NScript {
class CQ_FireHeartScript {                               // Ego_r.pdb; only the slot-0 destructor matters here
public:
    virtual ~CQ_FireHeartScript();                       // 0x00D3B010 (??1CQ_FireHeartScript@NScript@@UAE@XZ)
};
}''')
for a, txt in (('00619f50', TCI), ('00d3aff0', FH)):
    if a in _X: continue
    he.put(a, 'src', txt)
# 00ab0fc0: std::__uninitialized_copy<CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, ..>(first, last, result, const __false_type&) -- the
#   out-of-line copy helper of vector<CCountedPointer<CBoneInfo> >::_M_insert_overflow 0x00AB3300, which binds this spelling; re-authored from
#   the landed 0x00AB3300 STLport text (the same-TU noinline __fastcall helper), explicitly instantiated. Was a free `Uninit_copy` stand-in.
_T = open('/home/user/FableDecomp/rebuild/src/compiled/' + he.R['00ab3300']['Source'], encoding='utf-8', errors='surrogateescape', newline='').read()
_i = _T.index('extern "C" void* __cdecl malloc'); _j = _T.index('template <class _ForwardIter, class _Size, class _Tp>')
UC = '''// std::__uninitialized_copy<CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, CCountedPointer<C3DAnimationBuilder::CBoneInfo>*>(first, last, result, const __false_type&) -- STLport 4.6 copy-construct [first, last) into raw storage at result (Data and Info copied, Info->RefCount bumped when set), return the end
// Leaf: ??$__uninitialized_copy@PAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@PAV1@@std@@YIPAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB0FC0, 50 bytes, `__fastcall` (first in ecx, last in edx, result and the tag on the stack, `ret 8`).
// Identity (naming wave 21, chunk 689 NAME_FIXES): the out-of-line head / tail copy of vector<CCountedPointer<C3DAnimationBuilder::CBoneInfo> >::
//   _M_insert_overflow 0x00AB3300 (landed; binds exactly this spelling). Text: that row's STLport model (the element's own __fastcall COMDAT),
//   explicitly instantiated. Was a free `Uninit_copy` stand-in over an `Elem` record (one of 31 bodies of this shape, template 0x004477C0).
''' + _T[_i:_j] + '''template CCountedPointer<C3DAnimationBuilder::CBoneInfo>* __fastcall __uninitialized_copy(CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, CCountedPointer<C3DAnimationBuilder::CBoneInfo>*,
    CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, const __false_type&);
}
'''
if '00ab0fc0' not in _X: he.put('00ab0fc0', 'src', UC)
# 00ad30c0: std::vector<C3DVector>::vector(size_type, const C3DVector&, const allocator&) -- 0x00AC9D50 MeasureSegments (chunk 689) builds its two
#   bounding-box vectors through it and binds the STLport spelling; the landed model's hand names (CSTLportVector / CVectorBase / CAllocProxy /
#   CAllocator) move to the STLport class names in namespace std (size_type unsigned int), the text otherwise unchanged.
a = '00ad30c0'
if a not in _X:
    he.edit(a, 'src', '// CSTLportVector<C3DVector>::CSTLportVector(unsigned long count, const C3DVector& value, const allocator&)', '// std::vector<C3DVector>::vector(size_type count, const C3DVector& value, const allocator<C3DVector>&)')
    he.edit(a, 'src', '// Leaf: ??0?$CSTLportVector@\n', '// Leaf: ??0?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAE@IABVC3DVector@@ABV?$allocator@VC3DVector@@@1@@Z\n// Naming (naming wave 21, chunk 689 NAME_FIXES): the STLport class names in namespace std (vector / _Vector_base / _STLP_alloc_proxy / allocator,\n//   size_type unsigned int) replace the hand names CSTLportVector / CVectorBase / CAllocProxy / CAllocator; MeasureSegments 0x00AC9D50 (chunk 689)\n//   binds this spelling. Text and bytes unchanged.\n')
    he.edit(a, 'src', 'template <class T> __forceinline T* UninitializedFillN(T* first, unsigned long count, const T& value)', 'namespace std {\n\ntemplate <class T> __forceinline T* UninitializedFillN(T* first, unsigned int count, const T& value)')
    he.edit(a, 'src', '''template <class T> struct CAllocator {         // STLport allocator<T>: empty
    CAllocator() { }
    T* allocate(unsigned long count)''', '''template <class T> class allocator {           // STLport allocator<T>: empty
public:
    allocator() { }
    T* allocate(unsigned int count)''')
    he.edit(a, 'src', '''template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }''', '''template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& alloc, _Value p) : _MaybeReboundAlloc(alloc), _M_data(p) { }''')
    he.edit(a, 'src', '''template <class T> struct CVectorBase {        // STLport _Vector_base<T, allocator<T> >
    T*             m_pStart;                    // +0
    T*             m_pFinish;                   // +4
    CAllocProxy<T> m_endOfStorage;              // +8
    CVectorBase(unsigned long count, const CAllocator<T>& alloc)
        : m_pStart(0), m_pFinish(0), m_endOfStorage(alloc, 0)
    {
        m_pStart = m_endOfStorage.allocate(count);
        m_pFinish = m_pStart;
        m_endOfStorage.m_data = m_pStart + count;
    }
};''', '''template <class T, class _Alloc> class _Vector_base {
public:
    T*             _M_start;                    // +0
    T*             _M_finish;                   // +4
    _STLP_alloc_proxy<T*, T, _Alloc> _M_end_of_storage;   // +8
    _Vector_base(unsigned int count, const _Alloc& alloc)
        : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0)
    {
        _M_start = _M_end_of_storage.allocate(count);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + count;
    }
};''')
    he.edit(a, 'src', '''template <class T> struct CSTLportVector : CVectorBase<T> {     // STLport vector<T>
    explicit CSTLportVector(unsigned long count)
        : CVectorBase<T>(count, CAllocator<T>())
    {
        this->m_pFinish = UninitializedFillN(this->m_pStart, count, T());
    }
    CSTLportVector(unsigned long count, const T& value, const CAllocator<T>& alloc = CAllocator<T>())
        : CVectorBase<T>(count, alloc)
    {
        this->m_pFinish = UninitializedFillN(this->m_pStart, count, value);
    }
};

template CSTLportVector<C3DVector>::CSTLportVector(unsigned long, const C3DVector&, const CAllocator<C3DVector>&);''', '''template <class T, class _Alloc = allocator<T> > class vector : public _Vector_base<T, _Alloc> {     // STLport vector<T>
public:
    explicit vector(unsigned int count)
        : _Vector_base<T, _Alloc>(count, _Alloc())
    {
        this->_M_finish = UninitializedFillN(this->_M_start, count, T());
    }
    vector(unsigned int count, const T& value, const _Alloc& alloc = _Alloc())
        : _Vector_base<T, _Alloc>(count, alloc)
    {
        this->_M_finish = UninitializedFillN(this->_M_start, count, value);
    }
};

template vector<C3DVector>::vector(unsigned int, const C3DVector&, const allocator<C3DVector>&);

}''')
