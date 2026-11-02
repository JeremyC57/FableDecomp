# h28_c697.py: naming wave 28, family c697 (chunk 697 queue rows with one binder each):
#  0x00B81E20 ?Clear@CEnginePrimitiveSimulationInfo@@QAEXXZ (own row; Ego_r 0x00A29FB0 / FableWin 0x02D7CC40, donor order after the ctor);
#  0x00BDDFB0 ??1CQuadTreeElement@CLocalDetailCacheMap@@UAE@XZ (RTTI .?AVCQuadTreeElement@CLocalDetailCacheMap@@; binder 0x00BDFA10);
#  0x00A60CE0 ??1CChecksumCacheFile@@QAE@XZ with its ctor 0x00A60C90 ??0CChecksumCacheFile@@QAE@J@Z and Open 0x00A60D40
#  ?Open@CChecksumCacheFile@@QAEXABVCWideString@@@Z (Ego_r / FableWin; landed binders) -- binder 0x009B05F0's CChecksumFile is that class.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
F = 'c697'
a = '00b81e20'
he.edit(a, 'src', '// CEnginePrimitiveSimulationInfo::ClearSimulations() -- empties', '// CEnginePrimitiveSimulationInfo::Clear() -- empties')
he.edit(a, 'src', '    void ClearSimulations();\n};\n\nvoid CEnginePrimitiveSimulationInfo::ClearSimulations()', '    void Clear();\n};\n\nvoid CEnginePrimitiveSimulationInfo::Clear()')
note(a, F, 'own row is ?Clear@CEnginePrimitiveSimulationInfo@@QAEXXZ (chunk 697: Ego_r.pdb 0x00A29FB0 /\n'
     '//   FableWin.pdb 0x02D7CC40; Ego_r donor order ctor -> Clear = retail ctor 0x00B81E00 -> 0x00B81E20; the landed caller\n'
     '//   ~CEngineInternalPrimitiveMeshBase already binds it). Was the descriptive ClearSimulations.', leaf='?Clear@CEnginePrimitiveSimulationInfo@@QAEXXZ')

a = '00bdfa10'
he.edit(a, 'src', '''class CLocalDetailCacheMap;
// Native RTTI: QuadTreeElement -> CMovableResource; four genuine slots.''', '''class CLocalDetailCacheMap { public:
 class CQuadTreeElement; // nested (RTTI .?AVCQuadTreeElement@CLocalDetailCacheMap@@; Ego_r / FableWin)
};
// Native RTTI: CLocalDetailCacheMap::CQuadTreeElement -> CMovableResource; four genuine slots.''')
he.edit(a, 'src', '''struct QuadTreeElement:MovableNative {
 QuadTreeElement(CLocalDetailCacheMap*,long,long,long,long); // PDB00a8f6f0 donor signature
 virtual ~QuadTreeElement(); //0x00bddfb0, VT012a7df0 slot0''', '''class CLocalDetailCacheMap::CQuadTreeElement:public MovableNative { public:
 CQuadTreeElement(CLocalDetailCacheMap*,long,long,long,long); // PDB00a8f6f0 donor signature
 virtual ~CQuadTreeElement(); //0x00bddfb0, VT012a7df0 slot0''')
he.edit(a, 'src', '''void* QuadTreeElement::ScalarDeletingDestructor(unsigned flags) {
 this->QuadTreeElement::~QuadTreeElement();''', '''void* CLocalDetailCacheMap::CQuadTreeElement::ScalarDeletingDestructor(unsigned flags) {
 this->CLocalDetailCacheMap::CQuadTreeElement::~CQuadTreeElement();''')
note(a, F, 'the destructor 0x00BDDFB0 is ??1CQuadTreeElement@CLocalDetailCacheMap@@UAE@XZ (chunk 697: vtable\n'
     '//   0x012A7DF0 RTTI .?AVCQuadTreeElement@CLocalDetailCacheMap@@; Ego_r.pdb + FableWin.pdb), the class nested in CLocalDetailCacheMap.\n'
     '//   Was a global QuadTreeElement (??1QuadTreeElement@@UAE@XZ).')

a = '009b05f0'
he.edit(a, 'src', '''class CChecksumCacheFile { public:
    unsigned long GetChecksum(CWideString const&a0);   // 0x00A60FB0 (Ego_r.pdb ?GetChecksum@CChecksumCacheFile@@QAEKABVCWideString@@@Z)
};
class CChecksumFile {
public:''', '''class CChecksumCacheFile {
public:''')
he.edit(a, 'src', '''    CChecksumFile(long bufferSize);                     // 0x00A60C90
    ~CChecksumFile();                                   // 0x00A60CE0
    void Open(const CWideString& path);                 // 0x00A60D40''', '''    CChecksumCacheFile(long bufferSize);                // 0x00A60C90
    ~CChecksumCacheFile();                              // 0x00A60CE0
    void Open(const CWideString& path);                 // 0x00A60D40
    unsigned long GetChecksum(CWideString const&a0);   // 0x00A60FB0 (Ego_r.pdb ?GetChecksum@CChecksumCacheFile@@QAEKABVCWideString@@@Z)''')
he.edit_all(a, 'src', 'CChecksumFile checksums', 'CChecksumCacheFile checksums')
note(a, F, 'the checksum file is CChecksumCacheFile (chunk 697: ~CChecksumCacheFile 0x00A60CE0, Ego_r\n'
     '//   0x00C00700 after Close; ctor 0x00A60C90 ??0CChecksumCacheFile@@QAE@J@Z and Open 0x00A60D40 as the landed binders). Was a\n'
     '//   separate CChecksumFile class (??0 / ??1 / ?Open@CChecksumFile@@).')
