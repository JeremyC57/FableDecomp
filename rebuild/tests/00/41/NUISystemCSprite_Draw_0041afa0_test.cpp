// Behaviour test for 0x0041AFA0 NUISystem::CSprite::Draw (links the REAL row object, which also carries the two
// CEnginePrimitive2DSprite box constructors and the bank pointer's destructor it calls). No class is modelled: every object is raw
// native-layout storage, the row is entered and its out-of-line callees are bound through linker aliases of their retail names
// (thiscall = fastcall with a spare edx), and the virtual callees are slots of hand-built vtables:
//   sprite    vtable slots 101 IsIndependent (+0x194), 104 IsLayerIndependent (+0x1A0), 106 IsPositionIsCenter (+0x1A8);
//             RenderZoom +0x7C, RenderColour +0x94 (B, G, R, A), Size +0xCC, RelativeRenderPosition +0xF8, RelativeRenderZoom +0x108,
//             Layer +0x12F (signed char), Handle +0x15C (vptr, Data +0x160), Angle +0x164, Width +0x168, Height +0x16C,
//             NoRenderNextFrame +0x170, Sprite2DFlag +0x174, GraphicIndex +0x178, PTexture +0x17C (Data, Info)
//   engine    vtable slots 23 SetPersistentPrimitive (+0x5C), 28 SetChildPrimitive (+0x70), 30 ClearChildPrimitive (+0x78)
//   primitive Type +0 (0x22), InterpolationSet +4, theme bytes +8..+0xA, ClipBox +0xC, Layer +0x1C, BankIndex +0x20, Frame +0x24,
//             StartAngle +0x28, EndAngle +0x2C, BlendMode +0x30, Colour +0x34, ClipBoxIsPos +0x38, Flags +0x3C, Texture +0x40,
//             UVOffsetsStart +0x44, UVOffsetsEnd +0x4C (0x54 bytes; +0xB and +0x39..+0x3B are padding)
//   manager   vtable slot 36 GetLayer (+0x90); PGraphicsBank +0x10 (Data, Info {RefCount, DeleteFunc, Data})
//   bank      GraphicList +0x1E0 / +0x1E4 (0x2C-byte CGraphicInfo, Frames +0x28), CurrentFrame +0x194
//   frame     CGraphicInfoFrame (16 bytes): Data +8 (CFrameData*), GraphicInfo +0xC (Bank +0x20)
//   data      CFrameData: vptr, RefCount +4, ResourceList +8, LastUsedFrame +0x18, Frame +0x1C, ManagedTexture.Data +0x24,
//             FileData +0x30, CurrentMipmapLevel +0x38, TouchedMipmapLevel +0x3A, TouchedFrame +0x3C
//   resource list  Head +4 (Head.PrevResource = list +0x10), CurrentFrame +0x2C
//   frame data     ReferenceCount +0, PTexture +8 (Data, Info), IsInitialised = bit 4 of +0x1A
// Every callee records an event with its `this` and arguments; the test compares the event list and every object byte for byte
// with the independent model in RunModel().
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void*) { ExitProcess(3); return 0; }

extern "C" void __fastcall CallDraw(void* self, int spare, void* engine, void* p_handle, long layer, long* pprimitive_index, void* pparent);
#pragma comment(linker, "/alternatename:@CallDraw@28=?Draw@CSprite@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z")
#pragma comment(linker, "/alternatename:?ListInsert@CResource@@AAEXPAV1@0@Z=@StubListInsert@16")
#pragma comment(linker, "/alternatename:?ListRemove@CResource@@AAEXXZ=@StubListRemove@4")
#pragma comment(linker, "/alternatename:?Uninitialise@CGraphicFrameData@@QAEXXZ=@StubUninitialise@4")
#pragma comment(linker, "/alternatename:?InternalGetFrameForcedLoading@CGraphicInfoFrame@@ABE_NK@Z=@StubForcedLoading@12")
#pragma comment(linker, "/alternatename:??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z=@StubFrameDataCtor@12")
#pragma comment(linker, "/alternatename:?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z=@StubDataBeginLoading@16")
#pragma comment(linker, "/alternatename:?BeginLoading@CGraphicInfoFrame@@QBEXKK@Z=@StubFrameBeginLoading@16")
#pragma comment(linker, "/alternatename:?GetInstance@CManager@NUISystem@@SIPAV12@XZ=@StubGetInstance@0")

// ---- deterministic random numbers
static unsigned long g_seed = 0x2468ace1;
static unsigned long Rnd() { g_seed = g_seed * 1664525ul + 1013904223ul; return g_seed >> 8; }
static long RndRange(long lo, long hi) { return lo + (long)(Rnd() % (unsigned long)(hi - lo + 1)); }
static void Fill(void* p, int n) { unsigned char* b = (unsigned char*)p; for (int i = 0; i < n; i++) b[i] = (unsigned char)Rnd(); }
template <class T> static T& At(void* base, int off) { return *(T*)((char*)base + off); }

// ---- event log (the real one in g_ev, the model's in g_want)
struct Ev { char t; unsigned long a, b, c, d; unsigned char prim[0x54]; };
enum { MAX_EV = 48 };
static Ev g_ev[MAX_EV], g_want[MAX_EV]; static int g_evCount, g_wantCount; static int g_modelling;
static Ev& Log(char t, unsigned long a = 0, unsigned long b = 0, unsigned long c = 0, unsigned long d = 0)
{
    static Ev junk;
    int& n = g_modelling ? g_wantCount : g_evCount; Ev* list = g_modelling ? g_want : g_ev;
    if (n >= MAX_EV) { n++; return junk; }
    Ev& e = list[n++]; memset(&e, 0, sizeof e); e.t = t; e.a = a; e.b = b; e.c = c; e.d = d; return e;
}
static unsigned long Rel(const void* p);
static void LogPrimitive(Ev& e, const void* primitive)
{
    memcpy(e.prim, primitive, 0x54);
    e.prim[0xb] = 0; e.prim[0x39] = 0; e.prim[0x3a] = 0; e.prim[0x3b] = 0;             // padding
    At<unsigned long>(e.prim, 0x40) = Rel(At<void*>(e.prim, 0x40));                    // the texture pointer, as an offset
}

// ---- the objects: one set for the row, an identically built one for the model; the two are compared afterwards
struct World
{
    unsigned char sprite[0x184];
    unsigned char handle_data[0x10];        // the sprite handle's persistent primitive: vptr, RefCount +4
    unsigned char manager[0x20];
    unsigned char bank_info[0xc];           // CCPPointerInfo of the manager's bank pointer
    unsigned char bank[0x1e8];
    unsigned char graphics[5 * 0x2c];
    unsigned char frames[2 * 0x10];
    unsigned char data[0x44];               // the frame's CFrameData
    unsigned char texture_res[0x1c];        // the managed texture's CResource
    unsigned char list_a[0x3c], list_b[0x3c];
    unsigned char frame_data[0x1c];         // CGraphicFrameData
    unsigned char texture_info[0xc];        // its PTexture count block
    unsigned char new_block[0x44];          // what operator new hands out
    long primitive_index;
};
static World g_real, g_model;
static World* g_w;                          // the set the stubs / the model act on

// what the scripted callees do in this case
static int g_layerSalt, g_forcedResult, g_forcedInstallsFrame, g_isIndependent, g_isLayerIndependent, g_isCentre;
static int g_fail;

static unsigned long Rel(const void* p)   // a pointer as an offset into its World, so both sets log the same value
{
    const char* c = (const char*)p;
    if (c >= (const char*)g_w && c < (const char*)(g_w + 1)) return (unsigned long)(c - (const char*)g_w) + 0x10000000ul;
    return (unsigned long)(size_t)p;
}

// ---- virtual callees
static bool __fastcall SpriteIsIndependent(void* self) { Log('i', Rel(self)); return g_isIndependent != 0; }
static bool __fastcall SpriteIsLayerIndependent(void* self) { Log('l', Rel(self)); return g_isLayerIndependent != 0; }
static bool __fastcall SpriteIsCentre(void* self) { Log('c', Rel(self)); return g_isCentre != 0; }
static void __fastcall SpriteTrap(void*) { printf("FAIL unexpected virtual call\n"); g_fail = 1; }
static void* g_spriteVtable[149];
static long LayerFor(long arg) { return (long)(((unsigned long)arg * 2654435761ul) ^ (unsigned long)g_layerSalt) % 83; }
static long __fastcall ManagerGetLayer(void* self, int, long arg) { Log('L', Rel(self), (unsigned long)arg); return LayerFor(arg); }
static void* g_managerVtable[40];
static void __fastcall EngineSetPersistent(void* self, int, void* handle, void* primitive, unsigned long flags)
{
    LogPrimitive(Log('P', Rel(self), Rel(handle), flags), primitive);
}
static void __fastcall EngineSetChild(void* self, int, void* handle, unsigned long index, void* primitive, unsigned long flags)
{
    LogPrimitive(Log('S', Rel(self), Rel(handle), index, flags), primitive);
}
static void __fastcall EngineClearChild(void* self, int, void* handle, unsigned long index) { Log('C', Rel(self), Rel(handle), index); }
static void* g_engineVtable[32];
static void* g_engine[2] = { g_engineVtable, 0 };
static void __fastcall PrimitiveDestroy(void* self) { Log('D', Rel(self)); }
static void __fastcall PrimitiveNotifyReleased(void* self, int, void* handle) { Log('N', Rel(self), Rel(handle)); }
static void* g_primitiveVtable[4];
static void __fastcall DataRelease(void* self) { Log('r', Rel(self)); }
static void* g_dataVtable[2];
static void __fastcall BankDelete(void* data) { Log('T', Rel(data)); }
static void __fastcall TextureDelete(void* data) { Log('t', Rel(data)); }

// ---- out-of-line callees (bound by the aliases above)
extern "C" void* __fastcall StubGetInstance() { Log('G'); return g_w->manager; }
extern "C" void __fastcall StubFrameBeginLoading(void* self, int, unsigned long mip, unsigned long flags) { Log('B', Rel(self), mip, flags); }
extern "C" void __fastcall StubDataBeginLoading(void* self, int, unsigned long mip, unsigned long flags) { Log('b', Rel(self), mip, flags); }
extern "C" void __fastcall StubListRemove(void* self) { Log('R', Rel(self)); }
extern "C" void __fastcall StubListInsert(void* self, int, void* a0, void* a1) { Log('I', Rel(self), Rel(a0), Rel(a1)); }
extern "C" void __fastcall StubUninitialise(void* self) { Log('U', Rel(self)); }
static void InitNewData(World* w, void* frame)
{
    memset(w->new_block, 0x5a, 0x44);
    At<void*>(w->new_block, 0) = g_dataVtable; At<long>(w->new_block, 4) = 1; At<void*>(w->new_block, 8) = 0;
    At<void*>(w->new_block, 0x1c) = 0; At<void*>(w->new_block, 0x24) = 0; At<void*>(w->new_block, 0x30) = 0;
    At<void*>(w->new_block, 0x40) = frame;
}
extern "C" void* __fastcall StubFrameDataCtor(void* self, int, void* frame)
{
    Log('F', Rel(self), Rel(frame));
    if (self != g_w->new_block) { printf("FAIL CFrameData constructed outside the new block\n"); g_fail = 1; return self; }
    InitNewData(g_w, frame);
    return self;
}
static void ForcedInstall(World* w, void* frame)
{
    void* data = At<void*>(frame, 8);
    if (data && g_forcedInstallsFrame) At<void*>(data, 0x1c) = g_forcedInstallsFrame == 1 ? (void*)w->frame_data : 0;
}
extern "C" bool __fastcall StubForcedLoading(void* self, int, unsigned long flags)
{
    Log('X', Rel(self), flags);
    ForcedInstall(g_w, self);
    return g_forcedResult != 0;
}
void* operator new(size_t size) { Log('n', (unsigned long)size); return g_w->new_block; }
void operator delete(void* p) { Log('d', Rel(p)); }

// ---- model arithmetic (the row's x87 runs at 53-bit precision: double arithmetic, results rounded on the float stores)
static float RoundNear(float value)
{
    double d = (double)value; double f = floor(d); double r = d - f; long k = (long)f;
    if (r > 0.5) k = k + 1;
    else if (r == 0.5 && (k & 1)) k = k + 1;
    return (float)k;
}
static void ModelTouch(void* resource)
{
    void* list = At<void*>(resource, 8);
    if (list && At<unsigned long>(resource, 0x18) != At<unsigned long>(list, 0x2c))
    {
        Log('R', Rel(resource));
        Log('I', Rel(resource), Rel(At<void*>(list, 0x10)), Rel((char*)list + 4));
        At<unsigned long>(resource, 0x18) = At<unsigned long>(list, 0x2c);
    }
}
static void ModelRelease(void* info, char tag)      // a CCountedPointer letting go of its count block
{
    if (info)
    {
        if (--At<long>(info, 0) == 0)
        {
            Log(tag, Rel(At<void*>(info, 8)));
            Log('d', Rel(info));
        }
    }
}
static long g_statClearChild, g_statClearOwn, g_statNotify, g_statDestroy, g_statArm, g_statTexture, g_statGraphic, g_statNoGraphic,
            g_statCentre, g_statChild, g_statOwn, g_statTouchTex, g_statTouchData, g_statReload, g_statReloadPending, g_statNewData,
            g_statNoFrame, g_statForcedFail, g_statForcedNull, g_statFrameDeleted, g_statTextureDeleted, g_statUninit, g_statBankDeleted,
            g_statSummed, g_statBigWidth;

static void RunModel(void* p_handle, long layer, long* pprimitive_index)
{
    World* w = g_w; unsigned char* s = w->sprite;
    bool hidden = At<unsigned char>(s, 0x97) == 0 || (At<float>(s, 0x7c) <= 0.0f && At<float>(s, 0x80) <= 0.0f);
    if (hidden)
    {
        if (At<unsigned char>(s, 0x170) == 1)
        {
            if (p_handle) { Log('C', Rel(g_engine), Rel(p_handle), (unsigned long)(*pprimitive_index)++); g_statClearChild++; }
            else
            {
                void* d = At<void*>(s, 0x160);
                if (d && At<long>(d, 4) == 1) { Log('N', Rel(d), Rel(s + 0x15c)); g_statNotify++; }
                if (d) { if (--At<long>(d, 4) == 0) { Log('D', Rel(d)); g_statDestroy++; } }
                At<void*>(s, 0x160) = 0;
                g_statClearOwn++;
            }
            return;
        }
        At<unsigned char>(s, 0x170) = 1; g_statArm++;
    }
    else
        At<unsigned char>(s, 0x170) = 0;

    unsigned long width = At<unsigned long>(s, 0x168), height = At<unsigned long>(s, 0x16c);
    double size_x = width == 0 ? (double)At<float>(s, 0xcc) : (double)width;
    double size_y = height == 0 ? (double)At<float>(s, 0xd0) : (double)height;
    if (width > 0x7ffffffful || height > 0x7ffffffful) g_statBigWidth++;
    float pos_x = At<float>(s, 0xf8), pos_y = At<float>(s, 0xfc);
    float range_x = (float)(size_x * (double)At<float>(s, 0x108)), range_y = (float)(size_y * (double)At<float>(s, 0x10c));
    float box[4];
    box[0] = pos_x; box[1] = pos_y; box[2] = (float)((double)pos_x + (double)range_x); box[3] = (float)((double)pos_y + (double)range_y);
    unsigned long colour = At<unsigned long>(s, 0x94);
    Log('c', Rel(s));
    if (g_isCentre)
    {
        double move_x = (double)pos_x - (double)range_x * 0.5, move_y = (double)pos_y - (double)range_y * 0.5;
        box[2] = (float)((move_x - (double)box[0]) + (double)box[2]);
        box[3] = (float)((move_y - (double)box[1]) + (double)box[3]);
        box[0] = (float)move_x; box[1] = (float)move_y;
        g_statCentre++;
    }
    for (int i = 0; i < 4; i++) box[i] = RoundNear(box[i]);
    Log('i', Rel(s)); Log('l', Rel(s));
    long layer_arg = (long)At<signed char>(s, 0x12f);
    if (!g_isIndependent && !g_isLayerIndependent) { layer_arg += layer; g_statSummed++; }
    Log('G'); Log('L', Rel(w->manager), (unsigned long)layer_arg);
    long render_layer = LayerFor(layer_arg);

    unsigned char prim[0x54]; memset(prim, 0, sizeof prim);
    At<unsigned long>(prim, 0) = 0x22; At<unsigned long>(prim, 4) = 0;
    memcpy(prim + 0xc, box, 16); At<long>(prim, 0x1c) = render_layer;
    At<float>(prim, 0x28) = At<float>(s, 0x164); At<float>(prim, 0x2c) = At<float>(s, 0x164);
    At<unsigned long>(prim, 0x30) = 2; At<unsigned long>(prim, 0x34) = colour; At<unsigned long>(prim, 0x3c) = At<unsigned long>(s, 0x174);

    void* texture = At<void*>(s, 0x17c);
    if (texture) { At<unsigned long>(prim, 0x40) = Rel(texture); g_statTexture++; }
    else
    {
        unsigned long graphic = At<unsigned long>(s, 0x178);
        if (graphic > 0)
        {
            g_statGraphic++;
            Log('G');
            void* info = At<void*>(w->manager, 0x14);
            if (info) ++At<long>(info, 0);
            void* frame = At<void*>(w->graphics + graphic * 0x2c, 0x28);
            if (info && At<long>(info, 0) == 1) g_statBankDeleted++;
            ModelRelease(info, 'T');
            Log('B', Rel(frame), 0, 0x10);
            // CGraphicInfoFrame::GetFrame(frame_out, 0, 0x11)
            void* out = 0; bool have = false; bool forced = true;
            void* data = At<void*>(frame, 8);
            if (data)
            {
                if (At<void*>(data, 0x1c))
                {
                    At<unsigned long>(data, 0x3c) = At<unsigned long>(At<void*>(At<void*>(frame, 0xc), 0x20), 0x194);
                    void* managed = At<void*>(data, 0x24);
                    if (managed) { if (At<void*>(managed, 8) && At<unsigned long>(managed, 0x18) != At<unsigned long>(At<void*>(managed, 8), 0x2c)) g_statTouchTex++; ModelTouch(managed); }
                    if (At<void*>(data, 8) && At<unsigned long>(data, 0x18) != At<unsigned long>(At<void*>(data, 8), 0x2c)) g_statTouchData++;
                    ModelTouch(data);
                    if (At<unsigned char>(data, 0x3a) != 0) At<unsigned char>(data, 0x3a) = 0;
                    if (At<unsigned char>(data, 0x38) != 0)
                    {
                        Log('b', Rel(data), 0, 0x11); g_statReload++;
                        if (At<void*>(data, 0x30) == 0) { out = At<void*>(data, 0x1c); have = true; forced = false; }
                        else g_statReloadPending++;
                    }
                    else { out = At<void*>(data, 0x1c); have = true; forced = false; }
                }
                else g_statNoFrame++;
                if (forced) Log('b', Rel(data), 0, 0x11);
            }
            else
            {
                g_statNewData++;
                Log('n', 0x44);
                Log('F', Rel(w->new_block), Rel(frame));
                InitNewData(w, frame);
                At<void*>(frame, 8) = w->new_block; data = w->new_block;
                Log('b', Rel(data), 0, 0x11);
            }
            if (forced)
            {
                Log('X', Rel(frame), 0x11);
                ForcedInstall(w, frame);
                if (g_forcedResult) { out = At<void*>(At<void*>(frame, 8), 0x1c); have = true; if (!out) g_statForcedNull++; }
                else g_statForcedFail++;
            }
            if (have && out)
            {
                // the local CGraphicFrame takes a reference and lets it go at the end of the block
                ++At<long>(out, 0);
                if (--At<long>(out, 0) == 0)
                {
                    g_statFrameDeleted++;
                    if (At<unsigned char>(out, 0x1a) & 0x10) { Log('U', Rel(out)); g_statUninit++; }
                    void* texture_info = At<void*>(out, 0xc);
                    if (texture_info && At<long>(texture_info, 0) == 1) g_statTextureDeleted++;
                    ModelRelease(texture_info, 't');
                    At<void*>(out, 8) = 0; At<void*>(out, 0xc) = 0;
                    Log('d', Rel(out));
                }
            }
            At<unsigned long>(prim, 0x20) = graphic;
        }
        else g_statNoGraphic++;
    }
    if (p_handle && pprimitive_index)
    {
        Ev& e = Log('S', Rel(g_engine), Rel(p_handle), (unsigned long)(*pprimitive_index)++, 0xc0); memcpy(e.prim, prim, 0x54); g_statChild++;
    }
    else
    {
        Ev& e = Log('P', Rel(g_engine), Rel(s + 0x15c), 0xc0); memcpy(e.prim, prim, 0x54); g_statOwn++;
    }
}

static float RandomFloat()
{
    switch (Rnd() % 8)
    {
        case 0: return (float)RndRange(-200, 800);
        case 1: return (float)RndRange(-200, 800) + 0.5f;                  // a tie for the pixel snap
        case 2: return (float)RndRange(-1600, 6400) / 8.0f;
        default: return (float)RndRange(-200000, 800000) / 977.0f;
    }
}

static void BuildWorld(World* w)
{
    unsigned char* s = w->sprite;
    Fill(w, sizeof *w);
    At<void*>(s, 0) = g_spriteVtable;
    At<float>(s, 0x7c) = (Rnd() % 3 == 0) ? (float)RndRange(-2, 0) : (float)RndRange(1, 40) / 8.0f;
    At<float>(s, 0x80) = (Rnd() % 3 == 0) ? (float)RndRange(-2, 0) : (float)RndRange(1, 40) / 8.0f;
    if (Rnd() % 3 == 0) At<unsigned char>(s, 0x97) = 0;
    At<float>(s, 0xcc) = RandomFloat(); At<float>(s, 0xd0) = RandomFloat();
    At<float>(s, 0xf8) = RandomFloat(); At<float>(s, 0xfc) = RandomFloat();
    At<float>(s, 0x108) = (float)RndRange(-8, 40) / 8.0f; At<float>(s, 0x10c) = (float)RndRange(-8, 40) / 8.0f;
    At<void*>(s, 0x160) = (Rnd() % 4 == 0) ? 0 : (void*)w->handle_data;
    At<void*>(w->handle_data, 0) = g_primitiveVtable; At<long>(w->handle_data, 4) = RndRange(1, 3);
    At<float>(s, 0x164) = RandomFloat();
    switch (Rnd() % 4) { case 0: At<unsigned long>(s, 0x168) = 0; break; case 1: At<unsigned long>(s, 0x168) = 0x80000000ul + (unsigned long)RndRange(0, 4096) * 256; break; default: At<unsigned long>(s, 0x168) = (unsigned long)RndRange(1, 600); }
    switch (Rnd() % 4) { case 0: At<unsigned long>(s, 0x16c) = 0; break; case 1: At<unsigned long>(s, 0x16c) = 0x80000000ul + (unsigned long)RndRange(0, 4096) * 256; break; default: At<unsigned long>(s, 0x16c) = (unsigned long)RndRange(1, 600); }
    if (At<unsigned long>(s, 0x168) > 0x7ffffffful) At<float>(s, 0x108) = (float)RndRange(-8, 8) / 33554432.0f;   // keep the box in pixel range
    if (At<unsigned long>(s, 0x16c) > 0x7ffffffful) At<float>(s, 0x10c) = (float)RndRange(-8, 8) / 33554432.0f;
    At<unsigned char>(s, 0x170) = (unsigned char)(Rnd() % 4 == 3 ? Rnd() : Rnd() % 2);
    At<unsigned long>(s, 0x178) = (Rnd() % 5 == 0) ? 0 : (unsigned long)RndRange(1, 4);
    At<void*>(s, 0x17c) = (Rnd() % 3 == 0) ? (void*)(w->texture_res + 1) : 0;           // any non-null address: never dereferenced
    // manager and bank
    At<void*>(w->manager, 0) = g_managerVtable; At<void*>(w->manager, 0x10) = w->bank;
    At<void*>(w->manager, 0x14) = (Rnd() % 8 == 0) ? 0 : (void*)w->bank_info;
    At<long>(w->bank_info, 0) = (Rnd() % 6 == 0) ? 0 : RndRange(1, 3);                  // 0: the row's copy is the only reference
    At<void*>(w->bank_info, 4) = (void*)BankDelete; At<void*>(w->bank_info, 8) = w->bank;
    At<void*>(w->bank, 0x1e0) = w->graphics; At<void*>(w->bank, 0x1e4) = w->graphics + 5 * 0x2c;
    for (int g = 0; g < 5; g++) { At<void*>(w->graphics + g * 0x2c, 0x20) = w->bank; At<void*>(w->graphics + g * 0x2c, 0x28) = w->frames; }
    At<void*>(w->graphics + At<unsigned long>(s, 0x178) * 0x2c, 0x28) = w->frames + 0x10;   // this graphic's frames: the second slot
    void* frame = w->frames + 0x10;
    At<void*>(frame, 0xc) = w->graphics + At<unsigned long>(s, 0x178) * 0x2c;
    At<void*>(frame, 8) = (Rnd() % 5 == 0) ? 0 : (void*)w->data;
    At<void*>(w->data, 0) = g_dataVtable; At<long>(w->data, 4) = RndRange(1, 3);
    At<void*>(w->data, 8) = (Rnd() % 4 == 0) ? 0 : (void*)w->list_a;
    At<void*>(w->data, 0x1c) = (Rnd() % 4 == 0) ? 0 : (void*)w->frame_data;
    At<void*>(w->data, 0x24) = (Rnd() % 4 == 0) ? 0 : (void*)w->texture_res;
    At<void*>(w->data, 0x30) = (Rnd() % 2) ? 0 : (void*)w->bank;
    At<unsigned char>(w->data, 0x38) = (unsigned char)(Rnd() % 2 ? 0 : Rnd());
    At<unsigned char>(w->data, 0x3a) = (unsigned char)(Rnd() % 2 ? 0 : Rnd());
    At<void*>(w->texture_res, 8) = (Rnd() % 4 == 0) ? 0 : (void*)w->list_b;
    if (Rnd() % 2) At<unsigned long>(w->data, 0x18) = At<unsigned long>(w->list_a, 0x2c);
    if (Rnd() % 2) At<unsigned long>(w->texture_res, 0x18) = At<unsigned long>(w->list_b, 0x2c);
    At<long>(w->frame_data, 0) = (Rnd() % 4 == 0) ? 0 : RndRange(1, 3);                // 0: the local frame is the last reference
    At<void*>(w->frame_data, 0xc) = (Rnd() % 4 == 0) ? 0 : (void*)w->texture_info;
    At<long>(w->texture_info, 0) = RndRange(1, 2); At<void*>(w->texture_info, 4) = (void*)TextureDelete; At<void*>(w->texture_info, 8) = w->texture_res;
    w->primitive_index = RndRange(0, 1000);
}

static int Compare(int number)
{
    int bad = 0;
    if (g_evCount != g_wantCount) { printf("FAIL case %d: %d events, model %d\n", number, g_evCount, g_wantCount); bad = 1; }
    for (int i = 0; i < g_evCount && i < g_wantCount && i < MAX_EV && !bad; i++)
        if (memcmp(&g_ev[i], &g_want[i], sizeof(Ev)))
        {
            printf("FAIL case %d: event %d is %c(%lx,%lx,%lx,%lx), model %c(%lx,%lx,%lx,%lx)\n", number, i, g_ev[i].t, g_ev[i].a, g_ev[i].b, g_ev[i].c, g_ev[i].d,
                   g_want[i].t, g_want[i].a, g_want[i].b, g_want[i].c, g_want[i].d);
            if (memcmp(g_ev[i].prim, g_want[i].prim, 0x54))
                for (int k = 0; k < 0x54; k += 4)
                    if (memcmp(g_ev[i].prim + k, g_want[i].prim + k, 4)) printf("  primitive +0x%02x: %08lx, model %08lx\n", k, At<unsigned long>(g_ev[i].prim, k), At<unsigned long>(g_want[i].prim, k));
            bad = 1;
        }
    // the two object sets hold pointers into themselves: compare them as offsets
    const unsigned char* a = (const unsigned char*)&g_real; const unsigned char* b = (const unsigned char*)&g_model;
    for (int off = 0; off + 4 <= (int)sizeof(World) && !bad; off += 4)          // every pointer sits in an aligned slot
    {
        unsigned long va, vb; memcpy(&va, a + off, 4); memcpy(&vb, b + off, 4);
        if (va - (unsigned long)(size_t)&g_real == vb - (unsigned long)(size_t)&g_model && va >= (unsigned long)(size_t)&g_real && va < (unsigned long)(size_t)(&g_real + 1)) continue;
        if (va != vb) { printf("FAIL case %d: object dword +0x%x is %08lx, model %08lx\n", number, off, va, vb); bad = 1; }
    }
    return bad;
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    int i;
    for (i = 0; i < 149; i++) g_spriteVtable[i] = (void*)SpriteTrap;
    g_spriteVtable[101] = (void*)SpriteIsIndependent; g_spriteVtable[104] = (void*)SpriteIsLayerIndependent; g_spriteVtable[106] = (void*)SpriteIsCentre;
    for (i = 0; i < 40; i++) g_managerVtable[i] = (void*)SpriteTrap;
    g_managerVtable[36] = (void*)ManagerGetLayer;
    for (i = 0; i < 32; i++) g_engineVtable[i] = (void*)SpriteTrap;
    g_engineVtable[23] = (void*)EngineSetPersistent; g_engineVtable[28] = (void*)EngineSetChild; g_engineVtable[30] = (void*)EngineClearChild;
    g_primitiveVtable[0] = (void*)SpriteTrap; g_primitiveVtable[1] = (void*)PrimitiveDestroy; g_primitiveVtable[2] = (void*)SpriteTrap; g_primitiveVtable[3] = (void*)PrimitiveNotifyReleased;
    g_dataVtable[0] = (void*)SpriteTrap; g_dataVtable[1] = (void*)DataRelease;

    int cases = 0;
    for (int n = 0; n < 60000; n++)
    {
        unsigned long world_seed = g_seed;                                 // the two object sets are built from the same numbers
        BuildWorld(&g_model); g_seed = world_seed; BuildWorld(&g_real);
        g_layerSalt = (int)Rnd(); g_forcedResult = (int)(Rnd() % 4 != 0); g_forcedInstallsFrame = (int)(Rnd() % 4 == 0 ? 2 : Rnd() % 2);
        g_isIndependent = (int)(Rnd() % 3 == 0); g_isLayerIndependent = (int)(Rnd() % 3 == 0); g_isCentre = (int)(Rnd() % 2);
        long layer = RndRange(-40, 40);
        int have_handle = (int)(Rnd() % 2), have_index = (int)(Rnd() % 4 != 0);
        unsigned char* s = g_real.sprite;
        bool hidden = At<unsigned char>(s, 0x97) == 0 || (At<float>(s, 0x7c) <= 0.0f && At<float>(s, 0x80) <= 0.0f);
        if (hidden && At<unsigned char>(s, 0x170) == 1 && have_handle) have_index = 1;    // the clear path counts the child without a null test
        static unsigned char parent_handle[8];

        g_evCount = g_wantCount = 0;
        g_w = &g_model; g_modelling = 1;
        RunModel(have_handle ? parent_handle : 0, layer, have_index ? &g_model.primitive_index : 0);
        g_w = &g_real; g_modelling = 0;
        CallDraw(g_real.sprite, 0, g_engine, have_handle ? parent_handle : 0, layer, have_index ? &g_real.primitive_index : 0, (void*)(size_t)0xdead0001);
        if (g_fail || Compare(n)) { printf("FAIL\n"); return 1; }
        cases++;
    }
    long* stats[] = { &g_statClearChild, &g_statClearOwn, &g_statNotify, &g_statDestroy, &g_statArm, &g_statTexture, &g_statGraphic, &g_statNoGraphic,
                      &g_statCentre, &g_statChild, &g_statOwn, &g_statTouchTex, &g_statTouchData, &g_statReload, &g_statReloadPending, &g_statNewData,
                      &g_statNoFrame, &g_statForcedFail, &g_statForcedNull, &g_statFrameDeleted, &g_statTextureDeleted, &g_statUninit, &g_statBankDeleted,
                      &g_statSummed, &g_statBigWidth };
    for (int k = 0; k < (int)(sizeof stats / sizeof stats[0]); k++)
        if (*stats[k] == 0) { printf("FAIL path %d never taken\n", k); return 1; }
    printf("cases %d: clear child %ld, clear own %ld (notify %ld, destroy %ld), armed %ld, texture %ld, graphic %ld, no graphic %ld, centred %ld, child %ld, own %ld\n",
           cases, g_statClearChild, g_statClearOwn, g_statNotify, g_statDestroy, g_statArm, g_statTexture, g_statGraphic, g_statNoGraphic, g_statCentre, g_statChild, g_statOwn);
    printf("  touch texture %ld, touch data %ld, reload %ld (pending %ld), new data %ld, no frame %ld, forced fail %ld, forced null %ld, frame deleted %ld (texture %ld, uninitialise %ld), bank deleted %ld\n",
           g_statTouchTex, g_statTouchData, g_statReload, g_statReloadPending, g_statNewData, g_statNoFrame, g_statForcedFail, g_statForcedNull, g_statFrameDeleted, g_statTextureDeleted,
           g_statUninit, g_statBankDeleted);
    printf("0041afa0_TEST PASS\n");
    return 0;
}