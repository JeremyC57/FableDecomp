# sub-wave 1b (hand): CAnimInit::Init, GFIntToWideString. HE_OV=<overlay>
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, resub
# ---- 0x0070C050 ?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z (Ego_r + FableWin; no CAnimInit::Set in either PDB; landed
#      0x008619C0 calls it so): the inline 7-argument constructor calls Init.
INIT = '    void Init(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping, unsigned long flags, long priority);   // 0x0070C050 (Ego_r.pdb CAnimInit::Init)'
ENUM = 'enum EAnimUpdateMode { ANIM_UPDATE_MODE_0, ANIM_UPDATE_MODE_1, ANIM_UPDATE_MODE_2 };   // Ego_r.pdb enum (the Init parameter type)\n'
edit('008d88a0', 'src', '    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050', INIT)
edit('008d88a0', 'src', '        Set(animType, arg24, animIndex, mode, flag05, flags, percent);', '        Init((unsigned char)animType, arg24, animIndex, (EAnimUpdateMode)mode, flag05, flags, percent);')
edit('008d88a0', 'src', 'class CAnimInit { public:                         // 0x2c bytes', ENUM + 'class CAnimInit { public:                         // 0x2c bytes')
edit('008d6780', 'src', '    void Set(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags, long priority);   // 0x0070C050', INIT)
edit('008d6780', 'src', '        Set(type, typeFlags, anim, updateMode, looping, flags, priority);', '        Init(type, typeFlags, anim, (EAnimUpdateMode)updateMode, looping, flags, priority);')
edit('008d6780', 'src', 'class CAnimInit { public:                         // Ego_r.pdb CAnimInit (0x2c)', ENUM + 'class CAnimInit { public:                         // Ego_r.pdb CAnimInit (0x2c)')
edit('00861a80', 'src', '    void Set(unsigned char type, long mode, long anim, long a, bool b, unsigned long flags, long priority);   // 0x0070C050', INIT)
edit('00861a80', 'src', '        Set(type, mode, anim, a, b, flags, priority);', '        Init(type, mode, anim, (EAnimUpdateMode)a, b, flags, priority);')
edit('00861a80', 'src', 'class CAnimInit { public:\n    long           UpdateMode;                      // +0x00', ENUM + 'class CAnimInit { public:\n    long           UpdateMode;                      // +0x00')
# ---- 0x00405F00 ?GFIntToWideString@@YI?AVCWideString@@J@Z (Ego_r + FableWin; landed 0x00558170 binds it)
edit('005a45f6', 'src', 'CWideString __fastcall IntToWideString(long value);                                 // 0x00405F00', 'CWideString __fastcall GFIntToWideString(long value);                               // 0x00405F00 (Ego_r.pdb)')
resub('005a45f6', 'src', r'(?<!GF)\bIntToWideString\((10|50|count)\)', r'GFIntToWideString(\1)', 3)
# ---- 0x00A14440 ?AccurateNormalise@C3DVector@@QAEAAV1@XZ (Ego_r + FableWin; the body is fsqrt + 1/len: the accurate form; the
#      Ego_r out-of-line name set has no C3DVector::Normalise -- Normalise is the inline FastInvSqrt form, GOTCHAS chunk 278)
edit('007bebe0', 'src', '    void Normalise();                                   // 0x00A14440', '    C3DVector& AccurateNormalise();                     // 0x00A14440 (Ego_r.pdb; the sqrt form)')
edit('007bebe0', 'src', '        axis.Normalise();', '        axis.AccurateNormalise();')
edit('0075fe30', 'src', '    C3DVector& Normalise();                       // 0x00A14440', '    C3DVector& AccurateNormalise();               // 0x00A14440 (Ego_r.pdb; the sqrt form)')
edit('0075fe30', 'src', '    link1.Normalise();', '    link1.AccurateNormalise();')
edit('0075fe30', 'src', '    link2.Normalise();', '    link2.AccurateNormalise();')
