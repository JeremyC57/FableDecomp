#ifndef FABLE_ENGINE_D3DXVECTOR4_H
#define FABLE_ENGINE_D3DXVECTOR4_H
#ifdef FABLE_RETAIL_FONT2DVERTEX_PASS28
// Layout: Ego_r and the D3DX9 SDK place x, y, z, w at 0, 4, 8, 12. FableWin lib_font_bank calls the four-float constructor.
struct D3DXVECTOR4 {
    float x, y, z, w;
    D3DXVECTOR4() {}
        D3DXVECTOR4(float sx, float sy, float sz, float sw) : x(sx), y(sy), z(sz), w(sw) {}
};
#endif
#endif
