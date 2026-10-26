// CTCInventoryStats::OnSerialise(CPersistContext&, bool) -- serialises the inventory base, then the CreaturesKilled map under the key "CreaturesKilled2"
// Leaf: ?OnSerialise@CTCInventoryStats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D7FAB, 41 bytes. rtti-vtables: CTCInventoryStats 0x01254ADC slot 1
// (catalog label "vfunc_1"; the base call it makes is CTCInventoryBase::OnSerialise, so the slot is OnSerialise).
// Callees: CTCInventoryBase::OnSerialise(CPersistContext*, bool) @ 0x005BFD5E, CPersistContext::Transfer<map<unsigned
//          long, long>>(const char*, T*) @ 0x005D8C6A (manifest "Transfer<long>"; it forwards with a zeroed default).
// Data: the literal "CreaturesKilled2" @ 0x012554C8.
//
// Shape: a size-optimised TU (`#pragma optimize("s", on)`): both arguments re-pushed straight from their slots
// (`push dword ptr [esp+0Ch]` twice, `mov esi,ecx` between them), the context re-read for the Transfer call, the member
// address formed in place with `add esi,134h` (CreaturesKilled is the first Stats member, after the 4-byte CBase vptr
// at +0x130). Local model (no engine header for CTCInventoryStats).
#pragma optimize("s", on)
struct CCreatureKillMap {
    unsigned char _body[0xc];
};
class CPersistContext { public:
    template <class T> void Transfer(const char* name, T* value);
};
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    void OnSerialise(CPersistContext& context, bool loading);   // 0x005BFD5E
};
namespace NHeroInformationScreens {
class CBase { public:
    virtual long vfunc_0();
};
}
class CTCInventoryStats : public CTCInventoryBase_Retail, public NHeroInformationScreens::CBase { public:
    CCreatureKillMap CreaturesKilled;         // +0x134
    void OnSerialise(CPersistContext& context, bool loading);
};
void CTCInventoryStats::OnSerialise(CPersistContext& context, bool loading)
{
    CTCInventoryBase_Retail::OnSerialise(context, loading);
    context.Transfer("CreaturesKilled2", &CreaturesKilled);
}