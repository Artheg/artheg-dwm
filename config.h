#include "XF86keysym.h"
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "CozetteVector:size=10" };
static const char dmenufont[]       = "CozetteVector:size=10";
static const char rofifont[]       = "CozetteVector 10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_dirty_blue[]  = "#444455";
static const char col_white[]       = "#ffffff";
static const char col_gordon[]      = "#b35820";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1,  col_gordon  },
	[SchemeSel]  = { col_gray4, col_dirty_blue, col_white  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",       NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",    NULL,       NULL,       1 << 8,       0,           -1 },
	{ "st",  NULL,       "nmtui",    0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const unsigned int gappx     = 30;        /* gap pixel between windows */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "T",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "CM",      centeredmaster },
	{ "CFM",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define HOLDKEY XK_Super_L

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *screenshotcmd[] = { "flameshot", "gui", NULL };
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_dirty_blue, "-sf", col_gray4, NULL };
static const char *dmenu_nm_cmd[] = { "networkmanager_dmenu", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_dirty_blue, "-sf", col_gray4, NULL };
static const char *roficmd[] = { "rofi", "-show", "drun", "-show-icons", "-font", rofifont, "-theme", "DarkBlue", "-monitor", "-4", NULL };
static const char *dmenu_ytfzf_cmd[] = { "ytfzf", "-Df", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *fm_cmd[]  = { "st", "-e", "lfub", NULL};
static const char *select_screen_layout_cmd[]  = { "select_screen_layout", NULL };
static const char *dmenu_pulse_cmd[]  = { "dmenu_pulse", NULL };
static const char *dmenu_songname_cmd[] = {"/home/artheg/songname.sh", NULL };
static const char *lock_cmd[] = { "xlock", NULL };

static const char *upvol[]   = { "vol",  "up", NULL};
static const char *downvol[]   = { "vol", "down", NULL};
static const char *mutevol[]   = { "vol",  "mute", NULL};
static const char *prevtrack[]   = { "playerctl", "previous",    NULL };
static const char *playpause[] = { "playerctl", "play-pause",    NULL };
static const char *nexttrack[] = { "playerctl", "next",   "0", "toggle",  NULL };

#include "focusurgent.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
  { MODKEY|ControlMask,           XK_c,      spawn,          {.v = lock_cmd} },
	{ MODKEY,                       XK_space,  spawn,          {.v = roficmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      shiftview,      {.i = -1  } },
	{ MODKEY,                       XK_l,      shiftview,      {.i = 1   } },
	{ MODKEY|ControlMask,           XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|ControlMask,           XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_z, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = -0.25f }},
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = 0.25f }},
	{ MODKEY|ShiftMask,             XK_0,      setcfact,       {.f = 0 }},
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_f,      fullscreen,     {0} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_p,      setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = -10 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = +10 } },
	{ MODKEY|ControlMask,           XK_e,      spawn,          {.v = fm_cmd } },
	{ MODKEY|ControlMask,           XK_w,      spawn,          {.v = dmenu_nm_cmd } },
  { MODKEY|ControlMask,           XK_v,      spawn,          {.v = select_screen_layout_cmd } },
  { MODKEY|ControlMask,           XK_b,      spawn,          {.v = dmenu_pulse_cmd } },
	{ MODKEY|ControlMask,           XK_s,      spawn,          {.v = dmenu_songname_cmd } },
	{ MODKEY|ControlMask,           XK_y,      spawn,          {.v = dmenu_ytfzf_cmd } },
	{ 0,                            XK_Print,      spawn,      {.v = screenshotcmd } },
  { 0,                            XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
	{ 0,                            XF86XK_AudioMute, spawn, {.v = mutevol } },
	{ 0,                            XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },
  { 0,                            XF86XK_AudioPrev, spawn, {.v = prevtrack } },
	{ 0,                            XF86XK_AudioPlay, spawn, {.v = playpause } },
	{ 0,                            XF86XK_AudioNext, spawn, {.v = nexttrack } },
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
	{ MODKEY|ShiftMask,             XK_u,      focusurgent,    {0} },
	{ 0,                            HOLDKEY,   holdbar,        { 0 } }
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

