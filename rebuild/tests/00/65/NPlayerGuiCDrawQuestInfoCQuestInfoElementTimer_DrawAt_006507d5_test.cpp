// Behaviour test for NPlayerGui::CDrawQuestInfo::CQuestInfoElementTimer::DrawAt (0x006507D5): a copy of the drawing logic over
// recording fakes (graphic bank, engine, font, script interface timer, text output).
#include <cstdio>
#include <cstring>
struct Vec { float X, Y; };
struct Colour { unsigned char B, G, R, A; Colour(unsigned char r, unsigned char g, unsigned char b) { R=r; G=g; B=b; A=255; } };
struct Rec {
    int handles, persistent; float boxL, boxT, boxR, boxB; char text[32]; Vec textPos; Colour col; float scale;
    Rec() : handles(0), persistent(0), col(0,0,0) { text[0]=0; }
};
static Rec g;
static unsigned long g_w = 32, g_h = 16; static long g_fontH = 10; static long g_timer = 0;
static void Draw2DText(const Vec& p, const char* t, const Colour& c, float s) { g.textPos=p; std::strcpy(g.text,t); g.col=c; g.scale=s; }
struct Timer {
    float Scale; long TimerIndex, Graphic;
    float DrawAt(const Vec& pos)
    {
        float graphic_width=0.0f, graphic_height=0.0f;
        if(Graphic>0){
            g.handles=1;
            graphic_width=(float)g_w; graphic_height=(float)g_h;
            g.boxL=pos.X; g.boxT=pos.Y; g.boxR=pos.X+graphic_width*Scale; g.boxB=pos.Y+graphic_height*Scale;
            g.persistent++;
        }
        Vec text_pos={pos.X+graphic_width*Scale,pos.Y};
        text_pos.Y+=(graphic_height-(float)g_fontH)*0.5f;
        long seconds=g_timer;
        char text[32]; std::sprintf(text,"%02d:%02d",(int)(seconds/60),(int)(seconds%60));
        if(seconds<11)Draw2DText(text_pos,text,Colour(255,100,100),Scale);
        else Draw2DText(text_pos,text,Colour(255,255,255),Scale);
        if(graphic_height>0.0f){double r=graphic_height*Scale+2.0f;return (float)r;}
        double r=g_fontH*Scale; return (float)r;
    }
};
int main()
{
    Timer t; t.Scale=2.0f; t.TimerIndex=3; t.Graphic=5;
    Vec p={10.0f,20.0f};
    g_timer=125;
    float h=t.DrawAt(p);
    if(h!=34.0f||g.persistent!=1||g.boxR!=74.0f||g.boxB!=52.0f||std::strcmp(g.text,"02:05")!=0||g.textPos.X!=74.0f||g.textPos.Y!=23.0f
       ||g.col.G!=255||g.col.B!=255||g.col.R!=255||g.col.A!=255) { std::printf("006507d5_TEST FAIL\n"); return 1; }
    g=Rec(); t.Graphic=0; g_timer=9;
    h=t.DrawAt(p);
    if(h!=20.0f||g.persistent!=0||std::strcmp(g.text,"00:09")!=0||g.textPos.X!=10.0f||g.textPos.Y!=15.0f
       ||g.col.R!=255||g.col.G!=100||g.col.B!=100||g.col.A!=255) { std::printf("006507d5_TEST FAIL\n"); return 1; }
    g_timer=11; t.DrawAt(p);
    if(g.col.G!=255||std::strcmp(g.text,"00:11")!=0) { std::printf("006507d5_TEST FAIL\n"); return 1; }
    g_timer=10; t.DrawAt(p);
    if(g.col.G!=100) { std::printf("006507d5_TEST FAIL\n"); return 1; }
    std::printf("006507d5_TEST PASS\n");
    return 0;
}