// Behaviour test for CreateAndFadeInObject (0x00CBEFC1): create the object, then step its alpha from 0 by
// 1/(15*fadeTime) per script frame until it reaches 1, and return the new thing.
#include <cstdio>

struct CCharString { const char* text; };
struct C3DVector { float X, Y, Z; };
struct CScriptThing { int id; };
struct CGameScriptInterface {
    float alphas[64];
    int alphaCount;
    int frames;
    int created;
    CScriptThing CreateObject(const CCharString& def, const C3DVector& pos, const CCharString& name)
    {
        ++created;
        CScriptThing t;
        t.id = 40 + (int)pos.X + (def.text[0] == 'd' ? 1 : 0) + (name.text[0] == 'n' ? 1 : 0);
        return t;
    }
    void EntitySetAlpha(const CScriptThing& thing, float alpha, bool fadeChildren)
    {
        if (thing.id == 52 && fadeChildren && alphaCount < 64) alphas[alphaCount++] = alpha;
    }
    void NewScriptFrame() { ++frames; }
};
CGameScriptInterface* g_pGameScriptInterface = 0;

CScriptThing CreateAndFadeInObject(const CCharString& defName, const C3DVector& position, const CCharString& scriptName, float unused, float fadeTime)
{
    CScriptThing thing = g_pGameScriptInterface->CreateObject(defName, position, scriptName);
    float step = 1.0f / (15.0f * fadeTime);
    float alpha = 0.0f;
    while (alpha < 1.0f) {
        g_pGameScriptInterface->EntitySetAlpha(thing, alpha, true);
        g_pGameScriptInterface->NewScriptFrame();
        alpha += step;
    }
    return thing;
}

int main()
{
    CGameScriptInterface gsi;
    gsi.alphaCount = 0; gsi.frames = 0; gsi.created = 0;
    g_pGameScriptInterface = &gsi;
    CCharString def; def.text = "def";
    CCharString name; name.text = "name";
    C3DVector pos; pos.X = 10.0f; pos.Y = 0.0f; pos.Z = 0.0f;
    float fadeTime = 0.5f;                      // step = 1/7.5: alphas 0, 0.133, .. 0.933 (8 frames)
    CScriptThing t = CreateAndFadeInObject(def, pos, name, 0.0f, fadeTime);
    float step = 1.0f / (15.0f * fadeTime);
    bool ok = t.id == 52 && gsi.created == 1 && gsi.alphaCount == 8 && gsi.frames == 8 && gsi.alphas[0] == 0.0f;
    for (int i = 1; ok && i < gsi.alphaCount; ++i) {
        float d = gsi.alphas[i] - gsi.alphas[i - 1] - step;
        if (d > 1e-4f || d < -1e-4f || gsi.alphas[i] >= 1.0f) ok = false;
    }
    if (!ok) {
        std::printf("00cbefc1_TEST FAIL\n");
        return 1;
    }
    std::printf("00cbefc1_TEST PASS\n");
    return 0;
}