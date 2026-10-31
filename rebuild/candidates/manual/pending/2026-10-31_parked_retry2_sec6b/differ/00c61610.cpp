// CShadowRendererSceneData (constructor, catalog label) -- build a shadow renderer: store the device, owner, flags and output slot, initialise the three default shadow-parameter blocks, the four per-light colour/parameter tables from the four colour constants, the two clip blocks and the two default vectors, register with the device and hand back the device's current shadow handle
// Grade: DIFFER(1296v1292) -- body-ordered scalar stores, fastcall device init, clip block written member-by-member (ascending, this-relative) then copied, parameter block written then copied: 138 diff lines vs 249 at baseline; residue is the scheduler hoisting the param-block 0/1.0f stores ahead of the clip copy (retail keeps the clip rep movsd before the param block and loads 0x3f800000 into eax once per block); member declaration order and the class name are still not recovered (catalog label likely wrong: the PDB CShadowRendererSceneData is 0x240 bytes, this object is 0x384)
// Leaf: CShadowRendererSceneData
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C61610, 1296 bytes, `ret 0x14`, fs:[0] EH frame (base with a virtual dtor).
struct V4 { long X, Y, Z, W; };
extern const V4 g_DefaultVecA;                          // 0x012B9438
extern const V4 g_DefaultVecB;                          // 0x012B9458
extern const V4 g_Colour0;                              // 0x012B9468
extern const V4 g_Colour1;                              // 0x012B9478
extern const V4 g_Colour2;                              // 0x012B9488
extern const V4 g_Colour3;                              // 0x012B9498
struct CShadowDevice {
    unsigned char _pad[0x3c4];
    long CurrentShadow;                                 // +0x3c4
    void Register(void* renderer);                      // 0x00C609A0
};
void __fastcall InitShadowDevice(CShadowDevice* device, long owner);   // 0x00C60410
struct SShadowParams {
    long A[7];
    float Near, Far, Scale;
    long B;
    float C;
    long D;
    float E;
    long F, G, H;
    long Quality;
    void Init() { A[0] = 0; A[1] = 0; A[2] = 0; A[3] = 0; A[4] = 0; A[5] = 0; A[6] = 0; Near = 0.25f; Far = 1.5f; Scale = 1.0f; B = 0; C = 1.0f; D = 0; E = 1.0f; F = 0; G = 0; H = 0; Quality = 7; }
};
struct SLightRow { V4 Colour; long A, B; };
struct SLightParams { V4 Colour; long A; float Near, Far, Scale; };
struct SLightFade { V4 Colour; long A; float Scale; };
struct SClipParams {
    unsigned char _matrix[0x50];
    long A[6];
    long Width, Height;
    long B, C;
    float D;
    long E;
    float F;
    long MaxDistance;
    long G;
    void Init() { Width = 0x168; Height = 0x168; A[0] = 0; A[1] = 0; A[2] = 0; A[3] = 0; A[4] = 0; A[5] = 0; B = 0; C = 0; D = 1.0f; E = 0; F = 1.0f; *(float*)&MaxDistance = 1.0e9f; G = 0; }
};
class CRendererBase {
public:
    virtual ~CRendererBase();
};
class CShadowRendererSceneData : public CRendererBase {
public:
    long m_04, m_08;
    bool m_0c;
    long m_10;
    CShadowDevice* Device;                              // +0x14
    SShadowParams Params;                               // +0x18
    SLightRow Rows[4];                                  // +0x60
    SLightParams Lights[4];                             // +0xc0
    SLightFade Fades[4];                                // +0x140
    V4 VecA;                                            // +0x1a0
    V4 VecB;                                            // +0x1b0
    SShadowParams ParamsA;                              // +0x1c0
    SShadowParams ParamsB;                              // +0x208
    SClipParams ClipA;                                  // +0x250
    SClipParams ClipB;                                  // +0x2dc
    long m_368, m_36c, m_370, m_374, m_378, m_37c, m_380;
    CShadowRendererSceneData(CShadowDevice* device, long owner, long flags, long kind, long* outShadow);
};

CShadowRendererSceneData::CShadowRendererSceneData(CShadowDevice* device, long owner, long flags, long kind, long* outShadow)
{
    m_368 = 0x754;
    m_36c = 0;
    m_37c = 0;
    m_380 = 0;
    m_08 = 0;
    m_04 = 0;
    m_0c = true;
    Device = device;
    m_370 = owner;
    InitShadowDevice(device, owner);
    m_378 = flags;
    ClipA.Width = 0x168; ClipA.Height = 0x168; ClipA.A[0] = 0; ClipA.A[1] = 0; ClipA.A[2] = 0; ClipA.A[3] = 0; ClipA.A[4] = 0; ClipA.A[5] = 0; ClipA.B = 0; ClipA.C = 0; ClipA.D = 1.0f; ClipA.E = 0; ClipA.F = 1.0f; *(float*)&ClipA.MaxDistance = 1.0e9f; ClipA.G = 0;
    ClipB = ClipA;
    ParamsA.A[0] = 0; ParamsA.A[1] = 0; ParamsA.A[2] = 0; ParamsA.A[3] = 0; ParamsA.A[4] = 0; ParamsA.A[5] = 0; ParamsA.A[6] = 0; ParamsA.Near = 0.25f; ParamsA.Far = 1.5f; ParamsA.Scale = 1.0f; ParamsA.B = 0; ParamsA.C = 1.0f; ParamsA.D = 0; ParamsA.E = 1.0f; ParamsA.F = 0; ParamsA.G = 0; ParamsA.H = 0; ParamsA.Quality = 7;
    ParamsB = ParamsA;
    Params.Init();
    m_10 = kind;
    VecA = g_DefaultVecA;
    VecB = g_DefaultVecB;
    Rows[0].Colour = g_Colour0; Rows[0].A = 0; Rows[0].B = 0;
    Rows[1].Colour = g_Colour1; Rows[1].A = 0; Rows[1].B = 0;
    Rows[2].Colour = g_Colour2; Rows[2].A = 0; Rows[2].B = 0;
    Rows[3].Colour = g_Colour3; Rows[3].A = 0; Rows[3].B = 0;
    Lights[0].Colour = g_Colour0; Lights[0].A = 0; Lights[0].Near = 0.25f; Lights[0].Scale = 1.0f; Lights[0].Far = 1.5f;
    Lights[1] = Lights[0]; Lights[1].Colour = g_Colour1;
    Lights[2] = Lights[0]; Lights[2].Colour = g_Colour2;
    Lights[3] = Lights[0]; Lights[3].Colour = g_Colour3;
    Fades[0].Colour = g_Colour0; Fades[0].A = 0; Fades[0].Scale = 1.0f;
    Fades[1] = Fades[0]; Fades[1].Colour = g_Colour1;
    Fades[2] = Fades[0]; Fades[2].Colour = g_Colour2;
    Fades[3] = Fades[0]; Fades[3].Colour = g_Colour3;
    Device->Register(this);
    *outShadow = Device->CurrentShadow;
}
