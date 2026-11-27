#include <stdio.h>
#include <math.h>
#include "rebuild_abi.h"
#include "fable_compiler.h"
struct CRGBFloatColour;
struct CRGBColour {
    unsigned char R,G,B,A;
    CRGBColour(unsigned char red,unsigned char green,unsigned char blue,unsigned char alpha):R(red),G(green),B(blue),A(alpha) {}
    void SetA(unsigned char value) { A=value; }
    unsigned char GetA() const { return A; }
    unsigned long GetAsInt() const { return ((unsigned long)A<<24)|((unsigned long)R<<16)|((unsigned long)G<<8)|B; }
};
struct CRGBFloatColour {
    float R,G,B,A;
    CRGBFloatColour(const CRGBColour& c):R(c.R/255.0f),G(c.G/255.0f),B(c.B/255.0f),A(c.A/255.0f) {}
    operator CRGBColour() const { return CRGBColour((unsigned char)(R*255),(unsigned char)(G*255),(unsigned char)(B*255),(unsigned char)(A*255)); }
};
struct C4DVector {
    float X,Y,Z,W;
    C4DVector():X(0),Y(0),Z(0),W(0) {}
    C4DVector(const CRGBFloatColour& c):X(c.R),Y(c.G),Z(c.B),W(c.A) {}
};
enum ALPHA_BLEND_MODE { ALPHA_BLEND_SRC_ALPHA=4, ALPHA_BLEND_INV_SRC_ALPHA=5 };
struct CRenderManager {
    int log[6],count;
    unsigned long texture;
    CRenderManager():count(0),texture(0) {}
    void SetZBufferWriteEnable(bool b) { log[count++]=b?-1:1; }
    void SetAlphaBlendEnable(bool b) { log[count++]=b?2:-2; }
    void SetSourceAlphaBlendMode(ALPHA_BLEND_MODE m) { log[count++]=m==4?3:-3; }
    void SetDestAlphaBlendMode(ALPHA_BLEND_MODE m) { log[count++]=m==5?4:-4; }
    void SetTextureFactor(unsigned long c) { log[count++]=5; texture=c; }
};
struct CShaderRenderManager {
    bool enabled;
    int writes;
    unsigned long index;
    C4DVector value;
    CShaderRenderManager():enabled(false),writes(0),index(77) {}
    bool GetPixelShadersEnabled() const { return enabled; }
    void SetPixelShaderConstant(unsigned long slot,const C4DVector& v) { ++writes;index=slot;value=v; }
};
namespace NEngineInternal { CRenderManager* GPRenderManager; CShaderRenderManager* GPShaderRenderManager; }
class CEnginePrimitiveRenderUtil {
public:
    static void FABLE_FASTCALL SetTextureFactor(const CRGBFloatColour&);
    void SetupOverlayAlphaBlendingStates(unsigned long,CRGBColour);
};
// Inline-budget: the ordinary SetTextureFactor call produces a 564-byte leaf. Retail expands this shader/colour branch in the 855-byte alpha-blend function.
FABLE_FORCEINLINE void FABLE_FASTCALL CEnginePrimitiveRenderUtil::SetTextureFactor(const CRGBFloatColour& colour)
{
    if(NEngineInternal::GPShaderRenderManager->GetPixelShadersEnabled()) {
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2,C4DVector(colour));
    } else {
        NEngineInternal::GPRenderManager->SetTextureFactor(CRGBColour(colour).GetAsInt());
    }
}
void CEnginePrimitiveRenderUtil::SetupOverlayAlphaBlendingStates(unsigned long alpha,CRGBColour colour)
{
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    colour.SetA((unsigned char)((colour.GetA()*alpha)/255));
    SetTextureFactor(CRGBFloatColour(colour));
}
static bool close_to(float actual, unsigned char channel) {
    return fabs(actual - channel/255.0) < 0.000001;
}
int main() {
    CEnginePrimitiveRenderUtil util;
    unsigned long alphas[]={0,128,255,510,0xffffffffUL};
    for(int i=0;i<5;++i) for(int shader=0;shader<2;++shader) {
        CRenderManager render;
        CShaderRenderManager shaders;
        NEngineInternal::GPRenderManager=&render;
        NEngineInternal::GPShaderRenderManager=&shaders;
        shaders.enabled=shader!=0;
        CRGBColour input(255,0,127,173);
        unsigned char expected=(unsigned char)((173UL*alphas[i])/255UL);
        util.SetupOverlayAlphaBlendingStates(alphas[i],input);
        if(input.A!=173 || render.count!=(shader?4:5)) return 1;
        for(int j=0;j<render.count;++j) if(render.log[j]!=j+1) return 2;
        if(shader) {
            if(shaders.writes!=1 || shaders.index!=2 || !close_to(shaders.value.X,255) || !close_to(shaders.value.Y,0)
                || !close_to(shaders.value.Z,127) || !close_to(shaders.value.W,expected) || render.texture) return 3;
        } else if(shaders.writes || render.texture!=(((unsigned long)expected<<24)|0x00ff007fUL)) return 4;
    }
    puts("00bf1ee0_TEST PASS");
    return 0;
}