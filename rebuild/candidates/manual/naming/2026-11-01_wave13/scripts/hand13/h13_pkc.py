# family pkc (hand): the libpk object whose third vtable is 0x012B29F8 (built by 0x00C14E40, destroyed by 0x00C14F90) is
# PKCDeferredBank (owner decision 2026-11-01, DECISIONS_W12_RESOLVED: descriptive libpk name; its TU parses the
# LHAudioBankLookupTable / LHAudioBankCompData / LHFileSegmentBankInfo segments and checks `Expecting deferred bank version
# number`). CTCDiceGame (a 24-byte Ego_r class), CDataOwner_00c14a50, CSettingsProxy_012b29f8 and PKCPlaybackTemplate were
# inventions; the member names stay descriptive. HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, resub
N = 'PKCDeferredBank'
NOTE = ('// Class (naming wave 13, owner decision 2026-11-01): PKCDeferredBank, the libpk deferred-bank object whose third vtable is\n'
        '//   0x012B29F8 (built by 0x00C14E40, destroyed by 0x00C14F90); descriptive, libpk has no RTTI and no class strings.\n')
# 0x00C14E40: the constructor
A = '00c14e40'
edit(A, 'src', '// CTCDiceGame::CTCDiceGame(CThing* parent, const CDiceGameSetup* setup, long seed) -- (labelled Initialize: it is the constructor)',
     '// PKCDeferredBank::PKCDeferredBank(CThing* parent, const CDiceGameSetup* setup, long seed) -- (labelled Initialize: it is the constructor)')
edit(A, 'src', '// Leaf: CTCDiceGame\n', '// Leaf: ??0PKCDeferredBank@@\n' + NOTE)
edit(A, 'src', '// Layout: retail, local (the Ego_r.pdb CTCDiceGame is 24 bytes, so the CTCDiceGame attribution of this vptr-0x012B29F8 class is',
     '// Layout: retail, local (the Ego_r.pdb CTCDiceGame is 24 bytes, so the earlier CTCDiceGame attribution of this vptr-0x012B29F8 class was')
resub(A, 'src', r'(?<!earlier )(?<!Ego_r\.pdb )\bCTCDiceGame\b', N)
# 0x00C14A50 DeleteData
A = '00c14a50'
edit(A, 'src', '// Label: the catalog\'s `DeleteData` is descriptive; no PDB name set covers libpk, the classes are named by address.',
     '// Label: the catalog\'s `DeleteData` is descriptive; no PDB name set covers libpk.\n' + NOTE.rstrip('\n'))
edit(A, 'src', 'struct CDataOwner_00c14a50 {\n    void* m_vtable;', 'class PKCDeferredBank { public:\n    void* m_vtable;')
resub(A, 'src', r'\bCDataOwner_00c14a50\b', N)
# 0x00C14B50 / 0x00C14BC0 SetOptionCC / SetOptionCD (vtable 0x012B29F8 slots 1 / 2)
for A in ('00c14b50', '00c14bc0'):
    edit(A, 'src', '// by its vtable.\n', '// by its vtable.\n' + NOTE)
    resub(A, 'src', r'\bCSettingsProxy_012b29f8\b', N)
# 0x00C14A80 ApplyTo (a slot of the same object; keeps its descriptive name)
A = '00c14a80'
edit(A, 'src', '// Leaf: ?ApplyTo@PKCPlaybackTemplate@@\n', '// Leaf: ?ApplyTo@PKCDeferredBank@@\n' + NOTE)
resub(A, 'src', r'\bPKCPlaybackTemplate\b', N)
