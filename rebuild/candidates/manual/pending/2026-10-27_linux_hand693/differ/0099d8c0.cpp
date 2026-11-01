// ErrorDialogProc(HWND__* hDlg, unsigned int msg, unsigned int wParam, long lParam) -- the error dialog's procedure: on WM_INITDIALOG centre the dialog on the desktop client area, set its title and text, disable the don't-show-again box and both buttons when the error is fatal, turn the URL static into a hyperlink and fill the system-info line from the config detection info; on WM_COMMAND remember the check box and end (0x105 -> 1, 0x3EC -> 0), open the support URL (0x3EF), or destroy the owner and end with 2 (IDCANCEL / 0x3ED); WM_CLOSE likewise
// Grade: DIFFER(685v689) -- only the centring arithmetic: retail computes desk/2 + -(dialog/2) (`sar; neg; add`) after reading both desktop extents, VC7.1 here `sar; sub` with the dialog extents read first (63 normalised lines); the frame matches with the two RECTs as one array
// Leaf: ?ErrorDialogProc@@YGHPAUHWND__@@IIJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099D8C0, 689 bytes, __stdcall `ret 0x10`.
// Identity: the DLGPROC passed to LocalizedDialogBox by the error-reporting path; Ego_r.pdb donor order places
//   ?ErrorDialogProc@@YGHPAUHWND__@@IIJ@Z between ConvertStaticToHyperlink and nErrorStringFromErrorCode / SetDirtyExitFlag,
//   the same neighbours as retail. Globals (descriptive): g_bErrorDialogChecked 0x013BD6F4, g_hErrorDialogOwner 0x013BD0C0,
//   g_bErrorDialogFatal 0x013BCE3C, g_szErrorDialogTitle 0x013BCE40, g_szErrorDialogText 0x013BCA38, g_szErrorDialogURL
//   0x013BCEC0, g_szErrorDialogSystemInfo 0x013BD700; SConfigDetectionInfo DriverName +0xC4, DriverVersion +0xC8, DeviceID
//   +0xCC, SystemMemoryMB +0xDC, CPUSpeedMHz +0xE0, VideoMemory +0xE4.
// Next: two separate tagRECT locals put the dialog RECT at frame +0 whatever the order, scope or names (12 probes);
//   `tagRECT rects[2]` fixes the frame (an array is ordinary C, but unproven as retail's shape). Remaining lever: the centring
//   as a same-TU helper VC7.1 inlines (e.g. a static CentreWindow(HWND, const RECT&, const RECT&)) or int locals
//   holding the halves, to get retail's neg/add order.
struct HWND__;
struct HINSTANCE__;
struct tagRECT { long left, top, right, bottom; };
extern "C" __declspec(dllimport) unsigned int __stdcall IsDlgButtonChecked(HWND__* dlg, int id);
extern "C" __declspec(dllimport) int __stdcall EndDialog(HWND__* dlg, int result);
extern "C" __declspec(dllimport) int __stdcall DestroyWindow(HWND__* wnd);
extern "C" __declspec(dllimport) HINSTANCE__* __stdcall ShellExecuteA(HWND__* wnd, const char* op, const char* file, const char* params, const char* dir, int show);
extern "C" __declspec(dllimport) int __stdcall GetWindowRect(HWND__* wnd, tagRECT* rect);
extern "C" __declspec(dllimport) HWND__* __stdcall GetDesktopWindow();
extern "C" __declspec(dllimport) int __stdcall GetClientRect(HWND__* wnd, tagRECT* rect);
extern "C" __declspec(dllimport) int __stdcall MoveWindow(HWND__* wnd, int x, int y, int w, int h, int repaint);
extern "C" __declspec(dllimport) int __stdcall SetWindowTextA(HWND__* wnd, const char* text);
extern "C" __declspec(dllimport) int __stdcall SetDlgItemTextA(HWND__* dlg, int id, const char* text);
extern "C" __declspec(dllimport) HWND__* __stdcall GetDlgItem(HWND__* dlg, int id);
extern "C" __declspec(dllimport) int __stdcall EnableWindow(HWND__* wnd, int enable);
extern "C" int __cdecl sprintf(char* buffer, const char* format, ...);
int __fastcall ConvertStaticToHyperlink(HWND__* wnd);
struct SConfigDetectionInfo {
    unsigned char _pad00[0xc4];
    const char*   DriverName;      // +0xc4
    const char*   DriverVersion;   // +0xc8
    unsigned long DeviceID;        // +0xcc
    unsigned char _padd0[0xdc - 0xd0];
    long          SystemMemoryMB;  // +0xdc
    long          CPUSpeedMHz;     // +0xe0
    unsigned long VideoMemory;     // +0xe4
};
extern SConfigDetectionInfo* g_pConfigDetectionInfo;   // 0x013BD6E8
extern int g_bErrorDialogChecked;      // 0x013BD6F4
extern HWND__* g_hErrorDialogOwner;    // 0x013BD0C0
extern int g_bErrorDialogFatal;        // 0x013BCE3C
extern char g_szErrorDialogTitle[];    // 0x013BCE40
extern char g_szErrorDialogText[];     // 0x013BCA38
extern char g_szErrorDialogURL[];      // 0x013BCEC0
extern char g_szErrorDialogSystemInfo[];   // 0x013BD700

int __stdcall ErrorDialogProc(HWND__* hDlg, unsigned int msg, unsigned int wParam, long lParam)
{
    switch (msg) {
    case 0x110: {
        tagRECT rects[2];
        GetWindowRect(hDlg, &rects[1]);
        GetClientRect(GetDesktopWindow(), &rects[0]);
        int deskWidth = rects[0].right - rects[0].left;
        int deskHeight = rects[0].bottom - rects[0].top;
        int width = rects[1].right - rects[1].left;
        int height = rects[1].bottom - rects[1].top;
        MoveWindow(hDlg, deskWidth / 2 - width / 2, deskHeight / 2 - height / 2, width, height, 1);
        SetWindowTextA(hDlg, g_szErrorDialogTitle);
        SetDlgItemTextA(hDlg, 0x3ee, g_szErrorDialogText);
        if (g_bErrorDialogFatal) {
            EnableWindow(GetDlgItem(hDlg, 0x3e9), 0);
            EnableWindow(GetDlgItem(hDlg, 0x3ec), 0);
            EnableWindow(GetDlgItem(hDlg, 0x105), 0);
        }
        ConvertStaticToHyperlink(GetDlgItem(hDlg, 0x3ef));
        if (g_pConfigDetectionInfo) {
            if (g_pConfigDetectionInfo->DeviceID)
                sprintf(g_szErrorDialogSystemInfo, "%dMHz, %dMB, %dM %s %s (0x%04x)", g_pConfigDetectionInfo->CPUSpeedMHz,
                        g_pConfigDetectionInfo->SystemMemoryMB, g_pConfigDetectionInfo->VideoMemory >> 20,
                        g_pConfigDetectionInfo->DriverName, g_pConfigDetectionInfo->DriverVersion, g_pConfigDetectionInfo->DeviceID);
            else if (g_pConfigDetectionInfo->CPUSpeedMHz && g_pConfigDetectionInfo->SystemMemoryMB)
                sprintf(g_szErrorDialogSystemInfo, "%dMHz, %dMB", g_pConfigDetectionInfo->CPUSpeedMHz, g_pConfigDetectionInfo->SystemMemoryMB);
            else
                g_szErrorDialogSystemInfo[0] = 0;
            SetDlgItemTextA(hDlg, 0x3f1, g_szErrorDialogSystemInfo);
        }
        return 1;
    }
    case 0x111:
        switch ((unsigned short)wParam) {
        case 0x105:
            g_bErrorDialogChecked = IsDlgButtonChecked(hDlg, 0x3e9) != 0;
            EndDialog(hDlg, 1);
            return 1;
        case 0x3ec:
            g_bErrorDialogChecked = IsDlgButtonChecked(hDlg, 0x3e9) != 0;
            EndDialog(hDlg, 0);
            return 1;
        case 0x3ef:
            ShellExecuteA(hDlg, "open", g_szErrorDialogURL, 0, 0, 1);
            return 1;
        case 2:
        case 0x3ed:
            if (g_hErrorDialogOwner)
                DestroyWindow(g_hErrorDialogOwner);
            EndDialog(hDlg, 2);
            return 1;
        }
        break;
    case 0x10:
        if (g_hErrorDialogOwner)
            DestroyWindow(g_hErrorDialogOwner);
        EndDialog(hDlg, 2);
        return 1;
    }
    return 0;
}
