#include <cstdio>

static int g_render = 0;
struct CThing { void AddToRenderList() { ++g_render; } };
struct CRGBColour { unsigned char B, G, R, A; };
struct CTintColourMap {
    void* m_keys[4]; CRGBColour m_values[4]; int m_count;
    CRGBColour& operator[](void* const& key)
    {
        for (int i = 0; i < m_count; ++i) if (m_keys[i] == key) return m_values[i];
        m_keys[m_count] = key; CRGBColour z = { 0, 0, 0, 0 }; m_values[m_count] = z; return m_values[m_count++];
    }
};
struct CTCGraphicAppearance {
    CThing* ParentThing;
    CTintColourMap TintColours;
    unsigned char UpdatePrimitiveReasons;
    void SetTintColour(const CRGBColour& colour, void* key)
    {
        TintColours[key] = colour;
        ParentThing->AddToRenderList();
        UpdatePrimitiveReasons |= 4;
    }
};

int main()
{
    CThing t; CTCGraphicAppearance a; a.ParentThing = &t; a.TintColours.m_count = 0; a.UpdatePrimitiveReasons = 0x10;
    int k1, k2; CRGBColour red = { 0, 0, 255, 255 }, blue = { 255, 0, 0, 128 };
    a.SetTintColour(red, &k1); a.SetTintColour(blue, &k2); a.SetTintColour(blue, &k1);
    void* key1 = &k1;
    if (a.TintColours.m_count != 2 || a.TintColours[key1].B != 255 || a.TintColours[key1].A != 128 || g_render != 3 || a.UpdatePrimitiveReasons != 0x14)
        { std::printf("004c00b0_TEST FAIL\n"); return 1; }
    std::printf("004c00b0_TEST PASS\n");
    return 0;
}
