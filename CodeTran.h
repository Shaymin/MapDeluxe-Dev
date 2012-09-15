#pragma once

#define CODETRAN_FORMAT _T("%d:%s")

TCHAR* CodeTran_Bgm(u8 code);
TCHAR* CodeTran_SpcMap(u8 code);
TCHAR* CodeTran_Det(u8 code);
bool CodeTran_DetNset(u8 code);
u16 R8G8B8X8toR5G5B5X1(COLORREF value);
COLORREF R5G5B5X1toR8G8B8X8(u16 value);
