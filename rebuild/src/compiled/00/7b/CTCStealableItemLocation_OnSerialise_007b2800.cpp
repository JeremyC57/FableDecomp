// CTCStealableItemLocation::OnSerialise(CPersistContext* context, long unused) -- persist the two radii (zero defaults)
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B2800, length 73 bytes (`ret 8`); vtable 0x0123C65C
// (CTCStealableItemLocation) slot 1, the CTCBase persist hook.
// Callees: CPersistContext::Transfer(const char*, float&, const float&) @ 0x00410620 (twice) with the keys
// "RadiusToBeWithin" @ 0x0126D7B8 and "RadiusToTakeItemsBackTo" @ 0x0126D7A0.
// Layout: Ego_r.pdb CTCStealableItemLocation (0x14 bytes): RadiusToBeWithin +0x0c, RadiusToTakeItemsBackTo +0x10.
// Shape: each default is a zero dword in a frame slot (the first in the `push ecx` word, the second in the dead
// argument slot), stored with an integer immediate.
class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: float 0x00410620
};

class CTCStealableItemLocation { public:
    void*   __vftable;                                      // +0x00 (virtuals below)
    void*   ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    float   RadiusToBeWithin;                               // +0x0c
    float   RadiusToTakeItemsBackTo;                        // +0x10
    void OnSerialise(CPersistContext* context, long unused);
};

void CTCStealableItemLocation::OnSerialise(CPersistContext* context, long)
{
    float defaultWithin = 0.0f;
    context->Transfer("RadiusToBeWithin", RadiusToBeWithin, defaultWithin);
    float defaultBack = 0.0f;
    context->Transfer("RadiusToTakeItemsBackTo", RadiusToTakeItemsBackTo, defaultBack);
}