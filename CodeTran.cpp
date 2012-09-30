//�����˸�������˵��
//��д:wwylele

#include "stdafx.h"
#include "CodeTran.h"

COLORREF R5G5B5X1toR8G8B8X8(u16 value)
{
	return RGB(
		(value&0x1F)*256/32,
		((value&0x3E0)>>5)*256/32,
		(value>>10)*256/32);
}
u16 R8G8B8X8toR5G5B5X1(COLORREF value)
{
	return (GetRValue(value)*32/256)|
		((GetGValue(value)*32/256)<<5)|
		((GetBValue(value)*32/256)<<10);
}
TCHAR* CodeTran_Bgm(u8 code)
{
	switch(code)
	{
	case 0:return _T("Music#02");
	case 1:return _T("Music#13-WarpStar");
	case 2:return _T("Music#10");
	case 3:return _T("Music#26-����");
	case 4:return _T("Music#27-Level1���ͼ");
	case 5:return _T("Music#28-Level2���ͼ");
	case 6:return _T("Music#29-Level3���ͼ");
	case 7:return _T("Music#30-Level4���ͼ");
	case 8:return _T("Music#31-Level5���ͼ");
	case 9:return _T("Music#32-Level6���ͼ");
	case 10:return _T("Music#33-Level7���ͼ");
	case 0xB:return _T("Music#04");
	case 0xC:return _T("Music#07");
	case 0xD:return _T("Music#15-������");
	case 0xE:return _T("Music#14-���������̰�");
	case 0xF:return _T("Music#09");
	case 0x10:return _T("Music#43-GameOver");
	case 0x11:return _T("Music#05");
	case 0x12:return _T("Music#08");
	case 0x13:return _T("Music#42-�޵�");
	case 0x14:return _T("Music#19-GreenGreens");
	case 0x15:return _T("Music#03");
	case 0x16:return _T("Music#06");
	case 0x17:return _T("Music#39");
	case 0x18:return _T("Music#25-Ending");
	case 0x19:return _T("Music#17");
	case 0x1A:return _T("Music#00");
	case 0x1B:return _T("Music#20-VsDedede");
	case 0x1C:return _T("Music#40");
	case 0x1D:return _T("Music#41");
	case 0x1E:return _T("Music#12");
	case 0x1F:return _T("Music#21");
	case 0x20:return _T("Music#22");
	case 0x21:return _T("Music#23");
	case 0x22:return _T("Music#24");
	case 0x23:return _T("Music#34");
	//case 0x24:return _T("Music#");
	case 0x25:return _T("Music#11");
	case 0x26:return _T("Music#18");
	case 0x27:return _T("Music#16");
	case 0x28:return _T("Music#01");
	case 0x29:return _T("��");
	case 0x2A:return _T("Music#36");
	case 0x2B:return _T("Music#37");
	case 0x2C:return _T("Music#38");
	//case 0x2D:return _T("Music#");
	case 0x2E:return _T("Music#35");
	case 0x2F:return _T("��ͷMusic#02");
	case 0x30:return _T("��ͷMusic#04");
	case 0x31:return _T("��ͷMusic#07");
	case 0x32:return _T("��ͷMusic#05");
	case 0x33:return _T("��ͷMusic#08");
	case 0x34:return _T("��ͷMusic#19");
	case 0x35:return _T("��ͷMusic#03");
	case 0x36:return _T("��ͷMusic#06");
	case 0x37:return _T("��ͷMusic#09");
	case 0x38:return _T("���ʳ���-���Ǹ�ʲô��BGM?!");

	case 0xFF:return _T("������");
	default:
		return 0;
	}
}
TCHAR* CodeTran_SpcMap(u8 code)
{
	switch(code)
	{
	case 0:return _T("��");
	case 1:return _T("?");
	case 2:return _T("?");
	case 3:return _T("?");
	case 4:return _T("?");
	case 5:return _T("��BOSS��");
	case 6:return _T("?");
	case 7:return _T("?");
	case 8:return _T("?");
	case 9:return _T("?");
	default:
		return 0;
	}
}

bool CodeTran_DetNset(u8 code)
{
	switch(code)
	{
	case 0x20:return true;//return _T("����ש");
	case 0x24:return true;//return _T("����ש");
	case 0x2D:return true;//return _T("��ը����ש");
	case 0x2E:return true;//return _T("ը��ש");
	case 0x2F:return true;//return _T("��ש");
	case 0x33:return true;//return _T("ľ׮");
	case 0x34:return true;//return _T("��ը��������ƽ̨");
	case 0x35:return true;//return _T("���ر�ը����ש");
	case 0x38:return true;//return _T("��������");
	case 0x39:return true;//return _T("��������");
	case 0x3A:return true;//return _T("��������");
	case 0x3B:return true;//return _T("��������");
	case 0x3C:return true;//return _T("��������");
	case 0x3D:return true;//return _T("��������");
	default:return false;
	}
}
TCHAR* CodeTran_Det(u8 code)
{
	switch(code)
	{
	case 0x00:return _T("��");
	case 0x01:return _T("ǽ");
	case 0x02:return _T("б����");
	case 0x03:return _T("б����");
	case 0x04:return _T("��б������");
	case 0x05:return _T("��б������");
	case 0x06:return _T("��б������");
	case 0x07:return _T("��б������");
	case 0x08:return _T("��б����");
	case 0x09:return _T("��б����");
	case 0x0A:return _T("����б������");
	case 0x0B:return _T("����б������");
	case 0x0C:return _T("����б������");
	case 0x0D:return _T("����б������");
	//case 0x0E
	//case 0x0F
	case 0x10:return _T("��");
	case 0x11:return _T("����ƽ̨");
	case 0x12:return _T("����б��ƽ̨");
	case 0x13:return _T("����б��ƽ̨");
	case 0x14:return _T("����б��ƽ̨��");
	case 0x15:return _T("����б��ƽ̨��");
	case 0x16:return _T("����б��ƽ̨��");
	case 0x17:return _T("����б��ƽ̨��");
	//case 0x18
	case 0x19:return _T("˫��ƽ̨");
	case 0x1A:return _T("˫��б��ƽ̨");
	case 0x1B:return _T("˫��б��ƽ̨");
	case 0x1C:return _T("˫��б��ƽ̨��");
	case 0x1D:return _T("˫��б��ƽ̨��");
	case 0x1E:return _T("˫��б��ƽ̨��");
	case 0x1F:return _T("˫��б��ƽ̨��");
	case 0x20:return _T("����ש");
	case 0x24:return _T("����ש");
	case 0x2D:return _T("��ը����ש");
	case 0x2E:return _T("ը��ש");
	case 0x2F:return _T("��ש");
	case 0x30:return _T("����");
	case 0x31:return _T("���Ӷ���");
	case 0x33:return _T("ľ׮");
	case 0x34:return _T("��ը��������ƽ̨");
	case 0x35:return _T("���ر�ը����ש");
	case 0x36:return _T("������");
	case 0x37:return _T("����");
	case 0x38:return _T("��������");
	case 0x39:return _T("��������");
	case 0x3A:return _T("��������");
	case 0x3B:return _T("��������");
	case 0x3C:return _T("��������");
	case 0x3D:return _T("��������");
	case 0x40:return _T("���");
	case 0x41:return _T("��J");
	case 0x42:return _T("���");
	case 0x43:return _T("��K");
	case 0x44:return _T("���");
	case 0x45:return _T("��L");
	case 0x46:return _T("���");
	case 0x47:return _T("��I");
	case 0x48:return _T("���¨J");
	case 0x49:return _T("���¨I");
	case 0x4A:return _T("���¨L");
	case 0x4B:return _T("���¨K");
	case 0x4C:return _T("�����¨I");
	case 0x4D:return _T("�����¨J");
	case 0x4E:return _T("�����¨K");
	case 0x4F:return _T("�����¨L");
	case 0x60:return _T("�̡�");
	case 0x61:return _T("�̡�");
	case 0x62:return _T("�̡�");
	case 0x63:return _T("�̡�");
	case 0x70:return _T("��ǽ");
	case 0x71:return _T("��б����");
	case 0x72:return _T("��б����");
	case 0x73:return _T("����б������");
	case 0x74:return _T("����б������");
	case 0x75:return _T("����б������");
	case 0x76:return _T("����б������");
	case 0x80:return _T("ˮ");
	//case 0x01:return _T("ǽ");
	case 0x82:return _T("ˮ��б����");
	case 0x83:return _T("ˮ��б����");
	case 0x84:return _T("ˮ�л�б������");
	case 0x85:return _T("ˮ�л�б������");
	case 0x86:return _T("ˮ�л�б������");
	case 0x87:return _T("ˮ�л�б������");
	case 0x88:return _T("ˮ�з�б����");
	case 0x89:return _T("ˮ�з�б����");
	case 0x8A:return _T("ˮ�л���б������");
	case 0x8B:return _T("ˮ�л���б������");
	case 0x8C:return _T("ˮ�л���б������");
	case 0x8D:return _T("ˮ�л���б������");
	case 0x90:return _T("ˮ����");
	case 0x91:return _T("ˮ�е���ƽ̨");
	case 0xB6:return _T("ˮ��������");
	case 0xB7:return _T("ˮ�п���");
	case 0xC0:return _T("ˮ����");
	case 0xC1:return _T("ˮ���J");
	case 0xC2:return _T("ˮ����");
	case 0xC3:return _T("ˮ���K");
	case 0xC4:return _T("ˮ����");
	case 0xC5:return _T("ˮ���L");
	case 0xC6:return _T("ˮ����");
	case 0xC7:return _T("ˮ���I");
	case 0xC8:return _T("ˮ���¨J");
	case 0xC9:return _T("ˮ���¨I");
	case 0xCA:return _T("ˮ���¨L");
	case 0xCB:return _T("ˮ���¨K");
	case 0xCC:return _T("��ˮ���¨I");
	case 0xCD:return _T("��ˮ���¨J");
	case 0xCE:return _T("��ˮ���¨K");
	case 0xCF:return _T("����ˮ���¨L");
	case 0xE0:return _T("ˮ�д̡�");
	case 0xE1:return _T("ˮ�д̡�");
	case 0xE2:return _T("ˮ�д̡�");
	case 0xE3:return _T("ˮ�д̡�");
	default:return _T("?");
	}
}

TCHAR* CodeTran_Obj(u16 code)
{
	switch(code)
	{
	case 0x0001:return _T("�߶��");
	case 0x0101:return _T("��ʯ");
	case 0x0201:return _T("˯��");
	case 0x0301:return _T("ɨ��");
	case 0x0401:return _T("���");
	case 0x0501:return _T("����");
	case 0x0601:return _T("ѩ��");
	case 0x0701:return _T("������");
	case 0x0801:return _T("Ģ��");
	case 0x0901:return _T("�߶��");
	case 0x0A01:return _T("����");
	case 0x0B01:return _T("����");
	case 0x0C01:return _T("��Ӭ");
	case 0x0D01:return _T("����");
	case 0x0E01:return _T("�ޱ�");
	case 0x0F01:return _T("��ը���");
	case 0x1001:return _T("С��");
	case 0x1101:return _T("С��");
	case 0x1201:return _T("����");
	case 0x1301:return _T("����");
	case 0x1401:return _T("����");
	case 0x1501:return _T("Scarfy");
	case 0x1601:return _T("����");
	case 0x1701:return _T("����");
	case 0x1801:return _T("����");
	case 0x1901:return _T("����");
	case 0x1A01:return _T("��");
	case 0x1B01:return _T("��");
	case 0x1C01:return _T("��ʿA");
	case 0x1D01:return _T("��ʿB");
	case 0x1E01:return _T("Puppy.JR");
	case 0x1F01:return _T("Puppy.JRƻ��");
	case 0x2001:return _T("Puppy.JRM��");
	case 0x2101:return _T("����");
	case 0x2201:return _T("����");
	case 0x2301:return _T("����");
	case 0x2401:return _T("���");
	case 0x2501:return _T("UFO");
	//case 0x2601:return _T("��ɡ");//
	case 0x2701:return _T("��");
	//case 0x2801:return _T("��ʧ");//
	//case 0x2901:return _T("�����Ĵ���");//
	//case 0x2A01:return _T("��ͼ����");//
	case 0x0003:return _T("Dedede");
	case 0x0103:return _T("����");
	case 0x0203:return _T("÷��");
	case 0x0303:return _T("������");
	case 0x0403:return _T("����");
	case 0x0503:return _T("����");
	case 0x0603:return _T("��");
	case 0x0703:return _T("���ʵ�һ��̬");
	case 0x0803:return _T("��ħ�ڶ���̬");
	//case 0x0903:return _T("");//
	//case 0x0A03:return _T("");//
	//case 0x0004:return _T("������ʧ");//
	case 0x0104:return _T("1UP");
	case 0x0204:return _T("M��");
	case 0x0304:return _T("������");
	case 0x0404:return _T("ҩƿ");
	case 0x0504:return _T("���ǰ�");
	case 0x0105:return _T("��ť");
	case 0x0205:return _T("������ͷ");
	case 0x0305:return _T("����??");
	case 0x0405:return _T("������");
	//case 0x0505:return _T("");//
	case 0x0605:return _T("����");
	//case 0x0705:return _T("");//
	case 0x0805:return _T("��");
	case 0x0008:return _T("÷������");
	default:return _T("?");
	}
}