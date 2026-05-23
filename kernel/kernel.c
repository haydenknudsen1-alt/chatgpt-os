#define VGA ((char*)0xb8000)

/* ---------------- FRAMEBUFFER ---------------- */

char fb[80 * 25];
char fc[80 * 25];

void fb_clear(char c, char col) {
    for (int i = 0; i < 80 * 25; i++) {
        fb[i] = c;
        fc[i] = col;
    }
}

void fb_put(int x, int y, char c, char col) {
    if (x < 0 || x >= 80 || y < 0 || y >= 25) return;

    int i = y * 80 + x;
    fb[i] = c;
    fc[i] = col;
}

void flush() {
    for (int i = 0; i < 80 * 25; i++) {
        VGA[i * 2] = fb[i];
        VGA[i * 2 + 1] = fc[i];
    }
}

/* ---------------- STATE ---------------- */

static int mouse_x = 40;
static int mouse_y = 12;
static int mouse_lb = 0;

struct window {
    int x, y, w, h;
};

struct window win1 = {5, 3, 25, 8};
struct window win2 = {40, 6, 25, 10};

int mouse_old_x = 40;
int mouse_old_y = 12;

/* ---------------- IO ---------------- */

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

/* ---------------- MOUSE ---------------- */

unsigned char mouse_cycle = 0;
signed char mouse_byte[3];

void mouse_wait(int type) {
    int t = 100000;
    if (type == 0) {
        while (t--) if (inb(0x64) & 1) return;
    } else {
        while (t--) if (!(inb(0x64) & 2)) return;
    }
}

void mouse_write(unsigned char a) {
    mouse_wait(1);
    outb(0x64, 0xD4);
    mouse_wait(1);
    outb(0x60, a);
}

unsigned char mouse_read() {
    mouse_wait(0);
    return inb(0x60);
}

void mouse_init() {
    unsigned char status;

    mouse_wait(1);
    outb(0x64, 0xA8);

    mouse_wait(1);
    outb(0x64, 0x20);
    mouse_wait(0);
    status = inb(0x60);
    status |= 2;
    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, status);

    mouse_write(0xF4);
    mouse_read();
}

void mouse_handler(unsigned char data) {
    mouse_byte[mouse_cycle++] = data;

    if (mouse_cycle == 3) {
        mouse_cycle = 0;

        mouse_lb = mouse_byte[0] & 1;

        mouse_x += mouse_byte[1];
        mouse_y -= mouse_byte[2];

        if (mouse_x < 0) mouse_x = 0;
        if (mouse_y < 0) mouse_y = 0;
        if (mouse_x > 79) mouse_x = 79;
        if (mouse_y > 24) mouse_y = 24;
    }
}

void mouse_poll() {
    if (inb(0x64) & 1) {
        mouse_handler(inb(0x60));
    }
}

/* ---------------- WINDOW DRAG ---------------- */

void drag(struct window *w) {
    if (mouse_lb) {
        w->x = mouse_x - 5;
        w->y = mouse_y - 2;
    }
}

/* ---------------- DRAW WINDOW ---------------- */

void draw_box(struct window *w, char col) {
    for (int i = 0; i < w->w; i++) {
        fb_put(w->x + i, w->y, '-', col);
        fb_put(w->x + i, w->y + w->h, '-', col);
    }

    for (int j = 0; j < w->h; j++) {
        fb_put(w->x, w->y + j, '|', col);
        fb_put(w->x + w->w, w->y + j, '|', col);
    }
}

/* ---------------- CURSOR ---------------- */

void draw_mouse() {
    fb_put(mouse_old_x, mouse_old_y, ' ', 0x1F);
    fb_put(mouse_x, mouse_y, '+', 0x0F);

    mouse_old_x = mouse_x;
    mouse_old_y = mouse_y;
}

/* ---------------- KERNEL ---------------- */

void kernel_main() {
    mouse_init();

    while (1) {
        mouse_poll();

        drag(&win1);
        drag(&win2);

        fb_clear(' ', 0x1F);

        draw_box(&win1, 0x0F);
        draw_box(&win2, 0x07);
        draw_mouse();

        flush();
    }
}
