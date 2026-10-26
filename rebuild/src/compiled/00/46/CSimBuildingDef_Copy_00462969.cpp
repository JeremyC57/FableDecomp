// CSimBuildingDef::Copy(const CDefClassBase* other) -- the def's virtual copy: assign the other def (of the same class) to this one
// Leaf: ?Copy@CSimBuildingDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00462969, 5 bytes: `jmp 0x0046296E` (CSimBuildingDef::operator=, landed by family pass 2 G022
//   with Leaf ??4CSimBuildingDef@@QAEAAV0@ABV0@@Z): `this` stays in ecx and `other` in the caller's argument slot, so VC7.1 turns the
//   call into a tail jump.
// Identity (sweep3; the landed source was an invented extern "C" forwarder, FINDINGS line 631 "extern \"C\" Copy thunks",
//   agent951 G022): the CSimBuildingDef vtable's Copy slot (19) holds this thunk and the PDB name set has
//   ?Copy@CSimBuildingDef@@UAEXPBVCDefClassBase@@@Z next to ??4CSimBuildingDef@@QAEAAV0@ABV0@@Z. The class is modelled only as far as the
//   symbol needs: a CDefClassBase base at offset 0 (the cast is a no-op) and the out-of-line copy assignment.
class CDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
};

class CSimBuildingDef : public CDefClassBase {
public:
    CSimBuildingDef& operator=(const CSimBuildingDef& other);   // 0x0046296E
    virtual void Copy(const CDefClassBase* other);
};

void CSimBuildingDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CSimBuildingDef*>(other);
}
