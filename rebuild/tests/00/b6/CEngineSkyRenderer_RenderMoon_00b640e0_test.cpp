#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace {
void check(bool ok)
{
    if (!ok)
        std::abort();
}

enum Event {
    OpenCapture, MoonVectorRead, GraphicBankRead, PeekInfoFrame, FrameIsLoaded,
    RequestLoad, GraphicFrameCreate, GetFrameMipmap, TextureRead, AlphaState,
    GetEnvironment, GetParams, DrawSprite, GraphicFrameRelease, RestoreCapture
};

Event events[20];
unsigned event_count;
void record(Event event)
{
    check(event_count < sizeof(events) / sizeof(events[0]));
    events[event_count++] = event;
}

struct FixtureVector {
    float x, y, z;
    FixtureVector(float x_value = 0, float y_value = 0, float z_value = 0)
        : x(x_value), y(y_value), z(z_value) {}
    FixtureVector operator+(const FixtureVector& other) const {
        return FixtureVector(x + other.x, y + other.y, z + other.z);
    }
    FixtureVector operator*(float scale) const { return FixtureVector(x*scale,y*scale,z*scale); }
    FixtureVector& operator*=(float scale) {
        x *= scale;
        y *= scale;
        z *= scale;
        return *this;
    }
    FixtureVector operator^(const FixtureVector& other) const {
        return FixtureVector(y*other.z-z*other.y,z*other.x-x*other.z,x*other.y-y*other.x);
    }
    bool IsNonZero() const { return x*x+y*y+z*z > 1.0e-10f; }
    void Normalise() { *this *= 1.0f / std::sqrt(x*x + y*y + z*z); }
};

struct FixtureTexture {};
struct FixtureColour {
    unsigned char red, green, blue, alpha;
    void SetA(unsigned char value) { alpha = value; }
};
struct FixtureFloatColour {
    float red, green, blue;
    operator FixtureColour() const {
        FixtureColour colour = { static_cast<unsigned char>(red*255.0f),
                                 static_cast<unsigned char>(green*255.0f),
                                 static_cast<unsigned char>(blue*255.0f), 0 };
        return colour;
    }
};
struct FixtureThemeChannel { FixtureFloatColour Colours[9]; };
struct FixtureEnvironmentParams { FixtureThemeChannel ThemeChannels[1]; };
struct FixtureEnvironment {
    FixtureEnvironmentParams params;
    const FixtureEnvironmentParams& PeekInterpolatedParams() {
        record(GetParams);
        return params;
    }
};
struct FixtureWorld {
    FixtureEnvironment environment;
    FixtureEnvironment* DrawGetEnvironment() {
        record(GetEnvironment);
        return &environment;
    }
};
struct FixtureWorldMap {
    FixtureWorld world;
    FixtureWorld* PeekGameWorld() { return &world; }
};
struct FixtureStateBlocks {
    void ApplyAlphaStateBlock() { record(AlphaState); }
};
struct FixtureLighting {
    FixtureVector light;
    const FixtureVector& PeekMoonVector() const {
        record(MoonVectorRead);
        return light;
    }
};
struct FixtureCapture {
    FixtureCapture() { record(OpenCapture); }
    ~FixtureCapture() { record(RestoreCapture); }
};
struct FixtureFrame {
    FixtureTexture* result;
    FixtureFrame() : result(0) { record(GraphicFrameCreate); }
    ~FixtureFrame() { record(GraphicFrameRelease); }
    FixtureTexture* GetTexture() {
        check(result != 0);
        record(TextureRead);
        return result;
    }
};
struct FixtureFrameInfo {
    bool loaded;
    bool IsLoaded() const {
        record(FrameIsLoaded);
        return loaded;
    }
    void BeginLoading(unsigned a, unsigned b) {
        check(a == 0 && b == 0);
        record(RequestLoad);
    }
};
struct FixtureBank {
    FixtureFrameInfo info;
    FixtureTexture texture;
    bool get_frame;
    unsigned long wanted_index;
    FixtureFrameInfo* PeekGraphicInfoFrame(unsigned long index, unsigned mip) {
        check(index == wanted_index && mip == 0);
        record(PeekInfoFrame);
        return &info;
    }
    bool GetGraphicFrameMipmap(FixtureFrame& frame, unsigned long index, unsigned mip, long variant, unsigned flags) {
        check(index == wanted_index && mip == 0 && variant == 0 && flags == 0);
        record(GetFrameMipmap);
        if (get_frame)
            frame.result = &texture;
        return get_frame;
    }
};
enum { MAIN_GRAPHIC_BANK_NO = 0 };
struct FixtureResource {
    FixtureBank bank;
    FixtureBank* PeekGraphicBank(unsigned bank_index) {
        check(bank_index == MAIN_GRAPHIC_BANK_NO);
        record(GraphicBankRead);
        return &bank;
    }
};
struct FixtureSkyDef {
    unsigned long MoonTexture;
    float MoonRadius;
};
struct FixtureSky {
    FixtureSkyDef* PSkyDef;
    FixtureTexture* drawn;
    FixtureVector centre, right, up;
    FixtureColour colour;
    void RenderMoon(float, const FixtureVector&);
    void RenderSprite(FixtureTexture* texture, const FixtureVector& position,
                      const FixtureVector& right_axis, const FixtureVector& up_axis,
                      FixtureColour tint) {
        record(DrawSprite);
        drawn = texture;
        centre = position;
        right = right_axis;
        up = up_axis;
        colour = tint;
    }
};
}

namespace NEngineInternal {
static FixtureResource* GPResourceManager;
static FixtureLighting* GPLightingManager;
static FixtureWorldMap* GPEngineWorldMap;
static FixtureStateBlocks* GPEngineStateBlocks;
}

void FixtureSky::RenderMoon(float,const FixtureVector& moon_origin)
{
    FixtureCapture capture_block;
    FixtureVector moon_vector = NEngineInternal::GPLightingManager->PeekMoonVector();
    const unsigned long moon_index = PSkyDef->MoonTexture;
    if (!NEngineInternal::GPResourceManager->PeekGraphicBank(MAIN_GRAPHIC_BANK_NO)
          ->PeekGraphicInfoFrame(moon_index,0)->IsLoaded()) {
        NEngineInternal::GPResourceManager->PeekGraphicBank(MAIN_GRAPHIC_BANK_NO)
            ->PeekGraphicInfoFrame(moon_index,0)->BeginLoading(0,0);
        return;
    }

    FixtureFrame moon_graphic;
    if (NEngineInternal::GPResourceManager->PeekGraphicBank(MAIN_GRAPHIC_BANK_NO)
        ->GetGraphicFrameMipmap(moon_graphic,moon_index,0,0,0)) {
        FixtureTexture* moon_texture = moon_graphic.GetTexture();
        NEngineInternal::GPEngineStateBlocks->ApplyAlphaStateBlock();
        const FixtureEnvironmentParams& params = NEngineInternal::GPEngineWorldMap->PeekGameWorld()
            ->DrawGetEnvironment()->PeekInterpolatedParams();
        FixtureColour colour = params.ThemeChannels[0].Colours[8];
        colour.SetA(255);
        FixtureVector moon_pos = moon_origin + moon_vector * 2000.0f;
        FixtureVector right = moon_vector ^ FixtureVector(0.0f,0.0f,1.0f);
        if (right.IsNonZero())
            right.Normalise();
        else
            right = FixtureVector(1.0f,0.0f,0.0f);
        FixtureVector up = right ^ moon_vector;
        right *= PSkyDef->MoonRadius * 0.001f * 2000.0f;
        up *= PSkyDef->MoonRadius * 0.001f * 2000.0f;
        RenderSprite(moon_texture,moon_pos,right,up,colour);
    }
}

namespace {
template<unsigned Count> void check_events(const Event (&expected)[Count])
{
    check(event_count == Count);
    for (unsigned i = 0; i != Count; ++i)
        check(events[i] == expected[i]);
    event_count = 0;
}
void near_vector(const FixtureVector& v, float x, float y, float z)
{
    check(std::fabs(v.x - x) < 0.002f);
    check(std::fabs(v.y - y) < 0.002f);
    check(std::fabs(v.z - z) < 0.002f);
}
}

int main()
{
    FixtureResource resources;
    FixtureLighting lighting;
    FixtureWorldMap world_map;
    FixtureStateBlocks state_blocks;
    NEngineInternal::GPResourceManager = &resources;
    NEngineInternal::GPLightingManager = &lighting;
    NEngineInternal::GPEngineWorldMap = &world_map;
    NEngineInternal::GPEngineStateBlocks = &state_blocks;

    FixtureSkyDef def = { 47, 15.0f };
    FixtureSky sky;
    sky.PSkyDef = &def;
    sky.drawn = 0;
    resources.bank.wanted_index = def.MoonTexture;
    FixtureFloatColour moon_tint = { 0.5f, 0.25f, 1.0f };
    world_map.world.environment.params.ThemeChannels[0].Colours[8] = moon_tint;
    const FixtureVector view_origin(10,-20,30);

    static const Event nonresident[] = { OpenCapture, MoonVectorRead, GraphicBankRead,
        PeekInfoFrame, FrameIsLoaded, GraphicBankRead, PeekInfoFrame, RequestLoad, RestoreCapture };
    resources.bank.info.loaded = false;
    resources.bank.get_frame = true;
    sky.RenderMoon(0, view_origin);
    check_events(nonresident);
    check(sky.drawn == 0);

    static const Event missing_mip[] = { OpenCapture, MoonVectorRead, GraphicBankRead,
        PeekInfoFrame, FrameIsLoaded, GraphicFrameCreate, GraphicBankRead, GetFrameMipmap,
        GraphicFrameRelease, RestoreCapture };
    resources.bank.info.loaded = true;
    resources.bank.get_frame = false;
    sky.RenderMoon(0.5f, view_origin);
    check_events(missing_mip);
    check(sky.drawn == 0);

    static const Event drawn[] = { OpenCapture, MoonVectorRead, GraphicBankRead,
        PeekInfoFrame, FrameIsLoaded, GraphicFrameCreate, GraphicBankRead, GetFrameMipmap,
        TextureRead, AlphaState, GetEnvironment, GetParams, DrawSprite, GraphicFrameRelease, RestoreCapture };
    resources.bank.get_frame = true;
    lighting.light = FixtureVector(0.6f,0.8f,0);
    sky.RenderMoon(0.9f, view_origin);
    check_events(drawn);
    check(sky.drawn == &resources.bank.texture);
    check(sky.colour.red == 127 && sky.colour.green == 63 && sky.colour.blue == 255 && sky.colour.alpha == 255);
    near_vector(sky.centre, 1210,1580,30);
    near_vector(sky.right, 24,-18,0);
    near_vector(sky.up, 0,0,30);

    lighting.light = FixtureVector(0,0,1);
    sky.RenderMoon(0.2f, view_origin);
    check_events(drawn);
    near_vector(sky.centre, 10,-20,2030);
    near_vector(sky.right, 30,0,0);
    near_vector(sky.up, 0,-30,0);

    lighting.light = FixtureVector(0,0,-1);
    def.MoonRadius = 5.0f;
    sky.RenderMoon(1.0f, view_origin);
    check_events(drawn);
    near_vector(sky.centre, 10,-20,-1970);
    near_vector(sky.right, 10,0,0);
    near_vector(sky.up, 0,10,0);
    std::puts("00b640e0_TEST PASS");
}