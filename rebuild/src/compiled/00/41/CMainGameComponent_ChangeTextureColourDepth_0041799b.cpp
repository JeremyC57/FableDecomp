// CMainGameComponent::ChangeTextureColourDepth(long depth) -- switch the texture colour depth (16/24/32, when the display supports it and it changes), re-initialise the graphics, pass it to the display engine and the world
// Leaf: ?ChangeTextureColourDepth@CMainGameComponent@@UAEXJ@Z
// Retail: Fable.exe @ 0x0041799B, 83 bytes, `ret 4`; virtual (vtable reference 0x0122F18C); Ego_r.pdb
// ?ChangeTextureColourDepth@CMainGameComponent@@UAEXJ@Z.
// Body: only for depth 16, 24 or 32; the global display manager (0x013B8390) ->IsTextureBitDepthSupported(depth)
// (0x009BE530, Ego_r.pdb ?IsTextureBitDepthSupported@CDisplayManager@@QAE_NK@Z) and depth != the global texture colour
// depth (0x013B7D68); then store it, InitGraphics() (0x00416C8A, result dropped), PDisplayEngine (+0x28)
// ->ChangeTextureColourDepth(depth) (0x00434DC0), PWorld (+0x24) ->ChangeTextureColourDepth(depth) (0x0049D830).
// Layout: Ego_r.pdb CMainGameComponent PWorld +0x24, PDisplayEngine +0x28 (scoped_ptr data) -- retail agrees (landed
// 0x004179EE ChangeMaxTextureSize sibling uses the same two members).
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
class CDisplayManager {
public:
    bool IsTextureBitDepthSupported(unsigned long depth);   // 0x009BE530
};
extern CDisplayManager* g_pDisplayManager;                  // 0x013B8390
extern long g_TextureColourDepth;                           // 0x013B7D68
class CWorld {
public:
    void ChangeTextureColourDepth(long depth);              // 0x0049D830
};
class CDisplayEngine {
public:
    void ChangeTextureColourDepth(long depth);              // 0x00434DC0
};
class CMainGameComponent {
public:
    virtual void ChangeTextureColourDepth(long depth);
    unsigned char   _pad04[0x24 - 0x4];
    CWorld*         PWorld;                                 // +0x24 scoped_ptr<CWorld>
    CDisplayEngine* PDisplayEngine;                         // +0x28 scoped_ptr<CDisplayEngine>
protected:
    bool InitGraphics();                                    // 0x00416C8A
};
void CMainGameComponent::ChangeTextureColourDepth(long depth)
{
    if (depth == 16 || depth == 24 || depth == 32)
    {
        if (g_pDisplayManager->IsTextureBitDepthSupported(depth) && depth != g_TextureColourDepth)
        {
            g_TextureColourDepth = depth;
            InitGraphics();
            PDisplayEngine->ChangeTextureColourDepth(depth);
            PWorld->ChangeTextureColourDepth(depth);
        }
    }
}