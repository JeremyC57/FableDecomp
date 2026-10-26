// CDisplayEngineInit::CDisplayEngineInit() -- zero every field of the display-engine initialisation record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C44250, length 32 bytes.
// Callees: none. PDB CDisplayEngineInit (36 bytes): Component +0, DefinitionManager +4, PlayerManager +8, World +0xc,
// MainGraphicBank +0x10, Width +0x14, Height +0x18, CPixelFormat PixelFormat +0x1c (long FormatIndex), MeshBank +0x20.
// Shape: nine zero stores through one zero register, PlayerManager first (the only initialiser-list member; an
// all-initialiser-list form stores in declaration order), then the body assignments in order.
class CMainGameComponent; class CGameDefinitionManager; class CPlayerManager; class CWorld;
class CGraphicDataBank; class CMeshDataBank;

class CPixelFormat { public:
    long FormatIndex;
};

class CDisplayEngineInit { public:
    CMainGameComponent*     Component;             // +0x00
    CGameDefinitionManager* DefinitionManager;     // +0x04
    CPlayerManager*         PlayerManager;         // +0x08
    CWorld*                 World;                 // +0x0c
    CGraphicDataBank*       MainGraphicBank;       // +0x10
    unsigned long           Width;                 // +0x14
    unsigned long           Height;                // +0x18
    CPixelFormat            PixelFormat;           // +0x1c
    CMeshDataBank*          MeshBank;              // +0x20
    CDisplayEngineInit();
};

CDisplayEngineInit::CDisplayEngineInit()
    : PlayerManager(0)
{
    Component = 0;
    DefinitionManager = 0;
    World = 0;
    MainGraphicBank = 0;
    Width = 0;
    Height = 0;
    PixelFormat.FormatIndex = 0;
    MeshBank = 0;
}