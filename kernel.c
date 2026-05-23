typedef enum {
    MODE_NONE,
    MODE_TERMINAL,
    MODE_DESKTOP
} BootMode;

BootMode selected_mode = MODE_NONE;

void print(const char* str) {
    char* video = (char*)0xb8000;
    int i = 0;
    while (str[i]) {
        video[i * 2] = str[i];
        video[i * 2 + 1] = 0x07;
        i++;
    }
}

void kernel_main() {
    print("MyOS Installer\n");
    print("1) Terminal OS\n");
    print("2) Desktop OS\n");
    print("Select option...\n");

    while (1) {
        // next step: keyboard input
    }
}
