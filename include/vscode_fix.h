/*
Used to get VS Code Intellisense working correctly for header definitions
protected by compiler-specific feature flags.

from: https://github.com/cuibonobo/nds_vscode_template/blob/master/include/vscode_fix.h
*/
#ifdef __GNUC__
#define _ATTRIBUTE(attrs) __attribute__(attrs)
#else
#define _ATTRIBUTE(attrs)
#endif

int iprintf(const char *, ...)
    _ATTRIBUTE((__format__(__printf__, 1, 2)));