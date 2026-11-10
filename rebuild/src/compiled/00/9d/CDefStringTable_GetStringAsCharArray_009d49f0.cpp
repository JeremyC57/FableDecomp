// CDefStringTable::GetStringAsCharArray(long index) -- 0x009d49f0
// char * __fastcall(CDefStringTable *this, long index)
//
// Layout evidence from bytes:
//   push esi
//   mov esi, [esp+8]          ; esi = index (arg1, thiscall stack slot after ret addr)
//   cmp esi, -1
//   jnz L_lookup
//   mov eax, 0x0122d70e       ; shared global empty-string byte
//   pop esi
//   ret 4
// L_lookup:
//   add ecx, 0x0c             ; ecx = &this->m_stream (CMemoryDataOutputStream member at +0xc)
//   call CMemoryDataOutputStream::PeekData
//   lea eax, [eax+esi+4]      ; eax = (char*)base + index + 4
//   pop esi
//   ret 4
// Leaf: ?GetStringAsCharArray@CDefStringTable@@QBEPBDJ@Z
// Name fix (agentNF5 2026-11-10): Ego_r.pdb / FableWin.pdb ?GetStringAsCharArray@CDefStringTable@@QBEPBDJ@Z (const, const char* result),
//   was ?GetStringAsCharArray@CDefStringTable@@QAEPADJ@Z; the -1 answer is the empty literal "" (??_C@_00CNPNBAHC@?$AA@ 0x0122D70E),
//   was the stand-in extern C DAT_0122d70e.


class CMemoryDataOutputStream
{
public:
void const* PeekData() const;
private:
};

class CDefStringTable
{
public:
    char pad0[0xc];
    CMemoryDataOutputStream m_stream;

    const char *GetStringAsCharArray(long index) const;
};

const char *CDefStringTable::GetStringAsCharArray(long index) const
{
    if (index == -1)
    {
        return "";
    }

    const void *base = m_stream.PeekData();
    return (const char *)base + index + 4;
}