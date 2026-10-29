# wave 6: the CUserProfileManager option setter run 0x0040AF90..0x0040B750 (32 rows), whole-file rewrites of source and
# test (HE_OV=<overlay>). Every body is `if (UseNewProfileValues) XNewProfile = value; X = value; ApplyValues();
# if (save) Save();` (retail bytes: the +0x0C flag test, two stores, call 0x00407370, call 0x0040A740 on the bool).
# Names: Ego_r.pdb / FableWin.pdb `?Set<X>@CUserProfileManager@@QAEX<T>_N@Z` (bool: `_N0`), X chosen by the stored field:
# struct_layouts_egor CUserProfileManager, retail = PDB offset below the first STL vector (+0x54), PDB - 8 after the two
# AssignedInputs vectors, PDB - 4 in the new-profile block (each STLport vector is 4 bytes smaller); both offsets agree
# for every row. Callees: ApplyValues 0x00407370 (?ApplyValues@CUserProfileManager@@IAEXXZ, protected) and Save
# 0x0040A740 (?Save@CUserProfileManager@@QAEXXZ; it builds the profile path and writes the file through CMemoryFile).
# Before: ten rows called Save as Apply / NotifyReset / sub2 / Helper2 / free UPM_* or CUserProfileManager_Helper*
# stand-ins (one, 0x0040B050, bound ?Apply@CUserProfileManager@@QAEXXZ to 0x0040A740: the RELOC_CONFLICT with the real
# Apply 0x0040BC80), and several defined themselves as free __fastcall functions or under propagated labels
# (0x0040B310 / 0x0040B3D0 "SetAntialiasing" are SetHUDAlpha / SetDialogueVolume, 0x0040B490 "SetCameraResetting" is
# SetShowTargetingStatus, 0x0040B710 "SetAntialiasing" is SetScreenAspectRatio).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import path, R
T = {'b': ('bool', '_N0', 1), 'd': ('unsigned long', 'K_N', 4), 'f': ('float', 'M_N', 4)}
ROWS = [  # address, member, type, retail offset, new-profile retail offset
    ('0040af90', 'SubtitlingEnabled', 'b', 0x0d, 0xcd), ('0040afd0', 'VibrationEnabled', 'b', 0x0e, 0xce),
    ('0040b010', 'CameraXYInverted', 'b', 0x0f, 0xcf), ('0040b050', 'CameraYZInverted', 'b', 0x10, 0xd0),
    ('0040b090', 'CameraFPBowXYInverted', 'b', 0x11, 0xd1), ('0040b0d0', 'CameraFPBowYZInverted', 'b', 0x12, 0xd2),
    ('0040b110', 'CameraPCBowXYInverted', 'b', 0x13, 0xd3), ('0040b150', 'CameraPCBowYZInverted', 'b', 0x14, 0xd4),
    ('0040b190', 'CameraPCViewHeroXYInverted', 'b', 0x15, 0xd5), ('0040b1d0', 'CameraPCViewHeroYZInverted', 'b', 0x16, 0xd6),
    ('0040b210', 'TutorialSystemEnabled', 'b', 0x17, 0xd7), ('0040b250', 'GuildMasterEnabled', 'b', 0x18, 0xd8),
    ('0040b290', 'HUDTooltipEnabled', 'b', 0x19, 0xd9), ('0040b2d0', 'Gamma', 'f', 0x1c, 0xdc),
    ('0040b310', 'HUDAlpha', 'f', 0x20, 0xe0), ('0040b350', 'MusicVolume', 'f', 0x28, 0xe8),
    ('0040b390', 'SoundVolume', 'f', 0x24, 0xe4), ('0040b3d0', 'DialogueVolume', 'f', 0x2c, 0xec),
    ('0040b410', 'UsingHeadPhones', 'b', 0x30, 0xf0), ('0040b450', 'ShowingBuddyNames', 'b', 0x32, 0xf1),
    ('0040b490', 'ShowTargetingStatus', 'b', 0x6c, 0x134), ('0040b4d0', 'Antialiasing', 'd', 0x38, 0xf4),
    ('0040b510', 'CameraResetting', 'b', 0x79, 0x12c), ('0040b550', 'CameraSensitivity', 'f', 0x7c, 0x130),
    ('0040b590', 'ShadowDetail', 'f', 0x40, 0xfc), ('0040b5d0', 'TextureDetail', 'f', 0x3c, 0xf8),
    ('0040b610', 'MeshDetail', 'f', 0x44, 0x100), ('0040b650', 'EffectsDetail', 'f', 0x70, 0x124),
    ('0040b690', 'Resolution', 'd', 0x48, 0x104), ('0040b6d0', 'RefreshRate', 'd', 0x4c, 0x108),
    ('0040b710', 'ScreenAspectRatio', 'd', 0x74, 0x128), ('0040b750', 'VerticalSyncEnabled', 'b', 0x50, 0x10c),
]
def model(name, ty, off, npo):
    t, _, sz = T[ty]
    return ('class CUserProfileManager {\npublic:\n'
            '    unsigned char _pad00[0x0C];\n'
            '    bool          UseNewProfileValues;      // +0x0C\n' +
            ('    unsigned char _pad0D[0x%X];\n' % (off - 0x0d) if off > 0x0d else '') +
            '    %-13s %s;%s// +0x%02X\n' % (t, name, ' ' * max(1, 26 - len(name)), off) +
            '    unsigned char _pad%02X[0x%X];\n' % (off + sz, npo - off - sz) +
            '    %-13s %sNewProfile;%s// +0x%X\n' % (t, name, ' ' * max(1, 16 - len(name)), npo) +
            '    void Set%s(%s value, bool save);\n' % (name, t) +
            '    void Save();                                 // 0x0040A740\n'
            'protected:\n'
            '    void ApplyValues();                          // 0x00407370\n};\n')
def body(name, t):
    return ('void CUserProfileManager::Set%s(%s value, bool save)\n{\n    if (UseNewProfileValues)\n        %sNewProfile = value;\n'
            '    %s = value;\n    ApplyValues();\n    if (save)\n        Save();\n}\n') % (name, t, name, name)
def source(a, name, ty, off, npo):
    t, m, sz = T[ty]
    return ('// CUserProfileManager::Set%s(%s value, bool save) -- store the option (and its new-profile copy while\n'
            '// UseNewProfileValues is set), push the live values to the engine, then save the profile when asked\n'
            '// Leaf: ?Set%s@CUserProfileManager@@QAEX%s@Z\n'
            '// Retail: Fable.exe (TLC, VC7.1) function @ 0x%s, thiscall, `ret 8`.\n'
            '// Identity: PDB name set ?Set%s@CUserProfileManager@@QAEX%s@Z; the stored field is %s (retail +0x%X) and its\n'
            '//   new-profile copy %sNewProfile (retail +0x%X) by struct_layouts_egor (retail = PDB below the first STL vector at\n'
            '//   +0x54, PDB - 8 after the two AssignedInputs vectors, PDB - 4 in the new-profile block).\n'
            '// Callees: CUserProfileManager::ApplyValues() @ 0x00407370 (protected in the PDB name set), CUserProfileManager::Save()\n'
            '//   @ 0x0040A740 (builds the profile path and writes the file).\n' % (name, t, name, m, a.upper(), name, m, name, off, name, npo)
            + model(name, ty, off, npo) + '\n' + body(name, t))
def test(a, name, ty, off, npo, patt):
    t, m, sz = T[ty]
    v1, v2, v0 = {'b': ('true', 'false', 'false'), 'd': ('5', '7', '1'), 'f': ('0.75f', '1.5f', '0.25f')}[ty]
    return ('// behaviour test for 0x%s CUserProfileManager::Set%s: the live value is always stored, the new-profile copy only\n'
            '// while UseNewProfileValues; ApplyValues runs every time, Save only when asked (links the row\'s object)\n'
            '#include <cstdio>\n#include <cstring>\n' % (a.upper(), name)
            + model(name, ty, off, npo) +
            'static int g_applies = 0, g_saves = 0;\n'
            'void CUserProfileManager::ApplyValues() { ++g_applies; }\n'
            'void CUserProfileManager::Save() { ++g_saves; }\n'
            'int main()\n{\n    int fails = 0;\n    CUserProfileManager m;\n    std::memset(&m, 0, sizeof(m));\n'
            '    m.%s = %s; m.%sNewProfile = %s;\n' % (name, v0, name, v0) +
            '    m.Set%s(%s, false);\n' % (name, v1) +
            '    if (m.%s != %s || m.%sNewProfile != %s || g_applies != 1 || g_saves != 0) fails |= 1;\n' % (name, v1, name, v0) +
            '    m.UseNewProfileValues = true;\n    m.Set%s(%s, true);\n' % (name, v2) +
            '    if (m.%s != %s || m.%sNewProfile != %s || g_applies != 2 || g_saves != 1) fails |= 2;\n' % (name, v2, name, v2) +
            '    if (sizeof(m) != 0x%X) fails |= 4;\n' % ((npo + sz + sz - 1) & ~(sz - 1)) +
            '    if (fails) { std::printf("%s_TEST FAIL %%d\\n", fails); return 1; }\n' % a +
            '    std::printf("%s\\n");\n    return 0;\n}\n' % patt)
if __name__ == '__main__':
    for a, name, ty, off, npo in ROWS:
        patt = R[a].get('PassPattern', a + '_TEST PASS')
        for k, txt in (('src', source(a, name, ty, off, npo)), ('tst', test(a, name, ty, off, npo, patt))):
            p, rel = path(a, k)
            if open(p, encoding='utf-8', errors='surrogateescape').read() == txt: print('already', a, k); continue
            open(p, 'w', encoding='utf-8', newline='').write(txt); print('ok', a, k)
