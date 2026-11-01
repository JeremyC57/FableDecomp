# h20_c688.py: naming wave 20, family c688 -- the chunk 688 NAME_FIXES rows (QUEUE_W20_input.tsv, c688), byte-identical only:
#   00c69fb0 (caller): CCombFilter::Blend's first parameter is written (`*current = *target`), so `SCombSettings*`, not const (chunk 688);
#   00c6a080 (caller): the +0x25C stage is the CStereoDelayLine the landed ctor 0x00C75410 builds (?Process@CStereoDelayLine@@QAEXPAMK@Z 0x00C75140);
#   0051af00: ?DeleteData@?$CCountedPointer@VCObject@CLevelLoader@@@@SIXPAX@Z (FableWin + Ego_r name sets; the DeleteFunc CWorldMap::LoadLevelFromFiles
#            0x00501150 stores for its `new CLevelLoader::CObject`; the callee 0x005195B0 is the landed ~CObject) -- was a CTCMapwho::OnDie stand-in;
#   00513570: the STLport __uninitialized_fill_n over CLevelLoader::CObject::CLevelFiles (its callers 0x005158B0 vector(n) [landed, binds this spelling]
#            and 0x0051A430 _M_insert_overflow) -- was the descriptive UninitFillN_ThreeCountedPointers;
#   006c27a0: ?SetAsLoading@CObject@CLevelLoader@@QAEXABV?$vector@..CLevelFiles..@@ABV?$set@VCCharString@@..@@J@Z (FableWin name set; 0x00501150 calls it
#            with (files, sections, 0); the members RegionLevelFiles +0x10 / LoadableSections +0 / RegionIndex +0x1c of Ego_r CLevelLoader::CObject);
#   0050f980 + the CLevelFiles rows 00518b80 / 005195b0 / 0051b620 / 006c2d40: the class is Ego_r CLevelLoader::CObject::CLevelFiles (??1CLevelFiles@CObject@
#            CLevelLoader@@QAE@XZ in both name sets), not a global CLevelFiles; 006c2d40's vector gets its allocator parameter (the FableWin vector spelling);
#   00b6d250 / 00b6d0b0 / 00be4380: vector<CTVertexLandscapeBackground>::_M_clear / _M_insert_overflow / push_back (the chunk 688 _M_fill_insert 0x00B6CE00,
#            proven through the PDB resize 0x00B6CD10, calls 0x00B6D0B0 with (pos, x, __false_type, n, false); 0x00B6D250 is its _M_clear; 0x00BE4380 calls
#            it with (finish, x, tag, 1, true) = STLport push_back) -- were the descriptive CRetailVector_SRecord24 / CFirstMemberOwner stand-ins;
#   004507fb / 004da360 / 0067a840 / 0067a800 / 0067a7c0: the _M_erase instantiations of the five map<EOpinion..> trees ~COpinionReactionManagerDef 0x00678460
#            (chunk 688, landed) clears -- were LTextTreeWalkThrough::BuildTreeArray / FreeTree_CTreeOwner stand-ins;
#   009fc550: ??1CResource@@UAE@XZ (FableWin name set; 218 landed def destructors bind it; engine/CResource.h: CResource : CIVCountedPointeeBase, vtable
#            0x012354A4, the base's 0x012354D0; +8 ResourceList->RemoveResource(*this) 0x009FC020 = ?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z) -- was
#            the ~CMemoryBlock stand-in.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_c688.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
def P(a, txt, k='src'):
    if a not in _X: he.put(a, k, txt)

a = '00c69fb0'
if a not in _X:
    he.edit(a, 'src', '    void Blend(const SCombSettings* current, const SCombSettings* target, CRingBuffer* line, float* output,\n',
            '    void Blend(SCombSettings* current, const SCombSettings* target, CRingBuffer* line, float* output,                     // chunk 688: the body writes `*current = *target` -- not const (naming wave 20)\n')
a = '00c6a080'
if a not in _X:
    he.edit(a, 'src', 'struct PostStage {void Process(float*,unsigned long);}; //00c75140\n', 'struct CStereoDelayLine {void Process(float*,unsigned long);}; //00c75140 ?Process@CStereoDelayLine@@QAEXPAMK@Z (chunk 688: the CStereoDelayLine the landed ctor 0x00C75410 builds at CReverbProcessor+0x25C; naming wave 20)\n')
    he.edit(a, 'src', 'StageOne One;StageTwo Two;PostStage Post;', 'StageOne One;StageTwo Two;CStereoDelayLine Post;')
    he.resub(a, 'tst', r'\bPostStage\b', 'CStereoDelayLine')

P('0051af00', '''// CCountedPointer<CLevelLoader::CObject>::DeleteData(void* data) -- the counted pointer's per-type deleter: ~CObject (0x005195B0, out of line), then operator delete
// Leaf: ?DeleteData@?$CCountedPointer@VCObject@CLevelLoader@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051AF00, 24 bytes (static __fastcall: the object in ecx; plain `ret`).
// Identity (naming wave 20, chunk 688): CWorldMap::LoadLevelFromFiles 0x00501150 stores this address as the DeleteFunc of the CCPPointerInfo
//   it builds for `new CLevelLoader::CObject` (engine/CCountedPointer.h: DeleteFunc = CCountedPointer<T>::DeleteData, SIXPAX@Z); FableWin.pdb
//   and Ego_r.pdb both carry ?DeleteData@?$CCountedPointer@VCObject@CLevelLoader@@@@SIXPAX@Z. Was a CTCMapwho::OnDie stand-in (the catalog's
//   DeleteObject_CLevelLoader__CObject_0051af00 is descriptive).
// Callees: CLevelLoader::CObject::~CObject 0x005195B0 (landed), operator delete 0x00BFE9BC.
// Shape: `delete static_cast<T*>(data)` -- the null test, the out-of-line non-virtual destructor, operator delete.
#include "engine/CCountedPointer.h"

class CLevelLoader {
public:
    class CObject {
    public:
        ~CObject();                                    // 0x005195B0
    };
};

template void FABLE_FASTCALL CCountedPointer<CLevelLoader::CObject>::DeleteData(void*);
''')

CLF_PRE = '''inline void* operator new(unsigned int, void* p) { return p; }

class CAFile;

class CCPPointerInfo { public:                               // 12 bytes
    unsigned long RefCount;                           // +0x00
    void (__cdecl *DeleteFunc)(void*);                // +0x04
    void* Data;                                       // +0x08
};

template <class T> class CCountedPointer {            // 8 bytes
public:
    T* Data;                                          // +0x00
    CCPPointerInfo* Info;                             // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};

class CLevelLoader {
public:
    class CObject {
    public:
        class CLevelFiles {                           // 28 bytes (Ego_r.pdb)
        public:
            long LevelID;                             // +0x00
            CCountedPointer<CAFile> PTopologyFile;    // +0x04
            CCountedPointer<CAFile> PNavigationFile;  // +0x0C
            CCountedPointer<CAFile> PThingFile;       // +0x14
        };
    };
};
'''
P('00513570', '''// std::__uninitialized_fill_n(CLevelFiles* first, unsigned int n, const CLevelFiles& x, const __false_type&) -- (CLevelLoader::CObject::CLevelFiles) STLport 4.6 fill of n copies of a 28-byte level-file record (each copy bumps the three counted file pointers' blocks)
// Leaf: ??$__uninitialized_fill_n@PAVCLevelFiles@CObject@CLevelLoader@@IV123@@std@@YIPAVCLevelFiles@CObject@CLevelLoader@@PAV123@IABV123@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00513570, length 92 bytes (+4 int3 pad), `ret 8`.
// Callers: 0x0050121F (0x00501150), 0x00514D5A (0x00514C10), 0x00515911 (0x005158B0, the landed vector<CLevelFiles>(n) -- it binds this
//   spelling) and 0x0051A4FC (0x0051A430, the landed _M_insert_overflow of the same vector).
// Identity (naming wave 20, chunk 688): the STLport 4.6 `__uninitialized_fill_n(first, n, x, const __false_type&)` instantiation over
//   CLevelLoader::CObject::CLevelFiles (Ego_r.pdb, 28 bytes: LevelID, then CCountedPointer<CAFile> PTopologyFile / PNavigationFile / PThingFile),
//   the element of the vector the landed 0x005158B0 / 0x0051A430 / 0x0051AE60 rows model; was the descriptive UninitFillN_ThreeCountedPointers
//   (the catalog's `_Uninit_fill_n<pair<unsigned __int64, ...>>` label is propagated: the stride is 28).
// Shape: `__fastcall` (first in ecx -> eax as `cur`, n in edx -> ecx counted down, &x on the stack -> edx loaded once); the record is copied by
//   its implicit memberwise copy constructor (each counted pointer through its user copy constructor, placement-new null test); VC7.1 keeps a
//   second induction register at the middle pointer's Info (`lea esi,[eax+10h]`).
''' + CLF_PRE + '''
namespace std {
struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template CLevelLoader::CObject::CLevelFiles* __fastcall __uninitialized_fill_n<CLevelLoader::CObject::CLevelFiles*, unsigned int, CLevelLoader::CObject::CLevelFiles>(
    CLevelLoader::CObject::CLevelFiles*, unsigned int, const CLevelLoader::CObject::CLevelFiles&, const __false_type&);
}
''')

P('006c27a0', '''// CLevelLoader::CObject::SetAsLoading(const std::vector<CLevelFiles>& files, const std::set<CCharString>& sections, long index) -- assign the region level files (+0x10), the loadable sections (+0), then store the region index (+0x1c)
// Leaf: ?SetAsLoading@CObject@CLevelLoader@@QAEXABV?$vector@VCLevelFiles@CObject@CLevelLoader@@V?$allocator@VCLevelFiles@CObject@CLevelLoader@@@std@@@std@@ABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@4@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C27A0, length 39 bytes (48 with padding), `ret 0Ch`.
// Identity (naming wave 20, chunk 688): FableWin.pdb ?SetAsLoading@CObject@CLevelLoader@@QAEXABV?$vector@..CLevelFiles..@@ABV?$set@VCCharString..@@J@Z (12
//   occurrences); CWorldMap::LoadLevelFromFiles 0x00501150 calls it on its new CObject with (files vector, sections set, 0); the body assigns the vector
//   at +0x10 (RegionLevelFiles), the set at +0 (LoadableSections) and the long at +0x1C (RegionIndex) = Ego_r CLevelLoader::CObject (0x20 bytes retail:
//   12-byte STLport set and vector, PLevelLoader +0xc). Was the descriptive SMappedRecords_006c27a0::Set (the chunk label CSpellContainerList::AddChild
//   is propagated).
// Callees: vector<CLevelFiles>::operator= 0x006C2D40 (landed), _Rb_tree<CCharString, ..>::operator= 0x006B9E00 (landed: the set's inline operator=
//   forwards `_M_t = x._M_t`).
// Shape: each argument read from its slot right before its use (no frame), the vector assigned on `lea ecx,[esi+10h]`, the set on `this`, the word
//   stored last: three assignments in source order.
class CAFile;
class CCharString;

namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _Tp> struct _Identity { };

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    void*         _M_header;                          // +0x00
    unsigned long _M_node_count;                      // +0x04
    _Compare      _M_key_compare;                     // +0x08
    _Rb_tree& operator=(const _Rb_tree& __x);         // 0x006B9E00 (stl/_tree.c, out of line)
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _M_t;
    set& operator=(const set& __x) { _M_t = __x._M_t; return *this; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector& operator=(const vector& __x);             // 0x006C2D40 (stl/_vector.c, out of line)
};
}

class CLevelLoader {
public:
    class CObject {
    public:
        class CLevelFiles { public: unsigned char Data[0x1C]; };   // Ego_r.pdb, 28 bytes: LevelID + three CCountedPointer<CAFile>
        std::set<CCharString>    LoadableSections;    // +0x00
        CLevelLoader*            PLevelLoader;        // +0x0c
        std::vector<CLevelFiles> RegionLevelFiles;    // +0x10
        long                     RegionIndex;         // +0x1c
        void SetAsLoading(const std::vector<CLevelFiles>& files, const std::set<CCharString>& sections, long index);
    };
};

void CLevelLoader::CObject::SetAsLoading(const std::vector<CLevelFiles>& files, const std::set<CCharString>& sections, long index)
{
    RegionLevelFiles = files;
    LoadableSections = sections;
    RegionIndex = index;
}
''')

P('0050f980', '''// CLevelLoader::CObject::CLevelFiles::~CLevelFiles() -- release the three counted file pointers (PThingFile +0x14, PNavigationFile +0x0c, PTopologyFile +0x04, reverse declaration order): drop the reference, destroy the object and free its count block on the last one, then clear both words
// Leaf: ??1CLevelFiles@CObject@CLevelLoader@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0050F980, length 129 bytes.
// Identity (naming wave 20, chunk 688): Ego_r.pdb CLevelLoader::CObject::CLevelFiles {LevelID, PTopologyFile, PNavigationFile, PThingFile} (28 bytes), the
//   element of the vector the landed 0x005158B0 / 0x0051A430 / 0x0051AE60 / 0x005196A0 rows model; ??1CLevelFiles@CObject@CLevelLoader@@QAE@XZ is in
//   both PDB name sets. Was the global `CLevelFiles` (the nesting was missing).
// Callees: the per-type DeleteData stored in each count block (engine/CCountedPointer.h: `__fastcall`, object in ecx), operator delete @ 0x00BFE9BC.
// Shape: engine/CCountedPointer.h's inline destructor: the count is re-read after the decrement, the zero for the two clears is the shared `xor edi,edi`;
//   the member destructors are inline and run last member first. Word +0x00 is not touched by the destructor.
#include "engine/CCountedPointer.h"

class CAFile;

class CLevelLoader {
public:
    class CObject {
    public:
        class CLevelFiles {
        public:
            long                    LevelID;             // +0x00
            CCountedPointer<CAFile> PTopologyFile;       // +0x04
            CCountedPointer<CAFile> PNavigationFile;     // +0x0c
            CCountedPointer<CAFile> PThingFile;          // +0x14
            ~CLevelFiles();
        };
    };
};

CLevelLoader::CObject::CLevelFiles::~CLevelFiles()
{
}
''')

P('00518b80', '''// std::_Destroy(CLevelFiles* first, CLevelFiles* last) -- (CLevelLoader::CObject::CLevelFiles) run ~CLevelFiles over [first, last)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00518B80, length 33 bytes (padded to 48).
// Leaf: _Destroy_CLevelFiles
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The element destructor is
// ~CLevelFiles @ 0x0050F980 (landed) and the stride is 0x1c = sizeof(CLevelLoader::CObject::CLevelFiles) (Ego_r.pdb, 28
// bytes), so this is STLport `_Destroy(first, last)` over CLevelFiles emitted out of line (LTCG register convention:
// first in ecx, last in edx, no tag argument -> plain `ret`). The class is nested (naming wave 20, chunk 688: ??1CLevelFiles@CObject@CLevelLoader@@QAE@XZ).
// Callees: CLevelLoader::CObject::CLevelFiles::~CLevelFiles @ 0x0050F980.
// Shape: the for loop becomes the guarded do-while (`cmp ; je` once, `cmp ; jne` at the bottom); `last` stays in edi.
class CAFile;
struct CCountedPointerAFile { CAFile* Object; void* Block; };
class CLevelLoader { public: class CObject { public:
class CLevelFiles { public:                 // CLevelLoader::CObject::CLevelFiles (28 bytes)
    long LevelID;                           // +0x00
    CCountedPointerAFile PTopologyFile;     // +0x04
    CCountedPointerAFile PNavigationFile;   // +0x0c
    CCountedPointerAFile PThingFile;        // +0x14
    ~CLevelFiles();                         // 0x0050F980
};
}; };
typedef CLevelLoader::CObject::CLevelFiles CLevelFiles;

void __fastcall _Destroy_CLevelFiles(CLevelFiles* first, CLevelFiles* last)
{
    for (; first != last; ++first)
        first->~CLevelFiles();
}
''')

a = '005195b0'
if a not in _X:
    he.edit(a, 'src', '''struct CLevelFiles {                        // CLevelLoader::CObject::CLevelFiles (28 bytes)
    long LevelID;                           // +0x00
    CCountedPointerAFile PTopologyFile;     // +0x04
    CCountedPointerAFile PNavigationFile;   // +0x0c
    CCountedPointerAFile PThingFile;        // +0x14
    ~CLevelFiles();                         // 0x0050F980
};
''', '''class CLevelLoader { public: class CObject; };   // CObject's body (with its nested CLevelFiles) follows the container models
''')
    he.edit(a, 'src', '''class CLevelLoader { public:
class CObject { public:
    CCharStringSet LoadableSections;        // +0x00
''', '''class CLevelLoader::CObject {
public:
class CLevelFiles { public:                 // CLevelLoader::CObject::CLevelFiles (28 bytes; nested -- naming wave 20, chunk 688: ??1CLevelFiles@CObject@CLevelLoader@@QAE@XZ)
    long LevelID;                           // +0x00
    CCountedPointerAFile PTopologyFile;     // +0x04
    CCountedPointerAFile PNavigationFile;   // +0x0c
    CCountedPointerAFile PThingFile;        // +0x14
    ~CLevelFiles();                         // 0x0050F980
};
    CCharStringSet LoadableSections;        // +0x00
''')
    he.edit(a, 'src', '''    ~CObject();
};
};
''', '''    ~CObject();
};
''')

P('0051b620', '''// std::vector<CLevelFiles>::push_back() -- (CLevelLoader::CObject::CLevelFiles) append a default-constructed 28-byte level-file record
// Leaf: PushBackEmpty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051B620, length 50 bytes (+ int3 padding).
// Callees: vector<CLevelLoader::CObject::CLevelFiles>::push_back(const CLevelFiles&) @ 0x0051AE60 (landed; it copies
// the record into `_M_finish` when there is room: LevelID, then the three counted pointers with their add-refs),
// CLevelLoader::CObject::CLevelFiles::~CLevelFiles @ 0x0050F980 (landed).
// Identity: the manifest label "CTCActionUseBuyHouse::OnCreate" is propagated and wrong. `this` is handed to
// push_back untouched (ecx is never reloaded), so the function is a member of the vector of CLevelFiles itself
// (the CLevelLoader::CObject::RegionLevelFiles element type, nested per naming wave 20 / chunk 688); it has no
// direct caller in the image. STLport 4.6 _vector.h's `void push_back() { push_back(_Tp()); }` is the matching text
// (the no-argument overload under _STLP_DONT_SUP_DFLT_PARAM); FableWin.pdb does not carry the QAEXXZ spelling, so the
// member keeps the descriptive name PushBackEmpty (DEFERRED: the vector::push_back() identity).
//
// Layout = Ego_r.pdb CLevelFiles (0x1c bytes): LevelID +0x00, then three CCountedPointer<CAFile>
// {object, count block} at +0x04 / +0x0c / +0x14 (PTopologyFile, PNavigationFile, PThingFile).
// Shape: the temporary is built in the 0x1c-byte frame: the implicit constructor leaves LevelID
// uninitialised and the three counted pointers' inline constructors zero their six words from one
// register; it is passed by reference and destroyed through the out-of-line destructor.
class CAFile;

template<class T>
class CCountedPointer { public:
    T*    m_pObject;
    void* m_pBlock;
    CCountedPointer() : m_pObject(0), m_pBlock(0) { }
};

class CLevelLoader { public: class CObject { public:
class CLevelFiles { public:
    long                    LevelID;            // +0x00
    CCountedPointer<CAFile> PTopologyFile;      // +0x04
    CCountedPointer<CAFile> PNavigationFile;    // +0x0c
    CCountedPointer<CAFile> PThingFile;         // +0x14
    ~CLevelFiles();                             // 0x0050F980
};
}; };

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // retail STLport vector<CLevelFiles> (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void push_back(const _Tp& __x);             // 0x0051AE60
    void PushBackEmpty();                       // STLport `push_back()` (see above)
};
template <class _Tp, class _Alloc> void vector<_Tp, _Alloc>::PushBackEmpty()
{
    push_back(_Tp());
}
template void vector<CLevelLoader::CObject::CLevelFiles>::PushBackEmpty();
}
''')

a = '006c2d40'
if a not in _X:
    he.edit(a, 'src', '// Leaf: ??4?$vector@UCLevelFiles@@@std@@\n', '// Leaf: ??4?$vector@VCLevelFiles@CObject@CLevelLoader@@V?$allocator@VCLevelFiles@CObject@CLevelLoader@@@std@@@std@@QAEAAV01@ABV01@@Z\n')
    he.edit(a, 'src', '// CLevelFiles is CLevelLoader::CObject::CLevelFiles in the PDB (0x1c bytes: LevelID + three counted file pointers);\n// it is declared at file scope because engine/CLevelLoader.h is a flattened layout without the nested types.\n',
            '// CLevelFiles is CLevelLoader::CObject::CLevelFiles in the PDB (0x1c bytes: LevelID + three counted file pointers), nested here\n// (naming wave 20, chunk 688: ??1CLevelFiles@CObject@CLevelLoader@@QAE@XZ); the vector carries its allocator parameter (the FableWin spelling).\n')
    he.edit(a, 'src', 'struct CLevelFiles{long LevelID;CCountedPointer<CAFile>PTopologyFile,PNavigationFile,PThingFile;};\n',
            'class CLevelLoader{public:class CObject{public:class CLevelFiles{public:long LevelID;CCountedPointer<CAFile>PTopologyFile,PNavigationFile,PThingFile;};};};\ntypedef CLevelLoader::CObject::CLevelFiles CLevelFiles;\n')
    he.edit(a, 'src', 'template<class T>struct vector{T*_M_start,*_M_finish,*_M_end_of_storage;', 'template<class T>class allocator{};\ntemplate<class T,class A=allocator<T> >class vector{public:T*_M_start,*_M_finish,*_M_end_of_storage;')
    he.edit(a, 'src', 'template<class T>__declspec(noinline)void vector<T>::_M_clear()', 'template<class T,class A>__declspec(noinline)void vector<T,A>::_M_clear()')
    he.edit(a, 'src', 'template<class T>__forceinline T*vector<T>::_M_allocate_and_copy', 'template<class T,class A>__forceinline T*vector<T,A>::_M_allocate_and_copy')
    he.edit(a, 'src', 'template<class T>vector<T>&vector<T>::operator=(const vector&rhs){', 'template<class T,class A>vector<T,A>&vector<T,A>::operator=(const vector&rhs){')
    he.edit(a, 'src', 'template struct vector<CLevelFiles>;', 'template class vector<CLevelFiles>;')

VTX = '''class CTVertexLandscapeBackground {                // Ego_r.pdb, 24 bytes (CTVertexLandscapeBackgroundBase layout; the chunk 688 _M_fill_insert 0x00B6CE00 model)
public:
    unsigned short X;                              // +0x00
    unsigned short Y;                              // +0x02
    float Z;                                       // +0x04
    float NormalX;                                 // +0x08
    float NormalY;                                 // +0x0C
    float NormalZ;                                 // +0x10
    unsigned char UVPad1;                          // +0x14
    unsigned char V1;                              // +0x15
    unsigned char U1;                              // +0x16
    unsigned char UVPad2;                          // +0x17
};
'''
P('00b6d250', '''// std::vector::_M_clear() -- (vector<CTVertexLandscapeBackground>) STLport 4.6 `_M_clear()`: the trivial element destroy, then free the block when non-null
// Leaf: ?_M_clear@?$vector@VCTVertexLandscapeBackground@@V?$allocator@VCTVertexLandscapeBackground@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B6D250, 14 bytes (size-optimised: pop ecx).
// Identity (naming wave 20, chunk 688): the `_M_clear()` the landed vector<CTVertexLandscapeBackground>::_M_insert_overflow 0x00B6D0B0 calls
//   (itself the callee of the chunk 688 _M_fill_insert 0x00B6CE00, proven through the PDB resize 0x00B6CD10) in the 0x00B6CD10..0x00B6D250
//   vector<CTVertexLandscapeBackground> COMDAT run; the element's destructor is trivial, so only the block free remains. Was the descriptive
//   _unattributed::ReleaseFirstMember_00b6d250 (renamed in the fistp pass from the propagated CFileInstaller::CActiveFile::OnReadFinished).
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14 (MSVCR71)

''' + VTX + '''
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <> struct __type_traits<CTVertexLandscapeBackground> { typedef __true_type has_trivial_destructor; };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};
}

template void std::vector<CTVertexLandscapeBackground, std::allocator<CTVertexLandscapeBackground> >::_M_clear();
''')

P('00b6d0b0', '''// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- (vector<CTVertexLandscapeBackground>) STLport 4.6 growth insert: allocate size + max(size, fill_len), copy the head, place the one element or fill n, copy the tail unless at the end, _M_clear the old block, _M_set
// Leaf: ?_M_insert_overflow@?$vector@VCTVertexLandscapeBackground@@V?$allocator@VCTVertexLandscapeBackground@@@std@@@std@@IAEXPAVCTVertexLandscapeBackground@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B6D0B0, 404 bytes; `ret 14h`. Callees: malloc @ 0x00BFEA0E (allocator::allocate), the vector's
//          _M_clear @ 0x00B6D250 (out of line: `if (first) free(first)`).
// Identity (naming wave 20, chunk 688): the chunk 688 vector<CTVertexLandscapeBackground>::_M_fill_insert 0x00B6CE00 (proven through the PDB
//   resize 0x00B6CD10) calls this address with (pos, x, __false_type, n, false) = STLport 4.6 _M_insert_overflow; the element is Ego_r
//   CTVertexLandscapeBackground (24 bytes), the landed 0x00BE4380 push_back calls it with (finish, x, tag, 1, true). Was the descriptive
//   CRetailVector_SRecord24::InsertGrow (the catalog "CActiveFile::ExpandAndFillVertices" is propagated).
// Layout: the retail 12-byte STLport vector {_M_start, _M_finish, _M_end_of_storage}; the element's IMPLICIT copy constructor (an explicit
//         memberwise one folds the `mov edx,ecx ; mov esi,eax` pointer copies of each placement copy and shrinks the frame).
// Shape: STLport 4.6 _M_insert_overflow(__false_type): `len = size + max(size, n)` through the reference-returning max, allocate(len) is null
//        for zero, each copy is a placement-new `_Construct` (null-tested), the single-element case is separate from __uninitialized_fill_n,
//        then the out-of-line _M_clear and the inline _M_set. The helpers are __forceinline (plain inline leaves them out of line here).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

''' + VTX + '''
namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear();                                            // 0x00B6D250 (out of line)
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CTVertexLandscapeBackground>::_M_insert_overflow(CTVertexLandscapeBackground*, const CTVertexLandscapeBackground&, const __false_type&, unsigned int, bool);
}
''')

P('00be4380', '''// std::vector::push_back(const CTVertexLandscapeBackground& x) -- (vector<CTVertexLandscapeBackground>) append a 24-byte landscape vertex or grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@VCTVertexLandscapeBackground@@V?$allocator@VCTVertexLandscapeBackground@@@std@@@std@@QAEXABVCTVertexLandscapeBackground@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE4380, length 82 bytes, thiscall `ret 4`.
// Identity (naming wave 20, chunk 688): the call `_M_insert_overflow(_M_finish, x, __false_type(), 1, true)` into 0x00B6D0B0 -- the
//   vector<CTVertexLandscapeBackground>::_M_insert_overflow the chunk 688 _M_fill_insert 0x00B6CE00 proves (through the PDB resize 0x00B6CD10) --
//   is STLport 4.6 _vector.h push_back; a member of that vector is the only caller shape that passes (finish, x, 1, true). The element is Ego_r
//   CTVertexLandscapeBackground (24 bytes). Was the descriptive CRetailVector_SRecord24::PushBack_SRecord24_00be4380 (the manifest label
//   "CScriptModuleFullTransition::OnInitLeaf" is propagated). FableWin.pdb does not carry this push_back (the debug build never grew the vector
//   through it), so the spelling rests on the STLport text and the callee.
// Shape: `this` stays in ecx (no frame); the placement copy is six direct dword moves through esi (the trivially copyable element). The full
//   branch passes a const reference to the empty __false_type tag whose 1-byte temporary VC homes in this function's dead argument slot
//   (`lea edx,[esp+0Ch]` after the two `push 1`).
inline void* operator new(unsigned int, void* __p) { return __p; }

''' + VTX + '''
namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2>
__forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator { public: allocator() { } };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                  // 0x00B6D0B0
    public:
    void push_back(const _Tp& __x) {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template void vector<CTVertexLandscapeBackground>::push_back(const CTVertexLandscapeBackground&);
}
''')

# the five opinion-map _M_erase rows: the STLport 4.6 _tree text of the landed 0x00834FD0 model (as h20_erase.py)
TPL = open('/home/user/FableDecomp/rebuild/src/compiled/00/83/stdRbtreeCUnprovenKey00834fd0CUnprovenValue00834fd0CUnprovenKeyOfValue00834fd0st_Merase_00834fd0.cpp', encoding='utf-8', errors='surrogateescape', newline='').read()
i = TPL.index('extern "C" void* __cdecl malloc'); j = TPL.index('struct CUnprovenKey_00834fd0'); k = TPL.index('namespace std {')
BODY = TPL[k:TPL.index('template void _Rb_tree<CUnprovenKey_00834fd0')]
PRE = TPL[i:j]
ENUMS = '''enum EOpinionAttitudeType { };               // Ego_r.pdb enums (W4): the key / value types of COpinionReactionManagerDef's maps
enum EOpinionReactionType { };
enum EOpinionTargetingConditionType { };
'''
def ERASE(addr, nbytes, key, val, kc, vc, member, what, was, pragma=''):
    leaf = '?_M_erase@?$_Rb_tree@W4%s@@U?$pair@$$CBW4%s@@%s@std@@U?$_Select1st@U?$pair@$$CBW4%s@@%s@std@@@3@U?$less@W4%s@@@3@V?$allocator@U?$pair@$$CBW4%s@@%s@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBW4%s@@%s@std@@@2@@Z' % (key, key, vc, key, vc, key, key, vc, key, vc)
    return '''// std::_Rb_tree<%s, std::pair<const %s, %s>, std::_Select1st<std::pair<const %s, %s> >, std::less<%s>, std::allocator<std::pair<const %s, %s> > >::_M_erase(_Rb_tree_node<..>* x) -- STLport 4.6 recursive erase without rebalancing: erase the right subtree, destroy the node (trivial value), continue with the left
// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %s.
// Types (naming wave 20, chunk 688): the map<%s, %s> %s of COpinionReactionManagerDef -- the landed ~COpinionReactionManagerDef 0x00678460 (chunk 688)
//   erases it from the member's inline clear() (%s); the key and value are trivially destroyed, the node (0x18) freed. Was %s.
// Model: STLport 4.6 stl/_tree.h + stl/_tree.c text in namespace std; explicit instantiation emits the member as its own COMDAT, as retail kept every
//   used inline member (no /OPT:REF).
// Leaf: %s
%s''' % (key, key, val, key, val, key, key, val, addr.upper(), nbytes, key, val, member, what, was, leaf, pragma) + PRE + ENUMS + BODY + '''typedef pair<const %s, %s> _Val;
template void _Rb_tree<%s, _Val, _Select1st<_Val>, less<%s>, allocator<_Val> >::_M_erase(_Rb_tree_node<_Val>*);
}
''' % (key, val, key, key)
P('004507fb', ERASE('004507fb', 'the size-optimised body (pragma optimize s)', 'EOpinionReactionType', 'bool', 'W4', '_N', 'maps', 'the four maps at +0x78 / +0x84 / +0x90 / +0x9C: BlockFurtherReactions, AllowSpeechOnNonPureAISpeaker, AllowWhileCarrying, AllowWhileFollowingPlayer', 'the propagated LTextBinTree / LTextTreeWalkThrough::BuildTreeArray', '#pragma optimize("s",on)\n'))
P('004da360', ERASE('004da360', 'the size-optimised body (pragma optimize s)', 'EOpinionAttitudeType', 'float', 'W4', 'M', 'ToleranceToBeingHit', 'the map<EOpinionAttitudeType, float> at +0x6C', 'the propagated LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray', '#pragma optimize("s",on)\n'))
P('0067a840', ERASE('0067a840', 'oracle length 51 bytes', 'EOpinionReactionType', 'float', 'W4', 'M', 'PreReactionDelay', 'the map<EOpinionReactionType, float> at +0x60', 'the descriptive FreeTree_CTreeOwner_0067a840 (one of 23 bodies of this shape)'))
P('0067a800', ERASE('0067a800', 'oracle length 51 bytes', 'EOpinionReactionType', 'EOpinionTargetingConditionType', 'W4', 'W4EOpinionTargetingConditionType@@', 'TargetingCondition', 'the map<EOpinionReactionType, EOpinionTargetingConditionType> at +0x54', 'the descriptive FreeTree_CTreeOwner_0067a800 (one of 23 bodies of this shape)'))
P('0067a7c0', ERASE('0067a7c0', 'oracle length 51 bytes', 'EOpinionReactionType', 'EOpinionAttitudeType', 'W4', 'W4EOpinionAttitudeType@@', 'AttitudeCondition', 'the map<EOpinionReactionType, EOpinionAttitudeType> at +0x48', 'the descriptive FreeTree_CTreeOwner_0067a7c0 (one of 23 bodies of this shape)'))

P('009fc550', '''// CResource::~CResource() -- the virtual destructor of the resource base: unlink from the owning resource list when there is one (CResourceList::RemoveResource), then the inline ~CIVCountedPointeeBase
// Leaf: ??1CResource@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC550, 31 bytes, thiscall, plain `ret`.
// Identity (naming wave 20, chunk 688): FableWin.pdb ??1CResource@@UAE@XZ; engine/CResource.h (RTTI CResource : CIVCountedPointeeBase; vtable
//   0x012354A4 stored at entry, the base's 0x012354D0 at the end by the inline base destructor); the +8 member is ResourceList (Ego_r.pdb)
//   and the thiscall it makes with `this` pushed is ?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z 0x009FC020 (landed; both name sets).
//   Every def destructor tail-jumps here after its CDefPointeeBase vtable store (218 landed rows bind this spelling). Was the ~CMemoryBlock
//   stand-in `CMemoryBlock_dtor_009fc550` (a __fastcall free function over raw vtable stores).
// Callees: CResourceList::RemoveResource @ 0x009FC020.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CResource.h"                        // : CIVCountedPointeeBase (inline virtual destructor)

class CResourceList {
public:
    void RemoveResource(CResource& resource);        // 0x009FC020
};

CResource::~CResource()
{
    if (ResourceList != 0)
        ResourceList->RemoveResource(*this);
}
''')
a = '009fc550'
if a not in _X:
    for old, new in (('CMemoryBlockOwner', 'CResourceList'), ('CMemoryBlock', 'CResource'), ('BlockData', 'ResourceList'), ('Free', 'RemoveResource'), ('g_freed', 'g_removed'), ('~CMemoryBlock', '~CResource')):
        he.resub(a, 'tst', r'\b' + re.escape(old) + r'\b', new)
