#include <stdlib.h>
#include <stdio.h>
#include <string.h>
static void CheckDialogFixture(bool condition, int line)
{
    if (!condition) {
        printf("Dialog fixture failed at line %d\n", line);
        exit(1);
    }
}
#define CHECK_DIALOG(expression) CheckDialogFixture((expression), __LINE__)
#define FABLE_RETAIL_ERROR_DIALOG_PROC_PASS30
#define FABLE_RETAIL_WIN32_MOUSE
#include "engine/ErrorDialog.h"

ConfigDetection* pConfig;
window_data window_globals;
int GlobalForceExit;
unsigned int DontShowAgain;
char ErrorMessageTitle[128]="Error Title";
char ErrorMessage[1024]="Error Text";
char ErrorMessageURL[512]="test-help-target";

struct FixtureObservation {
    RECT Screen;
    RECT Bounds;
    long X,Y,W,H;
    int SeenGeometry;
    int SeenTitle;
    int SeenMessage;
    int SeenInfo;
    int LinkControl;
    int EnabledCount;
    int Disabled[3];
    char Info[256];
    unsigned int Checked;
    int CheckCount;
    int EndCount;
    int Result;
    int DestroyCount;
    int ShellCount;
};
static FixtureObservation Observation;
namespace DialogApiFixture {
typedef HWND FixtureHWND;
struct HINSTANCE__;
typedef HINSTANCE__* FixtureHINSTANCE;
static const HWND Dialog=reinterpret_cast<HWND>(9);
static const HWND Desktop=reinterpret_cast<HWND>(10);
static const HWND Owner=reinterpret_cast<HWND>(11);




static int __stdcall GetWindowRect(FixtureHWND hwnd, RECT* rect)
{
    CHECK_DIALOG(hwnd==Dialog);
    *rect=Observation.Bounds;
    return 1;
}

static FixtureHWND __stdcall GetDesktopWindow() { return Desktop; }

static int __stdcall GetClientRect(FixtureHWND hwnd, RECT* rect)
{
    CHECK_DIALOG(hwnd==Desktop);
    *rect=Observation.Screen;
    return 1;
}

static int __stdcall MoveWindow(FixtureHWND hwnd,int x,int y,int w,int h,int repaint)
{
    CHECK_DIALOG(hwnd==Dialog && repaint==1);
    Observation.X=x;
    Observation.Y=y;
    Observation.W=w;
    Observation.H=h;
    ++Observation.SeenGeometry;
    return 1;
}

static int __stdcall SetWindowTextA(FixtureHWND hwnd,const char* text)
{
    CHECK_DIALOG(hwnd==Dialog && strcmp(text,"Error Title")==0);
    ++Observation.SeenTitle;
    return 1;
}

static int __stdcall SetDlgItemTextA(FixtureHWND hwnd,int item,const char* text)
{
    CHECK_DIALOG(hwnd==Dialog);
    if (item==0x3ee) {
        CHECK_DIALOG(strcmp(text,"Error Text")==0);
        ++Observation.SeenMessage;
    } else {
        CHECK_DIALOG(item==0x3f1);
        CHECK_DIALOG(strlen(text)<sizeof(Observation.Info));
        strcpy(Observation.Info,text);
        ++Observation.SeenInfo;
    }
    return 1;
}

static FixtureHWND __stdcall GetDlgItem(FixtureHWND hwnd,int item)
{
    CHECK_DIALOG(hwnd==Dialog);
    return reinterpret_cast<HWND>(item);
}

static int __stdcall EnableWindow(FixtureHWND item,int enabled)
{
    CHECK_DIALOG(enabled==0 && Observation.EnabledCount<3);
    Observation.Disabled[Observation.EnabledCount++]=reinterpret_cast<int>(item);
    return 1;
}

static int RecordConvertStaticToHyperlink(FixtureHWND item)
{
    Observation.LinkControl=reinterpret_cast<int>(item);
    return 1;
}

static unsigned int __stdcall IsDlgButtonChecked(FixtureHWND hwnd,int item)
{
    CHECK_DIALOG(hwnd==Dialog && item==0x3e9);
    ++Observation.CheckCount;
    return Observation.Checked;
}

static int __stdcall EndDialog(FixtureHWND hwnd,int result)
{
    CHECK_DIALOG(hwnd==Dialog);
    ++Observation.EndCount;
    Observation.Result=result;
    return 1;
}

static int __stdcall DestroyWindow(FixtureHWND hwnd)
{
    CHECK_DIALOG(hwnd==Owner);
    ++Observation.DestroyCount;
    return 1;
}

static FixtureHINSTANCE __stdcall ShellExecuteA(FixtureHWND hwnd,const char* operation,const char* file,
                                      const char* arguments,const char* directory,int show)
{
    CHECK_DIALOG(hwnd==Dialog && strcmp(operation,"open")==0 && strcmp(file,ErrorMessageURL)==0);
    CHECK_DIALOG(!arguments && !directory && show==SW_SHOWNORMAL);
    ++Observation.ShellCount;
    return reinterpret_cast<FixtureHINSTANCE>(33);
}


}
#define GetWindowRect DialogApiFixture::GetWindowRect
#define GetDesktopWindow DialogApiFixture::GetDesktopWindow
#define GetClientRect DialogApiFixture::GetClientRect
#define MoveWindow DialogApiFixture::MoveWindow
#define SetWindowTextA DialogApiFixture::SetWindowTextA
#define SetDlgItemTextA DialogApiFixture::SetDlgItemTextA
#define GetDlgItem DialogApiFixture::GetDlgItem
#define EnableWindow DialogApiFixture::EnableWindow
#define IsDlgButtonChecked DialogApiFixture::IsDlgButtonChecked
#define EndDialog DialogApiFixture::EndDialog
#define DestroyWindow DialogApiFixture::DestroyWindow
#define ShellExecuteA DialogApiFixture::ShellExecuteA
#define ConvertStaticToHyperlink DialogApiFixture::RecordConvertStaticToHyperlink
int __stdcall ErrorDialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_INITDIALOG: {
        RECT ParentRect;
        RECT DialogRect;
        POINT DialogScreenPosition;
        GetWindowRect(hwndDlg, &DialogRect);
        GetClientRect(GetDesktopWindow(), &ParentRect);

        DialogScreenPosition.x = (ParentRect.right - ParentRect.left) / 2;
        DialogScreenPosition.y = (ParentRect.bottom - ParentRect.top) / 2;
        DialogScreenPosition.x -= (DialogRect.right - DialogRect.left) / 2;
        DialogScreenPosition.y -= (DialogRect.bottom - DialogRect.top) / 2;

        MoveWindow(hwndDlg, DialogScreenPosition.x, DialogScreenPosition.y,
                   DialogRect.right - DialogRect.left, DialogRect.bottom - DialogRect.top, 1);
        SetWindowTextA(hwndDlg, ErrorMessageTitle);
        SetDlgItemTextA(hwndDlg, 0x3ee, ErrorMessage);

        if (GlobalForceExit) {
            EnableWindow(GetDlgItem(hwndDlg, 0x3e9), 0);
            EnableWindow(GetDlgItem(hwndDlg, 0x3ec), 0);
            EnableWindow(GetDlgItem(hwndDlg, 0x105), 0);
        }
        ConvertStaticToHyperlink(GetDlgItem(hwndDlg, 0x3ef));

        if (pConfig) {
            static char Buffer[256];
            if (pConfig->DeviceID)
                sprintf(Buffer, "%dMHz, %dMB, %dM %s %s (0x%04x)",
                        pConfig->m_CpuSpeed, pConfig->m_SysMemory, pConfig->m_VideoMemory >> 20,
                        pConfig->VendorName, pConfig->DeviceName, pConfig->DeviceID);
            else if (pConfig->m_CpuSpeed && pConfig->m_SysMemory)
                sprintf(Buffer, "%dMHz, %dMB", pConfig->m_CpuSpeed, pConfig->m_SysMemory);
            else
                Buffer[0] = 0;

            SetDlgItemTextA(hwndDlg, 0x3f1, Buffer);
        }
        return 1;
    }

    case WM_CLOSE:
        if (window_globals.hWnd)
            DestroyWindow(window_globals.hWnd);
        EndDialog(hwndDlg, 2);
        return 1;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 0x3ec:
            DontShowAgain = IsDlgButtonChecked(hwndDlg, 0x3e9) != 0;
            EndDialog(hwndDlg, 0);
            return 1;
        case 0x105:
            DontShowAgain = IsDlgButtonChecked(hwndDlg, 0x3e9) != 0;
            EndDialog(hwndDlg, 1);
            return 1;
        case IDCANCEL:
        case 0x3ed:
            if (window_globals.hWnd)
                DestroyWindow(window_globals.hWnd);
            EndDialog(hwndDlg, 2);
            return 1;
        case 0x3ef:
            ShellExecuteA(hwndDlg, "open", ErrorMessageURL, 0, 0, SW_SHOWNORMAL);
            return 1;
        }
        break;
    }
    return 0;
}

#undef GetWindowRect
#undef GetDesktopWindow
#undef GetClientRect
#undef MoveWindow
#undef SetWindowTextA
#undef SetDlgItemTextA
#undef GetDlgItem
#undef EnableWindow
#undef IsDlgButtonChecked
#undef EndDialog
#undef DestroyWindow
#undef ShellExecuteA
#undef ConvertStaticToHyperlink
namespace DialogFixture {
static const HWND Dialog=reinterpret_cast<HWND>(9);
static const HWND Owner=reinterpret_cast<HWND>(11);
static void SetConfig(ConfigDetection& config,unsigned int device,unsigned int cpu,unsigned int memory,unsigned int video)
{
    static char vendor[]="A";
    static char name[]="B";
    config.VendorName=vendor;
    config.DeviceName=name;
    config.DeviceID=device;
    config.m_CpuSpeed=cpu;
    config.m_SysMemory=memory;
    config.m_VideoMemory=video;
}
static void Reset()
{
    memset(&Observation,0,sizeof(Observation));
    pConfig=0;
    window_globals.hWnd=0;
    GlobalForceExit=0;
    DontShowAgain=7;
}

static void CheckInit(ConfigDetection* config,bool fatal,const char* expected)
{
    Reset();
    const RECT parent = {-3, 5, 798, 604};
    const RECT dialog = {3, 13, 106, 88};
    Observation.Screen=parent;
    Observation.Bounds=dialog;
    GlobalForceExit=fatal;
    pConfig=config;
    CHECK_DIALOG(ErrorDialogProc(Dialog,WM_INITDIALOG,0,0)==1);
    CHECK_DIALOG(Observation.SeenGeometry==1 && Observation.SeenTitle==1 && Observation.SeenMessage==1);
    CHECK_DIALOG(Observation.X==349 && Observation.Y==262 && Observation.W==103 && Observation.H==75);
    CHECK_DIALOG(Observation.LinkControl==0x3ef);
    CHECK_DIALOG(Observation.EnabledCount==(fatal?3:0));
    if (fatal) {
        CHECK_DIALOG(Observation.Disabled[0]==0x3e9);
        CHECK_DIALOG(Observation.Disabled[1]==0x3ec);
        CHECK_DIALOG(Observation.Disabled[2]==0x105);
    }
    if (expected) {
        CHECK_DIALOG(Observation.SeenInfo==1 && strcmp(Observation.Info,expected)==0);
    } else {
        CHECK_DIALOG(Observation.SeenInfo==0);
    }
    CHECK_DIALOG(Observation.EndCount==0 && Observation.DestroyCount==0 && Observation.ShellCount==0);
}

static void CheckClose(unsigned int message,unsigned int command,bool owner)
{
    Reset();
    if (owner) window_globals.hWnd=Owner;
    CHECK_DIALOG(ErrorDialogProc(Dialog,message,command,0)==1);
    CHECK_DIALOG(Observation.EndCount==1 && Observation.Result==2);
    CHECK_DIALOG(Observation.DestroyCount==(owner?1:0) && DontShowAgain==7);
}

static void CheckCommands()
{
    Reset();
    CHECK_DIALOG(ErrorDialogProc(Dialog,0,0,0)==0);
    CHECK_DIALOG(ErrorDialogProc(Dialog,WM_COMMAND,0x3ea,0)==0);
    CHECK_DIALOG(Observation.EndCount==0 && Observation.DestroyCount==0 && DontShowAgain==7);

    Reset();
    CHECK_DIALOG(ErrorDialogProc(Dialog,WM_COMMAND,(5u<<16)|0x105,0)==1);
    CHECK_DIALOG(Observation.CheckCount==1 && DontShowAgain==0 && Observation.Result==1);
    CHECK_DIALOG(Observation.EndCount==1 && Observation.DestroyCount==0);

    Reset();
    Observation.Checked=2;
    CHECK_DIALOG(ErrorDialogProc(Dialog,WM_COMMAND,0x3ec,0)==1);
    CHECK_DIALOG(Observation.CheckCount==1 && DontShowAgain==1 && Observation.Result==0);
    CHECK_DIALOG(Observation.EndCount==1 && Observation.DestroyCount==0);

    Reset();
    CHECK_DIALOG(ErrorDialogProc(Dialog,WM_COMMAND,0x3ef,0)==1);
    CHECK_DIALOG(Observation.ShellCount==1 && Observation.EndCount==0 && DontShowAgain==7);

    for (int owner=0; owner<2; ++owner) {
        CheckClose(WM_CLOSE,0,owner!=0);
        CheckClose(WM_COMMAND,IDCANCEL,owner!=0);
        CheckClose(WM_COMMAND,0x3ed,owner!=0);
    }
}

}

int main()
{
    using namespace DialogFixture;
    ConfigDetection full,partial,no_cpu,no_memory;
    SetConfig(full,0x10de,2400,128,64u<<20);
    SetConfig(partial,0,2200,256,0);
    SetConfig(no_cpu,0,0,512,0);
    SetConfig(no_memory,0,200,0,0);
    CheckInit(0,false,0);
    CheckInit(&full,true,"2400MHz, 128MB, 64M A B (0x10de)");
    CheckInit(&partial,false,"2200MHz, 256MB");
    CheckInit(&no_cpu,false,"");
    CheckInit(&no_memory,true,"");
    CheckCommands();
    puts("0099d8c0_TEST PASS");
    return 0;
}