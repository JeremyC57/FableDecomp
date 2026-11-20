// Byte-exact reconstruction of CCharString::SetUseFastExtend @ 0x0099ef70
// Leaf: ?SetUseFastExtend@CCharString@@QAEX_N@Z
// Naming (NF129, 2026-11-20, q3967): was the free __fastcall stand-in CCharString_SetUseFastExtend(CCharString*, int, bool) calling a free
//   CCharString_EnsureUnique for 0x0099EB40. Both are members of the row's existing local CCharString model now (in-place respell):
//   Ego_r.pdb ?SetUseFastExtend@CCharString@@QAEX_N@Z (the spelling the landed callers 0x00521EB0 / 0x00523B40 / 0x00523CF0 bind; fable_string.h
//   FABLE_RETAIL_CCHARSTRING_RESERVE) and the protected ?MakeOwnCopyOfDataOrEmptyString@CCharString@@IAEXXZ (Ego_r + FableWin).

struct CharBuf {
    char pad[0xc];
    unsigned char flags; // offset 0xc
};

class CCharString { public:
    CharBuf* buf; // offset 0
    void SetUseFastExtend(bool value);
protected:
    void MakeOwnCopyOfDataOrEmptyString();    // 0x0099EB40 (protected)
};

// Retail is thiscall (ecx = this) with the bool on the stack and a `ret 4` epilogue.
void CCharString::SetUseFastExtend(bool value)
{
    MakeOwnCopyOfDataOrEmptyString();
    CharBuf* b = buf;
    unsigned char cur = b->flags;
    unsigned char x = (unsigned char)(cur ^ (unsigned char)value);
    x &= 1;
    b->flags = (unsigned char)(b->flags ^ x);
}