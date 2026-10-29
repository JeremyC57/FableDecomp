# wave 5 hand edits (exact single replacements; HE_OV=<overlay>): CDataInputStream::ReadWithSrcChunkOverflow
# (0x00993CA0) is private in Ego_r.pdb / FableWin.pdb: ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z
# (DECISIONS.tsv, proven). The two rows call it from another class's member, so the model befriends that caller
# (retail reaches it the same way: the persist traits / opinion reader are friends or inline members). 0046bf1c's test is a
# self-contained harness; 004793f8's test links the row object and defines the stub, so it gets the same edit.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
edit('0046bf1c', 'src',
     '    void ReadWithSrcChunkOverflow(unsigned char* destination, long size);   // 0x00993CA0\n};',
     'private:\n    friend struct CPersistTraitsRecord54;\n'
     '    void ReadWithSrcChunkOverflow(unsigned char* destination, long size);   // 0x00993CA0 (private: AAE)\n};')
for k in ('src', 'tst'):   # 004793f8's test links the row object and defines the stub out of line: same access
    edit('004793f8', k,
         '    void ReadWithSrcChunkOverflow(unsigned char* destination, long size);\n};',
         'private:\n    friend class COpinionTransientOffset;\n'
         '    void ReadWithSrcChunkOverflow(unsigned char* destination, long size);   // 0x00993CA0 (private: AAE)\n};')
