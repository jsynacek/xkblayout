#define _POSIX_C_SOURCE 20170901
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/XKBlib.h>

/* Parse the current layout from the symbol string.
 * Example: "pc+us+cz(qwerty):2+ru(phonetic):3+inet(evdev)+group(shifts_toggle)"
 * Assumes that groupidx is never out of range.
 */
char *current_layout(char *symbols, int groupidx)
{
        char *layout = symbols, *co, *pl;

        /* Skip pc. */
        layout = strchr(layout, '+') + 1;

        /* First symbol is without semicolon. */
        if (groupidx == 0) {
                pl = strchr(layout, '+');
                layout[pl-layout] = '\0';
                return strdup(layout);
        }

        while (groupidx > 0) {
                layout = strchr(layout, '+') + 1;
                --groupidx;
        }
        co = strchr(layout, ':');
        layout[co-layout] = '\0';

        return strdup(layout);
}

int main()
{
        Display *dpy;
        XkbDescRec *desc;
        XkbStateRec state;
        int event, error, reason, major = XkbMajorVersion, minor = XkbMinorVersion;
        char *name;

        dpy = XkbOpenDisplay("", &event, &error, &major, &minor, &reason);
        if (dpy == NULL) {
                fprintf(stderr, "XkbOpenDisplay failed\n");
                exit(1);
        }

        desc = XkbGetMap(dpy, XkbAllMapComponentsMask, XkbUseCoreKbd);
        XkbGetNames(dpy, XkbSymbolsNameMask, desc);
        XkbGetState(dpy, XkbUseCoreKbd, &state);
        name = XGetAtomName(dpy, desc->names->symbols);
        name = current_layout(name, state.group);
        printf("%s", name);
        free(name);
        XkbFreeNames(desc, XkbSymbolsNameMask, True);
}
