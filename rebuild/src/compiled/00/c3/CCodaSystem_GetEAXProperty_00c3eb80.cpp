// CCodaSystem::GetEAXProperty(const void* set, unsigned long id, void* data, unsigned long size) -- read a global EAX property through the hardware EAX device (+0x1d4, virtual slot 9); -9 when no hardware device is open
// Leaf: ?GetEAXProperty@CCodaSystem@@QAEJPBXKPAXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3EB80, 25 bytes (+ int3 padding), thiscall; the device call is a sibling
//   `jmp [eax+0x24h]` (identical stack arguments), the failure exit `mov eax,-9 ; ret 10h`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 440 "0xC3EB60 "CScriptThing::func_00c3eb60" = hardware EAX forwarder"):
//   - its only caller is the EAX manager's listener Get 0x00C4F280 (same switch on +0xe8; hardware case `mov ecx,[0x0143BB5C] ; call 0x00C3EB80`);
//   - `this` is the audio master held by 0x0143BB5C (and 0x0143BC2C): the CAudioPlatform-derived class whose constructor
//     0x00C3E520 calls CAudioPlatform::CAudioPlatform 0x00C4D000, zeroes +0x188 and this +0x1d4 device pointer, and stores
//     vtable 0x012B5BC0. Its 0x00C3EA60 (+0x188 accessor) is bound as ?GetOutputMode@CCodaSystem@@ by agent441's landed
//     0x00C59960 (on 0x0143BC2C) and is called on 0x0143BB5C at 0x00C3359F, so both globals hold this class: CCodaSystem;
//   - the pair 0x00C3EB60 / 0x00C3EB80 forwards to the device's slots 8 / 9 (Set / Get).
//   The landed CAudioOwner_Retail / CScriptThing::func_00c3eb60 / func_00c3eb80 names are propagated; descriptive names (no PDB/RTTI covers
//   GameCODA). agent440's 0x00C52980 row binds this address as ?SetProperty@CHardwareEAX@@ on `g_pHardwareEAX`.
class CEAXDevice {                                  // the hardware EAX device (+0x1d4)
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual long SetProperty(const void* set, unsigned long id, void* data, unsigned long size);   // slot 8 (+0x20)
    virtual long GetProperty(const void* set, unsigned long id, void* data, unsigned long size);   // slot 9 (+0x24)
};

class CCodaSystem {                                 // vtable 0x012B5BC0 (CAudioPlatform-derived)
public:
    unsigned char _pad00[0x1d4];
    CEAXDevice*   m_pEAXDevice;                     // +0x1d4 (0 until the hardware device opens)
    long GetEAXProperty(const void* set, unsigned long id, void* data, unsigned long size);
};

long CCodaSystem::GetEAXProperty(const void* set, unsigned long id, void* data, unsigned long size)
{
    if (m_pEAXDevice)
        return m_pEAXDevice->GetProperty(set, id, data, size);
    return -9;
}
