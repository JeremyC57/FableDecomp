// CUserProfileManager::GetMaxResolution() const -- index of the largest display mode allowed by the command-line width cap: with no cap (-1) the last mode, otherwise the last mode (in the sorted list) whose width does not exceed the cap, 0 when none does; works on a copy of DisplayModes
// Leaf: GetMaxResolution
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00408900, length 143 bytes (padded to 144), `ret`.
// Callees: vector<CDisplayModeDesc> copy ctor @ 0x00410460 (catalog label "std_vector_C2DLineF_Copy" is the
// propagated name of the same 16-byte-element body), free @ 0x00BFEA14 (the inline vector dtor, three copies).
// Data: g_FableConfigWidth @ 0x01375084 (the width cap set by GFConfigDetection, -1 = none).
// Layout: CUserProfileManager::DisplayModes at retail +0x80 (Ego_r.pdb 0x88: two earlier vectors are 12 bytes in
// retail); CDisplayModeDesc (16 bytes): Dimensions (C2DExtentsI: Width, Height) +0, BitsPerPixel +8, RefreshRate +0xc.
// Shape: the capped branch first (retail lays the uncapped `size() - 1` return last); the walk counts in esi from -1
// and stops on the first wider mode.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
extern long g_FableConfigWidth;                    // 0x01375084
struct C2DExtentsI { long Width; long Height; };
struct CDisplayModeDesc {
    C2DExtentsI Dimensions;                        // +0x00
    long        BitsPerPixel;                      // +0x08
    long        RefreshRate;                       // +0x0c
};
struct CDisplayModeVector {                        // STLport vector<CDisplayModeDesc> (12 bytes in retail)
    CDisplayModeDesc* _M_start;
    CDisplayModeDesc* _M_finish;
    CDisplayModeDesc* _M_end_of_storage;
    CDisplayModeVector(const CDisplayModeVector& other);   // 0x00410460
    ~CDisplayModeVector() { if (_M_start) free(_M_start); }
    CDisplayModeDesc* begin() const { return _M_start; }
    CDisplayModeDesc* end() const { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};
struct CUserProfileManager {
    unsigned char      _pad00[0x80];
    CDisplayModeVector DisplayModes;               // +0x80
    long GetMaxResolution() const;
};
long CUserProfileManager::GetMaxResolution() const
{
    CDisplayModeVector modes(DisplayModes);
    if (g_FableConfigWidth != -1)
    {
        long index = -1;
        for (CDisplayModeDesc* it = modes.begin(); it != modes.end(); ++it)
        {
            if (it->Dimensions.Width > g_FableConfigWidth)
                break;
            ++index;
        }
        if (index != -1)
            return index;
        return 0;
    }
    return modes.size() - 1;
}