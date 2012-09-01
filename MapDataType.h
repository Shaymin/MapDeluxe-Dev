//游戏数据结构
#pragma once
struct STEP_HEADER
{
	u8 level;
	u8 stage;
	u16 step;
	u8 bgm;
	u8 lz77;
	u16 x06;//=0
	u32 ptr_map;
	u32 ptr_nestmap;

	u32 ptr_nestlist;
	u16 width;
	u16 height;
	u32 ptr_plt_frt;
	u32 ptr_till_frt;

	u32 ptr_gra_map_frt;
	u16 clip_x;
	u16 clip_y;
	u32 ptr_plt_bck;
	u32 ptr_till_bck;

	u32 ptr_gra_map_bck;
	u16 bck_mov;
	u16 x36;
	s16 wind_index;//
	u16 door_count;
	u16 obj_count;
	u16 x3E;

	u32 gra_ani_index;
	u32 ptr_door;
	u32 ptr_obj;
	u32 x4C;//=0

	u16 begin_x;
	u16 begin_y;
	u8 x54;
	u8 x55;
	u8 bck_roll;
	u8 spc_map;

};

struct GRID_DATA
{
	u16 gra;
	u8 edg;
	u8 det;
};
/*struct DOOR_DATA
{
	u16 to_step;
	u16 from_x;
	u16 from_y;
	u16 to_x;
	u16 to_y;
	u16 x;
};*/
union DOOR_DATA
{
	struct{
		u16 to_step;
		u16 x;
		u16 y;
		u16 to_x;
		u16 to_y;
		u16 unk;
	}door;
	struct{
		u16 magic;//5555
		u16 x;
		u16 y;
		u16 unk1;
		u16 unk2;
		u16 unk3;
	}ret;
	struct{
		u16 magic;//6666
		u16 to_step;//0xFFFF mean end
		u16 x1;
		u16 x2;
		u16 x3;
		u16 unk2;
	}spc_rec;
	struct{
		u16 magic;//7777
		u16 to_step;
		u16 unk1;
		u16 x2;
		u16 x3;
		u16 unk2;
	}spc;
	struct{
		u16 magic;//8888
		u16 x;
		u16 y;
		u16 unk1;
		u16 unk2;
		u16 unk3;
	}goal;
};
struct OBJ_DATA
{
	u16 x00;
	u8 x02;
	u8 ext_len;
	u16 pos_x;
	u16 pos_y;
};