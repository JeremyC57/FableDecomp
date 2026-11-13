// Behaviour test for 0x00B54E30 CEngineWeatherRenderer::RenderSnow (self-contained host model; plain C++98, no engine headers).
// Models: the frame fetch (CGraphicInfoFrame::GetFrame with mipmap level 0 / flags 0: create + begin loading and fail without frame data,
// begin loading and fail while the frame is not loaded, touch + share the frame when loaded and request the finer level when the loaded
// level is coarser), the shared-frame reference count, the clamped scaled wind, the shader constants 1..4 + the common block + the five
// per-set constants, the box walk with the saved camera visibility, and the plain / clipped render lists.
#include <cstdio>
#include <cmath>

struct Vec { float X, Y, Z; Vec() : X(0), Y(0), Z(0) {} Vec(float x, float y, float z) : X(x), Y(y), Z(z) {} };

struct Constant { unsigned long Index; float X, Y, Z, W; };
static Constant g_constants[64];
static int g_constant_count = 0;
static void SetConstant(unsigned long index, float x, float y, float z, float w)
{
    Constant c = { index, x, y, z, w };
    g_constants[g_constant_count++] = c;
}

// --- frame data -------------------------------------------------------------------------------------------------------------------
struct FrameData { unsigned long ReferenceCount; int Texture; };
struct Frame {
    FrameData* Data;
    Frame() : Data(0) {}
    ~Frame() { if (Data) { if (--Data->ReferenceCount == 0) delete Data; Data = 0; } }
    void Share(FrameData* data) { Data = data; if (Data) ++Data->ReferenceCount; }
};
struct InfoFrameData { FrameData* Loaded; int CurrentMipmapLevel; int TouchedMipmapLevel; unsigned long TouchedFrame; int BeginLoadingCalls; };
struct InfoFrame {
    InfoFrameData* Data;
    InfoFrame() : Data(0) {}
    bool GetFrame(Frame& frame, unsigned long bank_frame)
    {
        if (Data) {
            if (Data->Loaded) {
                Data->TouchedFrame = bank_frame;
                if (Data->TouchedMipmapLevel > 0) Data->TouchedMipmapLevel = 0;
                if (0 < Data->CurrentMipmapLevel) ++Data->BeginLoadingCalls;
                frame.Share(Data->Loaded);
                return true;
            }
            ++Data->BeginLoadingCalls;
            return false;
        }
        Data = new InfoFrameData();
        Data->Loaded = 0; Data->CurrentMipmapLevel = 7; Data->TouchedMipmapLevel = 127; Data->TouchedFrame = 0; Data->BeginLoadingCalls = 1;
        return false;
    }
};

// --- renderer model ---------------------------------------------------------------------------------------------------------------
struct Settings {
    float SnowFadeStart, SnowFadeEnd, SnowMaxAlpha, SnowWindSpeedMultiplier, SnowflakeBaseSize, SnowflakeScaleOffset;
    float SnowflakeScaleDistanceFactor, SnowflakeScaleDistanceSqrFactor, SnowflakeScaleMax, SnowflakeScaleMin;
    Vec ParticleBoxSpan; float WeatherWindMaxSpeed, WeatherWindMinSpeed, WeatherWindSpeedMultiplier;
    bool ParticleBoxLocked; Vec ParticleBoxPos[81]; float SnowTimePassed; float SnowWindFactors[5], SnowSpeedFactors[5];
};
struct Renderer {
    Settings S; int SavedBoxVisibility[729];
    Vec Plain[800]; int PlainCount; Vec Clipped[800]; int ClippedCount;
    int AttachedStage3; bool PointSprite, PointScale;
    Vec WindVector; float WindSpeed; long ViewportTLY, ViewportBRY;
    int ClipBoxToCamera(const Vec& p, float radius) const { return p.X < radius ? 0 : 1; }
    int ClipBoxToLandscape(const Vec& p) const { return p.Z < -1.0f ? 1 : 0; }
    int ClipBoxToWeatherMaskSphere(const Vec& p) const { return p.Y > 10.0f ? 1 : 0; }
    void SetCommonParticleShaderConstants(long& index) { SetConstant(index++, S.ParticleBoxSpan.X, S.ParticleBoxSpan.Y, S.ParticleBoxSpan.Z, 8388608.0f); }
    void RenderSnow(InfoFrame& info, unsigned long bank_frame)
    {
        Frame frame;
        if (!info.GetFrame(frame, bank_frame))
            return;
        AttachedStage3 = frame.Data->Texture;
        PointSprite = true; PointScale = true;
        float speed = WindSpeed * S.WeatherWindSpeedMultiplier;
        float clamped = speed < S.WeatherWindMinSpeed ? S.WeatherWindMinSpeed : (speed > S.WeatherWindMaxSpeed ? S.WeatherWindMaxSpeed : speed);
        Vec wind(WindVector.X * clamped, WindVector.Y * clamped, WindVector.Z * clamped);
        SetConstant(1, wind.X * S.SnowWindSpeedMultiplier, wind.Y * S.SnowWindSpeedMultiplier, wind.Z * S.SnowWindSpeedMultiplier, S.SnowTimePassed);
        SetConstant(2, S.SnowFadeEnd / (S.SnowFadeStart - S.SnowFadeEnd) + 1.0f, 1.0f / (S.SnowFadeEnd - S.SnowFadeStart), S.SnowMaxAlpha, 0.0f);
        SetConstant(3, (float)(ViewportBRY - ViewportTLY) * S.SnowflakeBaseSize, S.SnowflakeScaleOffset, S.SnowflakeScaleDistanceFactor, S.SnowflakeScaleDistanceSqrFactor);
        long constant = 5;
        SetConstant(4, S.SnowflakeScaleMin, S.SnowflakeScaleMax, 0.0f, 0.0f);
        SetCommonParticleShaderConstants(constant);
        for (long set = 0; set < 5; ++set)
            SetConstant(constant++, S.SnowWindFactors[set % 5], S.SnowSpeedFactors[set % 5], 0.0f, 0.0f);
        float max_z = S.ParticleBoxSpan.Z * 5.0f;
        long level = 1;
        for (float z = S.ParticleBoxSpan.Z * -4.0f; z < max_z; z += S.ParticleBoxSpan.Z, ++level) {
            for (long i = 0; i < 81; ++i) {
                Vec pos(S.ParticleBoxPos[i].X, S.ParticleBoxPos[i].Y, S.ParticleBoxPos[i].Z + z);
                float radius = S.SnowFadeEnd;
                int& visible = SavedBoxVisibility[i * level];
                if (!S.ParticleBoxLocked) visible = ClipBoxToCamera(pos, radius);
                if (visible != 0) continue;
                if (ClipBoxToLandscape(pos) != 0) continue;
                if (ClipBoxToWeatherMaskSphere(pos) == 1) Clipped[ClippedCount++] = pos;
                else Plain[PlainCount++] = pos;
            }
        }
    }
};

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f * (1.0f + std::fabs(b)); }

int main()
{
    static Renderer r;
    r.PlainCount = r.ClippedCount = 0; r.AttachedStage3 = -1; r.PointSprite = r.PointScale = false;
    r.S.SnowFadeStart = 2.0f; r.S.SnowFadeEnd = 30.0f; r.S.SnowMaxAlpha = 0.75f; r.S.SnowWindSpeedMultiplier = 0.5f;
    r.S.SnowflakeBaseSize = 0.01f; r.S.SnowflakeScaleOffset = 1.0f; r.S.SnowflakeScaleDistanceFactor = 0.1f;
    r.S.SnowflakeScaleDistanceSqrFactor = 0.01f; r.S.SnowflakeScaleMax = 4.0f; r.S.SnowflakeScaleMin = 0.5f;
    r.S.ParticleBoxSpan = Vec(8.0f, 8.0f, 2.0f); r.S.WeatherWindMaxSpeed = 3.0f; r.S.WeatherWindMinSpeed = 1.0f;
    r.S.WeatherWindSpeedMultiplier = 2.0f; r.S.ParticleBoxLocked = false; r.S.SnowTimePassed = 12.5f;
    for (int s = 0; s < 5; ++s) { r.S.SnowWindFactors[s] = 0.1f * (s + 1); r.S.SnowSpeedFactors[s] = 1.0f + s; }
    for (int i = 0; i < 81; ++i) r.S.ParticleBoxPos[i] = Vec((float)(i % 9) * 8.0f - 20.0f, (float)(i / 9) * 2.0f, 0.0f);
    for (int k = 0; k < 729; ++k) r.SavedBoxVisibility[k] = 1;
    r.WindVector = Vec(1.0f, 0.0f, -2.0f); r.WindSpeed = 5.0f; r.ViewportTLY = 10; r.ViewportBRY = 490;

    InfoFrame info;
    // 1: no frame data yet -> created, loading begun, nothing drawn.
    r.RenderSnow(info, 3);
    if (!info.Data || info.Data->BeginLoadingCalls != 1 || g_constant_count != 0 || r.AttachedStage3 != -1) { std::printf("00b54e30_TEST FAIL\n"); return 1; }
    // 2: data but not loaded -> another load request, nothing drawn.
    r.RenderSnow(info, 4);
    if (info.Data->BeginLoadingCalls != 2 || g_constant_count != 0) { std::printf("00b54e30_TEST FAIL\n"); return 1; }
    // 3: loaded (coarse level 7): touched, finer level requested, frame shared and released again, everything drawn.
    FrameData* data = new FrameData(); data->ReferenceCount = 1; data->Texture = 42;
    info.Data->Loaded = data;
    r.RenderSnow(info, 5);
    if (info.Data->TouchedFrame != 5 || info.Data->TouchedMipmapLevel != 0 || info.Data->BeginLoadingCalls != 3 || data->ReferenceCount != 1) { std::printf("00b54e30_TEST FAIL\n"); return 1; }
    if (r.AttachedStage3 != 42 || !r.PointSprite || !r.PointScale) { std::printf("00b54e30_TEST FAIL\n"); return 1; }
    // constants: 1 (wind 5*2 = 10 clamped to 3 -> (3, 0, -6) * 0.5), 2, 3, 4, common 5, sets 6..10
    if (g_constant_count != 10) { std::printf("00b54e30_TEST FAIL\n"); return 1; }
    if (g_constants[0].Index != 1 || !Near(g_constants[0].X, 1.5f) || !Near(g_constants[0].Y, 0.0f) || !Near(g_constants[0].Z, -3.0f) || !Near(g_constants[0].W, 12.5f)) { std::printf("00b54e30_TEST FAIL\n"); return 1; }
    if (g_constants[1].Index != 2 || !Near(g_constants[1].X, 30.0f / (2.0f - 30.0f) + 1.0f) || !Near(g_constants[1].Y, 1.0f / 28.0f) || !Near(g_constants[1].Z, 0.75f)) { std::printf("00b54e30_TEST FAIL\n"); return 1; }
    if (g_constants[2].Index != 3 || !Near(g_constants[2].X, 480.0f * 0.01f) || !Near(g_constants[2].W, 0.01f)) { std::printf("00b54e30_TEST FAIL\n"); return 1; }
    if (g_constants[3].Index != 4 || !Near(g_constants[3].X, 0.5f) || !Near(g_constants[3].Y, 4.0f)) { std::printf("00b54e30_TEST FAIL\n"); return 1; }
    if (g_constants[4].Index != 5 || !Near(g_constants[4].W, 8388608.0f)) { std::printf("00b54e30_TEST FAIL\n"); return 1; }
    for (int s = 0; s < 5; ++s)
        if (g_constants[5 + s].Index != (unsigned long)(6 + s) || !Near(g_constants[5 + s].X, 0.1f * (s + 1)) || !Near(g_constants[5 + s].Y, 1.0f + s)) { std::printf("00b54e30_TEST FAIL\n"); return 1; }
    // boxes: independent count over the nine levels z = -8, -6, .., 8
    int plain = 0, clipped = 0;
    for (int lv = 0; lv < 9; ++lv) {
        float z = -8.0f + 2.0f * lv;
        for (int i = 0; i < 81; ++i) {
            float x = (float)(i % 9) * 8.0f - 20.0f, y = (float)(i / 9) * 2.0f;
            if (!(x < 30.0f)) continue;
            if (z < -1.0f) continue;
            if (y > 10.0f) ++clipped; else ++plain;
        }
    }
    if (r.PlainCount != plain || r.ClippedCount != clipped || plain == 0 || clipped == 0) { std::printf("00b54e30_TEST FAIL\n"); return 1; }
    // the saved camera visibility is reused when the boxes are locked
    for (int k = 0; k < 729; ++k) r.SavedBoxVisibility[k] = 1;
    r.S.ParticleBoxLocked = true; r.PlainCount = r.ClippedCount = 0; g_constant_count = 0;
    r.RenderSnow(info, 6);
    if (r.PlainCount != 0 || r.ClippedCount != 0 || data->ReferenceCount != 1) { std::printf("00b54e30_TEST FAIL\n"); return 1; }
    delete data; delete info.Data;
    std::printf("00b54e30_TEST PASS\n");
    return 0;
}
