#include <cassert>
#include <cstdio>
#include <vector>

struct TextureFixture {
    bool has_data, resident;
    unsigned mip, touched, requests, creations;
    bool BeginRequest()
    {
        if (has_data && resident) {
            ++touched;
            if (mip > 0)
                ++requests;
            return true;
        }
        if (!has_data) {
            ++creations;
            has_data = true;
        }
        ++requests;
        return false;
    }
};

struct CTexture {};
struct CGraphicInfoFrame {
    TextureFixture* fixture;
    bool GetTexture(const CTexture** out, int level, int flag) const {
        assert(!level && !flag && out);
        return fixture->BeginRequest();
    }
};
struct CGraphicBank {
    TextureFixture* const* table;
    unsigned count;
    std::vector<unsigned>* visits;
    mutable CGraphicInfoFrame graphic;
    const CGraphicInfoFrame* PeekGraphicInfoFrame(long key,int index) const {
        assert(index==0 && key>0);
        if ((unsigned long)key>=count || !table[key]) return 0;
        visits->push_back((unsigned)key);
        graphic.fixture=table[key];
        return &graphic;
    }
};
typedef int EGraphicBankNo;
struct CEngineResourceManager {
    CGraphicBank bank;
    CGraphicBank* PeekGraphicBank(EGraphicBankNo n) { assert(n==0); return &bank; }
};
struct CIDrawEnvironmentParams {
    struct CThemeChannel { struct TextureSet { unsigned Textures[2]; } TextureSets[3]; } ThemeChannels[1];
};
struct CEnvironment {
    CIDrawEnvironmentParams params;
    const CIDrawEnvironmentParams& PeekInterpolatedParams() const {return params;}
};
struct CWorld {
    CEnvironment environment;
    CEnvironment* DrawGetEnvironment(){return &environment;}
};
struct CEngineWorldMap { CWorld world; CWorld* PeekGameWorld(){return &world;} };
namespace NEngineInternal { CEngineWorldMap* GPEngineWorldMap; CEngineResourceManager* GPResourceManager; }
struct CEngineComponentLoadNewSceneParams {};
template<class T> class CArray:public std::vector<T> {};
struct CSkyDef {
    unsigned SunTexture,MoonTexture,StarTexture;
    struct Flare { unsigned Texture; };
    std::vector<Flare> FlareElements;
};
struct CEngineSkyRenderer {CSkyDef* PSkyDef;bool LoadNewScene(const CEngineComponentLoadNewSceneParams&);};

bool CEngineSkyRenderer::LoadNewScene(const CEngineComponentLoadNewSceneParams& load_params)
{
    const CIDrawEnvironmentParams& env_params = NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
    const CIDrawEnvironmentParams::CThemeChannel& theme_channel = env_params.ThemeChannels[0];
    CArray<unsigned long> texture_indices;
    for (unsigned long c_index = 0; c_index < 2; ++c_index) {
        texture_indices.push_back(theme_channel.TextureSets[0].Textures[c_index]);
        texture_indices.push_back(theme_channel.TextureSets[2].Textures[c_index]);
        texture_indices.push_back(theme_channel.TextureSets[1].Textures[c_index]);
    }
    texture_indices.push_back(PSkyDef->SunTexture);
    texture_indices.push_back(PSkyDef->MoonTexture);
    texture_indices.push_back(PSkyDef->StarTexture);
    long no_flare_elements = PSkyDef->FlareElements.size();
    for (long c_flare = 0; c_flare < no_flare_elements; ++c_flare)
        texture_indices.push_back(PSkyDef->FlareElements[c_flare].Texture);
    bool still_loading = false;
    for (unsigned long c_texture = 0; c_texture < texture_indices.size(); ++c_texture) {
        long texture_index = texture_indices[c_texture];
        if (texture_index) {
            const CGraphicInfoFrame* graphic_info = NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(texture_index, 0);
            if (graphic_info) {
                const CTexture* texture;
                if (!graphic_info->GetTexture(&texture, 0, 0))
                    still_loading = true;
            }
        }
    }
    return still_loading;
}

static bool LoadNewScene(const unsigned theme[3][2],const unsigned sky[3],const std::vector<unsigned>& flares,
                         TextureFixture* const* bank,unsigned bank_size,std::vector<unsigned>& visits) {
    CEngineWorldMap map;
    for(unsigned i=0;i<3;++i) for(unsigned j=0;j<2;++j)
        map.world.environment.params.ThemeChannels[0].TextureSets[i].Textures[j]=theme[i][j];
    CSkyDef def; def.SunTexture=sky[0];def.MoonTexture=sky[1];def.StarTexture=sky[2];
    for(unsigned i=0;i<flares.size();++i){CSkyDef::Flare f={flares[i]};def.FlareElements.push_back(f);}
    CEngineResourceManager resources;resources.bank.table=bank;resources.bank.count=bank_size;resources.bank.visits=&visits;
    NEngineInternal::GPResourceManager=&resources;NEngineInternal::GPEngineWorldMap=&map;
    CEngineSkyRenderer renderer={&def};CEngineComponentLoadNewSceneParams params;
    bool result=renderer.LoadNewScene(params);
    NEngineInternal::GPResourceManager=0;NEngineInternal::GPEngineWorldMap=0;
    return result;
}

int main()
{
    const unsigned theme[3][2] = { {1, 5}, {0, 3}, {2, 4} };
    const unsigned sky[3] = {1, 17, 0};
    const unsigned visit_order[] = {1, 2, 4, 3, 1, 3};
    for (unsigned resident = 0; resident < 16; ++resident) {
        for (unsigned data = resident; data < 16; ++data) {
            if ((data & resident) != resident)
                continue;
            TextureFixture textures[4] = { };
            TextureFixture* bank[6] = {0, textures, textures+1, textures+2, textures+3, 0};
            const unsigned visits_per_texture[] = {2, 1, 2, 1};
            for (unsigned i = 0; i < 4; ++i) {
                textures[i].resident = (resident & (1U << i)) != 0;
                textures[i].has_data = (data & (1U << i)) != 0;
                textures[i].mip = i%2;
            }
            std::vector<unsigned> flares(1, 3); flares.push_back(42);
            std::vector<unsigned> actual_order;
            assert(LoadNewScene(theme, sky, flares, bank, 6, actual_order) == (resident != 15));
            assert(actual_order == std::vector<unsigned>(visit_order, visit_order+6));
            for (unsigned i = 0; i < 4; ++i) {
                TextureFixture& texture = textures[i];
                assert(texture.touched == (texture.resident ? visits_per_texture[i] : 0));
                assert(texture.requests == (!texture.resident || texture.mip ? visits_per_texture[i] : 0));
                assert(texture.creations == ((data & (1U << i)) ? 0U : 1U));
            }
        }
    }
    const unsigned empty_theme[3][2] = { {0, 0}, {0, 0}, {0, 0} };
    const unsigned empty_sky[3] = {0, 0, 0};
    std::vector<unsigned> none;
    assert(!LoadNewScene(empty_theme, empty_sky, none, 0, 0, none));
    puts("00b63800_TEST PASS");
    return 0;
}
