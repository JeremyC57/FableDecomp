# item 2 (chunk 575-585 / mig queue rows), hand part: respellings proven by the queue evidence and both PDB name lists
# (each final spelling is in Ego_r.pdb and/or FableWin.pdb as noted; code unchanged: renames, access, class key, a static
# member called like the free function, a free function called like the static member)
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, resub
# 0x00D7C470 ?PlayCutscene@CQ_HeroSoulsThunderScript@NScript@@QAEXVCCharString@@@Z (Ego_r+FableWin; lower-case s)
resub('00d7ae80', 'src', r'\bPlayCutScene\b', 'PlayCutscene', 4)
# 0x00E1D7B0 ?CutsceneMazeDeath@CQ_WizardBattleScript@NScript@@QAEXXZ (Ego_r order CleanUp -> CutsceneMazeDeath = retail 0x00E1D690 -> 0x00E1D7B0)
resub('00e1d640', 'src', r'\bFinishMazeStage\b', 'CutsceneMazeDeath')
# 0x00EC1B70 ?Alloc@CRandomPopulationCreature@CV_RandomPopulationSimScript@NScript@@... (Ego_r+FableWin; the Alloc row's own Leaf)
resub('00ec1910', 'src', r'\bCRandomPopulationEntity\b', 'CRandomPopulationCreature')
# 0x00D18CA0 STLport 4.6 vector::_M_insert_overflow is protected (IAE; the twin 0x00F0D4E0 is bound IAE)
edit('00d18c00', 'src', '    T* _M_end_of_storage;\n    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00D18CA0\n    void push_back',
     '    T* _M_end_of_storage;\nprotected:\n    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00D18CA0\npublic:\n    void push_back')
# 0x00F3CF50 ?CalcSectionPolyMask@CTriangleStripifier2@@AAEJ... (Ego_r: private, the only spelling in both name lists)
edit('00f3d830', 'src', '    void* __vftable;                                           // +0x00 (4 bytes, Ego_r.pdb)\n    long CalcSectionPolyMask(',
     '    void* __vftable;                                           // +0x00 (4 bytes, Ego_r.pdb)\nprivate:\n    long CalcSectionPolyMask(')
edit('00f3d830', 'src', '                             const CArray<unsigned short>& section, CArray<bool>& polyMask);            // 0x00F3CF50\n',
     '                             const CArray<unsigned short>& section, CArray<bool>& polyMask);            // 0x00F3CF50\npublic:\n')
# 0x00E54CA0 ??0CConversation@@QAE@ABV0@@Z (FableWin; bound from 0x00E54AA0 / 0x00F33A10) and 0x00F27060
# ??0CHeroSoulsCreatureDef@@QAE@ABV0@@Z (FableWin; Ego_r CHeroSoulsCreatureDef 52 bytes): the element classes of the fill helpers
for a, old, new in (('00f339e0', 'CElement92_00e54ca0', 'CConversation'), ('00f279e0', 'CElement52_00f27060', 'CHeroSoulsCreatureDef')):
    resub(a, 'src', r'\bstruct ' + old + r' \{', 'class %s { public:' % new, 1)
    resub(a, 'src', r'\b' + old + r'\b', new)
# 0x00431F60 ??4CDefPointeeBase@@QAEAAV0@ABV0@@Z (FableWin 01937730; CDefClassBase : CDefPointeeBase, rtti_bases.tsv; migration wave 3)
for a in ('00432c70', '00432e60', '00434450', '004345a0'):
    resub(a, 'src', r'\bstruct CDefRootBase \{', 'class CDefPointeeBase { public:', 1)
    resub(a, 'src', r'\bCDefRootBase\b', 'CDefPointeeBase')
# 0x0099F830 ?GFIntToCharString@@YI?AVCCharString@@J@Z (Ego_r+FableWin): a free __fastcall function, not a CCharString static
for a in ('009ac9f0', '009baa90'):
    resub(a, 'src', r'\n[ \t]*static CCharString __fastcall FromLong\(long value\);[^\n]*\n\};', '\n};\nCCharString __fastcall GFIntToCharString(long value);                     // 0x0099F830', 1)
    resub(a, 'src', r'\bCCharString::FromLong\(', 'GFIntToCharString(')
# 0x00BEBAA0 ?CalcObjectMatrix@CEngineInternalPrimitiveMeshBase@@SIXAAVCMatrix3x4@@ABVC3DVector@@ABVCRightHandedSet@@M@Z (Ego_r: static
# member, __fastcall: matrix in ecx, position in edx, orientation and scale on the stack, ret 8)
for a in ('00bcef80', '00bcf760', '00bcf930'):
    resub(a, 'src', r'\nvoid __fastcall CalcObjectMatrix\(CMatrix3x4& out, const C3DVector& position, const CRightHandedSet& orientation, float scale\);(\s*// 0x00BEBAA0)',
          '\nclass CEngineInternalPrimitiveMeshBase { public:\n    static void __fastcall CalcObjectMatrix(CMatrix3x4& out, const C3DVector& position, const CRightHandedSet& orientation, float scale);\\1\n};', 1)
    resub(a, 'src', r'(?<![\w:])CalcObjectMatrix\((?!CMatrix3x4&)', 'CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(', 1)
