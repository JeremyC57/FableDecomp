// CBossDef::Transfer(CPersistContext& context) -- persist the def's own members: PhaseHealthPercent, Shots, TwinBladeMissesBeforeLunge, TwinBladeStuckLoops, TwinBladeTimeBeforeLunge
// Leaf: ?Transfer@CBossDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E7EE0, 68 bytes, thiscall, `ret 4`.
// Identity (proven): slot 18 of the CBossDef vtable 0x0124185C (rtti-vtables); FableWin.pdb ?Transfer@CBossDef@@UAEXAAVCPersistContext@@@Z;
//   the transferred members are Ego_r.pdb's CBossDef fields at their retail offsets (retail = Ego_r.pdb .. - 0xC (12-byte STLport vectors)).
// Callees: Transfer<std::vector<long>> 0x00466A47 (PhaseHealthPercent), Transfer<std::vector<long>> 0x00466A47 (Shots), Transfer<std::vector<long>> 0x00466A47 (TwinBladeMissesBeforeLunge), Transfer<long> 0x00431102 (TwinBladeStuckLoops), Transfer<float> 0x00431061 (TwinBladeTimeBeforeLunge).
// Shape (family K094, representative 0x00457F13): one CPersistContext::Transfer<T>(T&) call per member, the context kept in edi;
//   size TU (every member address formed in eax).
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport, 12 bytes
public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
};
}
class CPersistContext {
public:
    template <class T> void Transfer(T& value);          // per-type helpers (out of line)
};
class CSubDefClassBase {                                            // retail 0x28 bytes (members not touched here)
public:
    virtual ~CSubDefClassBase();
    unsigned char _body[0x28 - 4];
};
class CBossDef : public CSubDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);     // slot 18
    std::vector<long>   PhaseHealthPercent;                // +0x28
    std::vector<long>   Shots;                             // +0x34
    std::vector<long>   TwinBladeMissesBeforeLunge;        // +0x40
    long                TwinBladeStuckLoops;               // +0x4C
    float               TwinBladeTimeBeforeLunge;          // +0x50
};
void CBossDef::Transfer(CPersistContext& context)
{
    context.Transfer(PhaseHealthPercent);
    context.Transfer(Shots);
    context.Transfer(TwinBladeMissesBeforeLunge);
    context.Transfer(TwinBladeStuckLoops);
    context.Transfer(TwinBladeTimeBeforeLunge);
}