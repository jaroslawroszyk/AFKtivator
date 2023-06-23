#include <X11/Xlib.h>
#include <iostream>

int main()
{
    Display* display = XOpenDisplay(NULL);

    // if (display == NULL)
        // {
        //     std::cout << "Cant open window." << std::endl;
        //     return 1;
        // }

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

                std::cout << "Point: x = " << x << ", y = " << y << std::endl;

                break;
            }
        }
    }

    XCloseDisplay(display);

    return 0;
}
