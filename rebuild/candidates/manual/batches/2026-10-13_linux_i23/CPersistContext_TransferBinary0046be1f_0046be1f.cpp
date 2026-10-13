// CPersistContext::TransferBinary(T*) -- CheckCRC(\"\"), then GFSerialiseVectorBinaryOut_CReverbEnviro @ 0x0046BE4D on the save stream in mode 2, else GFSerialiseVectorBinaryIn_CBrainUpdateZo @ 0x0046BE89 on the load stream
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046BE1F, oracle length 46 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push offset "" ; mov esi,ecx ; call CPersistContext::CheckCRC ; mov eax,[esi+18h] ; mov edx,[value] ; dec eax ; dec eax ; je L ; mov ecx,[esi+28h] ; call GFSerialiseVectorBinaryOut_CReverbEnviro ; jmp R ; L: mov ecx,[esi+24h] ; call GFSerialiseVectorBinaryIn_CBrainUpdateZo ; R: pop esi ; ret 4
// Persist transfer of one value: after the CRC check with the (empty) key the mode at +0x18
// selects the direction -- 2 (load) hands the load stream (+0x24) and the value to the
// `__fastcall` reader @ 0x0046BE89, anything else hands the save stream (+0x28) and the value to
// the `__fastcall` writer @ 0x0046BE4D. The `dec/dec` sequence is the size-optimised lowering of
// the two-way switch on the mode (the lander's "s" pragma). Labels are propagated; the streams are modelled as opaque
// pointers. One of 24 bodies of this shape.
struct CDataOutputStream;
struct CMemoryDataInputStream;
struct SValue;

void __fastcall GFSerialiseVectorBinaryOut_CReverbEnviro_0046be4d(CDataOutputStream* stream, SValue* value);        // 0x0046BE4D
void __fastcall GFSerialiseVectorBinaryIn_CBrainUpdateZo_0046be89(CMemoryDataInputStream* stream, SValue* value);   // 0x0046BE89

struct CPersistContext {
    unsigned char _pad0[0x18];
    long m_mode;                           // +0x18 (EMode: 2 = save)
    unsigned char _pad1[8];
    CMemoryDataInputStream* m_pLoadStream; // +0x24
    CDataOutputStream* m_pSaveStream;      // +0x28
    void CheckCRC(const char* name);       // 0x00404500
    void TransferBinary_0046be1f(SValue* value);
};

void CPersistContext::TransferBinary_0046be1f(SValue* value)
{
    CheckCRC("");
    switch (m_mode) {
    case 2:                                // load: the stream at +0x24
        GFSerialiseVectorBinaryIn_CBrainUpdateZo_0046be89(m_pLoadStream, value);
        break;
    default:                               // save: the stream at +0x28
        GFSerialiseVectorBinaryOut_CReverbEnviro_0046be4d(m_pSaveStream, value);
        break;
    }
}
