// CScriptOwnerThing::IsUnreferenced() const -- true when the object's 16-bit reference count (+0x1E) is zero
// Leaf: ?IsUnreferenced@CScriptOwnerThing@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1EA20, 10 bytes: xor eax,eax ; cmp word ptr [ecx+1Eh],ax ; sete al ; ret
// Identity (descriptive): the landed CScriptHost::PurgeUnreferencedOwners 0x00C0EC90 (its only caller, 0x00C0ECB1) binds
//   ?IsUnreferenced@CScriptOwnerThing@@QBE_NXZ to this address and destroys the owner when it returns true. The same
//   +0x1E word is incremented by 0x00C1EA30 and decremented by 0x00C1EA40 (the reference count). The catalog's
//   "CManager::GetKeyPressed" is a propagated label. No PDB covers libpk.
struct CScriptOwnerThing {
    unsigned char  _pad00[0x1e];
    unsigned short m_refCount;                  // +0x1e
    bool IsUnreferenced() const;
};

bool CScriptOwnerThing::IsUnreferenced() const
{
    return m_refCount == 0;
}