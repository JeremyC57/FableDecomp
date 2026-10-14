// FreeComRecord_00cb1f20(SComRecord*) -- release a CoTaskMem record: its payload, its interface, then itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1F20, oracle length 69 bytes.
// Callees: CoTaskMemFree (ole32, IAT 0x01440630, loaded once into ebx), IUnknown::Release (virtual slot 2, __stdcall with
//          `this` on the stack).
//
// Retail bytes:  push esi ; mov esi,[esp+8] ; test esi,esi ; je E ; mov eax,[esi+40h] ; test eax,eax ; push ebx ; mov ebx,[__imp_CoTaskMemFree] ; je A ;
//                mov eax,[esi+44h] ; push eax ; call ebx ; mov dword ptr [esi+40h],0 ; mov dword ptr [esi+44h],0 ; A: mov eax,[esi+3Ch] ; test eax,eax ; je B ;
//                mov ecx,[eax] ; push eax ; call [ecx+8] ; mov dword ptr [esi+3Ch],0 ; B: push esi ; call ebx ; pop ebx ; E: pop esi ; ret 4
// The record comes from the stack (`ret 4`, ecx unused): a __stdcall free function.  The COM
// `Release` call pushes the interface pointer itself (stdcall member convention) and the import is
// hoisted into a callee-saved register for its two uses.  The manifest's "CBankFileAsyncData::FreeBuffer"
// label is propagated; the record is named by shape.
extern "C" __declspec(dllimport) void __stdcall CoTaskMemFree(void* block);   // IAT 0x01440630

struct IUnknown_00cb1f20 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();     // slot 2 (+0x08)
};

struct SComRecord_00cb1f20 {
    unsigned char      _pad[0x3c];
    IUnknown_00cb1f20* m_pInterface;   // +0x3c
    unsigned long      m_payloadSize;  // +0x40
    void*              m_pPayload;     // +0x44
};

void __stdcall FreeComRecord_00cb1f20(SComRecord_00cb1f20* record)
{
    if (record != 0) {
        if (record->m_payloadSize != 0) {
            CoTaskMemFree(record->m_pPayload);
            record->m_payloadSize = 0;
            record->m_pPayload = 0;
        }
        if (record->m_pInterface != 0) {
            record->m_pInterface->Release();
            record->m_pInterface = 0;
        }
        CoTaskMemFree(record);
    }
}