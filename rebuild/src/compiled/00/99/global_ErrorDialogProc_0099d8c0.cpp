// 0099d8c0 ErrorDialogProc -- the retail Windows error dialog callback.
// Leaf: ?ErrorDialogProc@@YGHPAUHWND__@@IIJ@Z
// Flags: /GS
#define FABLE_RETAIL_ERROR_DIALOG_PROC_PASS30
#define FABLE_RETAIL_WIN32_MOUSE

#include <stdio.h>
#include "engine/ErrorDialog.h"

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