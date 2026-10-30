# sub-wave 1b: the defining rows of the CGameScriptInterface message-frame pair become the protected const members
# (Ego_r.pdb 0x00710770 / 0x007107A0 and FableWin.pdb 0x024674E0 / 0x024676B0 list Min then Max; Min reads the +0x18 frame
# word, Max +0x1c, as the CGameScriptThing twin pair 0x008D0150 / 0x008D0170). Code unchanged: `this` was already unused.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
edit('006e7510', 'src', '// Byte-exact reconstruction of CGameScriptInterface::GetMaxWorldFrameForMessages @ 0x006e7510\n// Leaf: GetMaxWorldFrameForMessages\n// this-call member; body ignores this and reads a global chain.',
     '// Byte-exact reconstruction of CGameScriptInterface::GetMinWorldFrameForMessages() const @ 0x006e7510\n'
     '// Leaf: ?GetMinWorldFrameForMessages@CGameScriptInterface@@IBEJXZ\n'
     '// Protected const member; the body ignores this and reads a global chain (the +0x18 frame word; its twin\n'
     '// GetMaxWorldFrameForMessages 0x006E7530 reads +0x1c). Ego_r.pdb 0x00710770 / FableWin.pdb 0x024674E0 list Min before Max.')
edit('006e7510', 'src', 'class CGameScriptInterface;\n', 'class CGameScriptInterface {\nprotected:\n    long GetMinWorldFrameForMessages() const;   // 0x006E7510\n};\n')
edit('006e7510', 'src', 'long __fastcall GetMaxWorldFrameForMessages(CGameScriptInterface* self)\n{', 'long CGameScriptInterface::GetMinWorldFrameForMessages() const\n{')
edit('006e7530', 'src', 'class CGameScriptInterface;\n', 'class CGameScriptInterface {\nprotected:\n    long GetMaxWorldFrameForMessages() const;   // 0x006E7530\n};\n')
edit('006e7530', 'src', 'long __fastcall GetMaxWorldFrameForMessages(CGameScriptInterface* self)\n{\n    (void)self;\n', 'long CGameScriptInterface::GetMaxWorldFrameForMessages() const\n{\n')
edit('006e7530', 'src', '// Byte-exact reconstruction of CGameScriptInterface::GetMaxWorldFrameForMessages @ 0x006e7530\n// Reads a global singleton, walks a pointer chain, returns a long.',
     '// Byte-exact reconstruction of CGameScriptInterface::GetMaxWorldFrameForMessages() const @ 0x006e7530\n'
     '// Leaf: ?GetMaxWorldFrameForMessages@CGameScriptInterface@@IBEJXZ\n'
     '// Protected const member (both PDBs IBE); reads a global singleton, walks a pointer chain, returns a long (+0x1c).')
