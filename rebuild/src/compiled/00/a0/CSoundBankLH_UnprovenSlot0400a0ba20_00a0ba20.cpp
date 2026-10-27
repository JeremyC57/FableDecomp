// CSoundBankLH::UnprovenSlot04_00a0ba20(long a, long b) -- an empty CUnprovenValue_00a0ba20
// Leaf: ?UnprovenSlot04_00a0ba20@CSoundBankLH@@UAE?AVCUnprovenValue_00a0ba20@@JJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0BA20, 20 bytes, thiscall, `ret 12` (the result by hidden pointer).
// Identity (descriptive): slot 4 of vtable 0x0129C9DC (RTTI .?AVCSoundBankLH@@; slot 1 is Load 0x00A0B630, slot 9 IsSoundEventValid 0x00A0BA40); the PDBs carry no CSoundBankLH / CASoundBank virtual names, so the method and its 8-byte result are descriptive. The manifest's "vfunc" labels are placeholders.
// Shape (family M183, representative 0x00661CA0): `return CUnprovenValue_00a0ba20();` -- both words stored as zero into the hidden result. Speed TU.
class CUnprovenValue_00a0ba20 {                         // an 8-byte value returned by hidden pointer
public:
    CUnprovenValue_00a0ba20() : Word0(0), Word1(0) { }
    long Word0;
    long Word1;
};
class CSoundBankLH {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual CUnprovenValue_00a0ba20 UnprovenSlot04_00a0ba20(long a, long b);          // slot 4
};
CUnprovenValue_00a0ba20 CSoundBankLH::UnprovenSlot04_00a0ba20(long a, long b)
{
    return CUnprovenValue_00a0ba20();
}