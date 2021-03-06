/*************************************************************************
FILE:		FONT.C
AUTHORS:	! (James MacIsaac & Brad Ritten)
PURPOSE:	bitmaps for fonts
*************************************************************************/

#ifndef FONT_H
#define FONT_H

/*************************************************************************
FILE & LIBRARY REFERENCES
*************************************************************************/
#include "types.h"

extern const UINT8 font[];     /* 8x8 system font */

/*************************************************************************
CONSTANT DECLARATIONS
*************************************************************************/
#define FONT_HEIGHT 8
#define CHAR_BEL    7
#define CHAR_TAB    9
#define CHAR_LF    10
#define CHAR_VT    11
#define CHAR_FF    12
#define CHAR_CR    13
#define CHAR_DEL  127

/*************************************************************************
FUNCTION:
PARAMETERS:
RETURNS:
PURPOSE:
*************************************************************************/
#define IS_PRINTABLE(i) ((i) >= ' ' && (i) <= '~')

/*************************************************************************
FUNCTION:
PARAMETERS:
RETURNS:
PURPOSE:
*************************************************************************/
#define GLYPH_START(i) (font + (((i) - ' ') << 3))

#endif
