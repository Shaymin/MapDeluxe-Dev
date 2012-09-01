#pragma once

#define CODETRAN_FORMAT _T("%d:%s")

TCHAR* CodeTran_Bgm(u8 code);
TCHAR* CodeTran_SpcMap(u8 code);
TCHAR* CodeTran_Det(u8 code);
bool CodeTran_DetNset(u8 code);
