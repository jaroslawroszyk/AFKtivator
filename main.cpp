#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <iostream>
#include <unistd.h>

class MouseClicker
{
private:
    Display* display;
public:
    MouseClicker();
    ~MouseClicker();

    std::pair<int, int> getMouseClickPoint();
    void performMouseClick(int x, int y);
};

MouseClicker::MouseClicker() : display(XOpenDisplay(nullptr))
{
    // display = XOpenDisplay(nullptr);
    if (display == nullptr)
    {
        std::cerr << "The X11 display cannot be opened\n";
    }
}

MouseClicker::~MouseClicker()
{
    if (display != nullptr)
    {
        XCloseDisplay(display);
        display = nullptr;
    }
}

std::pair<int, int> MouseClicker::getMouseClickPoint()
{
    XEvent event;
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

                std::cout << "Clicking on a point x = " << x << ", y = " << y << std::endl;

                XUngrabPointer(display, CurrentTime);
                XFlush(display);

                return { x, y };
            }
        }
    }

    return {};
}

void MouseClicker::performMouseClick(int x, int y)
{
    while (true)
    {
        sleep(4);
        XTestFakeMotionEvent(display, -1, x, y, 0);
        XTestFakeButtonEvent(display, 1, true, 0);
        XFlush(display);
        XTestFakeButtonEvent(display, 1, false, 0);
        XFlush(display);
        std::cout << "Clicking on a point x = " << x << ", y = " << y << std::endl;
    }
}

int main()
{
    MouseClicker clicker;
    std::pair<int, int> clickPoint = clicker.getMouseClickPoint();
    if (clickPoint.first == 0 && clickPoint.second == 0)
    {
        std::cerr << "Unable to read mouse click pointn";
        return 1;
    }

    clicker.performMouseClick(clickPoint.first, clickPoint.second);

    return 0;
}
