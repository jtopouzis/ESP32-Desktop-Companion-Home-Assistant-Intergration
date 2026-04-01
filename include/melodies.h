#ifndef MELODIES_H
#define MELODIES_H

// RTTTL melodies (same style QBIT uses)
static const char WIFI_CONNECT_MELODY[] =
    "wifi_connect:d=32,o=6,b=180:"
    "c,8g,2p,c,p,e,p,c,p,e,p,c,p,e";

static const char WIFI_DISCONNECT_MELODY[] =
    "wifi_disconnect:d=16,o=6,b=160:"
    "c7,a,g";

static const char CANTINA_SONG_MELODY[] =
  "Cantina:o=5,d=8,b=250,b=250:"
  "a,p,d6,p,a,p,d6,p,a,d6,p,a,p,g#,4a,a,g#,a,4g,f#,g,f#,4f.,d.,16p,4p.,a,p,d6,p,a,p,d6,p,a,d6,p,a,p,g#,a,p,g,p,4g.,f#,g,p,c6,4a#,4a,4g";

static const char FUNKY_TOWN_MELODY[] =
  "Funky Town:o=4,d=8,b=125,b=200:"
  "c6,c6,a#5,c6,p,g5,p,g5,c6,f6,e6,c6";

static const char POPCORN_MELODY[] =
  "Popcorn:o=5,d=16,b=160,b=160:"
  "a,p,g,p,a,p,e,p,c,p,e,p,8a4,8p,a,p,g,p,a,p,e,p,c,p,e,p,8a4,8p,a,p,b,p,c6,p,b,p,c6,p,a,p,b,p,a,p,b,p,g,p,a,p,g,p,a,p,f,8a,8p,a,p,g,p,a,p,e,p,c,p,e,p,8a4,8p,a,p,g,p,a,p,e,p,c,p,e,p,8a4,8p,a,p,b,p,c6,p,b,p,c6,p,a,p,b,p,a,p,b,p,g,p,a,p,g,p,a,p,b,4c6";

// e.g. touch beep:
static const char TOUCH_BEEP[] =
  "touch:d=16,o=5,b=500:e6";

static const char ANGRY_MELODY[] =
  "angry:d=8,o=5,b=160:g5,c5";

// Boot animation melody
static const char BOOT_MELODY[] =
    "boot:d=16,o=5,b=115:"
    "c,e,g,c6,p,b,a,g,e,c,p,c6";

// Touch (coin) sound — played on GIF switch
static const char TOUCH_MELODY[] =
    "coin:d=16,o=5,b=600:b5,e6";

// Poke notification — ascending chime
static const char CONFIRM_MELODY[] =
    "poke:d=16,o=5,b=200:c6,e6,g6,c7";

// Claim confirmation — short arpeggio
static const char CLAIM_MELODY[] =
    "claim:d=8,o=5,b=180:e,g,b";

// Mute feedback — single descending tone
static const char MUTE_MELODY[] =
    "mute:d=16,o=5,b=200:g5,c5";

// Unmute feedback — single ascending tone
static const char UNMUTE_MELODY[] =
    "unmute:d=16,o=5,b=200:c5,g5";

#endif