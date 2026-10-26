// CPhysicsDef::Copy(const CDefClassBase* other) -- the def's virtual copy: assign the other def (of the same class) to this one
// Leaf: ?Copy@CPhysicsDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004537A0, 5 bytes: `jmp 0x004537A5` (CPhysicsDef::operator=, landed by family pass 2 G058
//   with Leaf ??4CPhysicsDef@@QAEAAV0@ABV0@@Z): `this` stays in ecx and `other` in the caller's argument slot, so VC7.1 turns the
//   call into a tail jump.
// Identity (sweep3; the landed source was an invented extern "C" forwarder, FINDINGS line 631 "extern \"C\" Copy thunks",
//   agent951 G058): the CPhysicsDef vtable's Copy slot (19) holds this thunk and the PDB name set has
//   ?Copy@CPhysicsDef@@UAEXPBVCDefClassBase@@@Z next to ??4CPhysicsDef@@QAEAAV0@ABV0@@Z. The class is modelled only as far as the
//   symbol needs: a CDefClassBase base at offset 0 (the cast is a no-op) and the out-of-line copy assignment.
class CDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
};

class CPhysicsDef : public CDefClassBase {
public:
    CPhysicsDef& operator=(const CPhysicsDef& other);   // 0x004537A5
    virtual void Copy(const CDefClassBase* other);
};

void CPhysicsDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CPhysicsDef*>(other);
}
