#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <iostream>
#include <unistd.h>

std::pair<int, int> getMouseClickPoint()
{
    Display* display = XOpenDisplay(nullptr);
    XEvent event{};
    Window rootWindow = DefaultRootWindow(display);

    XGrabPointer(display, rootWindow, False, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
    XSelectInput(display, rootWindow, ButtonPressMask);

    while (true)
    {
        XNextEvent(display, &event);

        if (event.type == ButtonPress)
        {
            if (event.xbutton.button == Button1)
            {
                int x = event.xbutton.x;
                int y = event.xbutton.y;

                std::cout << "You have set points in: x = " << x << ", y = " << y << std::endl;
                XCloseDisplay(display);
                return { x, y };
            }
        }
    }

    XCloseDisplay(display);
    return { 0,0 };
}

void performMouseClick(int x, int y)
{
    Display* display = XOpenDisplay(nullptr);

    while (true)
    {
        sleep(5);
        XTestFakeMotionEvent(display, -1, x, y, 0);
        XTestFakeButtonEvent(display, 1, true, 0);
        XFlush(display);
        XTestFakeButtonEvent(display, 1, false, 0);
        XFlush(display);
    }

    XCloseDisplay(display);
}

int main()
{
    const auto& clickPoint = getMouseClickPoint();
    if (clickPoint.first == 0 && clickPoint.second == 0)
    {
        std::cerr << "Unable to read mouse click point\n";
        return 1;
    }

    performMouseClick(clickPoint.first, clickPoint.second);

    return 0;
}
