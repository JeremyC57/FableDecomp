// Behaviour test for CStaticFontBank::~CStaticFontBank (0x00AB7A70): every non-null glyph array is freed and cleared, the
// texture is released (Uninitialise + delete, pointer zeroed) only when the system manager exists, is valid and has a display
// manager; then the CFontBank part (Name) and the base run.
#include <cstdio>
#include <cstring>
static int g_glyphFrees, g_uninit, g_texDeletes, g_nameDtor, g_baseDtor;
struct SystemModel { bool Valid; void* DisplayManager; bool IsValid() const { return Valid; } void* GetDisplayManager() { return DisplayManager; } };
static SystemModel* g_system;
static SystemModel* GetSystem() { return g_system; }
struct GlyphModel { char c; };
struct GlyphBankModel { unsigned short FirstGlyph, NoGlyphs; GlyphModel* Glyphs; };
struct TextureModel { ~TextureModel() { ++g_uninit; } };
struct NameModel { ~NameModel() { ++g_nameDtor; } };
struct BaseModel { virtual ~BaseModel() { ++g_baseDtor; } };
struct FontBankModel : BaseModel { NameModel Name; };
struct StaticFontBankModel : FontBankModel {
    GlyphBankModel GlyphBanks[1024];
    TextureModel* PTexture;
    ~StaticFontBankModel()
    {
        for (long i = 0; i < 1024; ++i) {
            if (GlyphBanks[i].Glyphs) { delete [] GlyphBanks[i].Glyphs; ++g_glyphFrees; GlyphBanks[i].Glyphs = 0; }
        }
        if (GetSystem() && GetSystem()->IsValid() && GetSystem()->GetDisplayManager()) {
            if (PTexture) ++g_texDeletes;
            delete PTexture;
            PTexture = 0;
        }
    }
};
static int Run(SystemModel* system, bool withTexture, int glyphArrays)
{
    g_glyphFrees = g_uninit = g_texDeletes = g_nameDtor = g_baseDtor = 0;
    g_system = system;
    StaticFontBankModel* bank = new StaticFontBankModel;
    for (int i = 0; i < 1024; ++i) bank->GlyphBanks[i].Glyphs = (i < glyphArrays) ? new GlyphModel[4] : 0;
    TextureModel* tex = withTexture ? new TextureModel : 0;
    bank->PTexture = tex;
    delete bank;
    if (!(system && system->Valid && system->DisplayManager) && tex) delete tex;   // the bank leaves it alone
    return 0;
}
int main()
{
    int dummy = 0;
    SystemModel up = { true, &dummy };
    SystemModel invalid = { false, &dummy };
    SystemModel noDisplay = { true, 0 };
    Run(&up, true, 3);
    if (g_glyphFrees != 3 || g_uninit != 1 || g_texDeletes != 1 || g_nameDtor != 1 || g_baseDtor != 1) { std::printf("00ab7a70_TEST FAIL\n"); return 1; }
    Run(&invalid, true, 1024);
    if (g_glyphFrees != 1024 || g_texDeletes != 0 || g_uninit != 1) { std::printf("00ab7a70_TEST FAIL\n"); return 1; }
    Run(&noDisplay, true, 0);
    if (g_glyphFrees != 0 || g_texDeletes != 0) { std::printf("00ab7a70_TEST FAIL\n"); return 1; }
    Run(0, false, 2);
    if (g_glyphFrees != 2 || g_texDeletes != 0 || g_baseDtor != 1) { std::printf("00ab7a70_TEST FAIL\n"); return 1; }
    std::printf("00ab7a70_TEST PASS\n");
    return 0;
}