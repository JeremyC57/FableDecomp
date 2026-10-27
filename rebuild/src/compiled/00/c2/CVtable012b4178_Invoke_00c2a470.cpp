// CVtable_012b4178::Invoke(CUnprovenTarget_00c371c0* target) -- run the target's update 0x00C371C0 with the flag byte the callback holds (+4)
// Leaf: ?Invoke@CVtable_012b4178@@UAEXPAUCUnprovenTarget_00c371c0@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A470, 17 bytes, thiscall, `ret 4`.
// Identity (descriptive): slot 0 of the two-slot non-RTTI callback vtable 0x012B4178 (slot 1 its scalar deleting
//   destructor 0x00C2ABE0), stored only by its constructor 0x00C2A620 (vtable, then the byte argument at +4); the
//   neighbouring callback vtables 0x012B4168 / 0x012B4170 / 0x012B4180 hold the landed one-call forwarders
//   0x00C2A450 / 0x00C2A460 / 0x00C2A580. libpk has no PDB or RTTI, so the callback is named by its vtable and the
//   target by address.
// Callees: the target's update @ 0x00C371C0 (thiscall, one dword argument tested for zero; not landed).
// Shape: the byte member is widened with `movzx` for the dword argument.
struct CUnprovenTarget_00c371c0 {
    void Update(long flag);                     // 0x00C371C0
};

class CVtable_012b4178 {                        // vtable 0x012B4178: { Invoke, scalar deleting destructor }
public:
    virtual void Invoke(CUnprovenTarget_00c371c0* target);
    unsigned char m_flag;                       // +0x04
};

void CVtable_012b4178::Invoke(CUnprovenTarget_00c371c0* target)
{
    target->Update(m_flag);
}