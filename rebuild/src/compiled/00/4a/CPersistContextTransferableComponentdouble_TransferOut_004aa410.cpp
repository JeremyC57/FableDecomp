// CPersistContext_TransferableComponent<double>::TransferOut(CPersistContext*, CCharString*) -- truncates the double behind the target pointer to a long local and hands its address to the long writer 0x009E2560
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA410, 36 bytes.
// Callees: 0x00BFEA70 __ftol2 (CRT, implicit), 0x009E2560 TransferOutLong (__fastcall: context in ecx,
// output string in edx, &value on the stack).
//
// Persist WRITE hook (vtable slot 0) of a double component that is saved as a whole number: the
// component holds a pointer to the double at +0x4; `fld qword ptr [eax] ; call __ftol2` is the C cast
// to long, the result is stored into the `push ecx` frame slot whose address goes to the long writer.
class CPersistContext;
class CCharString;
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560

struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, CCharString* out);
    double* m_pValue;                      // +0x4
};

void CPersistContext_TransferableComponent::TransferOut(CPersistContext* context, CCharString* out)
{
    long value = (long)*m_pValue;
    TransferOutLong(context, out, &value);
}