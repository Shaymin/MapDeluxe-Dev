//定义了各种文字说明
//编写:wwylele

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
	case 3:return _T("Music#26-死亡");
	case 4:return _T("Music#27-Level1大地图");
	case 5:return _T("Music#28-Level2大地图");
	case 6:return _T("Music#29-Level3大地图");
	case 7:return _T("Music#30-Level4大地图");
	case 8:return _T("Music#31-Level5大地图");
	case 9:return _T("Music#32-Level6大地图");
	case 10:return _T("Music#33-Level7大地图");
	case 0xB:return _T("Music#04");
	case 0xC:return _T("Music#07");
	case 0xD:return _T("Music#15-卡比舞");
	case 0xE:return _T("Music#14-卡比舞缩短版");
	case 0xF:return _T("Music#09");
	case 0x10:return _T("Music#43-GameOver");
	case 0x11:return _T("Music#05");
	case 0x12:return _T("Music#08");
	case 0x13:return _T("Music#42-无敌");
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
	case 0x29:return _T("风");
	case 0x2A:return _T("Music#36");
	case 0x2B:return _T("Music#37");
	case 0x2C:return _T("Music#38");
	//case 0x2D:return _T("Music#");
	case 0x2E:return _T("Music#35");
	case 0x2F:return _T("无头Music#02");
	case 0x30:return _T("无头Music#04");
	case 0x31:return _T("无头Music#07");
	case 0x32:return _T("无头Music#05");
	case 0x33:return _T("无头Music#08");
	case 0x34:return _T("无头Music#19");
	case 0x35:return _T("无头Music#03");
	case 0x36:return _T("无头Music#06");
	case 0x37:return _T("无头Music#09");
	case 0x38:return _T("梦魇出现-这是个什么鬼BGM?!");

	case 0xFF:return _T("无音乐");
	default:
		return 0;
	}
}
TCHAR* CodeTran_SpcMap(u8 code)
{
	switch(code)
	{
	case 0:return _T("无");
	case 1:return _T("?");
	case 2:return _T("?");
	case 3:return _T("?");
	case 4:return _T("?");
	case 5:return _T("大BOSS关");
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
	case 0x20:return true;//return _T("易碎砖");
	case 0x24:return true;//return _T("星星砖");
	case 0x2D:return true;//return _T("爆炸连锁砖");
	case 0x2E:return true;//return _T("炸弹砖");
	case 0x2F:return true;//return _T("钢砖");
	case 0x33:return true;//return _T("木桩");
	case 0x34:return true;//return _T("爆炸连锁单项平台");
	case 0x35:return true;//return _T("隐藏爆炸连锁砖");
	case 0x38:return true;//return _T("导火索│");
	case 0x39:return true;//return _T("导火索─");
	case 0x3A:return true;//return _T("导火索└");
	case 0x3B:return true;//return _T("导火索┘");
	case 0x3C:return true;//return _T("导火索┌");
	case 0x3D:return true;//return _T("导火索┐");
	default:return false;
	}
}
TCHAR* CodeTran_Det(u8 code)
{
	switch(code)
	{
	case 0x00:return _T("无");
	case 0x01:return _T("墙");
	case 0x02:return _T("斜上坡");
	case 0x03:return _T("斜下坡");
	case 0x04:return _T("缓斜上坡左");
	case 0x05:return _T("缓斜上坡右");
	case 0x06:return _T("缓斜下坡左");
	case 0x07:return _T("缓斜下坡右");
	case 0x08:return _T("反斜下坡");
	case 0x09:return _T("反斜上坡");
	case 0x0A:return _T("缓反斜下坡左");
	case 0x0B:return _T("缓反斜下坡右");
	case 0x0C:return _T("缓反斜上坡左");
	case 0x0D:return _T("缓反斜上坡右");
	//case 0x0E
	//case 0x0F
	case 0x10:return _T("门");
	case 0x11:return _T("单向平台");
	case 0x12:return _T("单向斜上平台");
	case 0x13:return _T("单向斜下平台");
	case 0x14:return _T("单向缓斜上平台左");
	case 0x15:return _T("单向缓斜上平台右");
	case 0x16:return _T("单向缓斜下平台左");
	case 0x17:return _T("单向缓斜下平台右");
	//case 0x18
	case 0x19:return _T("双向平台");
	case 0x1A:return _T("双向斜上平台");
	case 0x1B:return _T("双向斜下平台");
	case 0x1C:return _T("双向缓斜上平台左");
	case 0x1D:return _T("双向缓斜上平台右");
	case 0x1E:return _T("双向缓斜下平台左");
	case 0x1F:return _T("双向缓斜下平台右");
	case 0x20:return _T("易碎砖");
	case 0x24:return _T("星星砖");
	case 0x2D:return _T("爆炸连锁砖");
	case 0x2E:return _T("炸弹砖");
	case 0x2F:return _T("钢砖");
	case 0x30:return _T("梯子");
	case 0x31:return _T("梯子顶端");
	case 0x33:return _T("木桩");
	case 0x34:return _T("爆炸连锁单项平台");
	case 0x35:return _T("隐藏爆炸连锁砖");
	case 0x36:return _T("隐藏门");
	case 0x37:return _T("宽门");
	case 0x38:return _T("导火索│");
	case 0x39:return _T("导火索─");
	case 0x3A:return _T("导火索└");
	case 0x3B:return _T("导火索┘");
	case 0x3C:return _T("导火索┌");
	case 0x3D:return _T("导火索┐");
	case 0x40:return _T("风↑");
	case 0x41:return _T("风↗");
	case 0x42:return _T("风→");
	case 0x43:return _T("风↘");
	case 0x44:return _T("风↓");
	case 0x45:return _T("风↙");
	case 0x46:return _T("风←");
	case 0x47:return _T("风↖");
	case 0x48:return _T("风坡↗");
	case 0x49:return _T("风坡↖");
	case 0x4A:return _T("风坡↙");
	case 0x4B:return _T("风坡↘");
	case 0x4C:return _T("反风坡↖");
	case 0x4D:return _T("反风坡↗");
	case 0x4E:return _T("反风坡↘");
	case 0x4F:return _T("反风坡↙");
	case 0x60:return _T("刺↓");
	case 0x61:return _T("刺↑");
	case 0x62:return _T("刺←");
	case 0x63:return _T("刺→");
	case 0x70:return _T("冰墙");
	case 0x71:return _T("冰斜上坡");
	case 0x72:return _T("冰斜下坡");
	case 0x73:return _T("冰缓斜上坡左");
	case 0x74:return _T("冰缓斜上坡右");
	case 0x75:return _T("冰缓斜下坡左");
	case 0x76:return _T("冰缓斜下坡右");
	case 0x80:return _T("水");
	//case 0x01:return _T("墙");
	case 0x82:return _T("水中斜上坡");
	case 0x83:return _T("水中斜下坡");
	case 0x84:return _T("水中缓斜上坡左");
	case 0x85:return _T("水中缓斜上坡右");
	case 0x86:return _T("水中缓斜下坡左");
	case 0x87:return _T("水中缓斜下坡右");
	case 0x88:return _T("水中反斜下坡");
	case 0x89:return _T("水中反斜上坡");
	case 0x8A:return _T("水中缓反斜下坡左");
	case 0x8B:return _T("水中缓反斜下坡右");
	case 0x8C:return _T("水中缓反斜上坡左");
	case 0x8D:return _T("水中缓反斜上坡右");
	case 0x90:return _T("水中门");
	case 0x91:return _T("水中单向平台");
	case 0xB6:return _T("水中隐藏门");
	case 0xB7:return _T("水中宽门");
	case 0xC0:return _T("水流↑");
	case 0xC1:return _T("水流↗");
	case 0xC2:return _T("水流→");
	case 0xC3:return _T("水流↘");
	case 0xC4:return _T("水流↓");
	case 0xC5:return _T("水流↙");
	case 0xC6:return _T("水流←");
	case 0xC7:return _T("水流↖");
	case 0xC8:return _T("水流坡↗");
	case 0xC9:return _T("水流坡↖");
	case 0xCA:return _T("水流坡↙");
	case 0xCB:return _T("水流坡↘");
	case 0xCC:return _T("反水流坡↖");
	case 0xCD:return _T("反水流坡↗");
	case 0xCE:return _T("反水流坡↘");
	case 0xCF:return _T("反风水流坡↙");
	case 0xE0:return _T("水中刺↓");
	case 0xE1:return _T("水中刺↑");
	case 0xE2:return _T("水中刺←");
	case 0xE3:return _T("水中刺→");
	default:return _T("?");
	}
}

TCHAR* CodeTran_Obj(u16 code)
{
	switch(code)
	{
	case 0x0001:return _T("瓦多迪");
	case 0x0101:return _T("岩石");
	case 0x0201:return _T("睡觉");
	case 0x0301:return _T("扫帚");
	case 0x0401:return _T("企鹅");
	case 0x0501:return _T("激光");
	case 0x0601:return _T("雪人");
	case 0x0701:return _T("回力标");
	case 0x0801:return _T("蘑菇");
	case 0x0901:return _T("瓦多度");
	case 0x0A01:return _T("刺球");
	case 0x0B01:return _T("光明");
	case 0x0C01:return _T("苍蝇");
	case 0x0D01:return _T("卡布");
	case 0x0E01:return _T("巨爆");
	case 0x0F01:return _T("爆炸坚果");
	case 0x1001:return _T("小鸟");
	case 0x1101:return _T("小炮");
	case 0x1201:return _T("闪电");
	case 0x1301:return _T("旋风");
	case 0x1401:return _T("乌贼");
	case 0x1501:return _T("Scarfy");
	case 0x1601:return _T("球球");
	case 0x1701:return _T("超人");
	case 0x1801:return _T("火焰");
	case 0x1901:return _T("海葵");
	case 0x1A01:return _T("蛙");
	case 0x1B01:return _T("鱼");
	case 0x1C01:return _T("剑士A");
	case 0x1D01:return _T("剑士B");
	case 0x1E01:return _T("Puppy.JR");
	case 0x1F01:return _T("Puppy.JR苹果");
	case 0x2001:return _T("Puppy.JRM果");
	case 0x2101:return _T("海螺");
	case 0x2201:return _T("车轮");
	case 0x2301:return _T("火球");
	case 0x2401:return _T("针刺");
	case 0x2501:return _T("UFO");
	//case 0x2601:return _T("阳伞");//
	case 0x2701:return _T("猪");
	//case 0x2801:return _T("消失");//
	//case 0x2901:return _T("不动的锤子");//
	//case 0x2A01:return _T("贴图错误");//
	case 0x0003:return _T("Dedede");
	case 0x0103:return _T("画画");
	case 0x0203:return _T("梅塔");
	case 0x0303:return _T("挖土机");
	case 0x0403:return _T("日月");
	case 0x0503:return _T("大树");
	case 0x0603:return _T("云");
	case 0x0703:return _T("梦魇第一形态");
	case 0x0803:return _T("梦魔第二形态");
	//case 0x0903:return _T("");//
	//case 0x0A03:return _T("");//
	//case 0x0004:return _T("怪物消失");//
	case 0x0104:return _T("1UP");
	case 0x0204:return _T("M果");
	case 0x0304:return _T("棒棒糖");
	case 0x0404:return _T("药瓶");
	case 0x0504:return _T("星星棒");
	case 0x0105:return _T("按钮");
	case 0x0205:return _T("导火索头");
	case 0x0305:return _T("大炮??");
	case 0x0405:return _T("传送星");
	//case 0x0505:return _T("");//
	case 0x0605:return _T("大炮");
	//case 0x0705:return _T("");//
	case 0x0805:return _T("风");
	case 0x0008:return _T("梅塔军队");
	default:return _T("?");
	}
}