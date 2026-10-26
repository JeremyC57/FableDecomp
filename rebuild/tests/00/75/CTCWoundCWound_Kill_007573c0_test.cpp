// Behaviour test (0x007573c0): CTCWound::CWound::Kill deletes each set decal id (wound/scar texture and bump) and zeroes it
#include <cstdio>
class CTCTextureDecal { public:
    long Deleted[8];
    int  Count;
    void DeleteDecal(long id) { Deleted[Count++] = id; }
};
class CWound {
public:
    unsigned char Desc[0x28];
    unsigned int  Duration;
    long          WoundTextureDecalEntryID;
    long          WoundBumpDecalEntryID;
    long          ScarTextureDecalEntryID;
    long          ScarBumpDecalEntryID;
    long          MorphIndex;
    void Kill(CTCTextureDecal& decals)
    {
        if (WoundTextureDecalEntryID) { decals.DeleteDecal(WoundTextureDecalEntryID); WoundTextureDecalEntryID = 0; }
        if (WoundBumpDecalEntryID) { decals.DeleteDecal(WoundBumpDecalEntryID); WoundBumpDecalEntryID = 0; }
        if (ScarTextureDecalEntryID) { decals.DeleteDecal(ScarTextureDecalEntryID); ScarTextureDecalEntryID = 0; }
        if (ScarBumpDecalEntryID) { decals.DeleteDecal(ScarBumpDecalEntryID); ScarBumpDecalEntryID = 0; }
    }
};
int main()
{
    CTCTextureDecal decals; decals.Count = 0;
    CWound w; w.WoundTextureDecalEntryID = 11; w.WoundBumpDecalEntryID = 0; w.ScarTextureDecalEntryID = 33; w.ScarBumpDecalEntryID = 44; w.MorphIndex = 5;
    w.Kill(decals);
    if (decals.Count != 3 || decals.Deleted[0] != 11 || decals.Deleted[1] != 33 || decals.Deleted[2] != 44) { std::printf("007573c0_TEST FAIL\n"); return 1; }
    if (w.WoundTextureDecalEntryID || w.WoundBumpDecalEntryID || w.ScarTextureDecalEntryID || w.ScarBumpDecalEntryID || w.MorphIndex != 5) { std::printf("007573c0_TEST FAIL\n"); return 1; }
    w.Kill(decals);                             // second kill: nothing left to delete
    if (decals.Count != 3) { std::printf("007573c0_TEST FAIL\n"); return 1; }
    std::printf("007573c0_TEST PASS\n");
    return 0;
}