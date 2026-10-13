// CQuestManager::GetRegisteredQuestFromIndex(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF3C0, oracle length 38 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[esp+4] ; test eax,eax ; jl E ; mov edx,[ecx+30h] ; sub edx,[ecx+2Ch] ; sar edx,2 ; cmp eax,edx ; jae E ; mov ecx,[ecx+2Ch] ; lea eax,[ecx+eax*4] ; ret 4 ; E: mov eax,offset g_EmptyQuestName ; ret 4
// RegisteredQuestNames is a std::vector<CCharString> (4-byte elements); retail keeps its
// first/last pointers at +0x2C/+0x30 where engine/CQuestManager.h (Ego_r.pdb) places the vector at
// +0x34: the shipped manager is 0xC bytes smaller before it, hence the retail-offset view. Out of
// range indices (negative, or >= size as an unsigned compare) return the shared empty name at
// 0x013BD804 instead of null.

struct CCharString { void* PStringData; };

extern CCharString g_EmptyQuestName;       // 0x013BD804

struct CQuestManager_Retail {
    unsigned char _pad_0x00[0x28];
    unsigned char RegisteredQuestNames_Alloc[4];   // +0x28 allocator proxy (unused)
    CCharString*  RegisteredQuestNames_First;      // +0x2C (PDB vector at +0x34)
    CCharString*  RegisteredQuestNames_Last;       // +0x30
    CCharString* GetRegisteredQuestFromIndex(long index);
};

CCharString* CQuestManager_Retail::GetRegisteredQuestFromIndex(long index)
{
    if (index >= 0 &&
        (unsigned long)index < (unsigned long)(this->RegisteredQuestNames_Last - this->RegisteredQuestNames_First))
        return &this->RegisteredQuestNames_First[index];
    return &g_EmptyQuestName;
}
