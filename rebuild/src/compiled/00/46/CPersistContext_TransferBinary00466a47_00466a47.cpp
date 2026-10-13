#pragma optimize("s",on)
// CPersistContext::TransferBinary(T*) -- CheckCRC(\"\"), then GFSerialiseVectorBinaryOut_float @ 0x00466A75 on the save stream in mode 2, else GFSerialiseVectorBinaryIn_CRGBColour @ 0x00466AA7 on the load stream
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00466A47, oracle length 46 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push offset "" ; mov esi,ecx ; call CPersistContext::CheckCRC ; mov eax,[esi+18h] ; mov edx,[value] ; dec eax ; dec eax ; je L ; mov ecx,[esi+28h] ; call GFSerialiseVectorBinaryOut_float ; jmp R ; L: mov ecx,[esi+24h] ; call GFSerialiseVectorBinaryIn_CRGBColour ; R: pop esi ; ret 4
// Persist transfer of one value: after the CRC check with the (empty) key the mode at +0x18
// selects the direction -- 2 (load) hands the load stream (+0x24) and the value to the
// `__fastcall` reader @ 0x00466AA7, anything else hands the save stream (+0x28) and the value to
// the `__fastcall` writer @ 0x00466A75. The `dec/dec` sequence is the size-optimised lowering of
// the two-way switch on the mode (the lander's "s" pragma). Labels are propagated; the streams are modelled as opaque
// pointers. One of 24 bodies of this shape.
struct CDataOutputStream;
struct CMemoryDataInputStream;
struct SValue;

void __fastcall GFSerialiseVectorBinaryOut_float_00466a75(CDataOutputStream* stream, SValue* value);        // 0x00466A75
void __fastcall GFSerialiseVectorBinaryIn_CRGBColour_00466aa7(CMemoryDataInputStream* stream, SValue* value);   // 0x00466AA7

struct CPersistContext {
    unsigned char _pad0[0x18];
    long m_mode;                           // +0x18 (EMode: 2 = save)
    unsigned char _pad1[8];
    CMemoryDataInputStream* m_pLoadStream; // +0x24
    CDataOutputStream* m_pSaveStream;      // +0x28
    void CheckCRC(const char* name);       // 0x00404500
    void TransferBinary_00466a47(SValue* value);
};

void CPersistContext::TransferBinary_00466a47(SValue* value)
{
    CheckCRC("");
    switch (m_mode) {
    case 2:                                // load: the stream at +0x24
        GFSerialiseVectorBinaryIn_CRGBColour_00466aa7(m_pLoadStream, value);
        break;
    default:                               // save: the stream at +0x28
        GFSerialiseVectorBinaryOut_float_00466a75(m_pSaveStream, value);
        break;
    }
}