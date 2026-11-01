# family decisions (hand): DECISIONS_W14_RESOLVED / brief item 2 -- slot 5 of the silent audio output vtable 0x012B9174
# (0x00C5EE50, zeroes the block count) is ReleaseBuffer (the chunk 661 label of 0x00C5EE20's tail-jump target; descriptive, no
# PDB/RTTI in the audio library). The other rows on this vtable spell it v05 / Close / Release: unified.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
for a in ('00c5ee60', '00c5eeb0'):
    edit(a, 'src', '    virtual void v05();\n', '    virtual void ReleaseBuffer();                   // slot 5 (0x00C5EE50)\n')
for a in ('00c5ee60', '00c5eeb0'): edit_all(a, 'src', '5 Close 0x00C5EE50', '5 ReleaseBuffer 0x00C5EE50')
edit('00c5eef0', 'src', '    virtual void Release();                // slot 5\n    __forceinline void DoRelease() { Release(); }',
     '    virtual void ReleaseBuffer();          // slot 5 (0x00C5EE50)\n    __forceinline void DoRelease() { ReleaseBuffer(); }')
edit('00c5eef0', 'src', 'calls its own virtual Release (slot 5,', 'calls its own virtual ReleaseBuffer (slot 5,')
