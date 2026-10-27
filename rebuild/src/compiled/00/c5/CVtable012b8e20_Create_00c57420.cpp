// CVtable_012b8e20::Create(unsigned long a) -- vtable slot 4 of the derived OpenAL streaming buffer: create and start the source through its own slot 3 (CreateAndPlay(1, a)); the base 0x012B8ED4 returns false here
// Leaf: ?Create@CVtable_012b8e20@@UAE_NK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C57420, 20 bytes (+ int3 padding), thiscall `ret 4`; slot 4 of the
//   vtable 0x012B8E20 (ctor 0x00C573E0: the base ctor 0x00C57780 then this vtable; landed under the propagated label
//   "C3DAnimationUtil", named by its vtable here).
// Callee: virtual slot 3 (+0x0C) CreateAndPlay, 0x00C57800 in this table.
// Name: descriptive (no PDB/RTTI); the DirectSound counterpart's slot 4 (0x00C56D70) also creates with a leading 1.
// Shape: `test al,al ; setne al` converts the slot-3 byte result (unsigned char) to bool; the if/return-false form, a
//   ternary, a named bool and an unsigned char result all give it, only `return f() != 0;` in a bool function gives
//   `neg al ; sbb eax,eax ; neg eax`.
class CVtable_012b8ed4 {                            // base: vtable 0x012B8ED4 (ctor 0x00C57780)
public:
    virtual void v00();
    virtual bool Initialize(const void* device, void* window, bool multithreaded);   // slot 1: 0x00C57460
    virtual void v02();
    virtual unsigned char CreateAndPlay(unsigned long a, unsigned long b);           // slot 3: 0x00C57800
    virtual bool Create(unsigned long a);                                            // slot 4: 0x00C572C0 (false)
    unsigned char _pad04[0x618];
};
class CVtable_012b8e20 : public CVtable_012b8ed4 {  // vtable 0x012B8E20
public:
    virtual bool Create(unsigned long a);           // slot 4: 0x00C57420
};

bool CVtable_012b8e20::Create(unsigned long a)
{
    if (!CreateAndPlay(1, a))
        return false;
    return true;
}