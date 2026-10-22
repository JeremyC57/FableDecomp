// Grade: DIFFER(603v607) -- compaction count lives in eax (retail ecx, entries base eax); RenderFlag bool extracted shr/and dl (retail and 2/shr 1 in ecx); the shadow-mode bool is sete (retail mov al,1/je/xor al,al); render-loop alignment jmp
// CEngineLandscapeRenderer::RenderSortedPatches() -- render the z-sorted landscape patches: with vertex and pixel shaders forced on and a render-state capture block open, compact the sorter list (registered entries move down and get their new sorter handle, unregistered ones get -1), shrink it, set the common states and the shadow-mode lighting flag, render every patch (updating its sort key and flag in place), then clear the render states and restore
// Leaf: RenderSortedPatches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B68DA0, length 607 bytes (plain `ret`).
// Callees: CShaderRenderManager::EnableVertexShaders @ 0x00987FE0 / DisableVertexShaders @ 0x009880E0 / EnablePixelShaders @ 0x00988110 /
//   DisablePixelShaders @ 0x00988190 (manager @ 0x01436E14), CRenderStateManager::RestoreCaptureBlock @ 0x00A05840 (@ 0x013CB508),
//   vector<CSortedPatchEntry>::_M_fill_insert @ 0x00B6C840, SetupCommonStates @ 0x00B67480, CEngineLightingManager::UpdateGlobalLightingStates
//   @ 0x00B46C80 (manager @ 0x01436E9C), CLandscapeBackgroundPatch::Render(bool, long*, bool*) @ 0x00BE7BE0, PostRenderClearRenderStates @ 0x00B67510.
// Layout: the sorter list is the retail STLport vector at +0x5E4 (landed UnregisterSubPatchForZSorting 0x00B677A0); entries are a patch pointer
//   and a flag word (bit 0 registered, bit 1 the render flag, bits 2.. the sort key); the patch's ZSorterHandle is +0x100; the lighting
//   manager's ShadowMode +0x4694, FadedShadowDiffuseFactorEnabled +0xCE, StateUpdateFlags +0xE4 (retail offsets, 12-byte vectors).
// Shape: the shader switch is the landed Render 0x00B6B0B0 scope (inline constructor / restoring destructor) through the manager POINTER,
//   in an inner block so the capture block is restored after it; capture block = the 0x00B68610 inline (`StateStackSize++`); the
//   shrink is STLport resize(n) (erase = the __copy loop + finish store, grow = _M_fill_insert with a zeroed T() temporary).
struct CStateInfo { unsigned char _body[0x14]; };
struct CStateStackElement { CStateInfo* State; unsigned long Value; };
struct CRenderStateManager {
    void*              __vftable;            // +0x00
    void*              PD3DDevice;           // +0x04
    CStateStackElement StateStack[1024];     // +0x08
    CStateInfo*        StateUpdateList[256]; // +0x2008
    CStateInfo*        AllStatesList[256];   // +0x2408
    long               StateStackSize;       // +0x2808
    long               StateUpdateListSize;  // +0x280c
    long               AllStatesListSize;    // +0x2810
    unsigned long      CurrentBookmarkMask;  // +0x2814
    unsigned char      DeviceState[0x3a24 - 0x2818];   // +0x2818 CDeviceState
    CStateInfo         BookmarkDummyState;   // +0x3a24
    void CreateCaptureBlock()
    {
        long& size = StateStackSize;
        StateStack[size].State = &BookmarkDummyState;
        StateStackSize++;
        CurrentBookmarkMask <<= 1;
    }
    void RestoreCaptureBlock();              // 0x00A05840
};
extern CRenderStateManager* g_pRenderStateManager;   // 0x013CB508

struct CShaderRenderManager {
    unsigned char _pad000[0x390];
    bool          VertexShadersEnabled;          // +0x390
    bool          PixelShadersEnabled;           // +0x391
    unsigned char _pad392[0x3a0 - 0x392];
    void EnableVertexShaders();                  // 0x00987FE0
    void DisableVertexShaders();                 // 0x009880E0
    void EnablePixelShaders();                   // 0x00988110
    void DisablePixelShaders();                  // 0x00988190
    void SetVertexShadersEnabled(bool enable, bool* old)
    {
        if (old)
            *old = VertexShadersEnabled;
        if (enable != VertexShadersEnabled) {
            if (enable)
                EnableVertexShaders();
            else
                DisableVertexShaders();
        }
    }
    void SetPixelShadersEnabled(bool enable, bool* old)
    {
        if (old)
            *old = PixelShadersEnabled;
        if (enable != PixelShadersEnabled) {
            if (enable)
                EnablePixelShaders();
            else
                DisablePixelShaders();
        }
    }
};
extern CShaderRenderManager* g_pShaderRenderManager;   // 0x01436E14

struct CShaderStatesScope {
    bool OldVertexShaders;                       // +0x00
    bool OldPixelShaders;                        // +0x01
    CShaderStatesScope(bool vertexShaders, bool pixelShaders)
    {
        g_pShaderRenderManager->SetVertexShadersEnabled(vertexShaders, &OldVertexShaders);
        g_pShaderRenderManager->SetPixelShadersEnabled(pixelShaders, &OldPixelShaders);
    }
    ~CShaderStatesScope()
    {
        g_pShaderRenderManager->SetVertexShadersEnabled(OldVertexShaders, 0);
        g_pShaderRenderManager->SetPixelShadersEnabled(OldPixelShaders, 0);
    }
};

struct CEngineLightingManager {
    unsigned char _pad0000[0xce];
    bool          FadedShadowDiffuseFactorEnabled;   // +0xce
    unsigned char _pad00cf[0xe4 - 0xcf];
    unsigned long StateUpdateFlags;              // +0xe4
    unsigned char _pad00e8[0x4694 - 0xe8];
    long          ShadowMode;                    // +0x4694
    void SetFadedShadowDiffuseFactorEnabled(bool enable)
    {
        if (FadedShadowDiffuseFactorEnabled != enable) {
            FadedShadowDiffuseFactorEnabled = enable;
            StateUpdateFlags |= 1;
        }
    }
    void UpdateGlobalLightingStates();           // 0x00B46C80
};
extern CEngineLightingManager* g_pEngineLightingManager;   // 0x01436E9C

struct CLandscapeBackgroundPatch {
    unsigned char _pad000[0x100];
    long          ZSorterHandle;                 // +0x100
    void Render(bool sorted, long* sortKey, bool* flag);   // 0x00BE7BE0
};

struct CSortedPatchEntry {                         // 8 bytes
    CLandscapeBackgroundPatch* Patch;
    unsigned long Registered : 1;
    unsigned long RenderFlag : 1;
    long          SortKey : 30;
};

template <class T> T* __copy(T* first, T* last, T* result)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

struct CSortedPatchVector {                        // STLport vector<CSortedPatchEntry>, 12 bytes
    CSortedPatchEntry* _M_start;
    CSortedPatchEntry* _M_finish;
    CSortedPatchEntry* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CSortedPatchEntry& operator[](unsigned long i) { return _M_start[i]; }
    void _M_fill_insert(CSortedPatchEntry* position, unsigned long count, const CSortedPatchEntry& value);   // 0x00B6C840
    void erase(CSortedPatchEntry* first, CSortedPatchEntry* last)
    {
        CSortedPatchEntry* i = __copy(last, _M_finish, first);
        _M_finish = i;
    }
    void resize(unsigned long newSize, const CSortedPatchEntry& value)
    {
        if (newSize < size())
            erase(_M_start + newSize, _M_finish);
        else
            _M_fill_insert(_M_finish, newSize - size(), value);
    }
    void resize(unsigned long newSize) { resize(newSize, CSortedPatchEntry()); }
};

struct CEngineLandscapeRenderer {
    unsigned char      _pad000[0x5e4];
    CSortedPatchVector SortedPatches;              // +0x5e4
    void SetupCommonStates();                      // 0x00B67480
    void PostRenderClearRenderStates();            // 0x00B67510
    void RenderSortedPatches();
};

void CEngineLandscapeRenderer::RenderSortedPatches()
{
    {
        CShaderStatesScope shaderStates(true, true);
        g_pRenderStateManager->CreateCaptureBlock();
        unsigned long count = 0;
        for (unsigned long i = 0; i < SortedPatches.size(); ++i) {
            CSortedPatchEntry* entries = SortedPatches._M_start;
            CSortedPatchEntry& entry = entries[i];
            if (entry.Registered) {
                if ((long)i > (long)count) {
                    entries[count].Patch = entry.Patch;
                    entries[count].SortKey = entry.SortKey;
                    entries[count].Registered = 0;
                    entries[count].Patch->ZSorterHandle = count;
                    ++count;
                } else {
                    entry.Registered = 0;
                    ++count;
                }
            } else if (entry.Patch) {
                entry.Patch->ZSorterHandle = -1;
            }
        }
        SortedPatches.resize(count);
        SetupCommonStates();
        g_pEngineLightingManager->SetFadedShadowDiffuseFactorEnabled(g_pEngineLightingManager->ShadowMode == 1);
        g_pEngineLightingManager->UpdateGlobalLightingStates();
        for (unsigned long j = 0; j < SortedPatches.size(); ++j) {
            CSortedPatchEntry& entry = SortedPatches[j];
            long sortKey = entry.SortKey;
            bool flag = entry.RenderFlag;
            entry.Patch->Render(true, &sortKey, &flag);
            entry.SortKey = sortKey;
            entry.RenderFlag = flag;
        }
        PostRenderClearRenderStates();
    }
    g_pRenderStateManager->RestoreCaptureBlock();
}
