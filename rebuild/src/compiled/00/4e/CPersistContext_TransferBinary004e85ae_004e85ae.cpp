#pragma optimize("s",on)
// CPersistContext::TransferBinary(T*) -- CheckCRC(\"\"), then GFSerialiseVectorBinaryOut_float @ 0x004E85DC on the save stream in mode 2, else GFSerialiseVectorBinaryIn_CRGBColour @ 0x004E860E on the load stream
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E85AE, oracle length 46 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push offset "" ; mov esi,ecx ; call CPersistContext::CheckCRC ; mov eax,[esi+18h] ; mov edx,[value] ; dec eax ; dec eax ; je L ; mov ecx,[esi+28h] ; call GFSerialiseVectorBinaryOut_float ; jmp R ; L: mov ecx,[esi+24h] ; call GFSerialiseVectorBinaryIn_CRGBColour ; R: pop esi ; ret 4
// Persist transfer of one value: after the CRC check with the (empty) key the mode at +0x18
// selects the direction -- 2 (load) hands the load stream (+0x24) and the value to the
// `__fastcall` reader @ 0x004E860E, anything else hands the save stream (+0x28) and the value to
// the `__fastcall` writer @ 0x004E85DC. The `dec/dec` sequence is the size-optimised lowering of
// the two-way switch on the mode (the lander's "s" pragma). Labels are propagated; the streams are modelled as opaque
// pointers. One of 24 bodies of this shape.
class CDataOutputStream;
class CMemoryDataInputStream;
struct SValue;

void __fastcall GFSerialiseVectorBinaryOut_float_004e85dc(CDataOutputStream* stream, SValue* value);        // 0x004E85DC
void __fastcall GFSerialiseVectorBinaryIn_CRGBColour_004e860e(CMemoryDataInputStream* stream, SValue* value);   // 0x004E860E

class CPersistContext { public:
    unsigned char _pad0[0x18];
    long m_mode;                           // +0x18 (EMode: 2 = save)
    unsigned char _pad1[8];
    CMemoryDataInputStream* m_pLoadStream; // +0x24
    CDataOutputStream* m_pSaveStream;      // +0x28
    protected: void CheckCRC(const char* name); public:       // 0x00404500
    void TransferBinary_004e85ae(SValue* value);
};

void CPersistContext::TransferBinary_004e85ae(SValue* value)
{
    CheckCRC("");
    switch (m_mode) {
    case 2:                                // load: the stream at +0x24
        GFSerialiseVectorBinaryIn_CRGBColour_004e860e(m_pLoadStream, value);
        break;
    default:                               // save: the stream at +0x28
        GFSerialiseVectorBinaryOut_float_004e85dc(m_pSaveStream, value);
        break;
    }
}