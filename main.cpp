#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <iostream>
#include <unistd.h>

struct Point
{
  int x{};
  int y{};
};

class MouseClicker
{
private:
    Display* display;
public:
    MouseClicker();
    ~MouseClicker();

    auto getMouseClickPoint() -> Point;
    auto performMouseClick(const Point&) -> void;
};

MouseClicker::MouseClicker() : display(XOpenDisplay(nullptr))
{
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

auto MouseClicker::getMouseClickPoint() -> Point
{
    Point point{0,0};
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
                point.x = event.xbutton.x;
                point.y = event.xbutton.y;

                std::cout << "Clicking on a point: { x, y }= " << point.x << ", y = " << point.y << std::endl;

                XUngrabPointer(display, CurrentTime);
                XFlush(display);

                return point;
            }
        }
    }

    return {};
}


auto MouseClicker::performMouseClick(const Point& point) -> void
{
  while (true)
    {
    sleep(4);
    XTestFakeMotionEvent(display, -1, point.x, point.y, 0);
    XTestFakeButtonEvent(display, 1, true, 0);
    XFlush(display);
    XTestFakeButtonEvent(display, 1, false, 0);
    XFlush(display);
    std::cout << "Clicking on a point x = " << point.x << ", y = " << point.y << std::endl;
  }
}

auto main() -> int
{
    MouseClicker clicker;
    auto clickPoint = clicker.getMouseClickPoint();
    if (clickPoint.x == 0 && clickPoint.y == 0)
    {
        std::cerr << "Unable to read mouse click point! \n";
        return 1;
    }

    clicker.performMouseClick(clickPoint);

    return 0;
}
