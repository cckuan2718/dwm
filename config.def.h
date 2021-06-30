/* See LICENSE file for copyright and license details. */

#define TERMINAL "xterm"
#define TERMCLASS "XTerm"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=11" };
static const char dmenufont[]       = "monospace:size=16";
static const char norm_fg_color[]     = "#ebdbb2";
static const char norm_bg_color[]     = "#282828";
static const char norm_border_color[] = "#282828";
static const char sel_fg_color[]      = "#282828";
static const char sel_bg_color[]      = "#fabd2f";
static const char sel_border_color[]  = "#fabd2f";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { norm_fg_color, norm_bg_color, norm_border_color },
	[SchemeSel]  = { sel_fg_color,  sel_bg_color,  sel_border_color  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     iscentered   isfloating   monitor */
	{ "Firefox",     NULL,       NULL,       1 << 4,       0,           0,           -1 },
	{ TERMCLASS,     "float",    NULL,       0,            1,           1,           -1 },
	{ TERMCLASS,     "tmuxwm",   NULL,       1 << 0,       1,           0,           -1 },
	{ "Emacs",       NULL,       NULL,       1 << 1,       1,           0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.70; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { TERMINAL, NULL };

/* Custom commands */
static const char *backlight_dec_cmd[]          = { "backlightctl", "-d", NULL                                                   };
static const char *backlight_inc_cmd[]          = { "backlightctl", "-i", NULL                                                   };
static const char *calculator_cmd[]             = { TERMINAL, "-name", "float", "-e", "bc", "-l",  NULL                          };
static const char *displayctl_default_cmd[]     = { "displayctl", "oi", NULL                                                     };
static const char *displayctl_interactive_cmd[] = { "displayctl", "-i",  NULL                                                    };
static const char *dmenu_mount_cmd[]            = { "dmenu_mount", NULL                                                          };
static const char *dmenu_open_documents_cmd[]   = { "/bin/sh", "-c", "dmenu_open ${HOME}/documents/", NULL                       };
static const char *dmenu_open_downloads_cmd[]   = { "/bin/sh", "-c", "dmenu_open ${HOME}/downloads/", NULL                       };
static const char *dmenu_pass_cmd[]             = { "dmenu_pass", NULL                                                           };
static const char *dmenu_unicode_cmd[]          = { "dmenu_unicode", NULL                                                        };
static const char *dmenu_unmount_cmd[]          = { "dmenu_unmount", NULL                                                        };
static const char *emacs_cmd[]                  = { "emacsclient", "-c", "-a", "", NULL                                          };
static const char *lockscreen_cmd[]             = { "powerctl", "lockscreen", NULL                                               };
static const char *mpc_next_cmd[]               = { "mpc_wrapper", "next", NULL                                                  };
static const char *mpc_prev_cmd[]               = { "mpc_wrapper", "prev", NULL                                                  };
static const char *mpc_status_cmd[]             = { "mpc_wrapper", "status", NULL                                                };
static const char *mpc_stop_cmd[]               = { "mpc_wrapper", "stop", NULL                                                  };
static const char *mpc_toggle_cmd[]             = { "mpc_wrapper", "toggle", NULL                                                };
static const char *music_player_cmd[]           = { TERMINAL, "-name", "float", "-e", "ncmpcpp", NULL                            };
static const char *powerctl_cmd[]               = { "powerctl", "-i", NULL                                                       };
static const char *screenshot_cmd[]             = { "screenshot", NULL                                                           };
static const char *screenshot_interactive_cmd[] = { "screenshot", "-i", NULL                                                     };
static const char *show_clipboard_cmd[]         = { "showclip", NULL                                                             };
static const char *tmux_cmd[]                   = { TERMINAL, "-name", "tmuxwm", "-e", "tmux", "new-session", "-As", "wm",  NULL };
static const char *top_cmd[]                    = { TERMINAL, "-name", "float", "-e", "top", NULL                                };
static const char *volume_dec_cmd[]             = { "volumectl", "-d", NULL                                                      };
static const char *volume_inc_cmd[]             = { "volumectl", "-i", NULL                                                      };
static const char *volume_toggle_cmd[]          = { "volumectl", "-t", NULL                                                      };
static const char *www_browser_cmd[]            = { "firefox", NULL                                                              };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

	/*  Custom key bindings */
	{ 0,                XK_Menu,      spawn,     {.v = dmenu_open_documents_cmd }   },
	{ 0,                XK_Print,     spawn,     {.v = screenshot_cmd }             },
	{ MODKEY,           XK_Down,      spawn,     {.v = mpc_toggle_cmd }             },
	{ MODKEY,           XK_F1,        spawn,     {.v = dmenu_open_downloads_cmd }   },
	{ MODKEY,           XK_F10,       spawn,     {.v = dmenu_unmount_cmd }          },
	{ MODKEY,           XK_F2,        spawn,     {.v = dmenu_open_documents_cmd }   },
	{ MODKEY,           XK_F7,        spawn,     {.v = displayctl_interactive_cmd } },
	{ MODKEY,           XK_F9,        spawn,     {.v = dmenu_mount_cmd }            },
	{ MODKEY,           XK_Insert,    spawn,     {.v = show_clipboard_cmd }         },
	{ MODKEY,           XK_Left,      spawn,     {.v = mpc_prev_cmd }               },
	{ MODKEY,           XK_Right,     spawn,     {.v = mpc_next_cmd }               },
	{ MODKEY,           XK_Up,        spawn,     {.v = mpc_stop_cmd }               },
	{ MODKEY,           XK_c,         spawn,     {.v = calculator_cmd }             },
	{ MODKEY,           XK_e,         spawn,     {.v = emacs_cmd }                  },
	{ MODKEY,           XK_equal,     spawn,     {.v = volume_inc_cmd }             },
	{ MODKEY,           XK_grave,     spawn,     {.v = dmenu_unicode_cmd }          },
	{ MODKEY,           XK_minus,     spawn,     {.v = volume_dec_cmd }             },
	{ MODKEY,           XK_n,         spawn,     {.v = mpc_status_cmd }             },
	{ MODKEY,           XK_r,         spawn,     {.v = top_cmd }                    },
	{ MODKEY,           XK_s,         view,      {.ui = 1 << 0}                     },
	{ MODKEY,           XK_w,         view,      {.ui = 1 << 4}                     },
	{ MODKEY|ShiftMask, XK_BackSpace, spawn,     {.v = powerctl_cmd }               },
	{ MODKEY|ShiftMask, XK_F7,        spawn,     {.v = displayctl_default_cmd }     },
	{ MODKEY|ShiftMask, XK_equal,     spawn,     {.v = backlight_inc_cmd }          },
	{ MODKEY|ShiftMask, XK_l,         spawn,     {.v = lockscreen_cmd }             },
	{ MODKEY|ShiftMask, XK_m,         spawn,     {.v = volume_toggle_cmd }          },
	{ MODKEY|ShiftMask, XK_minus,     spawn,     {.v = backlight_dec_cmd }          },
	{ MODKEY|ShiftMask, XK_n,         spawn,     {.v = music_player_cmd }           },
	{ MODKEY|ShiftMask, XK_p,         spawn,     {.v = dmenu_pass_cmd }             },
	{ MODKEY|ShiftMask, XK_s,         spawn,     {.v = tmux_cmd }                   },
	{ MODKEY|ShiftMask, XK_w,         spawn,     {.v = www_browser_cmd }            },
	{ ShiftMask,        XK_Menu,      spawn,     {.v = dmenu_open_downloads_cmd }   },
	{ ShiftMask,        XK_Print,     spawn,     {.v = screenshot_interactive_cmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

