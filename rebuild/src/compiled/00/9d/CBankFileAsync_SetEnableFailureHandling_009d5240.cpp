// CBankFileAsync::SetEnableFailureHandling(bool) -- static: choose the allocation flags the async bank reads use (0 when failure handling is on, 2 when off)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5240, length 17 bytes (+ int3 padding).
// PDB (FableWin): ?SetEnableFailureHandling@CBankFileAsync@@SIX_N@Z -- a static __fastcall member, the bool in cl.
// Global: the flags word at 0x013CA868, read by CBankFileAsyncData::SetupBuffer (0x009D5344: `or ecx,3` when the
// read is not optional) as the last argument of the movable-resource allocation. Shape: `xor eax,eax ; test cl,cl ;
// setne al ; dec eax ; and eax,2` is VC7.1's branch-free `flag ? 0 : 2`.
class CBankFileAsync
{
public:
    static void __fastcall SetEnableFailureHandling(bool enable);
    static unsigned long s_allocationFlags;              // 0x013CA868
};

void __fastcall CBankFileAsync::SetEnableFailureHandling(bool enable)
{
    s_allocationFlags = enable ? 0 : 2;
}