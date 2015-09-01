// 6 january 2015
#include "winapi.h"
#include "../ui.h"
#include "../ui_windows.h"
#include "../uipriv.h"
#include "resources.h"
#include "compilerver.h"

// ui internal window messages
enum {
	// redirected WM_COMMAND and WM_NOTIFY
	msgCOMMAND = WM_APP + 0x40,		// start offset just to be safe
	msgNOTIFY,
	msgHSCROLL,
	msgConsoleEndSession,
};

// init.c
extern HINSTANCE hInstance;
extern int nCmdShow;
extern HFONT hMessageFont;
extern HBRUSH hollowBrush;

// util.c
extern int windowClassOf(HWND, ...);
extern void mapWindowRect(HWND, HWND, RECT *);
extern DWORD getStyle(HWND);
extern void setStyle(HWND, DWORD);
extern DWORD getExStyle(HWND);
extern void setExStyle(HWND, DWORD);

// text.c
extern WCHAR *toUTF16(const char *);
extern char *toUTF8(const WCHAR *);
extern WCHAR *windowText(HWND);





/////////////////////////////////

// resize.c
extern void initResizes(void);
extern void uninitResizes(void);
extern void doResizes(void);
extern void setWindowInsertAfter(HWND, HWND);

// utilwindow.c
extern HWND utilWindow;
extern const char *initUtilWindow(HICON, HCURSOR);
extern void uninitUtilWindow(void);

// parent.c
extern void paintContainerBackground(HWND, HDC, RECT *);
extern BOOL handleParentMessages(HWND, UINT, WPARAM, LPARAM, LRESULT *);

// debug.c
extern HRESULT logLastError(const char *);
extern HRESULT logHRESULT(const char *, HRESULT);
extern HRESULT logMemoryExhausted(const char *);

// window.c
extern ATOM registerWindowClass(HICON, HCURSOR);
extern void unregisterWindowClass(void);

// container.c
#define containerClass L"libui_uiContainerClass"
extern ATOM initContainer(HICON, HCURSOR);
extern void uninitContainer(void);
extern HWND newContainer(void);

// menu.c
extern HMENU makeMenubar(void);
extern const uiMenuItem *menuIDToItem(UINT_PTR);
extern void runMenuEvent(WORD, uiWindow *);
extern void freeMenubar(HMENU);
extern void uninitMenus(void);

// alloc.c
extern int initAlloc(void);
extern void uninitAlloc(void);

// events.c
extern BOOL runWM_COMMAND(WPARAM, LPARAM, LRESULT *);
extern BOOL runWM_NOTIFY(WPARAM, LPARAM, LRESULT *);
extern BOOL runWM_HSCROLL(WPARAM, LPARAM, LRESULT *);
extern void issueWM_WININICHANGE(WPARAM, LPARAM);

// dialoghelper.c
extern void dialogHelperRegisterWindow(HWND);
extern void dialogHelperUnregisterWindow(HWND);
extern ATOM initDialogHelper(HICON, HCURSOR);
extern void uninitDialogHelper(void);
extern HWND beginDialogHelper(void);
extern void endDialogHelper(HWND);

// child.c
extern struct child *newChild(uiControl *child, uiControl *parent, HWND parentHWND);
extern struct child *newChildWithTabPage(uiControl *child, uiControl *parent, HWND parentHWND);
extern void childRemove(struct child *c);
extern void childDestroy(struct child *c);
extern HWND childHWND(struct child *c);
extern void childMinimumSize(struct child *c, uiWindowsSizing *d, intmax_t *width, intmax_t *height);
extern void childRelayout(struct child *c, intmax_t x, intmax_t y, intmax_t width, intmax_t height);
extern void childQueueRelayout(struct child *c);
extern void childUpdateState(struct child *c);
extern HWND childTabPage(struct child *c);
extern int childMargined(struct child *c);
extern void childSetMargined(struct child *c, int margined);
extern int childFlag(struct child *c);
extern void childSetFlag(struct child *c, int flag);
extern intmax_t childIntmax(struct child *c, int n);
extern void childSetIntmax(struct child *c, int n, intmax_t to);

// tabpage.c
extern void tabPageMargins(HWND, intmax_t *, intmax_t *, intmax_t *, intmax_t *);
extern HWND newTabPage(void);