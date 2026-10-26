// CBalverineBattleDef::Copy(const CDefClassBase* other) -- the def's virtual copy: assign the other def (of the same class) to this one
// Leaf: ?Copy@CBalverineBattleDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E4857, 5 bytes: `jmp 0x004E485C` (CBalverineBattleDef::operator=, landed by family pass 2 G022
//   with Leaf ??4CBalverineBattleDef@@QAEAAV0@ABV0@@Z): `this` stays in ecx and `other` in the caller's argument slot, so VC7.1 turns the
//   call into a tail jump.
// Identity (sweep3; the landed source was an invented extern "C" forwarder, FINDINGS line 631 "extern \"C\" Copy thunks",
//   agent951 G022): the CBalverineBattleDef vtable's Copy slot (19) holds this thunk and the PDB name set has
//   ?Copy@CBalverineBattleDef@@UAEXPBVCDefClassBase@@@Z next to ??4CBalverineBattleDef@@QAEAAV0@ABV0@@Z. The class is modelled only as far as the
//   symbol needs: a CDefClassBase base at offset 0 (the cast is a no-op) and the out-of-line copy assignment.
class CDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
};

class CBalverineBattleDef : public CDefClassBase {
public:
    CBalverineBattleDef& operator=(const CBalverineBattleDef& other);   // 0x004E485C
    virtual void Copy(const CDefClassBase* other);
};

void CBalverineBattleDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CBalverineBattleDef*>(other);
}
