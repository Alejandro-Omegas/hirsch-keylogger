#include <stdio.h>
#include <Windows.h>

// Hook procedure to process keyboard events
LRESULT CALLBACK hook_proc(int code, WPARAM wParam, LPARAM lParam) {
    if (code >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT *pkey = (KBDLLHOOKSTRUCT *)lParam;

        // Process the virtual key code
        switch (pkey->vkCode) {
            case VK_BACK: printf("(backspace)"); break;
            case VK_RETURN: printf("(enter)\n"); break;
            case VK_SPACE: printf(" "); break;
            case VK_LSHIFT: printf("(lshift)"); break;
            default: {
                // Convert virtual key to ASCII
                char key[2] = {0};
                BYTE keyboardState[256] = {0};
                if (ToAscii(pkey->vkCode, pkey->scanCode, keyboardState, (LPWORD)key, 0) == 1) {
                    printf("%c", key[0]);
                }
            }
        }
    }

    return CallNextHookEx(NULL, code, wParam, lParam);
}

int main() {
    // Install the keyboard hook
    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, hook_proc, NULL, 0);

    if (hook == NULL) {
        printf("Failed to install hook.\n");
        return -1;
    }

    printf("Keyboard hook installed. Press keys to see output.\n");

    // Message loop to keep the hook running
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Clean up and uninstall the hook
    UnhookWindowsHookEx(hook);

    return 0;
}
