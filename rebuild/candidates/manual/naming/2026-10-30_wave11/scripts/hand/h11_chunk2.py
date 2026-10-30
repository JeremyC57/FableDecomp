# sub-wave chunk (hand, part 2): Ego_r.pdb + FableWin.pdb backed chunk 587 / 592 callee spellings with few emitters.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
# ---- 0x00AE80F0 ?reserve@?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@std@@@std@@QAEXI@Z (Ego_r + FableWin; chunk 587): the STLport
#      reserve body over 16-byte elements (`sar eax,4`; C3DGroup2 = BoneIndex + CArray<C3DVertexBlend2> = 0x10 in Ego_r.pdb); its caller
#      0x00AE3330 named it CVector16::Reserve.
edit('00ae3330', 'src', '''struct SElement16 { unsigned long m_words[4]; };

struct CVector16 {
    SElement16* m_pFirst;
    SElement16* m_pLast;
    SElement16* m_pEnd;
    void Reserve(unsigned long count);          // 0x00AE80F0
};
''', '''class C3DGroup2 { public: unsigned long m_words[4]; };   // 16 bytes (Ego_r.pdb: BoneIndex + CArray<C3DVertexBlend2>)
namespace std {
template <class T> class allocator { };
template <class T, class A> class vector { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void reserve(unsigned int count);           // 0x00AE80F0 ?reserve@?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@std@@@std@@QAEXI@Z (Ego_r / FableWin)
};
}
typedef std::vector<C3DGroup2, std::allocator<C3DGroup2> > CVector16;
''')
edit('00ae3330', 'src', '''    m_first.Reserve(count);
    m_second.Reserve(count);''', '''    m_first.reserve(count);
    m_second.reserve(count);''')
edit('00ae3330', 'src', '''// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE3330, 37 bytes (+11 int3 padding). Callee: the 16-byte-element
// vector `reserve(n)` @ 0x00AE80F0''', '''// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE3330, 37 bytes (+11 int3 padding). Callee: the 16-byte-element
// std::vector<C3DGroup2>::reserve(n) @ 0x00AE80F0 (naming wave 11)''')
# ---- 0x00AE83D0 ?push_back@?$CArray@VC3DGroup2@@@@QAEXABVC3DGroup2@@@Z (Ego_r + FableWin; chunk 587): 0x00AE8490 named it
#      CArrayGroup2::push_back; the descriptive owner of AddDefault now derives from the CArray<C3DGroup2> template (no this adjustment).
edit('00ae8490', 'src', '''struct CArrayGroup2 {                                     // retail CArray<C3DGroup2>
    C3DGroup2* m_pFirst;                                  // +0x00
    C3DGroup2* m_pLast;                                   // +0x04
    C3DGroup2* m_pEnd;                                    // +0x08
    void push_back(const C3DGroup2& value);               // 0x00AE83D0
    void AddDefault();
};''', '''template <class T> class CArray { public:               // retail CArray<T> (12 bytes)
    T* m_pFirst;                                          // +0x00
    T* m_pLast;                                           // +0x04
    T* m_pEnd;                                            // +0x08
    void push_back(const T& value);                       // 0x00AE83D0 for C3DGroup2: ?push_back@?$CArray@VC3DGroup2@@@@QAEXABVC3DGroup2@@@Z (Ego_r / FableWin)
};
struct CArrayGroup2 : public CArray<C3DGroup2> {          // retail CArray<C3DGroup2>; AddDefault keeps its descriptive owner
    void AddDefault();
};''')
# ---- 0x00AF2250 ?GetSystemTransform@CParticleSystem@NParticleEngine@@QBEXAAVCMatrix3x4@@@Z and 0x00AF22C0
#      ?GetInverseSystemTransform@CParticleSystem@NParticleEngine@@QBEXAAVCMatrix3x4@@@Z (Ego_r 0x00AB5800 / 0x00AB5880 + FableWin, both
#      const, the same order in retail); 0x00AF2640 CPSCBase::GetCurrentPosition calls 0x00AF2250 (local to world), 0x00AF2780
#      CPSCBase::GetCurrentPositionLocal (Ego_r 0x00AB5A70 + FableWin) calls 0x00AF22C0. Both rows spelled their callee
#      GetInverseSystemTransform QAE (one name over two addresses). CParticleUpdateParameters is a class template of NParticleEngine
#      (?GetUpdateParam@?$CParticleUpdateParameters@KK@NParticleEngine@@QBE_NAAVC3DVector@@ABKM_NK@Z, Ego_r 0x00AB60B0 + FableWin).
for a in ('00b18af0', '00b16d50'):
    edit(a, 'src', 'template <class A, class B> struct CParticleUpdateParameters;\n', '')
    edit(a, 'src', 'namespace NParticleEngine {\n', 'namespace NParticleEngine {\n\ntemplate <class A, class B> class CParticleUpdateParameters;   // NParticleEngine (Ego_r / FableWin)\n')
edit('00b18af0', 'src', '    void GetInverseSystemTransform(CMatrix3x4& matrix);         // 0x00AF2250',
     '    void GetSystemTransform(CMatrix3x4& matrix) const;          // 0x00AF2250 ?GetSystemTransform@CParticleSystem@NParticleEngine@@QBEXAAVCMatrix3x4@@@Z (Ego_r / FableWin)')
edit('00b18af0', 'src', '            system.GetInverseSystemTransform(m);', '            system.GetSystemTransform(m);')
edit('00b18af0', 'src', 'CParticleSystem::GetInverseSystemTransform(CMatrix3x4&) @ 0x00AF2250', 'CParticleSystem::GetSystemTransform(CMatrix3x4&) const @ 0x00AF2250')
edit('00b16d50', 'src', '''    C3DVector GetCurrentPosition(const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                 const unsigned long& param, float scale, bool local, unsigned long flags) const;  // 0x00AF2780''',
     '''    C3DVector GetCurrentPositionLocal(const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                 const unsigned long& param, float scale, bool local, unsigned long flags) const;  // 0x00AF2780 (Ego_r / FableWin)''')
edit('00b16d50', 'src', '    C3DVector centre = GetCurrentPosition(params, CentreParam, 1.0f, false, 0);', '    C3DVector centre = GetCurrentPositionLocal(params, CentreParam, 1.0f, false, 0);')
edit('00b16d50', 'src', 'CPSCBase::GetCurrentPosition(params, const unsigned long& param, float, bool, unsigned long) const @ 0x00AF2780',
     'CPSCBase::GetCurrentPositionLocal(params, const unsigned long& param, float, bool, unsigned long) const @ 0x00AF2780')
# the defining row 0x00AF2780
A = '00af2780'
edit(A, 'src', '''// 00af2780 NParticleEngine::CPSCBase::GetCurrentPosition (catalog: CParticleUpdateParameters_TransformPosition; symbol of caller 00b16d50)''',
     '''// 00af2780 NParticleEngine::CPSCBase::GetCurrentPositionLocal (catalog: CParticleUpdateParameters_TransformPosition)
// Identity (naming wave 11): Ego_r.pdb 0x00AB5A70 + FableWin.pdb ?GetCurrentPositionLocal@CPSCBase@NParticleEngine@@QBE?AVC3DVector@@ABV?$CParticleUpdateParameters@KK@2@ABKM_NK@Z;
// its sibling 0x00AF2640 (GetCurrentPosition) applies GetSystemTransform 0x00AF2250, this body GetInverseSystemTransform 0x00AF22C0.''')
edit(A, 'src', '// Leaf: GetCurrentPosition\n', '// Leaf: ?GetCurrentPositionLocal@CPSCBase@NParticleEngine@@QBE?AVC3DVector@@ABV?$CParticleUpdateParameters@KK@2@ABKM_NK@Z\n')
edit(A, 'src', '''template<class A,class B> struct CParticleUpdateParameters {bool GetUpdateParam(C3DVector&,const unsigned long&,float,bool,unsigned long)const;};
namespace NParticleEngine {''', '''namespace NParticleEngine {
template<class A,class B> class CParticleUpdateParameters { public: bool GetUpdateParam(C3DVector&,const unsigned long&,float,bool,unsigned long)const;};''')
edit_all(A, 'src', 'GetInverseSystemTransform(CMatrix3x4&);', 'GetInverseSystemTransform(CMatrix3x4&)const;', 1)
edit_all(A, 'src', 'C3DVector GetCurrentPosition(const CParticleUpdateParameters<unsigned long,unsigned long>&,', 'C3DVector GetCurrentPositionLocal(const CParticleUpdateParameters<unsigned long,unsigned long>&,', 1)
edit_all(A, 'src', 'C3DVector CPSCBase::GetCurrentPosition(const', 'C3DVector CPSCBase::GetCurrentPositionLocal(const', 1)
from he import resub
# ---- libpk / GameCODA (no PDB): one retail address = one spelling; the caller takes the defining row's (or its class's other
#      landed rows') spelling. Evidence per item in the queue (chunks 589 / 591 / 592) plus the defining row named here.
# 0x00C57B60 ?QueueChunk@CVtable_012b8ed4@@QAEXXZ (its defining row's leaf); the caller 0x00C57D80 used the placeholder QueueChunk_00c57b60
edit('00c57d80', 'src', '    void QueueChunk_00c57b60();                     // 0x00C57B60', '    void QueueChunk();                              // 0x00C57B60 (its defining row\'s leaf)')
edit('00c57d80', 'src', '    QueueChunk_00c57b60();', '    QueueChunk();')
# 0x00C75FF0 / 0x00C76120 CCombFilter::Process / Blend take the landed CRingBuffer (the shared line goes straight to the CDspOps
#      ring helpers, 0x00C76680): the caller 0x00C69FB0 called the same type CDelayLine
resub('00c69fb0', 'src', r'\bCDelayLine\b', 'CRingBuffer')
# 0x00B6C700 ?_M_fill_insert@?$vector@UCLandscapeTheme@@...@std@@QAEXPAUCLandscapeTheme@@IABU3@@Z: STLport 4.6 _vector.h declares
#      _M_fill_insert public (the defining row 0x00B6C700 and the landed 0x00AE7340 / 0x00B7CAB0 / 0x00BCDCA0 are QAE)
edit('00b6cb20', 'src', '''    void resize(size_type __new_size, const _Tp& __x);
protected:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // 0x00B6C700''', '''    void resize(size_type __new_size, const _Tp& __x);
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // 0x00B6C700 (public in STLport 4.6 _vector.h: QAE)''')
# 0x00C1A110 / 0x00C1A070 PKCDefaultFileManager (trace .\\PKCDefaultFileManager.cpp; the landed dtor 0x00C1A230
#      ??1PKCDefaultFileManager@@UAE@XZ, vtable 0x012B30E4) and its open-file map PKCTRRedblackMap (the landed ctor 0x00C1AE20
#      ??0PKCTRRedblackMap@@QAE@PAVCMapNodeAllocator@@@Z; Find 0x00C1AE50): the rows 0x00C32F60 / 0x00C32F40 / 0x00C1A070 / 0x00C1AE50
#      named them CFileManager_00c1a070 / COpenFileMap_00c1a070 / PKCTRRedblackMap_00c1ae50
for a in ('00c32f60', '00c32f40', '00c1a070'):
    resub(a, 'src', r'\bCFileManager_00c1a070\b', 'PKCDefaultFileManager')
resub('00c1a070', 'src', r'\bCOpenFileMap_00c1a070\b', 'PKCTRRedblackMap')
edit('00c1a070', 'src', '// Leaf: GetFileSize@PKCDefaultFileManager\n', '// Leaf: ?GetFileSize@PKCDefaultFileManager@@QAE_NKPAK@Z\n')
edit('00c1a070', 'src', '// Label: the catalog\'s `FileManager_GetFileSizeByKey` is descriptive; the class is named by address.',
     '// Label: the catalog\'s `FileManager_GetFileSizeByKey` is descriptive; the class is PKCDefaultFileManager (naming wave 11: the trace\n//   .\\PKCDefaultFileManager.cpp and the landed dtor 0x00C1A230 on the same object), its +0x20 map PKCTRRedblackMap (ctor 0x00C1AE20).')
resub('00c1ae50', 'src', r'\bPKCTRRedblackMap_00c1ae50\b', 'PKCTRRedblackMap')
edit('00c1ae50', 'src', '// Leaf: Find\n', '// Leaf: ?Find@PKCTRRedblackMap@@QAEJPBKPAPAX@Z\n')
edit('00c1ae50', 'src', '(0x00C1A070). No PDB name set holds a PKCTR symbol, so the classes are named by address.',
     '(0x00C1A070). No PDB name set holds a PKCTR symbol; the map is PKCTRRedblackMap as its landed ctor 0x00C1AE20 spells it\n//   (naming wave 11), the node and allocator stay named by address.')
# 0x00C25A50 ?InitiateShutdown@PKCTimerManager@@QAEXXZ (its defining row; trace PKCTimerManager.cpp line 33 "Shutdown already
#      initiated"): 0x00C25C20 runs it on its +0 PKCTimerManager member (0x14 bytes), not on itself as CloseTimers
edit('00c25c20', 'src', '''struct CTimerHost_00c25c20 {
    unsigned char           _pad[0x14];
    CScriptModule_00c354a0* m_pModule;             // +0x14
    void CloseTimers();                            // 0x00C25A50
    void Close();
};''', '''struct PKCTimerManager {                           // +0 of the host, 0x14 bytes
    unsigned char _body[0x14];
    void InitiateShutdown();                       // 0x00C25A50 ?InitiateShutdown@PKCTimerManager@@QAEXXZ (its defining row)
};

struct CTimerHost_00c25c20 {
    PKCTimerManager         m_timers;              // +0x00
    CScriptModule_00c354a0* m_pModule;             // +0x14
    void Close();
};''')
edit('00c25c20', 'src', '''    CloseTimers();
    CScriptModule_00c354a0* module''', '''    m_timers.InitiateShutdown();
    CScriptModule_00c354a0* module''')
edit('00c25c20', 'src', '// Callees: the close @ 0x00C25A50 (on `this`; manifest "PKCTimerManager_Close")', '// Callees: PKCTimerManager::InitiateShutdown @ 0x00C25A50 (on the +0 timer manager; manifest "PKCTimerManager_Close")')
