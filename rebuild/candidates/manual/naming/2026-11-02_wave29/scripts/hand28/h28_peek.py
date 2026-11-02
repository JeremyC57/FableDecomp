# h28_peek.py: naming wave 28, family peek (c696 queue): 0x00995E70 is ?PeekData@CMemoryDataOutputStream@@QBEPBXXZ (chunk 696: Ego_r.pdb
#  donor order + landed callers). Binder 0x009D4A20 called it as ?GetData@CStringTableArray@@QAEPADXZ; its own Leaf is the Ego_r /
#  FableWin const ?GetStringCRC@CDefStringTable@@QBEKJ@Z, and the stream is CDefStringTable::StringStream (Ego_r member, retail +0x0C
#  after the 12-byte STLport StringMap; as the landed GetStringAsCharArray 0x009D49F0).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
he.put('009d4a20', 'src', '''// CDefStringTable::GetStringCRC(long index) const -- the CRC dword stored at the string's offset in the table's string stream; 0 for index -1
// Leaf: ?GetStringCRC@CDefStringTable@@QBEKJ@Z
// Naming wave 28 (family peek, c696 queue): the stream accessor 0x00995E70 is ?PeekData@CMemoryDataOutputStream@@QBEPBXXZ (chunk 696:
//   Ego_r.pdb donor order + landed callers) on the Ego_r member StringStream (retail +0x0C, after the 12-byte STLport StringMap, as the
//   landed GetStringAsCharArray 0x009D49F0); the own Leaf is the Ego_r.pdb / FableWin.pdb const member. Was CStringTableArray::GetData
//   (?GetData@CStringTableArray@@QAEPADXZ) on a non-const GetStringCRC.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D4A20: index -1 returns 0; else `add ecx,0Ch ; call PeekData` and the dword at data + index.
class CMemoryDataOutputStream {
public:
    const void* PeekData() const;                       // 0x00995E70
};

class CDefStringTable {
public:
    unsigned char StringMap[0xc];                       // +0x00 STLport map<CCharString, long>
    CMemoryDataOutputStream StringStream;               // +0x0C

    unsigned long GetStringCRC(long index) const;
};

unsigned long CDefStringTable::GetStringCRC(long index) const
{
    if (index == -1)
        return 0;
    const char* p = (const char*)StringStream.PeekData();
    p += index;
    return *(const unsigned long*)p;
}
''')

# the two other binders of 0x009D4A20 call the own row's PDB spelling (const, QBE)
from common import note
for a in ('0073e940', '0073ea00'):
    he.edit(a, 'src', '    unsigned long GetStringCRC(long stringIndex);      // 0x009D4A20', '    unsigned long GetStringCRC(long stringIndex) const;   // 0x009D4A20')
    note(a, 'peek', 'CDefStringTable::GetStringCRC 0x009D4A20 is the Ego_r.pdb / FableWin.pdb const\n'
         '//   ?GetStringCRC@CDefStringTable@@QBEKJ@Z (its own row\'s Leaf this wave). Was the non-const QAE spelling.')
