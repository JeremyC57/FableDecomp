// Grade: DIFFER(75v77) -- load CSE only: ours loads the element once (`mov ecx,[eax+edi*4] ; test ecx,ecx`), retail compares in memory and reloads for the call (`cmp [eax+edi*4],0 ; jle ; mov ecx,[eax+edi*4]`) ; a volatile compare reproduces the in-memory cmp + reload but folds the address (`lea eax,[eax+edi*4] ; cmp [eax],0`), 78v77
#pragma optimize("s", on)
// CTCInventory::IsContextSensitiveOpinionExpressionAvailable() -- true when any context-sensitive slot holds an item of the opinion-expression type (6)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581D8E, length 77 bytes; size-optimised TU ("s").
// Callees: GetContextSensitiveType(long) 0x005D9B90 (__fastcall, item type in ecx; returns EContextSensitiveType).
// Ego_r PDB: ?IsContextSensitiveOpinionExpressionAvailable@CTCInventory@@QAE_NXZ.
// Retail layout: engine/CTCInventory.h is the wider Ego_r.pdb layout (ContextSensitiveItems at PDB +0x178,
// 16-byte vectors); the shipped object keeps ContextSensitiveItems as a 12-byte {first,last,end} vector<long>
// at +0x154, so `this` is modelled locally.
// Shape: counted loop over `size()` (re-evaluated), the element re-indexed for the call; the found exit is
// placed after the `ret` (`mov al,1 ; jmp`). Enumerator 6 is EContextSensitiveType's opinion-expression value.
enum EContextSensitiveType {
    CONTEXT_SENSITIVE_TYPE_OPINION_EXPRESSION = 6
};

EContextSensitiveType __fastcall GetContextSensitiveType(long itemType);   // 0x005D9B90

struct CLongVector {
    long* first;                                     // +0x00
    long* last;                                      // +0x04
    long* end_of_storage;                            // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
};

struct CTCInventory {
    unsigned char _pad_0x000[0x154];
    CLongVector   ContextSensitiveItems;             // +0x154 (PDB +0x178)
    bool IsContextSensitiveOpinionExpressionAvailable();
};

bool CTCInventory::IsContextSensitiveOpinionExpressionAvailable()
{
    for (unsigned long i = 0; i < ContextSensitiveItems.size(); ++i) {
        if (ContextSensitiveItems[i] > 0 &&
            GetContextSensitiveType(ContextSensitiveItems[i]) == CONTEXT_SENSITIVE_TYPE_OPINION_EXPRESSION)
            return true;
    }
    return false;
}
