// CQuestManager::GetRegisteredQuestFromIndex(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF3C0, oracle length 38 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[esp+4] ; test eax,eax ; jl E ; mov edx,[ecx+30h] ; sub edx,[ecx+2Ch] ; sar edx,2 ; cmp eax,edx ; jae E ; mov ecx,[ecx+2Ch] ; lea eax,[ecx+eax*4] ; ret 4 ; E: mov eax,offset g_EmptyQuestName ; ret 4
// RegisteredQuestNames is a std::vector<CCharString> (4-byte elements). In the shipped binary a
// std::vector is {first, last, end} (12 bytes, `first` at +0), 4 bytes smaller than in Ego_r.pdb;
// the two containers before it move the vector from the header's +0x34 to +0x2C, hence the
// retail-offset view. Out of range indices (negative, or >= size as an unsigned compare) return
// the shared empty name at 0x013BD804 instead of null.

struct CCharString { void* PStringData; };

extern CCharString g_EmptyQuestName;       // 0x013BD804

struct CQuestManager_Retail {
    unsigned char _pad_0x00[0x2C];
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
