#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>

struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z) {}
};

struct CCharString : std::string {
    CCharString(const char* text=""):std::string(text) {}
    std::string ToWideString() const { return *this; }
};
struct CRGBColour {
    int R,G,B,A;
    CRGBColour(int r,int g,int b,int a):R(r),G(g),B(b),A(a) {}
};
struct FontInfo { float LineHeight; float GetFontHeight() const { return LineHeight; } };
struct Subsystem {
    FontInfo* DebugFont;
    FontInfo* Font;
    const FontInfo& PeekMainFont() const { return *Font; }
    const FontInfo& PeekDebugFont() const { return *DebugFont; }
};
Subsystem* GetGameSubsystem()
{
    static FontInfo font={5.0f};
    static Subsystem subsystem={&font,&font};
    return &subsystem;
}
enum { ENGINE_INTERPOLATION_SET_0 };
struct CEnginePrimitiveText {
    unsigned Id;
    C3DVector Position;
    std::string Text;
    bool Valid;
    CEnginePrimitiveText(unsigned id,const C3DVector& pos,float size,const std::string& text,const FontInfo&,
        const CRGBColour& colour,bool option,int layer)
        :Id(id),Position(pos),Text(text),Valid(size==1&&colour.R==0&&colour.G==255&&colour.B==0&&colour.A==255&&!option&&!layer) {}
};
struct Renderer {
    std::vector<CEnginePrimitiveText> Items;
    void AddPrimitive(const CEnginePrimitiveText& text) { Items.push_back(text); }
};
struct CEnginePrimitivePrepareInfo { Renderer* Engine; bool Edit2DEngine; };
struct CThing {
    bool BaseSucceeds;
    unsigned BaseCalls;
    CThing():BaseSucceeds(true),BaseCalls(0) {}
    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { ++BaseCalls; return BaseSucceeds; }
};
struct CWorld {
    C3DVector Position;
    const C3DVector& GetGameLODCentre() const { return Position; }
};
bool GFWorld3DDistanceSmallerThan(const C3DVector& a,const C3DVector& b,float radius)
{
    float x=a.X-b.X,y=a.Y-b.Y,z=a.Z-b.Z;return x*x+y*y+z*z<radius*radius;
}
struct CThingDrawInformation { CCharString DrawString; bool StringOn; };
struct CThingPhysicalDebugInfo { bool Moving,OnGround,OnMesh,Collidable,Interactable,Immoveable; };
struct Graphic {
    bool HaveFocal;
    Graphic():HaveFocal(true) {}
    bool GetStaticFocalPosIfLoaded(C3DVector& focal)
    { focal=C3DVector(1,2,3);return HaveFocal; }
    C3DVector GetFocalPos(float fallback) { return C3DVector(4,5,fallback); }
};
struct CTCCreatureGenerator {
    unsigned Calls;
    CTCCreatureGenerator():Calls(0) {}
    void DrawDebugInfo(const CEnginePrimitivePrepareInfo&) { ++Calls; }
};
bool g_DrawThingDebugState=false;
enum { TCI_CREATURE_GENERATOR=0x3d };
struct CThingPhysical : CThing {
    bool Drawable,Hidden,DebugName,HasGenerator,NameVisible,HasDebugState;
    unsigned UID;
    Graphic Graphics;
    Graphic* GraphicTC;
    CWorld World;
    C3DVector Position;
    CTCCreatureGenerator Generator;
    CThingPhysical():Drawable(true),Hidden(false),DebugName(true),HasGenerator(true),NameVisible(true),
        HasDebugState(true),UID(71),GraphicTC(&Graphics) {}
    bool DrawIsDrawable() const { return Drawable; }
    bool IsHidden() const { return Hidden; }
    bool DrawIsDisplayingInformation() const { return DebugName; }
    const CWorld& GetWorld() const { return World; }
    const C3DVector& GetPos() const { return Position; }
    Graphic* GetTCGraphicAppearance() { return GraphicTC; }
    unsigned GetUniqueID() const { return UID; }
    const Subsystem& PeekComponent() const { return *GetGameSubsystem(); }
    void DrawGetThingDisplayInformation(CThingDrawInformation* info) { info->DrawString="Creature";info->StringOn=NameVisible; }
    bool GetTC(long id,CTCCreatureGenerator** tc) { *tc=&Generator;return id==TCI_CREATURE_GENERATOR&&HasGenerator; }
    bool DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo& flags)
    {
        const CThingPhysicalDebugInfo state={true,true,false,true,false,true};
        flags=state;
        return HasDebugState;
    }
    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
};

bool CThingPhysical::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (!DrawIsDrawable() || IsHidden() || !CThing::DrawPreparePrimitivesForRendering(info) || info.Edit2DEngine)
        return false;
    if (DrawIsDisplayingInformation() && GFWorld3DDistanceSmallerThan(GetWorld().GetGameLODCentre(), GetPos(), 48.0f)) {
        CThingDrawInformation thing_info;
        C3DVector next_pos;
        if (GetTCGraphicAppearance()->GetStaticFocalPosIfLoaded(next_pos)) {
            DrawGetThingDisplayInformation(&thing_info);
            if (thing_info.StringOn) {
                CCharString str(thing_info.DrawString);
                info.Engine->AddPrimitive(CEnginePrimitiveText(GetUniqueID(), next_pos, 1.0f,
                    str.ToWideString(), PeekComponent().PeekMainFont(), CRGBColour(0, 255, 0, 255), false, ENGINE_INTERPOLATION_SET_0));
                next_pos.Z += PeekComponent().PeekDebugFont().GetFontHeight() * 0.2f;
            }
        }
    }
    CTCCreatureGenerator* pTCCreatureGen;
    if (GetTC(TCI_CREATURE_GENERATOR, &pTCCreatureGen))
        pTCCreatureGen->DrawDebugInfo(info);
    if (g_DrawThingDebugState) {
        CThingPhysicalDebugInfo physics_info;
        if (DrawGetPhysicsDebugInfo(physics_info)) {
            C3DVector next_pos = GetTCGraphicAppearance()->GetFocalPos(1.0f);
            next_pos.Z += 1.5f;
            CCharString str;
            str += physics_info.Moving ? "M " : "";
            str += physics_info.OnGround ? "g" : "";
            str += physics_info.OnMesh ? "m" : "";
            str += physics_info.Collidable ? "c" : "";
            str += physics_info.Interactable ? "i" : "";
            str += physics_info.Immoveable ? "r" : "";
            info.Engine->AddPrimitive(CEnginePrimitiveText(GetUniqueID(), next_pos, 1.0f,
                str.ToWideString(), PeekComponent().PeekMainFont(), CRGBColour(0, 255, 0, 255), false, ENGINE_INTERPOLATION_SET_0));
            next_pos.Z += PeekComponent().PeekDebugFont().GetFontHeight() * 0.2f;
        }
    }
    return true;
}

int main()
{
    CThingPhysical thing;
    Renderer renderer;
    CEnginePrimitivePrepareInfo info={&renderer,false};
    thing.Hidden=true;
    if(thing.DrawPreparePrimitivesForRendering(info)||thing.BaseCalls)
        return 1;
    thing.Hidden=false;info.Edit2DEngine=true;
    if(thing.DrawPreparePrimitivesForRendering(info)||thing.BaseCalls!=1||!renderer.Items.empty())
        return 2;
    info.Edit2DEngine=false;
    g_DrawThingDebugState=true;
    if(!thing.DrawPreparePrimitivesForRendering(info)||thing.Generator.Calls!=1||renderer.Items.size()!=2 ||
        renderer.Items[0].Text!="Creature"||renderer.Items[0].Position.Z!=3||renderer.Items[1].Text!="M gcr"||
        renderer.Items[1].Position.Z!=2.5f||renderer.Items[1].Id!=71||!renderer.Items[1].Valid)
        return 3;
    thing.World.Position.X=48;
    g_DrawThingDebugState=false;
    if(!thing.DrawPreparePrimitivesForRendering(info)||thing.Generator.Calls!=2||renderer.Items.size()!=2)
        return 4;
    puts("006a64d0_TEST PASS");
    return 0;
}
