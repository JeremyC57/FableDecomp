// CEngineCamera::ConsoleRunDebugScript(CEngineCamera& camera, const vector<CGenericVar>& args) -- console command: starts the camera debug script from its first element, stamping the element start time
// Leaf: ConsoleRunDebugScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2FC10, length 33 bytes (plain `ret`).
// Signature from Ego_r.pdb: ?ConsoleRunDebugScript@CEngineCamera@@CIXAAV1@ABV?$vector@VCGenericVar@@...@std@@@Z
// (private static __fastcall: camera in ecx, the unused argument vector in edx). Siblings in the PDB name set:
// ConsoleAddDebugScript / ConsoleStopDebugScript / ProcessDebugScript.
// Callees: 0x009E1BC0 GFGetTime() (returns a double on the x87 stack).
// Shape: DebugScriptRunning = true (byte), CurrentDebugScriptElement = 0, then the `fstp qword` of GFGetTime() into
// DebugScriptElementStartTime -- stores in source order.
// Model: engine/CEngineCamera.h puts these members 4 bytes high (CurrentDebugScriptElement +0x218, DebugScriptRunning
// +0x21C) because the PDB's CArray<CDebugScriptElement> DebugScript is 0x10 bytes; retail CArray is 0x0C (GOTCHAS wave 23),
// so retail has CurrentDebugScriptElement +0x214, DebugScriptRunning +0x218 and the 8-aligned double still at +0x220.
// The camera is therefore modelled locally with the retail offsets.
class CGenericVarVector;                                     // std::vector<CGenericVar> (unused here)

double __fastcall GFGetTime();                                          // 0x009E1BC0

class CEngineCamera {
public:
    unsigned char _pad_0x000[0x208];
    unsigned char DebugScript[0x0C];                         // +0x208 CArray<CEngineCamera::CDebugScriptElement> (retail 12 bytes)
    long          CurrentDebugScriptElement;                 // +0x214
    bool          DebugScriptRunning;                        // +0x218
    double        DebugScriptElementStartTime;               // +0x220

    static void __fastcall ConsoleRunDebugScript(CEngineCamera& camera, const CGenericVarVector& args);
};

void __fastcall CEngineCamera::ConsoleRunDebugScript(CEngineCamera& camera, const CGenericVarVector& args)
{
    camera.DebugScriptRunning = true;
    camera.CurrentDebugScriptElement = 0;
    camera.DebugScriptElementStartTime = GFGetTime();
}