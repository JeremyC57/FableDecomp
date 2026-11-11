#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void Check(bool b){if(!b)abort();}
struct HWND__{int ID;};typedef HWND__* HWND;
struct tagPOINT {long x,y;};typedef tagPOINT POINT;
struct tagRECT {long left,top,right,bottom;};typedef tagRECT RECT;
class C2DBoxF {public:float TLX,TLY,BRX,BRY;C2DBoxF(){}};
class C2DVector {public:float X,Y;};
class C2DExtentsI {public:long X,Y;};
static int Trace[64],TraceSize,SystemCalls,ButtonMask;
static void Record(int n){Check(TraceSize<64);Trace[TraceSize++]=n;}
static HWND__ Window;
static POINT Cursor,Origin;
static RECT Client;
class CSurface {public:C2DExtentsI Dimensions;C2DExtentsI GetDimensions()const{Record(20);return Dimensions;}};
class CDisplayManager {public:CSurface BackBufferSurface;CSurface& GetBackBuffer(){return BackBufferSurface;}};
class CSystemManager {
public:CDisplayManager* Display;HWND ApplicationWinHandle;
 CDisplayManager* GetDisplayManager(){return Display;}
 HWND GetApplicationWinHandle()const{return ApplicationWinHandle;}
 bool IsMouseButton0Pressed(){Record(30);return (ButtonMask&1)!=0;}
 bool IsMouseButton1Pressed(){Record(31);return (ButtonMask&2)!=0;}
 bool IsMouseButton2Pressed(){Record(32);return (ButtonMask&4)!=0;}
 bool IsMouseButton3Pressed(){Record(33);return (ButtonMask&8)!=0;}
 bool IsMouseButton4Pressed(){Record(34);return (ButtonMask&16)!=0;}
};
static CSystemManager System;
static CSystemManager* GFGetSystemManager(){++SystemCalls;Record(1);return &System;}
static int GetCursorPos(POINT* p){Record(10);*p=Cursor;return 1;}
static int ClientToScreen(HWND h,POINT* p){Check(h==&Window);Check(p->x==0&&p->y==0);Record(11);p->x+=Origin.x;p->y+=Origin.y;return 1;}
static int GetClientRect(HWND h,RECT* p){Check(h==&Window);Record(12);*p=Client;return 1;}
class CMouseDX {public:
 C2DVector Pos;
 bool NonExclusiveOldButton0Pressed,NonExclusiveOldButton1Pressed,NonExclusiveOldButton2Pressed,NonExclusiveOldButton3Pressed,NonExclusiveOldButton4Pressed;
 float NonExclusiveOldMouseX,NonExclusiveOldMouseY;
 void ResetNonExclusiveMouse();
};
void CMouseDX::ResetNonExclusiveMouse()
{
    C2DBoxF box;
    if (GFGetSystemManager()->GetDisplayManager()) {
        POINT mouse_pos;
        long mouse_x, mouse_y;
        POINT window_pos;
        RECT window_rect;
        HWND win_handle = GFGetSystemManager()->GetApplicationWinHandle();
        GetCursorPos(&mouse_pos);
        window_pos.x = 0;
        window_pos.y = 0;
        ClientToScreen(win_handle, &window_pos);
        GetClientRect(win_handle, &window_rect);
        if (mouse_pos.x >= window_pos.x && mouse_pos.x < window_rect.right + window_pos.x
            && mouse_pos.y >= window_pos.y && mouse_pos.y < window_rect.bottom + window_pos.y) {
            mouse_pos.x -= window_pos.x;
            mouse_pos.y -= window_pos.y;
            mouse_x = mouse_pos.x;
            mouse_y = mouse_pos.y;
            C2DExtentsI screen_dims = GFGetSystemManager()->GetDisplayManager()->GetBackBuffer().GetDimensions();
            if (mouse_x >= 0 && mouse_x < screen_dims.X && mouse_y >= 0 && mouse_y < screen_dims.Y) {
                NonExclusiveOldMouseX = (float)mouse_x;
                NonExclusiveOldMouseY = (float)mouse_y;
                Pos.X = (float)mouse_x;
                Pos.Y = (float)mouse_y;
            }
            NonExclusiveOldButton0Pressed = GFGetSystemManager()->IsMouseButton0Pressed();
            NonExclusiveOldButton1Pressed = GFGetSystemManager()->IsMouseButton1Pressed();
            NonExclusiveOldButton2Pressed = GFGetSystemManager()->IsMouseButton2Pressed();
            NonExclusiveOldButton3Pressed = GFGetSystemManager()->IsMouseButton3Pressed();
            NonExclusiveOldButton4Pressed = GFGetSystemManager()->IsMouseButton4Pressed();
        }
    }
}

int main(){
 CDisplayManager display;System.ApplicationWinHandle=&Window;
 const long origins[]={-17,0,23};const long coords[]={-1,0,1,6,7,8};
 const long dims[]={0,4,8};unsigned long cases=0;
 for(int active=0;active<2;++active)for(int ox=0;ox<3;++ox)for(int oy=0;oy<3;++oy)
 for(int x=0;x<6;++x)for(int y=0;y<6;++y)for(int w=0;w<3;++w)for(int h=0;h<3;++h)for(int mask=0;mask<32;++mask){
  CMouseDX mouse;mouse.Pos.X=-10.0f;mouse.Pos.Y=-20.0f;mouse.NonExclusiveOldMouseX=-30.0f;mouse.NonExclusiveOldMouseY=-40.0f;
  mouse.NonExclusiveOldButton0Pressed=false;mouse.NonExclusiveOldButton1Pressed=true;mouse.NonExclusiveOldButton2Pressed=false;mouse.NonExclusiveOldButton3Pressed=true;mouse.NonExclusiveOldButton4Pressed=false;
  Origin.x=origins[ox];Origin.y=origins[oy];Cursor.x=Origin.x+coords[x];Cursor.y=Origin.y+coords[y];
  Client.left=0;Client.top=0;Client.right=8;Client.bottom=7;
  display.BackBufferSurface.Dimensions.X=dims[w];display.BackBufferSurface.Dimensions.Y=dims[h];
  System.Display=active?&display:0;ButtonMask=mask;TraceSize=0;SystemCalls=0;
  mouse.ResetNonExclusiveMouse();
  bool inside=active&&coords[x]>=0&&coords[x]<8&&coords[y]>=0&&coords[y]<7;
  bool valid=inside&&coords[x]<dims[w]&&coords[y]<dims[h];
  Check(mouse.Pos.X==(valid?(float)coords[x]:-10.0f));Check(mouse.Pos.Y==(valid?(float)coords[y]:-20.0f));
  Check(mouse.NonExclusiveOldMouseX==(valid?(float)coords[x]:-30.0f));Check(mouse.NonExclusiveOldMouseY==(valid?(float)coords[y]:-40.0f));
  Check(mouse.NonExclusiveOldButton0Pressed==(inside?((mask&1)!=0):false));
  Check(mouse.NonExclusiveOldButton1Pressed==(inside?((mask&2)!=0):true));
  Check(mouse.NonExclusiveOldButton2Pressed==(inside?((mask&4)!=0):false));
  Check(mouse.NonExclusiveOldButton3Pressed==(inside?((mask&8)!=0):true));
  Check(mouse.NonExclusiveOldButton4Pressed==(inside?((mask&16)!=0):false));
  const int expected[]={1,1,10,11,12,1,20,1,30,1,31,1,32,1,33,1,34};
  int expectedSize=inside?17:(active?5:1);Check(TraceSize==expectedSize);for(int i=0;i<TraceSize;++i)Check(Trace[i]==expected[i]);
  Check(SystemCalls==(inside?8:(active?2:1)));++cases;
 }
 Check(cases==186624ul);puts("00ab5030_TEST PASS");return 0;
}