#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <iostream>
#include <unistd.h>

void performMouseClick(int x, int y)
{
    Display* display = XOpenDisplay(nullptr);
    if (display == nullptr)
    {
        std::cerr << "Nie można otworzyć wyświetlacza X11\n";
        return;
    }
    sleep(2);
    XTestFakeMotionEvent(display, -1, x, y, 0);
    XTestFakeButtonEvent(display, 1, true, 0);
    XFlush(display);
    XTestFakeButtonEvent(display, 1, false, 0);
    XFlush(display);

    XCloseDisplay(display);
}

int main()
{
    int targetX = 180, targetY = 77;
    // std::cout << "Podaj współrzędną X: ";
    // std::cin >> targetX;
    // std::cout << "Podaj współrzędną Y: ";
    // std::cin >> targetY;

    while (true)
        performMouseClick(targetX, targetY);

    return 0;
}

