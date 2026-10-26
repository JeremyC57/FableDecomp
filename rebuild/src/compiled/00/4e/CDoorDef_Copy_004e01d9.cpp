// CDoorDef::Copy(const CDefClassBase* other) -- the def's virtual copy: assign the other def (of the same class) to this one
// Leaf: ?Copy@CDoorDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E01D9, 5 bytes: `jmp 0x004E01DE` (CDoorDef::operator=, landed by family pass 2 G058
//   with Leaf ??4CDoorDef@@QAEAAV0@ABV0@@Z): `this` stays in ecx and `other` in the caller's argument slot, so VC7.1 turns the
//   call into a tail jump.
// Identity (sweep3; the landed source was an invented extern "C" forwarder, FINDINGS line 631 "extern \"C\" Copy thunks",
//   agent951 G058): the CDoorDef vtable's Copy slot (19) holds this thunk and the PDB name set has
//   ?Copy@CDoorDef@@UAEXPBVCDefClassBase@@@Z next to ??4CDoorDef@@QAEAAV0@ABV0@@Z. The class is modelled only as far as the
//   symbol needs: a CDefClassBase base at offset 0 (the cast is a no-op) and the out-of-line copy assignment.
class CDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
};

class CDoorDef : public CDefClassBase {
public:
    CDoorDef& operator=(const CDoorDef& other);   // 0x004E01DE
    virtual void Copy(const CDefClassBase* other);
};

void CDoorDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CDoorDef*>(other);
}
